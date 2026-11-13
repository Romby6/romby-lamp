/*
  Romby Lamp — circadian / progress-bar / Teams-busy desk lamp
  Hardware: QuinLED Dig-Uno v3 (ESP32), EC11 encoder, and either
            - a WS2811 dual-IC RGBCCT 24V strip (RGB chip + warm/cool white chip per segment), or
            - a WS2814 RGBW 24V strip
  ---------------------------------------------------------------------------
  Wiring for the QuinLED Dig-Uno v4 with the QuinLED-ESP32 (ESP32-PICO-MINI-02) module (see README):
    Strip data  -> LED1 (GPIO2)
    Encoder A   -> Q1  (GPIO32)
    Encoder B   -> Q4  (GPIO15)
    Encoder SW  -> Button (GPIO37, input only, pulled high on the board)
    KY-040 "+"  -> Q2  (GPIO33: the firmware drives it to 3.3 V to power the module's pull-up resistors)
  Never use GPIO16/17 on this module: they belong to its internal PSRAM.
  (Dig-Uno v3 used LED1 = GPIO16 and Button = GPIO0. Change the four PIN_ lines to build for v3.)
    Encoder C + other SW leg -> GND

  Schedule (Settings > Work hours):
    Work hours       lamp switches on automatically at the start, off at the end
    On before work   (within the "early" window) stays on through the workday
    On after hours   stays on for the after-hours timer (2 h default), then off
    Fades            1-minute fade up before work starts and down before it ends

  Controls:
    Turn knob        brightness (turning while off switches the lamp on)
    Short press      on / off  (also ends a Teams test)
    Double press     "busy" light on / off by hand (in-person meetings, phone calls)
    Long press       switch Circadian <-> Progress bar

  Library needed (Arduino Library Manager): "Adafruit NeoPixel"
  Board: "ESP32 Dev Module" (esp32 by Espressif, 3.x)
  Partition scheme: "Minimal SPIFFS (1.9MB APP with OTA/190KB SPIFFS)"  <- needed for WiFi updates

  Updating over WiFi: Sketch > Export Compiled Binary, then upload RombyLamp.ino.bin on the
  dashboard (Settings > Firmware), or pick the "rombylamp" network port in Arduino IDE
  (OTA password: rombylamp).

  USB control: the PC bridge (RombyLampBridge.ps1) sends the same commands as the web API,
  one per line over USB serial, e.g. "/api/set?bri=50". Replies are one line starting with "@".
*/

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <Preferences.h>
#include <Adafruit_NeoPixel.h>
#include <Update.h>
#include <ArduinoOTA.h>
#include <time.h>
#include <sys/time.h>
#include "dashboard.h"

#define FW_VERSION   "1.3.7"
#define PIN_LED      2     // Dig-Uno v4 LED1
#define PIN_ENC_A    32    // Dig-Uno v4 Q1
#define PIN_ENC_B    15    // Dig-Uno v4 Q4
#define PIN_BTN      37    // Dig-Uno v4 Button (input only; the board pulls it high)
#define PIN_ENC_PWR  33    // Dig-Uno v4 Q2: 3.3 V supply for a KY-040 module's "+" pin (about 0.7 mA)
#define MAX_PIXELS   600          // logical pixels (a dual-IC segment counts as one)
#define CH           5            // R, G, B, W1, W2 (RGBW uses W1; RGBCCT uses W1 = warm, W2 = cool)
#define HOSTNAME     "rombylamp"
#define AP_PASSWORD  "rombylamp"          // 8+ chars required for a WPA2 hotspot
#define CFG_VERSION  7

static const char* TZ_EASTERN = "EST5EDT,M3.2.0,M11.1.0";   // US Eastern with DST

// ---------------------------------------------------------------------------
// Settings (saved to flash)
// ---------------------------------------------------------------------------
struct Settings {
  uint8_t  version      = CFG_VERSION;
  uint8_t  stripType    = 1;      // 0 = RGBW single IC (WS2814), 1 = RGBCCT dual IC (WS2811)
  uint16_t pixels       = 60;     // RGBCCT: number of segments (each has two chips)
  uint8_t  order        = 0;      // RGBW: index into ORDERS[]
  uint8_t  rgbOrder     = 0;      // RGBCCT: wire order of the RGB chip, index into RGB_ORDERS[]
  uint8_t  cctOrder     = 0;      // RGBCCT: which channels of the white chip are warm / cool, index into CCT_ORDERS[]
  bool     cctFirst     = false;  // RGBCCT: the white chip comes before the RGB chip on the data line
  uint16_t warmK        = 3000;   // RGBCCT: warm white LED colour temperature
  uint16_t coolK        = 6500;   // RGBCCT: cool white LED colour temperature
  bool     reverse      = false;  // progress bar fills from the other end
  uint16_t whiteK       = 4000;   // colour temperature of the strip's W LED
  uint8_t  maxBri       = 80;     // overall brightness cap, % (protects the PSU)
  uint16_t progStart    = 480;    // work starts 8:00 (minutes after midnight)
  uint16_t progEnd      = 1050;   // work ends 17:30
  bool     weekdaysOnly = true;   // weekends count as off-hours
  bool     autoSchedule = true;   // on at work start, off at work end
  uint16_t afterHoursMin = 120;   // switched on outside work hours: auto-off after this (0 = stay on)
  uint8_t  earlyHrs     = 3;      // switched on this close before work starts: stays on through the day
  uint8_t  fadeMin      = 1;      // minutes to fade up before work starts and down before it ends (0 = instant)
  uint8_t  glow         = 10;     // % brightness of the not-yet-lit part of the bar
  uint16_t curveMin[5]  = {360, 480, 720, 1020, 1260};   // 6:00 8:00 12:00 17:00 21:00
  uint16_t curveK[5]    = {2700, 6000, 5000, 3500, 2400};
  uint8_t  redTrigger   = 2;      // 0 calls only, 1 calls + meetings, 2 whenever Teams shows red
  uint8_t  redBri       = 60;     // % brightness of the red busy light
  uint8_t  redStyle     = 0;      // 0 = red ends only, 1 = whole strip red
  uint8_t  redEndPct    = 10;     // % of the strip lit red at each end
  bool     redPulse     = true;   // gentle breathing on the red
  bool     redWhenOff   = true;   // show red even if the lamp is switched off
  uint8_t  autoClearHrs = 4;      // safety: drop red if no presence update for this long (0 = never)
  uint8_t  encSteps     = 4;      // encoder pulses per click (EC11 is usually 4, some are 2)
  bool     encReverse   = false;
  bool     lowTx        = true;   // 8.5 dBm WiFi power (fixes brown-out drops on QuinLED boards)
  bool     keepAP       = false;  // keep the setup hotspot on even when connected
  bool     wifiOff      = false;  // USB only: no WiFi, no hotspot (office use)
  // added in 1.3.5 (new fields go at the end so saved settings carry over)
  uint8_t  redPulseMin  = 10;     // busy-light pulse: lowest point, % of red brightness
  uint8_t  redPulseSec  = 4;      // busy-light pulse: seconds per breath
};

