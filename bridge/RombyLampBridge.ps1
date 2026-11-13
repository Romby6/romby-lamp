<#
  Romby Lamp USB bridge
  ---------------------
  Runs on the work PC. Talks to the lamp over its USB cable so nothing needs WiFi.

  - Finds the lamp on a COM port automatically
  - Opens the lamp dashboard at http://localhost:8765
  - Turns the busy light on while your microphone is in use (Teams, Zoom, phone calls)
  - Keeps the lamp's clock in sync with this PC
  - Accepts PresenceLight's Custom API at http://localhost:8765/api/presence

  Start it with "Start Lamp Bridge.cmd". Close the window (or press Ctrl+C) to stop.
  Close the bridge before flashing the lamp with Arduino IDE: only one program can use the COM port.

  Options (add after the script name in the .cmd file):
    -Port 8765         web port for the dashboard
    -ComPort COM5      skip auto-detect and use this port
    -NoMic             don't watch the microphone (use this if PresenceLight drives Teams status)
    -TeamsOnly         only count the microphone as busy when Teams is the app using it
    -NoBrowser         don't open the dashboard when the bridge starts
#>
param(
  [int]$Port = 8765,
  [string]$ComPort = '',
  [switch]$NoMic,
  [switch]$TeamsOnly,
  [switch]$NoBrowser
)

$ErrorActionPreference = 'Stop'
$Host.UI.RawUI.WindowTitle = 'Romby Lamp bridge'
$here = Split-Path -Parent $MyInvocation.MyCommand.Path
$dashCandidates = @((Join-Path $here 'dashboard.html'), (Join-Path (Split-Path -Parent $here) 'dashboard.html'))
$dashPath = $dashCandidates | Where-Object { Test-Path $_ } | Select-Object -First 1

$script:sp = $null
$script:micBusy = $false
$script:micSeen = 0
$script:lastClock = [datetime]::MinValue
$script:lastFind = [datetime]::MinValue

function Say([string]$msg, [string]$color = 'Gray') {
  Write-Host ("[{0:HH:mm:ss}] {1}" -f (Get-Date), $msg) -ForegroundColor $color
}

# ---------------------------------------------------------------- serial
function Open-Lamp([string]$name) {
  $p = New-Object System.IO.Ports.SerialPort $name, 115200
  $p.NewLine = "`n"
  $p.ReadTimeout = 250
  $p.WriteTimeout = 1000
  $p.DtrEnable = $false
  $p.RtsEnable = $false
  try { $p.Open() } catch { return $null }
  # opening the port can restart some ESP32 boards, so keep asking for up to 5 seconds
  $deadline = (Get-Date).AddSeconds(5)
  while ((Get-Date) -lt $deadline) {
    try { $p.DiscardInBuffer(); $p.WriteLine('ping') } catch { break }
    $until = (Get-Date).AddMilliseconds(700)
    while ((Get-Date) -lt $until) {
      try {
        $line = $p.ReadLine()
        if ($line -like 'pong RombyLamp*') { return @{ port = $p; version = $line.Substring(15).Trim() } }
      } catch [System.TimeoutException] { }
      catch { break }
    }
  }
  try { $p.Close() } catch { }
  return $null
}

function Find-Lamp {
  $names = if ($ComPort) { @($ComPort) } else { [System.IO.Ports.SerialPort]::GetPortNames() | Sort-Object -Unique }
  foreach ($n in $names) {
    $r = Open-Lamp $n
    if ($r) {
      $script:sp = $r.port
      Say "Lamp found on $n (firmware $($r.version))" 'Green'
      $script:lastClock = [datetime]::MinValue
      return $true
    }
  }
  return $false
}

function Send-Lamp([string]$line) {
  if (-not $script:sp) { throw 'not connected' }
  try {
    $script:sp.DiscardInBuffer()
    $script:sp.WriteLine($line)
    $deadline = (Get-Date).AddSeconds(4)
    while ((Get-Date) -lt $deadline) {
      try {
        $l = $script:sp.ReadLine()
        if ($l.StartsWith('@')) { return $l.Substring(1).Trim() }
      } catch [System.TimeoutException] { }
    }
    throw 'timeout'
  } catch {
    Say "Lost the lamp ($($_.Exception.Message)). Reconnecting..." 'Yellow'
    try { $script:sp.Close() } catch { }
    $script:sp = $null
    throw
  }
}

