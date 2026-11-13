# Romby Lamp

Circadian desk lamp for the QuinLED Dig-Uno v4 (QuinLED-ESP32) and a WS2811 dual-IC RGBCCT strip:
circadian white through the workday, a workday progress bar, and a busy light for Teams calls.

**Dashboard:** https://romby6.github.io/romby-lamp/ (Chrome or Edge). Plug the lamp in by USB, click **Connect USB**.
When a newer firmware is published here, the dashboard shows **Update lamp** and installs it over the same cable
(or over WiFi), keeping the lamp's settings.

| Folder | Contents |
|---|---|
| `index.html` | The dashboard (also built into the firmware) |
| `firmware/` | Compiled builds. `latest.json` tells the dashboard which one is newest. `-update.bin` goes at 0x10000 and keeps settings; `-full.bin` goes at 0x0 for a first install |
| `src/RombyLamp/` | Arduino source (ESP32 Arduino core 3.0.7, Adafruit NeoPixel 1.12.3; board ESP32 Dev Module, Minimal SPIFFS, DIO) |
| `bridge/` | Optional Windows USB bridge (PowerShell) for PresenceLight and microphone-based busy detection |

Designed by Romby with Claude.