const neoPixelType ORDERS[] = {NEO_GRBW, NEO_RGBW, NEO_RBGW, NEO_GBRW, NEO_BRGW, NEO_BGRW};
const char* ORDER_NAMES[]   = {"GRBW", "RGBW", "RBGW", "GBRW", "BRGW", "BGRW"};
const uint8_t ORDER_COUNT   = 6;
// RGB chip: for each wire byte, which colour it carries (0 R, 1 G, 2 B)
const uint8_t RGB_ORDERS[6][3] = {{0,1,2}, {0,2,1}, {1,0,2}, {1,2,0}, {2,0,1}, {2,1,0}};   // RGB RBG GRB GBR BRG BGR
// white chip: for each wire byte, 3 = warm, 4 = cool, 9 = unused
const uint8_t CCT_ORDERS[6][3] = {{3,4,9}, {4,3,9}, {3,9,4}, {4,9,3}, {9,3,4}, {9,4,3}};
const uint8_t RGB_ORDER_COUNT = 6, CCT_ORDER_COUNT = 6;

Settings cfg;
Preferences prefs;
Adafruit_NeoPixel strip(60, PIN_LED, NEO_GRBW + NEO_KHZ800);
WebServer server(80);
DNSServer dns;

// ---------------------------------------------------------------------------
// Runtime state
// ---------------------------------------------------------------------------
bool     powerOn  = true;
uint8_t  bri      = 5;       // %  (first power-up starts dim)
uint8_t  mode     = 0;       // 0 circadian, 1 progress, 2 manual white
uint16_t manualK  = 4000;
uint32_t stateDirtyAt = 0;

bool     teamsRed   = false;
bool     presSeen   = false;
String   presAvail  = "";
String   presAct    = "";
uint32_t presAtMs   = 0;
bool     teamsTest  = false;
bool     manualBusy = false;     // double-press / dashboard "I'm busy"
uint32_t manualBusyAt = 0;
bool     updating   = false;     // firmware update in progress
float    updProgress = 0;
uint32_t testUntil  = 0;

uint32_t offAtMs      = 0;   // after-hours auto-off deadline (0 = none)
int8_t   lastInWork   = -1;  // for detecting work start / end
bool     schedOn      = false; // true when the schedule (not a person) switched the lamp on
bool     scheduleBooted = false;

int      previewMin   = -1;  // dashboard time scrubber mirrored onto the lamp
uint32_t previewUntil = 0;
uint32_t identifyAt   = 0;   // colour-order test sequence start (0 = off)

String   wifiSsid, wifiPass;
bool     apOn = false;
bool     staWasUp = false;
uint32_t staDownSince = 0;
String   apSsid;
uint32_t rebootAt = 0;

float    cur[MAX_PIXELS][CH];       // eased output, 0..1
uint8_t  lastOut[MAX_PIXELS * CH];  // last bytes sent, to skip identical frames

// ---------------------------------------------------------------------------
// Encoder (interrupt driven quadrature)
// ---------------------------------------------------------------------------
volatile int32_t encRaw  = 0;
volatile uint32_t encEdges = 0;   // every change seen on the knob's turning wires (for the dashboard's knob test)
volatile uint8_t encPrev = 0;
const int8_t ENC_TABLE[16] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};

void IRAM_ATTR encISR() {
  uint8_t s = (digitalRead(PIN_ENC_A) << 1) | digitalRead(PIN_ENC_B);
  encPrev = ((encPrev << 2) | s) & 0x0F;
  encRaw += ENC_TABLE[encPrev];
  encEdges++;
}

// ---------------------------------------------------------------------------
// Persistence
// ---------------------------------------------------------------------------
void loadAll() {
  prefs.begin("lamp", true);
  size_t saved = prefs.getBytesLength("cfg");
  if (saved > 0 && saved <= sizeof(Settings)) {
    // older firmware may have saved a shorter block: fields it didn't have keep their defaults
    Settings tmp;
    prefs.getBytes("cfg", &tmp, saved);
    if (tmp.version == CFG_VERSION) cfg = tmp;
  }
  powerOn  = prefs.getBool("on", true);
  bri      = prefs.getUChar("bri", 5);
  mode     = prefs.getUChar("mode", 0);
  manualK  = prefs.getUShort("mk", 4000);
  wifiSsid = prefs.getString("ssid", "");
  wifiPass = prefs.getString("pass", "");
  prefs.end();
  cfg.pixels = constrain(cfg.pixels, 1, MAX_PIXELS);
  if (cfg.order >= ORDER_COUNT) cfg.order = 0;
  if (cfg.redPulseMin > 90) cfg.redPulseMin = 10;
  if (cfg.redPulseSec < 1 || cfg.redPulseSec > 20) cfg.redPulseSec = 4;
  if (cfg.stripType > 1) cfg.stripType = 1;
  if (cfg.rgbOrder >= RGB_ORDER_COUNT) cfg.rgbOrder = 0;
  if (cfg.cctOrder >= CCT_ORDER_COUNT) cfg.cctOrder = 0;
  if (cfg.coolK <= cfg.warmK) { cfg.warmK = 3000; cfg.coolK = 6500; }
  if (cfg.encSteps != 2 && cfg.encSteps != 4) cfg.encSteps = 4;
  if (mode > 2) mode = 0;
#ifdef ROMBY_EMULATOR
  cfg.wifiOff = true;              // the emulator has no WiFi radio
#endif
  bri = constrain(bri, 1, 100);
}

void saveCfg() {
  prefs.begin("lamp", false);
  prefs.putBytes("cfg", &cfg, sizeof(Settings));
  prefs.end();
}

void saveState() {
  prefs.begin("lamp", false);
  prefs.putBool("on", powerOn);
  prefs.putUChar("bri", bri);
  prefs.putUChar("mode", mode);
  prefs.putUShort("mk", manualK);
  prefs.end();
}

void markDirty() { stateDirtyAt = millis(); }

// Bigger loop stack as a safety margin (default is 8 KB)
SET_LOOP_TASK_STACK_SIZE(16 * 1024);

// Send the pixel buffer to the strip through the ESP32's RMT peripheral.
// The NeoPixel library builds this signal in a stack array (32 bytes per LED byte), which overflows
// the 8 KB task stack on longer strips (60 RGBCCT segments = 360 bytes = 11.5 KB). Here the signal
// buffer lives on the heap and is reused.
static rmt_data_t *rmtBuf = nullptr;
static size_t rmtCap = 0;
void stripShow() {
  uint8_t *px = strip.getPixels();
  if (!px) return;
  size_t nBytes = (size_t)strip.numPixels() * (cfg.stripType == 1 ? 3 : 4);
  size_t nSym = nBytes * 8;
  if (nSym > rmtCap) {
    free(rmtBuf);
    rmtBuf = (rmt_data_t *)malloc(nSym * sizeof(rmt_data_t));
    rmtCap = rmtBuf ? nSym : 0;
    if (!rmtBuf) { Serial.println("Not enough memory for the strip length"); return; }
  }
#ifndef ROMBY_EMULATOR            // the ESP32 emulator has no RMT peripheral
  if (!rmtInit(PIN_LED, RMT_TX_MODE, RMT_MEM_NUM_BLOCKS_1, 10000000)) return;   // 100 ns ticks
#endif
  size_t i = 0;
  for (size_t b = 0; b < nBytes; b++) {
    uint8_t v = px[b];
    for (int bit = 7; bit >= 0; bit--) {
      bool one = v & (1 << bit);
      rmtBuf[i].level0 = 1; rmtBuf[i].duration0 = one ? 8 : 4;   // 800 kHz WS281x timing
      rmtBuf[i].level1 = 0; rmtBuf[i].duration1 = one ? 4 : 8;
      i++;
    }
  }
#ifndef ROMBY_EMULATOR
  rmtWrite(PIN_LED, rmtBuf, nSym, RMT_WAIT_FOR_EVER);
#else
  static uint32_t frames = 0;
  if ((++frames % 250) == 1) Serial.printf("emu: strip frame %u, %u bytes, %u symbols, free heap %u\n", (unsigned)frames, (unsigned)nBytes, (unsigned)nSym, (unsigned)ESP.getFreeHeap());
#endif
}