# ---------------------------------------------------------------- microphone
$micRoot = 'HKCU:\Software\Microsoft\Windows\CurrentVersion\CapabilityAccessManager\ConsentStore\microphone'
function Test-MicInUse {
  $keys = @()
  try {
    $keys += Get-ChildItem $micRoot -ErrorAction SilentlyContinue | Where-Object { $_.PSChildName -ne 'NonPackaged' }
    $keys += Get-ChildItem "$micRoot\NonPackaged" -ErrorAction SilentlyContinue
  } catch { return $false }
  foreach ($k in $keys) {
    if ($TeamsOnly -and $k.PSChildName -notmatch 'Teams') { continue }
    $p = Get-ItemProperty -Path $k.PSPath -ErrorAction SilentlyContinue
    if ($p -and $p.LastUsedTimeStart -gt 0 -and $p.LastUsedTimeStop -eq 0) { return $true }
  }
  return $false
}

function Update-Mic {
  if ($NoMic) { return }
  $now = $false
  try { $now = [bool](Test-MicInUse) } catch { return }
  if ($now -eq $script:micBusy) { $script:micSeen = 0; return }
  # go busy at once; wait two checks (about 4 s) before clearing, so a brief mic release doesn't flicker
  if (-not $now) { $script:micSeen++; if ($script:micSeen -lt 2) { return } }
  $script:micSeen = 0
  $script:micBusy = $now
  $q = if ($now) { 'availability=Busy&activity=InACall' } else { 'availability=Available&activity=Available' }
  try {
    [void](Send-Lamp "/api/presence?$q")
    if ($now) { Say 'Microphone in use: busy light on' 'Red' } else { Say 'Microphone free: busy light off' 'Gray' }
  } catch { $script:micBusy = -not $now }
}

function Sync-Clock {
  $epoch = [DateTimeOffset]::UtcNow.ToUnixTimeSeconds()
  try { [void](Send-Lamp "/api/time?epoch=$epoch&force=1"); $script:lastClock = Get-Date } catch { }
}

# ---------------------------------------------------------------- web
function Send-Response($ctx, [int]$code, [string]$type, [string]$text) {
  $bytes = [System.Text.Encoding]::UTF8.GetBytes($text)
  $res = $ctx.Response
  $res.StatusCode = $code
  $res.ContentType = $type
  $res.Headers.Add('Access-Control-Allow-Origin', '*')
  $res.Headers.Add('Cache-Control', 'no-store')
  $res.ContentLength64 = $bytes.Length
  $res.OutputStream.Write($bytes, 0, $bytes.Length)
  $res.OutputStream.Close()
}

function Get-Field([string]$json, [string]$key) {
  $m = [regex]::Match($json, '"' + $key + '"\s*:\s*"([^"]*)"')
  if ($m.Success) { return $m.Groups[1].Value } else { return '' }
}

