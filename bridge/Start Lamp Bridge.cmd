@echo off
rem Starts the Romby Lamp USB bridge. Leave the window open; close it to stop.
rem Add options at the end of the next line, e.g.  -NoMic   or   -TeamsOnly   or   -ComPort COM5
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0RombyLampBridge.ps1"
if errorlevel 1 pause