void applyStripConfig() {
  if (cfg.stripType == 1) {                     // two 3-channel chips per segment; we order the bytes ourselves
    strip.updateType(NEO_RGB + NEO_KHZ800);
    strip.updateLength(cfg.pixels * 2);
  } else {
    strip.updateType(ORDERS[cfg.order] + NEO_KHZ800);
    strip.updateLength(cfg.pixels);
  }
  strip.begin();
  memset(lastOut, 0xFF, sizeof(lastOut));   // force a full refresh
}

// ---------------------------------------------------------------------------
// Colour engine (mirrored in the dashboard's JavaScript)
// ---------------------------------------------------------------------------
void kelvinToRgb(float k, float &r, float &g, float &b) {
  float t = constrain(k, 1000.0f, 12000.0f) / 100.0f;
  if (t <= 66.0f) {
    r = 255.0f;
    g = 99.4708025861f * logf(t) - 161.1195681661f;
    b = (t <= 19.0f) ? 0.0f : 138.5177312231f * logf(t - 10.0f) - 305.0447927307f;
  } else {
    r = 329.698727446f * powf(t - 60.0f, -0.1332047592f);
    g = 288.1221695283f * powf(t - 60.0f, -0.0755148492f);
    b = 255.0f;
  }
  r = constrain(r, 0.0f, 255.0f) / 255.0f;
  g = constrain(g, 0.0f, 255.0f) / 255.0f;
  b = constrain(b, 0.0f, 255.0f) / 255.0f;
}

// Target colour temperature -> RGBW, using as much of the W LED as possible
void kelvinToRgbw(float k, float out[4]) {
  float tr, tg, tb, wr, wg, wb;
  kelvinToRgb(k, tr, tg, tb);
  kelvinToRgb(cfg.whiteK, wr, wg, wb);
  float a = 1e9f;
  if (wr > 0.001f) a = min(a, tr / wr);
  if (wg > 0.001f) a = min(a, tg / wg);
  if (wb > 0.001f) a = min(a, tb / wb);
  if (a > 1e8f) a = 0.0f;
  float r = max(0.0f, tr - a * wr);
  float g = max(0.0f, tg - a * wg);
  float b = max(0.0f, tb - a * wb);
  float m = max(max(a, r), max(g, b));
  if (m <= 0.0f) m = 1.0f;
  out[0] = r / m; out[1] = g / m; out[2] = b / m; out[3] = a / m;
}

// Target colour temperature -> R, G, B, W1, W2 for the selected strip type
void kelvinToOut(float k, float out[CH]) {
  for (int c = 0; c < CH; c++) out[c] = 0;
  if (cfg.stripType == 0) { kelvinToRgbw(k, out); return; }
  float w = cfg.warmK, c = cfg.coolK;
  if (k >= w) {
    // between the two whites: blend in mireds (how the eye sees colour temperature), constant total light
    float kk = min(k, c);
    float t = (1e6f / w - 1e6f / kk) / (1e6f / w - 1e6f / c);
    t = constrain(t, 0.0f, 1.0f);
    out[3] = 1.0f - t; out[4] = t;
  } else {
    // warmer than the warm white (late evening): keep warm white, blend in RGB of the target colour
    float f = constrain((w - k) / max(1.0f, w - 1800.0f), 0.0f, 1.0f);
    float r, g, b; kelvinToRgb(k, r, g, b);
    out[3] = 1.0f - 0.55f * f;
    out[0] = r * 0.55f * f; out[1] = g * 0.55f * f; out[2] = b * 0.55f * f;
  }
}

float smooth(float f) { return f * f * (3.0f - 2.0f * f); }

float circadianK(float m) {
  const int n = 5;
  for (int i = 0; i < n; i++) {
    float t0 = cfg.curveMin[i];
    float t1 = cfg.curveMin[(i + 1) % n];
    float mm = m;
    if (i == n - 1) { t1 += 1440.0f; if (mm < t0) mm += 1440.0f; }
    if (mm >= t0 && mm < t1 && t1 > t0) {
      float f = smooth((mm - t0) / (t1 - t0));
      return cfg.curveK[i] + (cfg.curveK[(i + 1) % n] - (float)cfg.curveK[i]) * f;
    }
  }
  return cfg.curveK[0];
}

float briLevel(float pct) {
  if (pct <= 0.0f) return 0.0f;
  return 0.006f + 0.994f * powf(pct / 100.0f, 2.2f);
}

bool localNow(float &minOfDay, int &wday) {
  time_t t = time(nullptr);
  if (t < 1700000000) return false;
  struct tm tm;
  localtime_r(&t, &tm);
  minOfDay = tm.tm_hour * 60 + tm.tm_min + tm.tm_sec / 60.0f;
  wday = tm.tm_wday;
  return true;
}

bool workday(int wday) { return !(cfg.weekdaysOnly && (wday == 0 || wday == 6)); }

bool inWorkHours(float m, int wday) {
  return workday(wday) && cfg.progEnd > cfg.progStart && m >= cfg.progStart && m < cfg.progEnd;
}

// The schedule's window opens fadeMin early so the lamp reaches full brightness right at work start
bool inSchedule(float m, int wday) {
  return workday(wday) && cfg.progEnd > cfg.progStart && m >= (float)cfg.progStart - cfg.fadeMin && m < cfg.progEnd;
}

// 0..1 brightness multiplier: fade up in the minute(s) before work, fade down in the last minute(s)
float scheduleFade(float m, int wday, bool fadeIn) {
  if (!cfg.autoSchedule || !cfg.fadeMin || !workday(wday)) return 1.0f;
  float s = cfg.progStart, e = cfg.progEnd, f = cfg.fadeMin;
  if (fadeIn && m >= s - f && m < s) return constrain((m - (s - f)) / f, 0.0f, 1.0f);
  if (m >= e - f && m < e) return constrain((e - m) / f, 0.0f, 1.0f);
  return 1.0f;
}

// Turning the lamp on or off by hand (knob or dashboard)
void setPower(bool on) {
  powerOn = on;
  offAtMs = 0;
  schedOn = false;
  markDirty();
  if (!on || !cfg.autoSchedule) return;
  float m; int wd;
  if (!localNow(m, wd)) return;
  if (inSchedule(m, wd)) return;                                // work hours: schedule turns it off at the end
  if (workday(wd) && m < cfg.progStart && m >= (int)cfg.progStart - cfg.earlyHrs * 60) return;   // early start: on through the day
  if (cfg.afterHoursMin) offAtMs = (millis() + cfg.afterHoursMin * 60000UL) | 1;
}