function Handle-Request($ctx) {
  $req = $ctx.Request
  $path = $req.Url.AbsolutePath
  if ($req.HttpMethod -eq 'OPTIONS') {
    $ctx.Response.Headers.Add('Access-Control-Allow-Methods', 'GET,POST,OPTIONS')
    $ctx.Response.Headers.Add('Access-Control-Allow-Headers', 'Content-Type')
    Send-Response $ctx 204 'text/plain' ''
    return
  }
  if ($path -eq '/' -or $path -eq '/index.html') {
    if ($dashPath) { Send-Response $ctx 200 'text/html; charset=utf-8' ([IO.File]::ReadAllText($dashPath)) }
    else { Send-Response $ctx 404 'text/plain' 'dashboard.html is missing. Keep it in the same folder as the bridge.' }
    return
  }
  if (-not $path.StartsWith('/api/')) { Send-Response $ctx 404 'text/plain' 'Not found'; return }
  if ($path -eq '/api/update') {
    Send-Response $ctx 200 'application/json' '{"ok":false,"error":"Over USB, update the lamp from Arduino IDE (close this bridge first)"}'
    return
  }

  $query = $req.Url.Query.TrimStart('?')
  if ($req.HasEntityBody) {
    $reader = New-Object System.IO.StreamReader($req.InputStream, $req.ContentEncoding)
    $body = $reader.ReadToEnd()
    $reader.Close()
    if ($req.ContentType -like 'application/x-www-form-urlencoded*') {
      $query = (@($query, $body) | Where-Object { $_ }) -join '&'
    } elseif ($path -eq '/api/presence') {
      # PresenceLight sends JSON: {"availability":"Busy","activity":"InACall"}
      $a = Get-Field $body 'availability'; $c = Get-Field $body 'activity'
      if (-not $a -and -not $c) { $a = $body.Trim() }
      $query = 'availability=' + [Uri]::EscapeDataString($a) + '&activity=' + [Uri]::EscapeDataString($c)
    }
  }
  $query = $query -replace "[`r`n]", ''

  if (-not $script:sp) {
    Send-Response $ctx 503 'application/json' ('{"error":"Lamp not connected. Check the USB cable.","bridge":{"waiting":true,"port":' + $Port + '}}')
    return
  }
  try {
    $reply = Send-Lamp ($(if ($query) { "$path`?$query" } else { $path }))
    if ($path -eq '/api/state' -and $reply.StartsWith('{')) {
      # tell the dashboard about the bridge
      $mic = if ($NoMic) { 'false' } else { 'true' }
      $busy = if ($script:micBusy) { 'true' } else { 'false' }
      $com = $script:sp.PortName
      $reply = '{"bridge":{"mic":' + $mic + ',"micBusy":' + $busy + ',"port":' + $Port + ',"com":"' + $com + '"},' + $reply.Substring(1)
    }
    Send-Response $ctx 200 'application/json' $reply
  } catch {
    Send-Response $ctx 503 'application/json' '{"error":"Lamp stopped answering. Reconnecting."}'
  }
}

# ---------------------------------------------------------------- main
Write-Host ''
Write-Host '  Romby Lamp bridge' -ForegroundColor White
Write-Host "  Dashboard: http://localhost:$Port" -ForegroundColor White
if ($NoMic) { Write-Host '  Microphone watching: off' } else { Write-Host '  Microphone watching: on (busy light while the mic is in use)' }
Write-Host '  Leave this window open. Close it to stop.' -ForegroundColor DarkGray
Write-Host ''

$listener = New-Object System.Net.HttpListener
$listener.Prefixes.Add("http://localhost:$Port/")
try { $listener.Start() }
catch {
  Say "Could not open port $Port. Another program may be using it. Try: -Port 8766" 'Red'
  Read-Host 'Press Enter to close'
  exit 1
}

if (-not (Find-Lamp)) { Say 'Looking for the lamp on USB... (plug it in, or close Arduino IDE''s Serial Monitor)' 'Yellow' }
$script:lastFind = Get-Date
if (-not $NoBrowser) { Start-Process "http://localhost:$Port" }

$lastMic = Get-Date
$task = $listener.GetContextAsync()
try {
  while ($listener.IsListening) {
    if ($task.Wait(200)) {
      $ctx = $task.Result
      $task = $listener.GetContextAsync()
      try { Handle-Request $ctx } catch { try { Send-Response $ctx 500 'text/plain' 'Bridge error' } catch { } }
    }
    $now = Get-Date
    if (-not $script:sp -and ($now - $script:lastFind).TotalSeconds -ge 5) {
      $script:lastFind = $now
      [void](Find-Lamp)
    }
    if ($script:sp) {
      try {
        if (($now - $script:lastClock).TotalMinutes -ge 30) { Sync-Clock }
        if (($now - $lastMic).TotalSeconds -ge 2) { $lastMic = $now; Update-Mic }
      } catch { Say "Background check failed: $($_.Exception.Message)" 'DarkGray' }
    }
  }
} finally {
  if ($script:sp) {
    if ($script:micBusy) { try { [void](Send-Lamp '/api/presence?availability=Available&activity=Available') } catch { } }
    try { $script:sp.Close() } catch { }
  }
  $listener.Stop()
}