// Automatic on at work start, off at work end, and the after-hours timer
void handleSchedule() {
  static uint32_t lastCheck = 0;
  if (millis() - lastCheck < 1000) return;
  lastCheck = millis();

  if (offAtMs && (int32_t)(millis() - offAtMs) >= 0) { offAtMs = 0; powerOn = false; markDirty(); }
  if (!cfg.autoSchedule) { lastInWork = -1; return; }

  float m; int wd;
  if (!localNow(m, wd)) return;
  int8_t in = inSchedule(m, wd) ? 1 : 0;
  if (lastInWork == -1) {
    lastInWork = in;
    if (!scheduleBooted) { scheduleBooted = true; powerOn = in; schedOn = in; offAtMs = 0; }   // after power-up, match the schedule
    return;
  }
  if (in != lastInWork) {
    lastInWork = in;
    offAtMs = 0;
    powerOn = in;
    schedOn = in;
    markDirty();
  }
}

// Fills target[i][0..3] for every pixel
void computeTarget(float target[][CH]) {
  uint16_t n = cfg.pixels;
  uint32_t now = millis();

  // 0) firmware update: soft blue progress bar
  if (updating) {
    float lit = updProgress * n;
    for (int i = 0; i < n; i++) {
      float a = (i < lit) ? 0.25f : 0.02f;
      target[i][0] = 0; target[i][1] = a * 0.35f; target[i][2] = a; target[i][3] = 0; target[i][4] = 0;
    }
    return;
  }

  // 1) colour-order test: R, G, B, W (RGBW) or R, G, B, warm, cool (RGBCCT), one second each
  if (identifyAt) {
    uint32_t e = now - identifyAt;
    uint32_t steps = cfg.stripType == 1 ? 5 : 4;
    if (e >= steps * 1000) identifyAt = 0;
    else {
      int ch = e / 1000;
      for (int i = 0; i < n; i++)
        for (int c = 0; c < CH; c++) target[i][c] = (c == ch) ? 0.35f : 0.0f;
      return;
    }
  }

  // time of day (live, or the dashboard's preview time)
  if (previewMin >= 0 && (int32_t)(now - previewUntil) >= 0) previewMin = -1;
  float m; int wday;
  bool tv = localNow(m, wday);
  bool on = powerOn, fromSched = schedOn;
  if (previewMin >= 0) {
    m = previewMin; wday = 3; tv = true;                              // previews always show a weekday
    if (cfg.autoSchedule) { on = inSchedule(m, wday); fromSched = true; }   // show what the schedule would do
  }

  // 2) the light itself (or dark)
  if (!on) {
    for (int i = 0; i < n; i++) for (int c = 0; c < CH; c++) target[i][c] = 0;
  } else {
    // no clock yet (never connected): plain warm white rather than a guessed time of day
    float k = (mode == 2) ? manualK : (tv ? circadianK(m) : (cfg.stripType == 1 ? (float)cfg.warmK : 3000.0f));
    float col[CH];
    kelvinToOut(k, col);
    float fade = tv ? scheduleFade(m, wday, fromSched) : 1.0f;
    float lv = briLevel(bri * fade) * cfg.maxBri / 100.0f;

    float lit = n;   // number of fully lit pixels (float)
    if (mode == 1 && tv && inSchedule(m, wday)) {
      float f = (m - cfg.progStart) / (float)(cfg.progEnd - cfg.progStart);
      lit = constrain(f, 0.0f, 1.0f) * n;
    }
    float glow = cfg.glow / 100.0f;

    for (int i = 0; i < n; i++) {
      float amt;
      if (i + 1 <= lit)      amt = 1.0f;
      else if (i < lit)      amt = glow + (1.0f - glow) * (lit - i);   // leading pixel fades in
      else                   amt = glow;
      int p = cfg.reverse ? (n - 1 - i) : i;
      for (int c = 0; c < CH; c++) target[p][c] = col[c] * lv * amt;
    }
  }

  // 3) Teams busy: red on the ends (or the whole strip), breathing gently
  if (teamsTest && (int32_t)(now - testUntil) >= 0) teamsTest = false;
  bool red = teamsRed || teamsTest || manualBusy;
  if (red && (powerOn || cfg.redWhenOff)) {
    float lv = briLevel(cfg.redBri) * cfg.maxBri / 100.0f;
    if (cfg.redPulse) {
      // slow breath between redPulseMin % and 100 %
      float wave = 0.5f - 0.5f * cosf(now * (2.0f * PI / (cfg.redPulseSec * 1000.0f)));
      float lo = cfg.redPulseMin / 100.0f;
      lv *= lo + (1.0f - lo) * wave;
    }
    int e = n;
    if (cfg.redStyle == 0) e = max(1, (int)lroundf(n * cfg.redEndPct / 100.0f));
    for (int i = 0; i < n; i++) {
      if (i < e || i >= n - e) { target[i][0] = lv; target[i][1] = 0; target[i][2] = 0; target[i][3] = 0; target[i][4] = 0; }
    }
  }
}

void renderFrame() {
  static float target[MAX_PIXELS][CH];
  computeTarget(target);
  uint16_t n = cfg.pixels;
  bool changed = false;
  for (int i = 0; i < n; i++) {
    uint8_t v[CH + 1];
    for (int c = 0; c < CH; c++) {
      cur[i][c] += (target[i][c] - cur[i][c]) * 0.18f;
      if (fabsf(target[i][c] - cur[i][c]) < 0.0005f) cur[i][c] = target[i][c];
      v[c] = (uint8_t)constrain(lroundf(cur[i][c] * 255.0f), 0L, 255L);
      if (lastOut[i * CH + c] != v[c]) { lastOut[i * CH + c] = v[c]; changed = true; }
    }
    if (cfg.stripType == 1) {
      v[CH] = 0;                                         // slot 9 below = an unused channel
      const uint8_t *ro = RGB_ORDERS[cfg.rgbOrder], *co = CCT_ORDERS[cfg.cctOrder];
      auto pick = [&](uint8_t idx) -> uint8_t { return idx == 9 ? 0 : v[idx]; };
      uint16_t rgbIc = i * 2 + (cfg.cctFirst ? 1 : 0), cctIc = i * 2 + (cfg.cctFirst ? 0 : 1);
      strip.setPixelColor(rgbIc, v[ro[0]], v[ro[1]], v[ro[2]]);
      strip.setPixelColor(cctIc, pick(co[0]), pick(co[1]), pick(co[2]));
    } else {
      strip.setPixelColor(i, v[0], v[1], v[2], v[3]);
    }
  }
  // also resend the frame every half second, so a strip that just got power (or a glitch on the
  // data line) is corrected right away instead of showing its bright power-up default
  static uint32_t lastSend = 0;
  if (changed || millis() - lastSend > 500) { stripShow(); lastSend = millis(); }
}

// ---------------------------------------------------------------------------
// Teams presence
// ---------------------------------------------------------------------------
void applyPresence(const String &avail, const String &act) {
  presAvail = avail; presAct = act;
  presAtMs = millis(); presSeen = true;
  String a = avail; a.toLowerCase();
  String c = act;   c.toLowerCase();
  bool call    = c.indexOf("inacall") >= 0 || c.indexOf("inaconferencecall") >= 0 || c.indexOf("presenting") >= 0;
  bool meeting = c.indexOf("inameeting") >= 0;
  bool redUi   = a.indexOf("busy") >= 0 || a.indexOf("donotdisturb") >= 0 || a.indexOf("presenting") >= 0;
  if (cfg.redTrigger == 0)      teamsRed = call;
  else if (cfg.redTrigger == 1) teamsRed = call || meeting;
  else                          teamsRed = call || meeting || redUi;
  Serial.printf("Presence: %s / %s -> %s\n", avail.c_str(), act.c_str(), teamsRed ? "RED" : "normal");
}

String jsonField(const String &s, const char *key) {
  String k = String("\"") + key + "\"";
  int i = s.indexOf(k);
  if (i < 0) return "";
  i = s.indexOf(':', i + k.length());
  if (i < 0) return "";
  i = s.indexOf('"', i);
  if (i < 0) return "";
  int j = s.indexOf('"', i + 1);
  if (j < 0) return "";
  return s.substring(i + 1, j);
}

// ---------------------------------------------------------------------------
// Controls
// ---------------------------------------------------------------------------
void handleTurn(int d) {
  static uint32_t lastTurn = 0;
  uint32_t now = millis();
  int step = (now - lastTurn < 80) ? 5 : 2;
  lastTurn = now;
  if (!powerOn) { if (d > 0) { bri = max<uint8_t>(bri, 5); setPower(true); } return; }
  bri = constrain((int)bri + d * step, 1, 100);
  markDirty();
}

void shortPress() {
  if (teamsTest) { teamsTest = false; return; }
  setPower(!powerOn);
}

void doublePress() {
  teamsTest = false;
  manualBusy = !manualBusy;
  manualBusyAt = millis();
}

void longPress() {
  mode = (mode == 1) ? 0 : 1;
  if (!powerOn) setPower(true);
  markDirty();
}

void handleInputs() {
  // Count clicks. Whenever the knob is sitting in a click (both lines high for a 4-step encoder,
  // or both lines equal for a 2-step one), round to the nearest whole click and re-align, so a
  // missed edge or a half-turn can never leave the count out of step.
  static int32_t used = 0;
  int32_t raw = encRaw;
  int32_t steps = cfg.encSteps;
  int32_t diff = raw - used;
  int a = digitalRead(PIN_ENC_A), b = digitalRead(PIN_ENC_B);
  bool atRest = (steps == 4) ? (a && b) : (a == b);
  int32_t d = 0;
  if (atRest) {
    if (diff) { d = (diff + (diff > 0 ? steps / 2 : -steps / 2)) / steps; used = raw; }
  } else if (diff >= steps * 2 || diff <= -steps * 2) {        // spinning fast past several clicks
    d = diff / steps; used += d * steps;
  }
  if (d != 0) handleTurn(cfg.encReverse ? -d : d);

  // single press waits 350 ms to see whether a second press follows
  static bool last = HIGH, stable = HIGH, longFired = false, waitingSecond = false;
  static uint32_t changeMs = 0, downMs = 0, upMs = 0;
  static uint8_t presses = 0;
  bool r = digitalRead(PIN_BTN);
  // ignore the button until it has read "released" for a moment, so a floating pin
  // (module off the board) or a button held at power-up can't trigger phantom presses
  static bool armed = false;
  static uint32_t highSince = 0;
  if (!armed) {
    if (r == HIGH) { if (!highSince) highSince = millis() | 1; if (millis() - highSince > 200) { armed = true; last = stable = HIGH; } }
    else highSince = 0;
    return;
  }
  if (r != last) { last = r; changeMs = millis(); }
  if (millis() - changeMs > 30 && r != stable) {
    stable = r;
    if (stable == LOW) { downMs = millis(); longFired = false; }
    else if (!longFired) {
      presses++;
      upMs = millis();
      waitingSecond = true;
      if (presses >= 2) { presses = 0; waitingSecond = false; doublePress(); }
    }
  }
  if (stable == LOW && !longFired && millis() - downMs > 700) { longFired = true; presses = 0; waitingSecond = false; longPress(); }
  if (waitingSecond && stable == HIGH && millis() - upMs > 350) { waitingSecond = false; presses = 0; shortPress(); }
}

// ---------------------------------------------------------------------------
// WiFi
// ---------------------------------------------------------------------------
void applyTxPower() {
  if (cfg.lowTx) WiFi.setTxPower(WIFI_POWER_8_5dBm);
}

void startAP() {
  if (apOn) return;
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(apSsid.c_str(), AP_PASSWORD);
  applyTxPower();
  dns.start(53, "*", WiFi.softAPIP());
  apOn = true;
  Serial.printf("Hotspot %s on %s\n", apSsid.c_str(), WiFi.softAPIP().toString().c_str());
}

void stopAP() {
  if (!apOn) return;
  dns.stop();
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_STA);
  applyTxPower();
  apOn = false;
}

void handleWifi() {
  bool up = WiFi.status() == WL_CONNECTED;
  if (up && !staWasUp) {
    staWasUp = true;
    configTzTime(TZ_EASTERN, "pool.ntp.org", "time.nist.gov");
    Serial.printf("WiFi connected: %s\n", WiFi.localIP().toString().c_str());
  }
  if (!up && staWasUp) { staWasUp = false; staDownSince = millis(); }
  if (!up && !apOn && millis() - staDownSince > 25000) startAP();
  if (up && apOn && !cfg.keepAP && WiFi.softAPgetStationNum() == 0 && millis() - staDownSince > 30000) stopAP();
  if (apOn) dns.processNextRequest();
  static uint32_t lastNote = 0;
  if (millis() - lastNote > 30000) {
    lastNote = millis();
    if (apOn) Serial.printf("Hotspot %s is on (%d connected)\n", apSsid.c_str(), WiFi.softAPgetStationNum());
  }
}

// ---------------------------------------------------------------------------
// Web API
// ---------------------------------------------------------------------------
// Request arguments come from the web server, or from a USB serial command line
struct SerialArgs { String names[24]; String vals[24]; int n = 0; };
SerialArgs sargs;
bool   viaSerial = false;
String serialReply;

bool hasA(const String &k) {
  if (!viaSerial) return server.hasArg(k);
  for (int i = 0; i < sargs.n; i++) if (sargs.names[i] == k) return true;
  return false;
}
String argA(const String &k) {
  if (!viaSerial) return server.arg(k);
  for (int i = 0; i < sargs.n; i++) if (sargs.names[i] == k) return sargs.vals[i];
  return "";
}

String jsonEsc(const String &s) {
  String o; o.reserve(s.length() + 4);
  for (size_t i = 0; i < s.length(); i++) {
    char c = s[i];
    if (c == '"' || c == '\\') { o += '\\'; o += c; }
    else if ((uint8_t)c < 0x20) o += ' ';
    else o += c;
  }
  return o;
}

void sendJson(const String &body) {
  if (viaSerial) { serialReply = body; return; }
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.sendHeader("Cache-Control", "no-store");
  server.send(200, "application/json", body);
}

String stateJson() {
  float m = 0; int wday = 0;
  bool tv = localNow(m, wday);
  String j; j.reserve(1400);
  j += "{\"v\":\"" FW_VERSION "\"";
  j += ",\"via\":\"" + String(viaSerial ? "usb" : "wifi") + "\"";
  j += ",\"up\":" + String((unsigned long)(millis() / 1000));
  j += ",\"id\":\"" + jsonEsc(apSsid.substring(10, apSsid.length())) + "\"";
  j += ",\"on\":" + String(powerOn ? "true" : "false");
  j += ",\"bri\":" + String(bri);
  j += ",\"mode\":" + String(mode);
  j += ",\"manualK\":" + String(manualK);
  j += ",\"teams\":{\"red\":" + String(teamsRed ? "true" : "false");
  j += ",\"test\":" + String(teamsTest ? "true" : "false");
  j += ",\"manual\":" + String(manualBusy ? "true" : "false");
  j += ",\"seen\":" + String(presSeen ? "true" : "false");
  j += ",\"avail\":\"" + jsonEsc(presAvail) + "\",\"act\":\"" + jsonEsc(presAct) + "\"";
  j += ",\"ago\":" + String(presSeen ? (long)((millis() - presAtMs) / 1000) : -1L) + "}";
  j += ",\"time\":{\"valid\":" + String(tv ? "true" : "false");
  j += ",\"min\":" + String(m, 2) + ",\"wday\":" + String(wday) + "}";
  j += ",\"inWork\":" + String(tv && inWorkHours(m, wday) ? "true" : "false");
  j += ",\"schedOn\":" + String(schedOn ? "true" : "false");
  j += ",\"offIn\":" + String(offAtMs ? (long)((int32_t)(offAtMs - millis()) / 1000) : -1L);
  j += ",\"preview\":" + String(previewMin);
  j += ",\"net\":{\"sta\":" + String(WiFi.status() == WL_CONNECTED ? "true" : "false");
  j += ",\"ssid\":\"" + jsonEsc(wifiSsid) + "\"";
  j += ",\"ip\":\"" + WiFi.localIP().toString() + "\"";
  j += ",\"rssi\":" + String(WiFi.status() == WL_CONNECTED ? WiFi.RSSI() : 0);
  j += ",\"ap\":" + String(apOn ? "true" : "false");
  j += ",\"apSsid\":\"" + jsonEsc(apSsid) + "\",\"apIp\":\"" + WiFi.softAPIP().toString() + "\"}";
  j += ",\"cfg\":{\"pixels\":" + String(cfg.pixels);
  j += ",\"stripType\":" + String(cfg.stripType);
  j += ",\"rgbOrder\":" + String(cfg.rgbOrder);
  j += ",\"cctOrder\":" + String(cfg.cctOrder);
  j += ",\"cctFirst\":" + String(cfg.cctFirst ? "true" : "false");
  j += ",\"warmK\":" + String(cfg.warmK);
  j += ",\"coolK\":" + String(cfg.coolK);
  j += ",\"order\":" + String(cfg.order);
  j += ",\"reverse\":" + String(cfg.reverse ? "true" : "false");
  j += ",\"whiteK\":" + String(cfg.whiteK);
  j += ",\"maxBri\":" + String(cfg.maxBri);
  j += ",\"progStart\":" + String(cfg.progStart);
  j += ",\"progEnd\":" + String(cfg.progEnd);
  j += ",\"weekdaysOnly\":" + String(cfg.weekdaysOnly ? "true" : "false");
  j += ",\"autoSchedule\":" + String(cfg.autoSchedule ? "true" : "false");
  j += ",\"afterHoursMin\":" + String(cfg.afterHoursMin);
  j += ",\"earlyHrs\":" + String(cfg.earlyHrs);
  j += ",\"fadeMin\":" + String(cfg.fadeMin);
  j += ",\"glow\":" + String(cfg.glow);
  j += ",\"curveMin\":[";
  for (int i = 0; i < 5; i++) { if (i) j += ","; j += String(cfg.curveMin[i]); }
  j += "],\"curveK\":[";
  for (int i = 0; i < 5; i++) { if (i) j += ","; j += String(cfg.curveK[i]); }
  j += "],\"redTrigger\":" + String(cfg.redTrigger);
  j += ",\"redBri\":" + String(cfg.redBri);
  j += ",\"redStyle\":" + String(cfg.redStyle);
  j += ",\"redEndPct\":" + String(cfg.redEndPct);
  j += ",\"redPulse\":" + String(cfg.redPulse ? "true" : "false");
  j += ",\"redPulseMin\":" + String(cfg.redPulseMin);
  j += ",\"redPulseSec\":" + String(cfg.redPulseSec);
  j += ",\"redWhenOff\":" + String(cfg.redWhenOff ? "true" : "false");
  j += ",\"autoClearHrs\":" + String(cfg.autoClearHrs);
  j += ",\"encSteps\":" + String(cfg.encSteps);
  j += ",\"encReverse\":" + String(cfg.encReverse ? "true" : "false");
  j += ",\"lowTx\":" + String(cfg.lowTx ? "true" : "false");
  j += ",\"keepAP\":" + String(cfg.keepAP ? "true" : "false");
  j += ",\"wifiOff\":" + String(cfg.wifiOff ? "true" : "false");
  j += "}}";
  return j;
}

bool argBool(const char *name) {
  String v = argA(name);
  return v == "1" || v == "true" || v == "on";
}

void handleSet() {
  if (hasA("on"))   setPower(argBool("on"));
  if (hasA("bri"))  { bri = constrain(argA("bri").toInt(), 1, 100); markDirty(); }
  if (hasA("mode")) { mode = constrain(argA("mode").toInt(), 0, 2); markDirty(); }
  if (hasA("k"))    { manualK = constrain(argA("k").toInt(), 1800, 7500); markDirty(); }
  if (hasA("test")) {
    teamsTest = argBool("test");
    testUntil = millis() + 5UL * 60UL * 1000UL;   // a test call auto-ends after 5 minutes
  }
  if (hasA("busy")) { manualBusy = argBool("busy"); manualBusyAt = millis(); teamsTest = false; }
  if (hasA("clearTeams")) { teamsRed = false; teamsTest = false; manualBusy = false; }
  if (hasA("preview")) {
    previewMin = argA("preview").toInt();
    if (previewMin >= 1440) previewMin = 1439;
    previewUntil = millis() + 60000;               // falls back to live after a minute idle
  }
  if (hasA("identify")) identifyAt = millis() | 1;
  sendJson(stateJson());
}

void handleSettings() {
  auto u16 = [](const char *n, uint16_t &dst, int lo, int hi) { if (hasA(n)) dst = constrain(argA(n).toInt(), lo, hi); };
  auto u8  = [](const char *n, uint8_t &dst, int lo, int hi)  { if (hasA(n)) dst = constrain(argA(n).toInt(), lo, hi); };
  auto bl  = [](const char *n, bool &dst) { if (hasA(n)) dst = argBool(n); };

  u16("pixels", cfg.pixels, 1, MAX_PIXELS);
  u8("order", cfg.order, 0, ORDER_COUNT - 1);
  u8("stripType", cfg.stripType, 0, 1);
  u8("rgbOrder", cfg.rgbOrder, 0, RGB_ORDER_COUNT - 1);
  u8("cctOrder", cfg.cctOrder, 0, CCT_ORDER_COUNT - 1);
  bl("cctFirst", cfg.cctFirst);
  u16("warmK", cfg.warmK, 1800, 4500);
  u16("coolK", cfg.coolK, 4500, 8000);
  if (cfg.coolK <= cfg.warmK + 200) cfg.coolK = cfg.warmK + 200;
  bl("reverse", cfg.reverse);
  u16("whiteK", cfg.whiteK, 2000, 7000);
  u8("maxBri", cfg.maxBri, 10, 100);
  u16("progStart", cfg.progStart, 0, 1439);
  u16("progEnd", cfg.progEnd, 1, 1440);
  bl("weekdaysOnly", cfg.weekdaysOnly);
  bool oldAuto = cfg.autoSchedule;
  bl("autoSchedule", cfg.autoSchedule);
  u16("afterHoursMin", cfg.afterHoursMin, 0, 720);
  u8("earlyHrs", cfg.earlyHrs, 0, 12);
  u8("fadeMin", cfg.fadeMin, 0, 30);
  if (cfg.autoSchedule != oldAuto) lastInWork = -1;   // resync quietly, no sudden on/off
  u8("glow", cfg.glow, 0, 60);
  for (int i = 0; i < 5; i++) {
    String a = "cm" + String(i), b = "ck" + String(i);
    if (hasA(a)) cfg.curveMin[i] = constrain(argA(a).toInt(), 0, 1439);
    if (hasA(b)) cfg.curveK[i] = constrain(argA(b).toInt(), 1800, 7500);
  }
  // keep curve points in time order
  for (int i = 0; i < 5; i++) for (int j = 0; j < 4 - i; j++)
    if (cfg.curveMin[j] > cfg.curveMin[j + 1]) {
      uint16_t t = cfg.curveMin[j]; cfg.curveMin[j] = cfg.curveMin[j + 1]; cfg.curveMin[j + 1] = t;
      t = cfg.curveK[j]; cfg.curveK[j] = cfg.curveK[j + 1]; cfg.curveK[j + 1] = t;
    }
  u8("redTrigger", cfg.redTrigger, 0, 2);
  u8("redBri", cfg.redBri, 5, 100);
  u8("redStyle", cfg.redStyle, 0, 1);
  u8("redEndPct", cfg.redEndPct, 1, 25);
  bl("redPulse", cfg.redPulse);
  u8("redPulseMin", cfg.redPulseMin, 0, 90);
  u8("redPulseSec", cfg.redPulseSec, 1, 20);
  bl("redWhenOff", cfg.redWhenOff);
  u8("autoClearHrs", cfg.autoClearHrs, 0, 24);
  if (hasA("encSteps")) cfg.encSteps = argA("encSteps").toInt() == 2 ? 2 : 4;
  bl("encReverse", cfg.encReverse);
  bool oldTx = cfg.lowTx;
  bl("lowTx", cfg.lowTx);
  bl("keepAP", cfg.keepAP);
  bool oldOff = cfg.wifiOff;
  bl("wifiOff", cfg.wifiOff);

  saveCfg();
  applyStripConfig();
  if (cfg.lowTx != oldTx) { if (cfg.lowTx) applyTxPower(); else WiFi.setTxPower(WIFI_POWER_19_5dBm); }
  if (cfg.keepAP && !apOn && !cfg.wifiOff) startAP();
  if (cfg.wifiOff != oldOff) rebootAt = millis() + 1500;   // WiFi on/off takes effect after a restart
  sendJson(stateJson());
}

void handlePresence() {
  String body = argA("plain");
  String a = hasA("availability") ? argA("availability") : jsonField(body, "availability");
  String c = hasA("activity") ? argA("activity") : jsonField(body, "activity");
  if (a.length() == 0 && c.length() == 0 && body.length() > 0 && body.length() < 60) a = body;   // plain-text body
  applyPresence(a, c);
  sendJson(String("{\"ok\":true,\"red\":") + (teamsRed ? "true" : "false") + "}");
}

void handleTime() {
  // the dashboard sends the browser's clock so the lamp works without internet
  long epoch = argA("epoch").toInt();
  if (epoch > 1700000000L && (time(nullptr) < 1700000000 || argBool("force"))) {
    struct timeval tv = {epoch, 0};
    settimeofday(&tv, nullptr);
  }
  sendJson(stateJson());
}

void handleWifiSave() {
  wifiSsid = argA("ssid");
  wifiPass = argA("pass");
  prefs.begin("lamp", false);
  prefs.putString("ssid", wifiSsid);
  prefs.putString("pass", wifiPass);
  prefs.end();
  sendJson("{\"ok\":true,\"rebooting\":true}");
  rebootAt = millis() + 1500;
}

void handleScan() {
  int n = WiFi.scanComplete();
  if (n == WIFI_SCAN_FAILED || (n < 0 && n != WIFI_SCAN_RUNNING)) {
    WiFi.scanNetworks(true);
    sendJson("{\"scanning\":true}");
    return;
  }
  if (n == WIFI_SCAN_RUNNING) { sendJson("{\"scanning\":true}"); return; }
  String j = "{\"scanning\":false,\"nets\":[";
  for (int i = 0; i < n; i++) {
    if (i) j += ",";
    j += "{\"ssid\":\"" + jsonEsc(WiFi.SSID(i)) + "\",\"rssi\":" + String(WiFi.RSSI(i)) +
         ",\"open\":" + String(WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? "true" : "false") + "}";
  }
  j += "]}";
  WiFi.scanDelete();
  sendJson(j);
}

// Firmware upload from the dashboard (multipart form, field "firmware")
void handleUpdateDone() {
  bool ok = !Update.hasError();
  server.sendHeader("Connection", "close");
  if (ok) {
    sendJson("{\"ok\":true}");
    rebootAt = millis() + 1500;
  } else {
    updating = false;
    sendJson(String("{\"ok\":false,\"error\":\"") + jsonEsc(Update.errorString()) + "\"}");
  }
}

void handleUpdateChunk() {
  HTTPUpload &up = server.upload();
  static size_t total = 0;
  static uint32_t lastDraw = 0;
  if (up.status == UPLOAD_FILE_START) {
    Serial.printf("Update: %s\n", up.filename.c_str());
    total = server.clientContentLength();
    updating = true; updProgress = 0;
    if (!Update.begin(UPDATE_SIZE_UNKNOWN)) Update.printError(Serial);
  } else if (up.status == UPLOAD_FILE_WRITE) {
    if (Update.isRunning() && Update.write(up.buf, up.currentSize) != up.currentSize) Update.printError(Serial);
    if (total) updProgress = constrain((float)up.totalSize / total, 0.0f, 1.0f);
    if (millis() - lastDraw > 60) { lastDraw = millis(); renderFrame(); }   // keep the strip moving while the loop is busy
  } else if (up.status == UPLOAD_FILE_END) {
    if (Update.end(true)) { updProgress = 1; Serial.printf("Update OK: %u bytes\n", up.totalSize); }
    else Update.printError(Serial);
  } else if (up.status == UPLOAD_FILE_ABORTED) {
    Update.abort();
    updating = false;
  }
}

void setupArduinoOTA() {
  ArduinoOTA.setHostname(HOSTNAME);
  ArduinoOTA.setPassword(AP_PASSWORD);
  ArduinoOTA.setMdnsEnabled(false);          // mDNS is already running; we advertise the OTA port below
  ArduinoOTA.onStart([]() { updating = true; updProgress = 0; });
  ArduinoOTA.onProgress([](unsigned int done, unsigned int total) {
    static uint32_t lastDraw = 0;
    if (total) updProgress = (float)done / total;
    if (millis() - lastDraw > 60) { lastDraw = millis(); renderFrame(); }
  });
  ArduinoOTA.onError([](ota_error_t e) { updating = false; });
  ArduinoOTA.begin();
  MDNS.enableArduino(3232, true);
}

// Small live readout for the dashboard's knob test
String knobJson() {
  return String("{\"a\":") + digitalRead(PIN_ENC_A) + ",\"b\":" + digitalRead(PIN_ENC_B) +
         ",\"btn\":" + digitalRead(PIN_BTN) + ",\"edges\":" + (uint32_t)encEdges +
         ",\"count\":" + (int32_t)encRaw + ",\"bri\":" + bri + "}";
}

void handleRoot() {
  server.sendHeader("Cache-Control", "no-store");
  server.send_P(200, "text/html", DASHBOARD_HTML);
}

void handleNotFound() {
  if (server.method() == HTTP_OPTIONS) {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET,POST,OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(204);
    return;
  }
  // captive portal: phones/laptops joining the hotspot get sent to the dashboard
  if (apOn) {
    server.sendHeader("Location", String("http://") + WiFi.softAPIP().toString() + "/", true);
    server.send(302, "text/plain", "");
    return;
  }
  server.send(404, "text/plain", "Not found");
}

void setupServer() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/api/state", HTTP_GET, []() { sendJson(stateJson()); });
  server.on("/api/set", handleSet);
  server.on("/api/settings", handleSettings);
  server.on("/api/presence", handlePresence);
  server.on("/api/time", handleTime);
  server.on("/api/wifi", HTTP_POST, handleWifiSave);
  server.on("/api/scan", HTTP_GET, handleScan);
  server.on("/api/update", HTTP_POST, handleUpdateDone, handleUpdateChunk);
  server.on("/api/knob", HTTP_GET, []() { sendJson(knobJson()); });
  server.on("/api/reboot", HTTP_POST, []() { sendJson("{\"ok\":true}"); rebootAt = millis() + 800; });
  server.onNotFound(handleNotFound);
  server.begin();
}

// ---------------------------------------------------------------------------
// USB serial commands (used by the PC bridge)
// ---------------------------------------------------------------------------
String urlDecode(const String &s) {
  String o; o.reserve(s.length());
  for (size_t i = 0; i < s.length(); i++) {
    char c = s[i];
    if (c == '+') o += ' ';
    else if (c == '%' && i + 2 < s.length()) {
      char h[3] = {s[i + 1], s[i + 2], 0};
      o += (char)strtol(h, nullptr, 16);
      i += 2;
    } else o += c;
  }
  return o;
}

void handleSerialLine(String line) {
  line.trim();
  if (!line.length()) return;
  if (line == "ping") { Serial.println("pong RombyLamp " FW_VERSION); return; }
  int q = line.indexOf('?');
  String path  = q >= 0 ? line.substring(0, q) : line;
  String query = q >= 0 ? line.substring(q + 1, line.length()) : String("");
  sargs.n = 0;
  int start = 0;
  while (start < (int)query.length() && sargs.n < 24) {
    int amp = query.indexOf('&', start);
    if (amp < 0) amp = query.length();
    String pair = query.substring(start, amp);
    int eq = pair.indexOf('=');
    sargs.names[sargs.n] = urlDecode(eq >= 0 ? pair.substring(0, eq) : pair);
    sargs.vals[sargs.n]  = eq >= 0 ? urlDecode(pair.substring(eq + 1, pair.length())) : String("");
    sargs.n++;
    start = amp + 1;
  }
  viaSerial = true;
  serialReply = "{\"error\":\"unknown command\"}";
  if (path == "/api/state")         sendJson(stateJson());
  else if (path == "/api/set")      handleSet();
  else if (path == "/api/settings") handleSettings();
  else if (path == "/api/presence") handlePresence();
  else if (path == "/api/time")     handleTime();
  else if (path == "/api/wifi")     handleWifiSave();
  else if (path == "/api/scan")     handleScan();
  else if (path == "/api/knob")     sendJson(knobJson());
  else if (path == "/api/reboot")   { sendJson("{\"ok\":true}"); rebootAt = millis() + 800; }
  viaSerial = false;
  Serial.print('@');
  Serial.println(serialReply);
}

void handleSerial() {
  static String buf;
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\n') { handleSerialLine(buf); buf = ""; }
    else if (c != '\r' && buf.length() < 700) buf += c;
  }
}

// ---------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(50);
  Serial.println("\nRomby Lamp " FW_VERSION);

  setenv("TZ", TZ_EASTERN, 1);
  tzset();
  loadAll();

  memset(cur, 0, sizeof(cur));
  applyStripConfig();
  strip.clear();
  stripShow();

  pinMode(PIN_ENC_PWR, OUTPUT);          // power the knob module's pull-ups before reading it
  digitalWrite(PIN_ENC_PWR, HIGH);
  delay(5);
  pinMode(PIN_ENC_A, INPUT_PULLUP);
  pinMode(PIN_ENC_B, INPUT_PULLUP);
  pinMode(PIN_BTN, INPUT);
  encPrev = (digitalRead(PIN_ENC_A) << 1) | digitalRead(PIN_ENC_B);   // start from where the knob really is              // GPIO37 has no internal pull-up; the Dig-Uno v4 provides one
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_A), encISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_B), encISR, CHANGE);

  uint64_t chip = ESP.getEfuseMac();             // factory ID, works even with WiFi off
  char buf[24];
  snprintf(buf, sizeof(buf), "RombyLamp-%04X", (unsigned)((chip >> 32) & 0xFFFF));
  apSsid = buf;

  if (cfg.wifiOff) {                            // USB-only mode: radio stays off
    WiFi.mode(WIFI_OFF);
    Serial.println("WiFi is off (USB only)");
    return;
  }

  WiFi.persistent(false);
  WiFi.setHostname(HOSTNAME);
  WiFi.setAutoReconnect(true);
  staDownSince = millis();
  if (wifiSsid.length()) {
    WiFi.mode(cfg.keepAP ? WIFI_AP_STA : WIFI_STA);
    WiFi.begin(wifiSsid.c_str(), wifiPass.c_str());
    applyTxPower();
    if (cfg.keepAP) startAP();
  } else {
    startAP();                                  // first boot: setup hotspot only
  }

  MDNS.begin(HOSTNAME);
  MDNS.addService("http", "tcp", 80);
  setupServer();
  setupArduinoOTA();
}

void loop() {
  if (!cfg.wifiOff) {
    server.handleClient();
    handleWifi();
    ArduinoOTA.handle();
  }
  handleSerial();
  handleInputs();
  handleSchedule();

  static uint32_t lastFrame = 0;
  if (millis() - lastFrame >= 20) { lastFrame = millis(); renderFrame(); }

  if (teamsRed && cfg.autoClearHrs && millis() - presAtMs > cfg.autoClearHrs * 3600000UL) teamsRed = false;
  if (manualBusy && cfg.autoClearHrs && millis() - manualBusyAt > cfg.autoClearHrs * 3600000UL) manualBusy = false;
  if (stateDirtyAt && millis() - stateDirtyAt > 4000) { stateDirtyAt = 0; saveState(); }
  if (rebootAt && (int32_t)(millis() - rebootAt) >= 0) ESP.restart();
}
