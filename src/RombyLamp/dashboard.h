// Generated from dashboard.html - edit that file, then regenerate.
#pragma once
const char DASHBOARD_HTML[] PROGMEM = R"rawliteral(<!doctype html><html lang="en"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover"><title>Romby Lamp</title>
<meta name="claude:viewport-fit-sides" content="cover">
<link rel="preconnect" href="https://fonts.googleapis.com">
<link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Space+Grotesk:wght@500;600;700&family=Inter:wght@400;500;600&family=JetBrains+Mono:wght@400;500;700&display=swap">
<style>
/* Romby design language: Mission Control Minimal. Black, hairlines, zero radius, red only for live/busy. */
:root {
  --bg: #000;  --bg-card: #030303;  --bg-hover: rgba(255,255,255,0.02);
  --white: #fff;  --gray-1: #a3a3a3;  --gray-2: #5c5c5c;  --gray-3: #2a2a2a;
  --line: rgba(255,255,255,0.11);  --line-strong: rgba(255,255,255,0.28);
  --red: #ff4433;
  --font-display: 'Space Grotesk', 'Arial Narrow', system-ui, sans-serif;
  --font-body: 'Inter', 'Segoe UI', system-ui, -apple-system, sans-serif;
  --font-mono: 'JetBrains Mono', ui-monospace, Consolas, 'SFMono-Regular', monospace;
  color-scheme: dark;
}
html.dim {
  --bg: #0f1115;  --bg-card: #13161c;  --bg-hover: rgba(255,255,255,0.03);
  --white: #d8dde8;  --gray-1: #8a909e;  --gray-2: #4e5464;  --gray-3: #252932;
  --line: rgba(255,255,255,0.09);  --line-strong: rgba(255,255,255,0.2);
}
*, *::before, *::after { box-sizing: border-box; margin: 0; padding: 0; }
[hidden] { display: none !important; }
html, body { background: var(--bg); transition: background .3s, color .3s; }
body { color: var(--white); font-family: var(--font-body); -webkit-font-smoothing: antialiased; min-height: 100vh; }
::selection { background: var(--white); color: var(--bg); }
button, input, select { font: inherit; color: inherit; }
.page { max-width: 1240px; margin: 0 auto; padding-block: 0 100px; padding-left: max(28px, env(safe-area-inset-left, 0px)); padding-right: max(28px, env(safe-area-inset-right, 0px)); }
@media (max-width: 600px) { .page { padding-left: max(16px, env(safe-area-inset-left, 0px)); padding-right: max(16px, env(safe-area-inset-right, 0px)); } }
.mono { font-family: var(--font-mono); font-variant-numeric: tabular-nums; }
.muted { color: var(--gray-2); }
kbd { font-family: var(--font-mono); font-size: .56rem; color: var(--gray-2); border: 1px solid var(--gray-3); padding: 1px 5px; letter-spacing: .04em; }

/* top bar */
.topbar { display: flex; justify-content: space-between; align-items: center; gap: 14px; flex-wrap: wrap; padding: 26px 0; border-bottom: 1px solid var(--line); }
.wordmark { display: flex; align-items: baseline; gap: 8px; flex-wrap: wrap; }
.wordmark small { font-family: var(--font-mono); font-size: .62rem; color: var(--gray-2); letter-spacing: .1em; }
.wordmark .credit { font-family: var(--font-mono); font-size: .6rem; color: var(--gray-2); letter-spacing: .08em; padding-left: 12px; border-left: 1px solid var(--gray-2); }
.top-right { display: flex; align-items: center; gap: 18px; flex-wrap: wrap; }
.readout { display: inline-flex; align-items: center; gap: 8px; font-family: var(--font-mono); font-size: .62rem; letter-spacing: .1em; text-transform: uppercase; color: var(--gray-1); }
.clock { font-family: var(--font-mono); font-weight: 500; font-size: .72rem; letter-spacing: .06em; color: var(--white); }
.theme { display: flex; gap: 4px; }

/* status dot */
.status-dot { width: 6px; height: 6px; border-radius: 50%; background: var(--white); flex: none; }
.status-dot.pulse { animation: dotpulse 1.8s infinite; }
.status-dot.idle { background: var(--gray-2); }
.readout .status-tag { color: var(--gray-1); transition: color .3s ease; }
.readout .status-tag.live { color: var(--white); }
.status-dot.err { background: var(--red); animation: none; }
.status-dot.red { background: var(--red); animation: dotpulse 1s infinite; }
@keyframes dotpulse { 0%,100% { opacity: 1 } 50% { opacity: .2 } }

/* loading */
.topbar { position: relative; }
.loading.off { opacity: 0; pointer-events: none; }
.loading { position: absolute; left: 0; top: calc(100% + 14px); display: flex; align-items: center; gap: 14px; transition: opacity .4s ease; font-family: var(--font-mono); font-size: .62rem; letter-spacing: .14em; color: var(--gray-2); }
.scanline { position: relative; width: 160px; height: 1px; background: var(--gray-3); overflow: hidden; }
.scanline span { position: absolute; top: 0; left: -40px; width: 40px; height: 1px; background: var(--white); animation: scan 1.4s ease-in-out infinite; }
@keyframes scan { to { left: 160px; } }

/* connection strip under the top bar */
.statusbar { display: grid; grid-template-columns: repeat(6, minmax(0, 1fr)); border-bottom: 1px solid var(--line); }
.sb { display: flex; flex-direction: column; gap: 5px; padding: 12px 14px; border-left: 1px solid var(--line); min-width: 0; }
.sb:first-child { border-left: 0; padding-left: 0; }
.sb-v { font-family: var(--font-mono); font-size: .72rem; letter-spacing: .02em; color: var(--white); overflow-wrap: anywhere; transition: color .3s ease; }
.sb-v.dim { color: var(--gray-2); }
.sb-v.bad { color: var(--red); }
@media (max-width: 900px) {
  .statusbar { grid-template-columns: repeat(3, minmax(0, 1fr)); }
  .sb:nth-child(3n+1) { border-left: 0; padding-left: 0; }
  .sb:nth-child(n+4) { border-top: 1px solid var(--line); }
}
@media (max-width: 480px) {
  .statusbar { grid-template-columns: repeat(2, minmax(0, 1fr)); }
  .sb:nth-child(n) { border-left: 1px solid var(--line); padding-left: 14px; border-top: 0; }
  .sb:nth-child(2n+1) { border-left: 0; padding-left: 0; }
  .sb:nth-child(n+3) { border-top: 1px solid var(--line); }
}

/* hero */
.hero { padding: 56px 0 34px; display: flex; flex-direction: column; gap: 18px; }
.eyebrow { display: flex; align-items: center; gap: 12px; font-family: var(--font-mono); font-size: .68rem; letter-spacing: .18em; text-transform: uppercase; color: var(--gray-1); }
.eyebrow::before { content: ''; width: 28px; height: 1px; background: var(--gray-1); }
h1 { font-family: var(--font-display); font-weight: 700; font-size: clamp(2.4rem, 6vw, 4.2rem); line-height: .98; letter-spacing: -.01em; text-transform: uppercase; text-wrap: balance; }
.intro { font-size: .95rem; color: var(--gray-1); line-height: 1.6; max-width: 480px; }
.notice { border: 1px solid var(--line); padding: 14px 16px; font-size: .77rem; color: var(--gray-1); line-height: 1.6; }
.notice.reveal { margin-top: 0 !important; }
.notice.reveal:not(.open) { padding-top: 0; padding-bottom: 0; border-width: 0; }
.notice b { color: var(--white); font-family: var(--font-mono); font-weight: 400; font-size: .72rem; }

/* feature panel */
.feature { border: 1px solid var(--line-strong); background: radial-gradient(ellipse 60% 90% at 88% 20%, rgba(255,255,255,.035), transparent 70%), var(--bg-card); }
.feature.alert { border-color: var(--red); }
.feature-top { display: grid; grid-template-columns: 1fr auto; gap: 24px; padding: 22px 22px 6px; }
@media (max-width: 720px) { .feature-top { grid-template-columns: 1fr; } }
.feature-left { display: flex; flex-direction: column; gap: 14px; min-width: 0; }
.label { font-family: var(--font-mono); font-size: .62rem; letter-spacing: .12em; text-transform: uppercase; color: var(--gray-2); }
.feature-title { font-family: var(--font-display); font-weight: 700; font-size: clamp(1.4rem, 3vw, 2.2rem); text-transform: uppercase; line-height: 1.1; min-height: 1.1em; }
.meta { display: flex; flex-direction: column; gap: 8px; max-width: 380px; }
.feature-right { display: flex; flex-direction: column; align-items: flex-end; gap: 10px; text-align: right; min-width: 8.5ch; }
.feature-right .big { min-width: 7ch; justify-content: flex-end; }
@media (max-width: 720px) { .feature-right .big { justify-content: flex-start; } }
@media (max-width: 720px) { .feature-right { align-items: flex-start; text-align: left; } }
.big { font-family: var(--font-mono); font-weight: 700; font-size: clamp(2.4rem, 7vw, 4.6rem); letter-spacing: .02em; line-height: 1; color: var(--white); }
.badge { display: inline-flex; opacity: 0; visibility: hidden; transition: opacity .3s ease, visibility .3s; align-items: center; gap: 8px; font-family: var(--font-mono); font-size: .62rem; letter-spacing: .12em; text-transform: uppercase; color: var(--red); border: 1px solid var(--red); background: rgba(255,68,51,.04); padding: 5px 10px; }
.badge.on { opacity: 1; visibility: visible; }
#strip { width: 100%; height: 80px; display: block; }
.scrub { padding: 4px 22px 20px; display: flex; flex-direction: column; gap: 10px; }
.track { position: relative; height: 26px; cursor: pointer; touch-action: none; outline-offset: 4px; }
.track .win { position: absolute; top: -5px; bottom: -5px; border: 1px dashed rgba(255,255,255,.55); pointer-events: none; }
.track .now { position: absolute; top: -7px; bottom: -7px; width: 1px; background: var(--red); pointer-events: none; }
.track .thumb { position: absolute; top: -9px; bottom: -9px; width: 2px; margin-left: -1px; background: var(--white); outline: 2px solid var(--bg-card); pointer-events: none; }
.ticks { display: flex; justify-content: space-between; font-family: var(--font-mono); font-size: .58rem; letter-spacing: .08em; color: var(--gray-2); }
.scrub-row { display: flex; gap: 10px; flex-wrap: wrap; align-items: center; justify-content: space-between; padding-top: 6px; }
.scrub-row .left { display: flex; gap: 6px; flex-wrap: wrap; align-items: center; }
.keys { display: flex; gap: 6px; align-items: center; flex-wrap: wrap; font-family: var(--font-mono); font-size: .56rem; letter-spacing: .08em; color: var(--gray-2); text-transform: uppercase; }

/* buttons */
.btn { background: none; border: 1px solid var(--line-strong); color: var(--white); font-family: var(--font-mono); font-size: .68rem; letter-spacing: .08em; text-transform: uppercase; padding: 7px 16px; cursor: pointer; transition: background .15s, color .15s; }
.btn:hover { background: var(--white); color: var(--bg); }
.btn:disabled { opacity: .4; pointer-events: none; }
.chip { background: none; border: 1px solid var(--gray-3); color: var(--gray-2); font-family: var(--font-mono); font-size: .6rem; letter-spacing: .08em; text-transform: uppercase; padding: 4px 10px; cursor: pointer; transition: border-color .2s, color .2s; }
.chip:hover { border-color: var(--gray-2); color: var(--gray-1); }
.chip.active, .chip[aria-pressed="true"] { border-color: var(--white); color: var(--white); }
.chk-line { display: inline-flex; gap: 8px; align-items: center; font-family: var(--font-mono); font-size: .6rem; letter-spacing: .08em; text-transform: uppercase; color: var(--gray-1); cursor: pointer; }
button:focus-visible, input:focus-visible, select:focus-visible, summary:focus-visible, .track:focus-visible { outline: 1px solid var(--white); outline-offset: 2px; }

/* section label */
.section-head { display: flex; justify-content: space-between; align-items: baseline; padding: 48px 0 16px; }

/* cards: shared hairline grid */
.grid2 { display: grid; grid-template-columns: 1fr; padding: 1px 0 0 1px; margin-top: 28px; }
@media (min-width: 720px) { .grid2 { grid-template-columns: 1fr 1fr; } }
.card { border: 1px solid var(--line); margin: -1px 0 0 -1px; display: flex; flex-direction: column; transition: background .2s, border-color .2s; min-width: 0; }
.card:hover { background: var(--bg-hover); }
.card.alert { border-color: var(--red); background: rgba(255,68,51,.04); position: relative; z-index: 1; }
.card-index { display: flex; justify-content: space-between; gap: 10px; padding: 10px 14px; border-bottom: 1px solid var(--line); font-family: var(--font-mono); font-size: .62rem; letter-spacing: .08em; color: var(--gray-2); text-transform: uppercase; }
.card-body { padding: 16px 16px 18px; flex: 1; display: flex; flex-direction: column; gap: 14px; }
.card-title { font-family: var(--font-display); font-weight: 600; font-size: 1rem; text-transform: uppercase; line-height: 1.25; }
.card-desc { font-size: .77rem; color: var(--gray-1); line-height: 1.6; }
.card-footer { margin-top: auto; padding-top: 12px; border-top: 1px solid var(--line); display: flex; justify-content: space-between; align-items: center; gap: 10px; flex-wrap: wrap; }
.status-tag { font-family: var(--font-mono); font-size: .6rem; letter-spacing: .06em; text-transform: uppercase; color: var(--white); white-space: nowrap; }
.status-tag::before { content: '[ '; color: var(--gray-2); }
.status-tag::after { content: ' ]'; color: var(--gray-2); }
.status-tag.red, .status-tag.bad { color: var(--red); }

/* data rows */
.spec-row { display: flex; justify-content: space-between; align-items: baseline; gap: 10px; border-bottom: 1px dotted var(--gray-3); padding-bottom: 6px; min-width: 0; }
.spec-label { font-family: var(--font-mono); font-size: .6rem; color: var(--gray-2); letter-spacing: .08em; text-transform: uppercase; flex-shrink: 0; }
.spec-value { font-size: .73rem; color: var(--gray-1); text-align: right; min-width: 0; overflow-wrap: anywhere; }
.spec-value.mono { font-size: .7rem; }
.ctl { display: flex; flex-direction: column; gap: 8px; }

/* toggle */
.toggle { position: relative; width: 34px; height: 16px; flex: none; display: inline-block; }
.toggle input { position: absolute; inset: 0; opacity: 0; margin: 0; cursor: pointer; z-index: 1; }
.toggle span { position: absolute; inset: 0; border: 1px solid var(--gray-2); transition: border-color .2s; }
.toggle span::after { content: ''; position: absolute; left: 2px; top: 2px; width: 10px; height: 10px; background: var(--gray-2); transition: transform .2s, background .2s; }
.toggle input:checked + span { border-color: var(--white); }
.toggle input:checked + span::after { transform: translateX(18px); background: var(--white); }
.toggle input:focus-visible + span { outline: 1px solid var(--white); outline-offset: 2px; }

/* mode chips */
.seg { display: flex; flex-wrap: wrap; gap: 6px; }

/* ranges */
input[type=range] { -webkit-appearance: none; appearance: none; width: 100%; height: 20px; background: transparent; cursor: pointer; }
input[type=range]::-webkit-slider-runnable-track { height: 1px; background: var(--line-strong); }
input[type=range]::-webkit-slider-thumb { -webkit-appearance: none; width: 8px; height: 16px; margin-top: -7.5px; background: var(--white); border: 0; }
input[type=range]::-moz-range-track { height: 1px; background: var(--line-strong); }
input[type=range]::-moz-range-progress { height: 1px; background: var(--white); }
input[type=range]::-moz-range-thumb { width: 8px; height: 16px; background: var(--white); border: 0; border-radius: 0; }
.ktrack::-webkit-slider-runnable-track { height: 4px; background: linear-gradient(90deg,#ff8b2a,#ffc98a,#fff1e0,#e6ecff,#bcd0ff); }
.ktrack::-webkit-slider-thumb { margin-top: -6px; }
.ktrack::-moz-range-track { height: 4px; background: linear-gradient(90deg,#ff8b2a,#ffc98a,#fff1e0,#e6ecff,#bcd0ff); }

/* busy actions */
.action { display: grid; grid-template-columns: auto 1fr auto; align-items: center; gap: 12px; width: 100%; text-align: left; border: 1px solid var(--line); background: none; padding: 12px 14px; cursor: pointer; transition: border-color .2s, background .2s; }
.action:hover { border-color: var(--line-strong); }
.action .status-dot { background: var(--gray-2); }
.action strong { display: block; font-family: var(--font-display); font-weight: 600; font-size: .9rem; text-transform: uppercase; letter-spacing: .01em; }
.action .sub { display: block; font-size: .72rem; color: var(--gray-1); line-height: 1.5; margin-top: 2px; }
.action[aria-pressed="true"] { border-color: var(--red); background: rgba(255,68,51,.04); }
.action[aria-pressed="true"] .status-dot { background: var(--red); animation: dotpulse 1s infinite; }
.action[aria-pressed="true"] strong { color: var(--red); }
.codebox { display: flex; flex-direction: column; gap: 8px; border: 1px solid var(--line); padding: 12px; }
.codeline { display: grid; grid-template-columns: 52px 1fr auto; align-items: center; gap: 10px; min-width: 0; }
.codeline code { font-family: var(--font-mono); font-size: .66rem; color: var(--gray-1); overflow-x: auto; white-space: nowrap; min-width: 0; }
.hint { font-size: .72rem; color: var(--gray-2); line-height: 1.6; }
.hint .mono { color: var(--gray-1); }

/* settings drawers */
.drawers { display: grid; grid-template-columns: 1fr; padding: 1px 0 0 1px; }
@media (min-width: 720px) { .drawers { grid-template-columns: 1fr 1fr; } }
@media (min-width: 1080px) { .drawers { grid-template-columns: 1fr 1fr 1fr; } }
details.drawer { border: 1px solid var(--line); margin: -1px 0 0 -1px; min-width: 0; transition: background .2s; }
details.drawer:hover { background: var(--bg-hover); }
details.drawer > summary { list-style: none; cursor: pointer; padding: 14px 16px; display: grid; grid-template-columns: 1fr auto; gap: 4px 12px; align-items: center; }
details.drawer > summary::-webkit-details-marker { display: none; }
.dr-title { font-family: var(--font-display); font-weight: 600; font-size: 1rem; text-transform: uppercase; line-height: 1.25; }
.dr-sub { grid-column: 1; font-family: var(--font-mono); font-size: .6rem; letter-spacing: .08em; text-transform: uppercase; color: var(--gray-2); }
.dr-sign { grid-column: 2; grid-row: 1 / span 2; font-family: var(--font-mono); font-size: .8rem; color: var(--gray-2); }
.dr-sign::before { content: '+'; }
details[open] > summary .dr-sign::before { content: '−'; }
details[open] > summary { border-bottom: 1px solid var(--line); }
.set-body { padding: 16px; display: flex; flex-direction: column; gap: 14px; }
.sub-head { font-family: var(--font-mono); font-size: .6rem; letter-spacing: .14em; text-transform: uppercase; color: var(--gray-1); padding-top: 6px; border-top: 1px solid var(--line); padding-top: 14px; }
.two { display: grid; grid-template-columns: 1fr 1fr; gap: 12px; }
@media (max-width: 420px) { .two { grid-template-columns: 1fr; } }
.field { display: flex; flex-direction: column; gap: 6px; min-width: 0; }
.field label, .lbl { font-family: var(--font-mono); font-size: .6rem; color: var(--gray-2); letter-spacing: .08em; text-transform: uppercase; line-height: 1.5; }
.row { display: flex; align-items: center; justify-content: space-between; gap: 12px; }
.val { font-family: var(--font-mono); font-size: .7rem; color: var(--white); font-variant-numeric: tabular-nums; }
input[type=number], input[type=time], input[type=text], input[type=password], select {
  font-family: var(--font-mono); font-size: .74rem; color: var(--white); background: var(--bg); border: 1px solid var(--gray-3); border-radius: 0; padding: 8px 10px; width: 100%; min-width: 0; transition: border-color .2s; }
input[type=number]:hover, input[type=time]:hover, input[type=text]:hover, input[type=password]:hover, select:hover { border-color: var(--gray-2); }
select { -webkit-appearance: none; appearance: none; padding-right: 28px; cursor: pointer;
  background-image: url("data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='10' height='6'%3E%3Cpath d='M0 0l5 6 5-6z' fill='%238a8a8a'/%3E%3C/svg%3E"); background-repeat: no-repeat; background-position: right 10px center; }
input[type=time]::-webkit-calendar-picker-indicator { filter: invert(.6); }
input[type=file] { font-family: var(--font-mono); font-size: .66rem; color: var(--gray-1); }
input[type=file]::file-selector-button { background: none; border: 1px solid var(--line-strong); color: var(--white); font-family: var(--font-mono); font-size: .62rem; letter-spacing: .08em; text-transform: uppercase; padding: 6px 12px; margin-right: 12px; cursor: pointer; }
.check { display: flex; gap: 10px; align-items: flex-start; font-size: .75rem; color: var(--gray-1); line-height: 1.5; cursor: pointer; }
.check input, .chk-line input { -webkit-appearance: none; appearance: none; width: 12px; height: 12px; border: 1px solid var(--gray-2); margin-top: 3px; flex: none; cursor: pointer; display: grid; place-items: center; }
.chk-line input { margin-top: 0; }
.check input:checked, .chk-line input:checked { border-color: var(--white); background: var(--white); box-shadow: inset 0 0 0 2px var(--bg); }
.curve-row { display: grid; grid-template-columns: 1fr 1fr; gap: 10px; }
#curve { width: 100%; height: 120px; display: block; }
.nets { display: flex; flex-direction: column; max-height: 220px; overflow: auto; padding: 1px 0 0; }
.net { display: flex; justify-content: space-between; align-items: center; padding: 8px 10px; border: 1px solid var(--line); margin-top: -1px; background: none; cursor: pointer; font-size: .75rem; color: var(--gray-1); text-align: left; transition: background .2s, color .2s; }
.net:hover { background: var(--bg-hover); color: var(--white); }
.status-line { display: flex; align-items: baseline; gap: 10px; flex-wrap: wrap; font-size: .73rem; color: var(--gray-1); }
.status-line .chip { pointer-events: none; }
.prog { height: 1px; background: var(--gray-3); position: relative; }
.prog span { position: absolute; left: 0; top: 0; height: 1px; width: 0; background: var(--white); transition: width .15s; }
.banner { border: 1px solid var(--line); padding: 10px 12px; font-size: .73rem; color: var(--gray-1); line-height: 1.6; }

/* calm layout: things fade and slide open instead of popping */
.reveal { max-height: 0; opacity: 0; overflow: hidden; margin-top: -14px; pointer-events: none; transition: max-height .35s ease, opacity .3s ease, margin-top .35s ease; }
.reveal.open { max-height: 480px; opacity: 1; margin-top: 0; pointer-events: auto; }
.keep { transition: opacity .3s ease, visibility .3s; }
.keep.off { opacity: 0; visibility: hidden; }
.fx { transition: opacity .18s ease; }
.slot { transition: opacity .3s ease; min-height: 24px; }
.slot.empty { opacity: 0; }
.meta { min-height: calc(4 * 1.55rem); }
#modeHint { min-height: 4.9em; }
.feature, .feature-title { transition: border-color .3s ease, opacity .18s ease; }
.track .win { transition: opacity .3s ease; }
.track .win.off { opacity: 0; }

/* rolling digits */
.roll { display: inline-flex; white-space: pre; }
.rd { display: inline-block; position: relative; overflow: hidden; height: 1.05em; line-height: 1.05; vertical-align: bottom; perspective: 300px; }
.rd-i { display: block; height: 1.05em; line-height: 1.05; }
.rd-out { position: absolute; left: 0; top: 0; animation: rdOut .65s cubic-bezier(0.25, 1.15, 0.5, 1) both; }
.rd-in { animation: rdIn .65s cubic-bezier(0.25, 1.15, 0.5, 1) both; }
@keyframes rdOut { from { transform: translateY(0) rotateX(0); opacity: 1; } to { transform: translateY(-105%) rotateX(60deg); opacity: 0; } }
@keyframes rdIn { from { transform: translateY(105%) rotateX(-60deg); opacity: 0; } to { transform: none; opacity: 1; } }

/* footer */
.foot { margin-top: 64px; padding-top: 18px; border-top: 1px solid var(--line); display: flex; justify-content: space-between; align-items: center; gap: 14px; flex-wrap: wrap; font-family: var(--font-mono); font-size: .6rem; letter-spacing: .1em; text-transform: uppercase; color: var(--gray-2); }
.footer-brand { display: flex; align-items: center; gap: 8px; opacity: .35; transition: opacity .3s; }
.footer-brand:hover { opacity: .7; }
.footer-logo { width: 22px; height: 22px; object-fit: contain; filter: invert(1); }

.toast { position: fixed; left: 50%; bottom: calc(24px + env(safe-area-inset-bottom, 0px)); transform: translateX(-50%) translateY(16px); background: var(--bg); color: var(--white); border: 1px solid var(--line-strong); padding: 9px 16px; font-family: var(--font-mono); font-size: .64rem; letter-spacing: .1em; text-transform: uppercase; opacity: 0; transition: opacity .2s, transform .2s; pointer-events: none; z-index: 20; max-width: calc(100% - 32px); text-align: center; }
.toast.show { opacity: 1; transform: translateX(-50%) translateY(0); }
</style>
</head><body>
<div class="page">
  <header class="topbar">
    <div class="wordmark"><small>ROMBY LAMP</small><span class="credit">DESIGNED BY ROMBY WITH CLAUDE</span></div>
    <div class="top-right">
      <span class="readout"><span class="status-dot pulse idle" id="connDot"></span><span class="status-tag" id="connText">Connecting</span></span>
      <button class="chip" id="usbBtn" hidden>Connect USB</button>
      <span class="clock roll" id="clock" aria-label="Lamp time"></span>
      <div class="theme" role="group" aria-label="Theme">
        <button class="chip active" data-theme-btn="dark">Dark</button>
        <button class="chip" data-theme-btn="dim">Grey</button>
      </div>
    </div>
  </header>
  <div class="statusbar" aria-label="Lamp connection">
    <div class="sb"><span class="spec-label">Link</span><span class="sb-v" id="sbLink">Searching</span></div>
    <div class="sb"><span class="spec-label">Port / address</span><span class="sb-v" id="sbAddr">—</span></div>
    <div class="sb"><span class="spec-label">Firmware</span><span class="sb-v" id="sbFw">—</span></div>
    <div class="sb"><span class="spec-label">Uptime</span><span class="sb-v" id="sbUp">—</span></div>
    <div class="sb"><span class="spec-label">Lamp ID</span><span class="sb-v" id="sbId">—</span></div>
    <div class="sb"><span class="spec-label" id="sbLastL">Clock</span><span class="sb-v" id="sbLast">—</span></div>
  </div>

  <div class="notice reveal" id="updNotice" style="margin-top:20px">
    <div class="row" style="flex-wrap:wrap">
      <span><b id="updTitle">Update available</b> <span id="updNotes"></span></span>
      <button class="btn" id="updBtn">Update lamp</button>
    </div>
    <div class="prog keep off" id="updProg" style="margin-top:12px"><span id="updBar"></span></div>
    <span class="hint" id="updMsg"></span>
  </div>
  <section class="hero">
    <div class="eyebrow"><span>Desk light control</span><span id="fw">FW —</span></div>
    <h1>Romby Lamp</h1>
    <p class="intro">Circadian white through the workday, a progress bar that fills as the day runs out, and a busy light for calls.</p>
  </section>

  <div class="notice reveal" id="demoBanner" style="margin-bottom:0">Demo mode. Nothing here reaches a lamp. Once the lamp is flashed, this page opens at <b>localhost:8765</b> through the USB bridge, at <b>rombylamp.local</b> on WiFi, or at <b>192.168.4.1</b> on the lamp's hotspot.</div>

  <div style="height:28px" aria-hidden="true"></div>
  <section class="feature" id="feature" aria-label="Strip preview">
    <div class="feature-top">
      <div class="feature-left">
        <div class="label" id="viewLabel">Live · now</div>
        <div class="feature-title fx" id="rdMode">Circadian</div>
        <div class="meta" id="plateMeta">
          <div class="spec-row slot empty"><span class="spec-label"></span><span class="spec-value"></span></div>
          <div class="spec-row slot empty"><span class="spec-label"></span><span class="spec-value"></span></div>
          <div class="spec-row slot empty"><span class="spec-label"></span><span class="spec-value"></span></div>
          <div class="spec-row slot empty"><span class="spec-label"></span><span class="spec-value"></span></div>
        </div>
      </div>
      <div class="feature-right">
        <span class="badge" id="busyTag"><span class="status-dot red"></span><span id="busyText">In a meeting</span></span>
        <div class="big roll" id="rdK"></div>
        <div class="label fx" id="bigLabel">Colour temperature</div>
      </div>
    </div>
    <canvas id="strip" aria-label="Simulated LED strip"></canvas>
    <div class="scrub">
      <div class="track" id="track" tabindex="0" role="slider" aria-label="Preview time of day" aria-valuemin="0" aria-valuemax="1439">
        <div class="win" id="win"></div>
        <div class="now" id="nowMark"></div>
        <div class="thumb" id="thumb"></div>
      </div>
      <div class="ticks"><span>00</span><span>03</span><span>06</span><span>09</span><span>12</span><span>15</span><span>18</span><span>21</span><span>24</span></div>
      <div class="scrub-row">
        <div class="left">
          <button class="chip" id="playBtn" aria-pressed="false">Play a day</button>
          <button class="chip" id="nowBtn">Back to now</button>
          <label class="chk-line keep off" id="mirrorWrap"><input type="checkbox" id="mirror"> Show on lamp</label>
        </div>
        <div class="keys"><kbd>←</kbd><kbd>→</kbd> Scrub <kbd>P</kbd> Play <kbd>N</kbd> Now <kbd>B</kbd> Busy <kbd>T</kbd> Test call</div>
      </div>
    </div>
  </section>

  <div class="grid2">
    <section class="card" id="lightCard" aria-labelledby="lightH">
      <div class="card-index"><span id="lightH">Light</span><span class="status-tag" id="lightTag">On</span></div>
      <div class="card-body">
        <div class="spec-row"><span class="spec-label">Power</span><label class="toggle" title="Power"><input type="checkbox" id="power" aria-label="Lamp power"><span></span></label></div>
        <div class="ctl">
          <div class="spec-row" style="border:0;padding:0"><span class="spec-label">Brightness</span><span class="val" id="briVal">70%</span></div>
          <input type="range" id="bri" min="1" max="100" value="70" aria-label="Brightness">
        </div>
        <div class="ctl">
          <span class="spec-label">Mode</span>
          <div class="seg" role="group" aria-label="Mode">
            <button class="chip" data-mode="0" aria-pressed="true">Circadian</button>
            <button class="chip" data-mode="1" aria-pressed="false">Progress bar</button>
            <button class="chip" data-mode="2" aria-pressed="false">Fixed white</button>
          </div>
          <p class="card-desc" id="modeHint"></p>
        </div>
        <div class="ctl reveal" id="manualWrap">
          <div class="spec-row" style="border:0;padding:0"><span class="spec-label">Colour temperature</span><span class="val" id="mkVal">4000 K</span></div>
          <input type="range" class="ktrack" id="mk" min="1800" max="7500" step="50" value="4000" aria-label="Fixed colour temperature">
        </div>
        <div class="card-footer">
          <span class="hint">Knob: turn = brightness · press = on/off · double-press = busy · hold = switch mode</span>
        </div>
      </div>
    </section>

    <section class="card" id="teamsCard" aria-labelledby="teamsH">
      <div class="card-index"><span id="teamsH">Busy light</span><span class="status-tag" id="teamsChip">Normal</span></div>
      <div class="card-body">
        <button class="action" id="testCall" aria-pressed="false">
          <span class="status-dot"></span>
          <span><strong id="testTitle">Simulate a Teams call</strong><span class="sub" id="testSub">Shows the busy light. Ends by itself after 5 minutes.</span></span>
          <kbd>T</kbd>
        </button>
        <button class="action" id="busyBtn" aria-pressed="false">
          <span class="status-dot"></span>
          <span><strong id="busyTitle">I'm busy</strong><span class="sub">Red until you turn it off. Same as double-pressing the knob.</span></span>
          <kbd>B</kbd>
        </button>
        <div class="spec-row reveal" id="micLine"><span class="spec-label">Microphone</span><span class="spec-value" id="micText"></span></div>
        <div class="spec-row"><span class="spec-label">PresenceLight</span><span class="spec-value" id="presText">No update received yet</span></div>
        <div class="codebox">
          <span class="hint">PresenceLight → Custom API. Paste into each status you use:</span>
          <div class="codeline"><span class="spec-label">Method</span><code>POST</code><span></span></div>
          <div class="codeline"><span class="spec-label">URL</span><code id="presUrl">http://rombylamp.local/api/presence</code><button class="chip" data-copy="presUrl">Copy</button></div>
          <div class="codeline"><span class="spec-label">Body</span><code id="presBody">{"availability":"{{availability}}","activity":"{{activity}}"}</code><button class="chip" data-copy="presBody">Copy</button></div>
        </div>
        <div class="card-footer"><span class="hint">Red shows on the ends of the strip.</span><button class="btn" id="clearTeams">Clear red</button></div>
      </div>
    </section>
  </div>

  <div class="section-head"><div class="eyebrow">Settings</div></div>
  <div class="drawers">
    <details class="drawer" open>
      <summary><span class="dr-title">Work hours</span><span class="dr-sign"></span><span class="dr-sub">Auto on/off · progress bar</span></summary>
      <div class="set-body">
        <div class="two">
          <div class="field"><label for="progStart">Work starts</label><input type="time" id="progStart" data-set="progStart" data-kind="time"></div>
          <div class="field"><label for="progEnd">Work ends</label><input type="time" id="progEnd" data-set="progEnd" data-kind="time"></div>
        </div>
        <label class="check"><input type="checkbox" id="weekdaysOnly" data-set="weekdaysOnly" data-kind="bool"> Weekdays only. Weekends count as off-hours.</label>
        <label class="check"><input type="checkbox" id="autoSchedule" data-set="autoSchedule" data-kind="bool"> Turn on at work start and off at work end</label>
        <div class="two reveal open" id="schedOpts">
          <div class="field"><label for="afterHoursMin">On after hours: off after</label>
            <select id="afterHoursMin" data-set="afterHoursMin" data-kind="int">
              <option value="30">30 minutes</option><option value="60">1 hour</option><option value="120">2 hours</option><option value="180">3 hours</option><option value="240">4 hours</option><option value="0">Never</option>
            </select></div>
          <div class="field"><label for="earlyHrs">On this early: stay on all day</label>
            <select id="earlyHrs" data-set="earlyHrs" data-kind="int">
              <option value="1">1 hour before</option><option value="2">2 hours before</option><option value="3">3 hours before</option><option value="4">4 hours before</option><option value="6">6 hours before</option>
            </select></div>
        </div>
        <div class="field"><label for="fadeMin">Fade up before start / down before end</label>
          <select id="fadeMin" data-set="fadeMin" data-kind="int">
            <option value="0">No fade</option><option value="1">1 minute</option><option value="2">2 minutes</option><option value="5">5 minutes</option><option value="10">10 minutes</option><option value="15">15 minutes</option><option value="30">30 minutes</option>
          </select></div>
        <span class="hint" id="schedHint"></span>
        <div class="sub-head">Progress bar</div>
        <div class="field">
          <div class="row"><label for="glow">Unlit part glows at</label><span class="val" id="glowVal"></span></div>
          <input type="range" id="glow" min="0" max="60" data-set="glow" data-kind="int">
        </div>
        <label class="check"><input type="checkbox" id="reverse" data-set="reverse" data-kind="bool"> Fill from the other end</label>
      </div>
    </details>

    <details class="drawer">
      <summary><span class="dr-title">Daylight curve</span><span class="dr-sign"></span><span class="dr-sub">Colour temperature over 24 h</span></summary>
      <div class="set-body">
        <canvas id="curve" aria-label="Colour temperature over 24 hours"></canvas>
        <div id="curveRows" style="display:flex;flex-direction:column;gap:10px"></div>
        <span class="hint">The lamp blends smoothly between these five points. Lower K is warmer.</span>
      </div>
    </details>

    <details class="drawer">
      <summary><span class="dr-title">Busy light</span><span class="dr-sign"></span><span class="dr-sub">What turns it red · how it looks</span></summary>
      <div class="set-body">
        <div class="field"><label for="redTrigger">Turn red when</label>
          <select id="redTrigger" data-set="redTrigger" data-kind="int">
            <option value="2">Teams shows red (Busy, Do not disturb)</option>
            <option value="1">In a call or a scheduled meeting</option>
            <option value="0">Only while in a call or presenting</option>
          </select></div>
        <div class="field"><label for="redStyle">Where the red shows</label>
          <select id="redStyle" data-set="redStyle" data-kind="int">
            <option value="0">The two ends. Middle stays your light</option>
            <option value="1">The whole strip</option>
          </select></div>
        <div class="field reveal open" id="redEndWrap">
          <div class="row"><label for="redEndPct">Red at each end</label><span class="val" id="redEndVal"></span></div>
          <input type="range" id="redEndPct" min="1" max="25" data-set="redEndPct" data-kind="int">
        </div>
        <label class="check"><input type="checkbox" id="redPulse" data-set="redPulse" data-kind="bool"> Pulse</label>
        <div class="reveal open" id="pulseOpts" style="display:flex;flex-direction:column;gap:14px">
          <div class="field">
            <div class="row"><label for="redPulseMin">Pulse dims down to</label><span class="val" id="redPulseMinVal"></span></div>
            <input type="range" id="redPulseMin" min="0" max="90" step="5" data-set="redPulseMin" data-kind="int">
          </div>
          <div class="field"><label for="redPulseSec">Pulse speed</label>
            <select id="redPulseSec" data-set="redPulseSec" data-kind="int">
              <option value="2">Fast (2 s per breath)</option><option value="3">3 s per breath</option><option value="4">Slow (4 s per breath)</option><option value="6">Slower (6 s per breath)</option><option value="8">Very slow (8 s per breath)</option>
            </select></div>
        </div>
        <div class="field">
          <div class="row"><label for="redBri">Red brightness</label><span class="val" id="redBriVal"></span></div>
          <input type="range" id="redBri" min="5" max="100" data-set="redBri" data-kind="int">
        </div>
        <label class="check"><input type="checkbox" id="redWhenOff" data-set="redWhenOff" data-kind="bool"> Show red even when the lamp is off</label>
        <div class="field"><label for="autoClearHrs">Safety reset if Teams goes quiet</label>
          <select id="autoClearHrs" data-set="autoClearHrs" data-kind="int">
            <option value="0">Never</option><option value="2">After 2 hours</option><option value="4">After 4 hours</option><option value="8">After 8 hours</option>
          </select></div>
      </div>
    </details>

    <details class="drawer">
      <summary><span class="dr-title">LED strip</span><span class="dr-sign"></span><span class="dr-sub">Type · length · colour order · power cap</span></summary>
      <div class="set-body">
        <div class="field"><label for="stripType">Strip type</label>
          <select id="stripType" data-set="stripType" data-kind="int">
            <option value="1">RGBCCT · dual IC (WS2811)</option>
            <option value="0">RGBW · single IC (WS2814)</option>
          </select></div>
        <div class="field">
          <label for="lenVal">Strip length</label>
          <div style="display:grid;grid-template-columns:1fr auto;gap:8px">
            <input type="number" id="lenVal" min="0" step="0.25" inputmode="decimal">
            <select id="lenUnit" aria-label="Length unit" style="width:auto"><option value="in">inches</option><option value="ft">feet</option><option value="cm">cm</option></select>
          </div>
          <div class="row" style="flex-wrap:wrap">
            <span class="hint" id="lenHint"></span>
            <label class="hint" style="display:flex;gap:6px;align-items:center">Cut marks every
              <input type="number" id="cutMm" min="5" max="500" step="0.5" style="width:80px"> mm</label>
          </div>
        </div>
        <div class="field"><label for="pixels" id="pixelsLabel">Segments (cuttable sections)</label><input type="number" id="pixels" min="1" max="600" data-set="pixels" data-kind="int">
          <span class="hint" id="pixelsHint">Each segment has two chips. If only part of the strip lights up, raise this number.</span></div>

        <div class="reveal open" id="cctOpts" style="display:flex;flex-direction:column;gap:14px">
          <div class="two">
            <div class="field"><label for="warmK">Warm white LED</label>
              <select id="warmK" data-set="warmK" data-kind="int">
                <option value="2200">2200 K</option><option value="2700">2700 K</option><option value="3000">3000 K</option><option value="3500">3500 K</option>
              </select></div>
            <div class="field"><label for="coolK">Cool white LED</label>
              <select id="coolK" data-set="coolK" data-kind="int">
                <option value="5000">5000 K</option><option value="5700">5700 K</option><option value="6000">6000 K</option><option value="6500">6500 K</option><option value="7000">7000 K</option>
              </select></div>
          </div>
          <span class="hint">Use the two white temperatures from the strip's listing (for example "2700K–6500K" or "3000K–6500K").</span>
          <div class="two">
            <div class="field"><label for="rgbOrder">RGB chip order</label>
              <select id="rgbOrder" data-set="rgbOrder" data-kind="int">
                <option value="0">RGB</option><option value="1">RBG</option><option value="2">GRB</option><option value="3">GBR</option><option value="4">BRG</option><option value="5">BGR</option>
              </select></div>
            <div class="field"><label for="cctOrder">White chip order</label>
              <select id="cctOrder" data-set="cctOrder" data-kind="int">
                <option value="0">Warm · Cool · –</option><option value="1">Cool · Warm · –</option><option value="2">Warm · – · Cool</option><option value="3">Cool · – · Warm</option><option value="4">– · Warm · Cool</option><option value="5">– · Cool · Warm</option>
              </select></div>
          </div>
          <label class="check"><input type="checkbox" id="cctFirst" data-set="cctFirst" data-kind="bool"> White chip comes first in each segment</label>
        </div>

        <div class="reveal" id="rgbwOpts" style="display:flex;flex-direction:column;gap:14px">
          <div class="two">
            <div class="field"><label for="order">Colour order</label>
              <select id="order" data-set="order" data-kind="int">
                <option value="0">GRBW</option><option value="1">RGBW</option><option value="2">RBGW</option><option value="3">GBRW</option><option value="4">BRGW</option><option value="5">BGRW</option>
              </select></div>
            <div class="field"><label for="whiteK">White LED</label>
              <select id="whiteK" data-set="whiteK" data-kind="int">
                <option value="2700">2700 K warm</option><option value="3000">3000 K warm</option><option value="4000">4000 K neutral</option><option value="5000">5000 K cool</option><option value="6500">6500 K daylight</option>
              </select></div>
          </div>
        </div>

        <div class="row" style="flex-wrap:wrap;align-items:flex-start"><span class="hint" style="flex:1;min-width:180px" id="testHint"></span><button class="btn" id="identify">Colour test</button></div>
        <div class="field">
          <div class="row"><label for="maxBri">Brightness cap</label><span class="val" id="maxBriVal"></span></div>
          <input type="range" id="maxBri" min="10" max="100" data-set="maxBri" data-kind="int">
          <span class="hint">100% on the knob equals this cap. Keeps the supply and strip cool.</span>
        </div>
      </div>
    </details>

    <details class="drawer">
      <summary><span class="dr-title">Knob</span><span class="dr-sign"></span><span class="dr-sub">EC11 encoder</span></summary>
      <div class="set-body">
        <div class="field"><label for="encSteps">Pulses per click</label>
          <select id="encSteps" data-set="encSteps" data-kind="int"><option value="4">4 (most EC11s)</option><option value="2">2 (if each click jumps twice)</option></select></div>
        <label class="check"><input type="checkbox" id="encReverse" data-set="encReverse" data-kind="bool"> Reverse direction (clockwise dims)</label>
        <div class="sub-head">Knob test</div>
        <span class="hint">Turn the knob slowly while this section is open. Each wire should flip between 1 and 0, and "Changes seen" should count up.</span>
        <div class="spec-row"><span class="spec-label">Wire A (Q1)</span><span class="spec-value mono" id="kA">—</span></div>
        <div class="spec-row"><span class="spec-label">Wire B (Q4)</span><span class="spec-value mono" id="kB">—</span></div>
        <div class="spec-row"><span class="spec-label">Button</span><span class="spec-value mono" id="kBtn">—</span></div>
        <div class="spec-row"><span class="spec-label">Changes seen</span><span class="spec-value mono" id="kEdges">—</span></div>
        <span class="hint" id="kHint"></span>
      </div>
    </details>

    <details class="drawer">
      <summary><span class="dr-title">WiFi</span><span class="dr-sign"></span><span class="dr-sub" id="wifiSub">Network connection</span></summary>
      <div class="set-body">
        <div class="status-line" id="netStatus"></div>
        <label class="check"><input type="checkbox" id="wifiOff" data-set="wifiOff" data-kind="bool"> Turn WiFi off and use USB only. No hotspot is broadcast. The lamp restarts when you change this.</label>
        <div class="reveal open" id="wifiOpts" style="display:flex;flex-direction:column;gap:14px">
          <div class="row"><span class="lbl">Nearby networks</span><button class="chip" id="scanBtn">Scan</button></div>
          <div class="nets" id="nets"></div>
          <div class="field"><label for="ssid">Network name</label><input type="text" id="ssid" autocomplete="off" spellcheck="false"></div>
          <div class="field"><label for="pass">Password</label><input type="password" id="pass" autocomplete="off"></div>
          <div><button class="btn" id="saveWifi">Save and restart lamp</button></div>
          <label class="check"><input type="checkbox" id="keepAP" data-set="keepAP" data-kind="bool"> Keep the setup hotspot on after joining WiFi</label>
          <label class="check"><input type="checkbox" id="lowTx" data-set="lowTx" data-kind="bool"> Low WiFi power (8.5 dBm). Fixes dropouts on QuinLED boards.</label>
          <span class="hint" id="apHint"></span>
        </div>
      </div>
    </details>

    <details class="drawer" id="fwSection">
      <summary><span class="dr-title">Firmware</span><span class="dr-sign"></span><span class="dr-sub" id="fwSub">Update over WiFi</span></summary>
      <div class="set-body">
        <div class="spec-row"><span class="spec-label">Installed</span><span class="spec-value mono" id="fwInstalled">—</span></div>
        <div class="spec-row"><span class="spec-label">Newest online</span><span class="spec-value mono" id="fwOnline">Not checked</span></div>
        <div class="row" style="flex-wrap:wrap;gap:8px;justify-content:flex-start">
          <button class="btn" id="fwCheck">Check for updates</button>
        </div>
        <span class="hint">Updates install over the USB cable (Chrome or Edge) or over WiFi, and keep your settings.</span>
        <div class="sub-head">Install a file by hand</div>
        <span class="hint">Pick a <span class="mono">-update.bin</span> file (or Arduino IDE's exported <span class="mono">RombyLamp.ino.bin</span>). Over WiFi only.</span>
        <div class="banner reveal" id="fwUsbNote">Connected over USB: use Check for updates above, or the web flasher (address 0x10000).</div>
        <div class="field reveal open" id="fwFileWrap"><label for="fwFile">Firmware file (.bin)</label><input type="file" id="fwFile" accept=".bin,application/octet-stream"></div>
        <div class="prog keep off" id="fwProg"><span id="fwBar"></span></div>
        <span class="hint" id="fwMsg"></span>
        <div class="row" style="flex-wrap:wrap;gap:8px;justify-content:flex-start">
          <button class="btn keep" id="fwUpload">Install update</button>
          <button class="btn" id="reboot">Restart lamp</button>
        </div>
        <span class="hint">Or upload from Arduino IDE: Tools → Port → <span class="mono">rombylamp</span> network port, password <span class="mono">rombylamp</span>.</span>
      </div>
    </details>
  </div>

  <footer class="foot">
    <span>Romby Lamp · <span id="footFw">Demo</span> · Designed by Romby with Claude</span>
    <span class="footer-brand"><span>Romby Innovations</span></span>
  </footer>
</div>
<div class="toast" id="toast" role="status" aria-live="polite"></div>

<script>
(() => {
'use strict';
const $ = id => document.getElementById(id);
const MODES = ['Circadian', 'Progress bar', 'Fixed white'];

/* theme: Dark (default) or Grey, remembered on this device */
function setTheme(t) {
  document.documentElement.classList.toggle('dim', t === 'dim');
  document.querySelectorAll('[data-theme-btn]').forEach(b => b.classList.toggle('active', b.dataset.themeBtn === t));
  try { localStorage.setItem('rombyLampTheme', t); } catch (e) {}
  if (typeof drawCurve === 'function') setTimeout(drawCurve, 320);
}
try { setTheme(localStorage.getItem('rombyLampTheme') === 'dim' ? 'dim' : 'dark'); } catch (e) { setTheme('dark'); }
document.querySelectorAll('[data-theme-btn]').forEach(b => b.addEventListener('click', () => setTheme(b.dataset.themeBtn)));

/* rolling digits: changed characters roll up and out, right to left like an odometer */
const escH = c => c === '<' ? '&lt;' : c === '&' ? '&amp;' : c;
function setRoll(el, text) {
  text = String(text);
  if (el._t === text) return;
  const prev = el._t; el._t = text;
  const paint = t => { el.innerHTML = [...t].map(ch => `<span class="rd"><span class="rd-i">${escH(ch)}</span></span>`).join(''); };
  if (prev == null) { paint(text); return; }
  if (prev.length !== text.length) {
    // different shape (e.g. "5,500 K" to "BUSY"): quick fade instead of a jump
    el.style.transition = 'opacity .18s ease'; el.style.opacity = 0;
    clearTimeout(el._rt); el._rt = setTimeout(() => { paint(el._t); el.style.opacity = 1; }, 180);
    return;
  }
  if (el._rt && el.style.opacity === '0') return;   // mid-fade: the fade will paint the latest text
  const slots = el.children; let order = 0;
  for (let i = text.length - 1; i >= 0; i--) {
    if (prev[i] === text[i]) continue;
    const slot = slots[i], delay = (order++) * 70;
    const old = slot.lastElementChild;
    const nu = document.createElement('span');
    nu.className = 'rd-i rd-in'; nu.textContent = text[i]; nu.style.animationDelay = delay + 'ms';
    if (old) { old.className = 'rd-i rd-out'; old.style.animationDelay = delay + 'ms'; setTimeout(() => old.remove(), 680 + delay); }
    slot.appendChild(nu);
    setTimeout(() => { if (nu.classList.contains('rd-in')) { nu.className = 'rd-i'; nu.style.animationDelay = ''; } }, 680 + delay);
  }
}
const row = (l, v) => [l, v];
/* open/close a block smoothly (it slides open and fades in) */
function reveal(el, on) { el.classList.toggle('open', !!on); el.inert = !on; }
/* fade out/in an element that keeps its place in the layout */
function keep(el, on) { el.classList.toggle('off', !on); el.inert = !on; }
/* swap text with a quick fade instead of a jump */
function fadeText(el, text) {
  if (el._ft === text) return;
  const first = el._ft == null; el._ft = text;
  if (first) { el.textContent = text; return; }
  clearTimeout(el._ftT); el.style.opacity = 0;
  el._ftT = setTimeout(() => { el.textContent = el._ft; el.style.opacity = 1; }, 180);
}
/* the readout rows under the mode title: four fixed slots, so nothing below moves */
function setMeta(pairs) {
  const slots = $('plateMeta').children;
  for (let i = 0; i < slots.length; i++) {
    const sl = slots[i], p = pairs[i], lab = sl.children[0], val = sl.children[1];
    if (!p) { if (sl._label != null) { clearTimeout(sl._t); sl._swap = false; sl.classList.add('empty'); sl._label = null; } continue; }
    if (sl._label !== p[0]) {
      const wasShown = sl._label != null;
      sl._label = p[0]; sl._v = p[1];
      clearTimeout(sl._t);
      if (wasShown) {                       // different row in this slot: fade out, swap, fade in
        sl._swap = true; sl.classList.add('empty');
        sl._t = setTimeout(() => { lab.textContent = sl._label; val.innerHTML = sl._v; sl._swap = false; sl.classList.remove('empty'); }, 220);
      } else {                              // empty slot: fill it, then fade in
        lab.textContent = p[0]; val.innerHTML = p[1];
        requestAnimationFrame(() => { if (sl._label != null) sl.classList.remove('empty'); });
      }
    } else if (sl._v !== p[1]) {            // same row, new value: update in place
      sl._v = p[1];
      if (!sl._swap) val.innerHTML = p[1];
    }
  }
}
function fmtClock(m) {
  const tot = Math.floor(m * 60) % 86400, h = Math.floor(tot / 3600), mi = Math.floor(tot / 60) % 60, se = tot % 60;
  return `${String((h % 12) || 12).padStart(2, '0')}:${String(mi).padStart(2, '0')}:${String(se).padStart(2, '0')} ${h < 12 ? 'AM' : 'PM'}`;
}

/* ---------- state ---------- */
const defaults = () => ({
  v: 'demo', on: true, bri: 70, mode: 1, manualK: 4000,
  teams: { red: false, test: false, manual: false, seen: false, avail: '', act: '', ago: -1 },
  time: { valid: true, min: 0, wday: 3 }, preview: -1, inWork: false, offIn: -1,
  net: { sta: false, ssid: '', ip: '0.0.0.0', rssi: 0, ap: true, apSsid: 'RombyLamp-4F2A', apIp: '192.168.4.1' },
  cfg: { stripType: 1, pixels: 60, order: 0, rgbOrder: 0, cctOrder: 0, cctFirst: false, warmK: 3000, coolK: 6500, reverse: false, whiteK: 4000, maxBri: 80, progStart: 480, progEnd: 1050,
    weekdaysOnly: true, autoSchedule: true, afterHoursMin: 120, earlyHrs: 3, fadeMin: 1, glow: 10, curveMin: [360, 480, 720, 1020, 1260], curveK: [2700, 6000, 5000, 3500, 2400],
    redTrigger: 2, redBri: 60, redStyle: 0, redEndPct: 10, redPulse: true, redPulseMin: 10, redPulseSec: 4, redWhenOff: true, autoClearHrs: 4, encSteps: 4, encReverse: false, lowTx: true, keepAP: false }
});
let st = defaults();
let live = false, gotAt = performance.now();
let viewMin = null;          // preview time (null = now)
let playing = false, identifyAt = 0, testUntil = 0;
let demoOffAt = 0, demoLastIn = null;
let fwPct = null;   // firmware upload progress shown on the strip preview   // demo-mode copy of the lamp's schedule
let disp = [];               // eased display colours

/* ---------- colour engine (same maths as the firmware) ---------- */
function k2rgb(k) {
  const t = Math.min(Math.max(k, 1000), 12000) / 100; let r, g, b;
  if (t <= 66) { r = 255; g = 99.4708025861 * Math.log(t) - 161.1195681661; b = t <= 19 ? 0 : 138.5177312231 * Math.log(t - 10) - 305.0447927307; }
  else { r = 329.698727446 * Math.pow(t - 60, -0.1332047592); g = 288.1221695283 * Math.pow(t - 60, -0.0755148492); b = 255; }
  return [r, g, b].map(v => Math.min(Math.max(v, 0), 255) / 255);
}
/* colour temperature -> R, G, B, W1, W2 for the selected strip (same maths as the firmware) */
function kOut(k, c) {
  if (c.stripType !== 1) return [...k2rgbw(k, c.whiteK), 0];
  const w = c.warmK, cl = c.coolK;
  if (k >= w) {
    const kk = Math.min(k, cl);
    const t = Math.min(1, Math.max(0, (1e6 / w - 1e6 / kk) / (1e6 / w - 1e6 / cl)));
    return [0, 0, 0, 1 - t, t];
  }
  const f = Math.min(1, Math.max(0, (w - k) / Math.max(1, w - 1800)));
  const T = k2rgb(k);
  return [T[0] * 0.55 * f, T[1] * 0.55 * f, T[2] * 0.55 * f, 1 - 0.55 * f, 0];
}
/* what that drive level looks like on screen */
function whites(c) { return c.stripType === 1 ? [k2rgb(c.warmK), k2rgb(c.coolK)] : [k2rgb(c.whiteK), [0, 0, 0]]; }
function k2rgbw(k, wK) {
  const T = k2rgb(k), W = k2rgb(wK); let a = 1e9;
  for (let i = 0; i < 3; i++) if (W[i] > 0.001) a = Math.min(a, T[i] / W[i]);
  if (a > 1e8) a = 0;
  const rgb = T.map((v, i) => Math.max(0, v - a * W[i]));
  const m = Math.max(a, ...rgb) || 1;
  return [rgb[0] / m, rgb[1] / m, rgb[2] / m, a / m];
}
const smooth = f => f * f * (3 - 2 * f);
function circK(m, c) {
  const n = 5;
  for (let i = 0; i < n; i++) {
    let t0 = c.curveMin[i], t1 = c.curveMin[(i + 1) % n], mm = m;
    if (i === n - 1) { t1 += 1440; if (mm < t0) mm += 1440; }
    if (mm >= t0 && mm < t1 && t1 > t0) return c.curveK[i] + (c.curveK[(i + 1) % n] - c.curveK[i]) * smooth((mm - t0) / (t1 - t0));
  }
  return c.curveK[0];
}
const briLevel = p => p <= 0 ? 0 : 0.006 + 0.994 * Math.pow(p / 100, 2.2);
const workdayOk = (wd, c) => !(c.weekdaysOnly && (wd === 0 || wd === 6));
const progActive = (m, wd, c) => workdayOk(wd, c) && m >= c.progStart && m < c.progEnd && c.progEnd > c.progStart;
/* the schedule's window opens fadeMin early so the lamp is at full brightness right at work start */
const inSched = (m, wd, c) => workdayOk(wd, c) && c.progEnd > c.progStart && m >= c.progStart - c.fadeMin && m < c.progEnd;
function schedFade(m, wd, c, fadeIn) {
  if (!c.autoSchedule || !c.fadeMin || !workdayOk(wd, c)) return 1;
  const s = c.progStart, e = c.progEnd, f = c.fadeMin;
  if (fadeIn && m >= s - f && m < s) return Math.min(1, Math.max(0, (m - (s - f)) / f));
  if (m >= e - f && m < e) return Math.min(1, Math.max(0, (e - m) / f));
  return 1;
}

function nowMin() {
  if (live && st.time.valid) return (st.time.min + (performance.now() - gotAt) / 60000) % 1440;
  const d = new Date(); return d.getHours() * 60 + d.getMinutes() + d.getSeconds() / 60;
}
function nowWday() { return live ? st.time.wday : new Date().getDay(); }

/* demo mode only: the same on/off schedule the firmware runs */
let demoSchedOn = true;
function demoSetPower(on) {
  st.on = on; demoOffAt = 0; demoSchedOn = false;
  const c = st.cfg, m = nowMin(), wd = nowWday();
  if (!on || !c.autoSchedule || inSched(m, wd, c)) return;
  if (workdayOk(wd, c) && m < c.progStart && m >= c.progStart - c.earlyHrs * 60) return;
  if (c.afterHoursMin) demoOffAt = Date.now() + c.afterHoursMin * 60000;
}
function demoSchedule() {
  if (live) return;
  const c = st.cfg, inW = inSched(nowMin(), nowWday(), c);
  if (demoOffAt && Date.now() >= demoOffAt) { demoOffAt = 0; st.on = false; syncUI(); }
  if (!c.autoSchedule) { demoLastIn = null; }
  else if (demoLastIn === null) demoLastIn = inW;
  else if (inW !== demoLastIn) { demoLastIn = inW; demoOffAt = 0; st.on = inW; demoSchedOn = inW; syncUI(); }
  st.inWork = progActive(nowMin(), nowWday(), c); st.offIn = demoOffAt ? Math.round((demoOffAt - Date.now()) / 1000) : -1;
}

/* returns pixel RGBW array + info for the readout (same order of layers as the firmware) */
function frame() {
  const c = st.cfg, n = c.pixels, px = new Array(n);
  const info = { k: null, lit: null, kind: 'light', red: false };
  const t = performance.now();
  if (fwPct != null) {
    const lit = fwPct * n;
    for (let i = 0; i < n; i++) { const a = i < lit ? 0.25 : 0.02; px[i] = [0, a * 0.35, a, 0, 0]; }
    info.kind = 'update'; return { px, info };
  }
  if (identifyAt && t - identifyAt < (c.stripType === 1 ? 5000 : 4000)) {
    const ch = Math.floor((t - identifyAt) / 1000);
    for (let i = 0; i < n; i++) px[i] = [0, 1, 2, 3, 4].map(j => j === ch ? 0.35 : 0);
    info.kind = 'identify'; info.ch = ch; return { px, info };
  }
  if (st.teams.test && !live && testUntil && t > testUntil) st.teams.test = false;
  const m = viewMin ?? nowMin(), wd = viewMin != null ? 3 : nowWday();
  const previewSched = viewMin != null && c.autoSchedule;
  const on = previewSched ? inSched(m, wd, c) : st.on;             // previews show what the schedule does
  const fromSched = previewSched || (live ? st.schedOn !== false : demoSchedOn);
  info.m = m; info.wd = wd;

  if (!on) { for (let i = 0; i < n; i++) px[i] = [0, 0, 0, 0, 0]; info.kind = 'off'; }
  else {
    const k = st.mode === 2 ? st.manualK : (live && !st.time.valid && viewMin == null) ? (c.stripType === 1 ? c.warmK : 3000) : circK(m, c);
    const fade = schedFade(m, wd, c, fromSched);
    const col = kOut(k, c), lv = briLevel(st.bri * fade) * c.maxBri / 100;
    let lit = n;
    if (st.mode === 1 && inSched(m, wd, c)) {
      const f = Math.min(Math.max((m - c.progStart) / (c.progEnd - c.progStart), 0), 1);
      lit = f * n; info.lit = f;
    }
    const glow = c.glow / 100;
    for (let i = 0; i < n; i++) {
      const amt = i + 1 <= lit ? 1 : (i < lit ? glow + (1 - glow) * (lit - i) : glow);
      const p = c.reverse ? n - 1 - i : i;
      px[p] = col.map(v => v * lv * amt);
    }
    info.k = k; info.fade = fade;
  }

  const red = st.teams.red || st.teams.test || st.teams.manual;
  if (red && (st.on || c.redWhenOff)) {
    let lv = briLevel(c.redBri) * c.maxBri / 100;
    const lo = (c.redPulseMin ?? 10) / 100, per = (c.redPulseSec || 4) * 1000;
    const pulse = c.redPulse ? lo + (1 - lo) * (0.5 - 0.5 * Math.cos(Date.now() * 2 * Math.PI / per)) : 1;
    lv *= pulse;
    const e = c.redStyle === 1 ? n : Math.max(1, Math.round(n * c.redEndPct / 100));
    for (let i = 0; i < n; i++) if (i < e || i >= n - e) px[i] = [lv, 0, 0, 0, 0];
    info.red = true; info.redE = e; info.pulse = pulse;
    if (c.redStyle === 1) info.kind = 'red';
  }
  return { px, info };
}

/* RGBW drive levels -> what it looks like on screen */
function toScreen(v, W) {
  const a = W[0], b2 = W[1], w2 = v[4] || 0;
  const r = v[0] + v[3] * a[0] + w2 * b2[0], g = v[1] + v[3] * a[1] + w2 * b2[1], b = v[2] + v[3] * a[2] + w2 * b2[2];
  return [r, g, b].map(x => Math.min(1, Math.pow(Math.max(0, x), 0.5)));
}

/* ---------- drawing ---------- */
const sc = $('strip'), sctx = sc.getContext('2d');
function fit(c) {
  const dpr = Math.min(window.devicePixelRatio || 1, 2), w = c.clientWidth, h = c.clientHeight;
  if (c.width !== Math.round(w * dpr) || c.height !== Math.round(h * dpr)) { c.width = Math.round(w * dpr); c.height = Math.round(h * dpr); }
  return { w, h, dpr };
}
function drawStrip(cols, info) {
  const { w, h, dpr } = fit(sc);
  const ctx = sctx; ctx.setTransform(dpr, 0, 0, dpr, 0, 0); ctx.clearRect(0, 0, w, h);
  const n = cols.length, pad = 26, len = w - pad * 2, y = Math.round(h * 0.42), th = 3, top = y - 1;
  // one continuous bar; each pixel gets a hard-edged slice so nothing smears
  const css = c => `rgb(${c[0] * 255 | 0},${c[1] * 255 | 0},${c[2] * 255 | 0})`;
  const g = ctx.createLinearGradient(pad, 0, pad + len, 0);
  cols.forEach((c, i) => { const col = css(c); g.addColorStop(i / n, col); g.addColorStop((i + 1) / n, col); });
  // unlit hairline, so an off strip still reads as a bar
  ctx.fillStyle = 'rgba(255,255,255,0.09)'; ctx.fillRect(pad, top + 1, len, 1);
  // light falling down onto the desk only: a short fade below the bar, nothing above
  ctx.fillStyle = g;
  for (let k = 0; k < 26; k++) { ctx.globalAlpha = 0.2 * Math.pow(1 - k / 26, 2); ctx.fillRect(pad, top + th + k, len, 1); }
  ctx.globalAlpha = 1;
  // the bar
  ctx.fillRect(pad, top, len, th);
}
function roundRect(ctx, x, y, w, h, r) { ctx.beginPath(); ctx.moveTo(x + r, y); ctx.arcTo(x + w, y, x + w, y + h, r); ctx.arcTo(x + w, y + h, x, y + h, r); ctx.arcTo(x, y + h, x, y, r); ctx.arcTo(x, y, x + w, y, r); ctx.closePath(); }

const fmtK = k => Math.round(k / 10) * 10 + ' K';
const fmtKc = k => (Math.round(k / 10) * 10).toLocaleString() + ' K';
function fmtT(m) { m = ((Math.round(m) % 1440) + 1440) % 1440; const h = Math.floor(m / 60), mi = m % 60, ap = h < 12 ? 'AM' : 'PM'; return `${(h % 12) || 12}:${String(mi).padStart(2, '0')} ${ap}`; }
const toHHMM = m => `${String(Math.floor(m / 60) % 24).padStart(2, '0')}:${String(m % 60).padStart(2, '0')}`;
const fromHHMM = s => { const [h, m] = s.split(':').map(Number); return h * 60 + m; };

function render() {
  const { px, info } = frame();
  const wRgb = whites(st.cfg);
  const red = [1, 68 / 255, 51 / 255];   // the dashboard's red, so the strip's busy ends match the alert box
  const target = px.map((v, i) => (info.red && (i < info.redE || i >= px.length - info.redE)) ? red.map(x => x * info.pulse) : toScreen(v, wRgb));
  if (disp.length !== target.length) disp = target.map(c => c.slice());
  else disp = disp.map((c, i) => c.map((v, j) => v + (target[i][j] - v) * 0.25));
  drawStrip(disp, info);

  const label = viewMin != null ? `Preview · ${fmtT(viewMin)}` : `Live · lamp time`;
  $('viewLabel').textContent = label;
  const meta = [];
  if (info.kind === 'red') { setRoll($('rdK'), 'BUSY'); fadeText($('rdMode'), 'Busy light'); }
  else if (info.kind === 'off') {
    setRoll($('rdK'), 'OFF');
    fadeText($('rdMode'), st.cfg.autoSchedule && !progActive(info.m, viewMin != null ? 3 : nowWday(), st.cfg) ? 'Outside work hours' : 'Lamp off');
    if (viewMin == null && st.cfg.autoSchedule) meta.push(row('Turns on', fmtT(st.cfg.progStart) + (st.cfg.weekdaysOnly ? ' · weekdays' : '')));
  }
  else if (info.kind === 'update') { setRoll($('rdK'), String(Math.round(fwPct * 100)).padStart(3, ' ') + '%'); fadeText($('rdMode'), 'Installing update'); }
  else if (info.kind === 'identify') { setRoll($('rdK'), (st.cfg.stripType === 1 ? ['RED', 'GREEN', 'BLUE', 'WARM', 'COOL'] : ['RED', 'GREEN', 'BLUE', 'WHITE'])[info.ch]); fadeText($('rdMode'), 'Colour test'); }
  else {
    setRoll($('rdK'), fmtKc(info.k)); fadeText($('rdMode'), MODES[st.mode]);
    meta.push(info.fade < 1 ? row('Brightness', `${Math.round(st.bri * info.fade)}% · fading to ${st.bri}%`) : row('Brightness', st.bri + '%'));
    if (st.mode === 1) {
      if (info.lit != null) {
        const left = st.cfg.progEnd - info.m;
        meta.push(row('Workday', Math.round(info.lit * 100) + '% done'), row('Time left', `${Math.floor(left / 60)} h ${Math.floor(left % 60)} min`));
      } else meta.push(row('Progress', 'Outside work hours · full strip'));
    }
    if (viewMin == null && st.cfg.autoSchedule) {
      if (st.offIn > 0) meta.push(row('Auto-off in', `${Math.floor(st.offIn / 3600)} h ${Math.floor(st.offIn % 3600 / 60)} min`));
      else if (st.on) meta.push(row('On until', fmtT(st.cfg.progEnd)));
    }
  }
  if (live && !st.time.valid) meta.push(row('Clock', '<span style="color:var(--red)">Not set</span>'));
  setMeta(meta);
  fadeText($('bigLabel'), info.kind === 'update' ? 'Upload' : info.kind === 'identify' ? 'Colour test' : (info.kind === 'red' || info.kind === 'off') ? 'Status' : 'Colour temperature');
  setRoll($('clock'), fmtClock(nowMin()));
  if (connMode === 'live') updateStatusBar();
  $('feature').classList.toggle('alert', info.red);
  $('busyTag').classList.toggle('on', info.red);
  $('busyText').textContent = st.teams.test ? 'Test call' : (st.teams.manual && !st.teams.red) ? 'Busy' : 'In a meeting';

  const tm = viewMin ?? nowMin();
  $('thumb').style.left = (tm / 1440 * 100) + '%';
  $('nowMark').style.left = (nowMin() / 1440 * 100) + '%';
  $('track').setAttribute('aria-valuenow', Math.round(tm));
  $('track').setAttribute('aria-valuetext', fmtT(tm));
}

/* scrubber gradient + work window */
function paintTrack() {
  const c = st.cfg, wRgb = whites(c), stops = [];
  for (let m = 0; m <= 1440; m += 30) {
    const s = toScreen(kOut(circK(m % 1440, c), c), wRgb).map(v => v * 255 | 0);
    stops.push(`rgb(${s[0]},${s[1]},${s[2]}) ${(m / 1440 * 100).toFixed(2)}%`);
  }
  $('track').style.background = `linear-gradient(90deg, ${stops.join(',')})`;
  $('win').style.left = (c.progStart / 1440 * 100) + '%';
  $('win').style.width = ((c.progEnd - c.progStart) / 1440 * 100) + '%';
  $('win').classList.toggle('off', st.mode !== 1);
  drawCurve();
}
function drawCurve() {
  const cv = $('curve'); if (!cv.clientWidth) return;
  const { w, h, dpr } = fit(cv), ctx = cv.getContext('2d'), c = st.cfg;
  ctx.setTransform(dpr, 0, 0, dpr, 0, 0); ctx.clearRect(0, 0, w, h);
  const cs = getComputedStyle(document.documentElement);
  const line = cs.getPropertyValue('--gray-3').trim(), muted = cs.getPropertyValue('--gray-2').trim(), fg = cs.getPropertyValue('--white').trim();
  const L = 40, R = 8, T = 8, B = 20, kMin = 1800, kMax = 7000;
  const X = m => L + m / 1440 * (w - L - R), Y = k => T + (1 - (k - kMin) / (kMax - kMin)) * (h - T - B);
  ctx.font = '10px ' + cs.getPropertyValue('--font-mono'); ctx.fillStyle = muted; ctx.strokeStyle = line; ctx.lineWidth = 1;
  [2000, 4000, 6000].forEach(k => { ctx.beginPath(); ctx.moveTo(L, Y(k)); ctx.lineTo(w - R, Y(k)); ctx.stroke(); ctx.fillText(k + 'K', 2, Y(k) + 3); });
  [0, 360, 720, 1080, 1440].forEach(m => ctx.fillText(['12a', '6a', '12p', '6p', '12a'][m / 360], Math.min(X(m) - 8, w - 24), h - 5));
  const wRgb = whites(c);
  for (let m = 0; m < 1440; m += 6) {
    const k = circK(m, c), s = toScreen(kOut(k, c), wRgb).map(v => v * 255 | 0);
    ctx.fillStyle = `rgba(${s[0]},${s[1]},${s[2]},0.55)`;
    ctx.fillRect(X(m), Y(k), (w - L - R) / 240 + 0.5, h - B - Y(k));
  }
  ctx.strokeStyle = fg; ctx.lineWidth = 1.5; ctx.beginPath();
  for (let m = 0; m <= 1440; m += 6) { const k = circK(m % 1440, c); m ? ctx.lineTo(X(m), Y(k)) : ctx.moveTo(X(m), Y(k)); }
  ctx.stroke();
  ctx.fillStyle = fg;
  c.curveMin.forEach((m, i) => { ctx.beginPath(); ctx.arc(X(m), Y(c.curveK[i]), 3.5, 0, Math.PI * 2); ctx.fill(); });
}

/* ---------- sync UI from state ---------- */
const focused = el => document.activeElement === el;
function syncUI() {
  const c = st.cfg;
  const usb = live && st.via === 'usb', br = st.bridge;
  $('fw').textContent = 'FW ' + st.v;
  $('footFw').textContent = live ? 'Firmware ' + st.v : 'Demo';
  $('lightTag').textContent = st.on ? 'On' : 'Off';
  $('power').checked = st.on;
  if (!focused($('bri'))) $('bri').value = st.bri;
  $('briVal').textContent = st.bri + '%';
  document.querySelectorAll('.seg button').forEach(b => b.setAttribute('aria-pressed', String(+b.dataset.mode === st.mode)));
  reveal($('manualWrap'), st.mode === 2);
  if (!focused($('mk'))) $('mk').value = st.manualK;
  $('mkVal').textContent = st.manualK + ' K';
  $('modeHint').textContent = st.mode === 0 ? 'Cool in the morning, warm toward evening. Follows the daylight curve.'
    : st.mode === 1 ? `Fills from empty at ${fmtT(c.progStart)} to full at ${fmtT(c.progEnd)}, still following the daylight colour.`
    : 'One colour temperature all day.';

  document.querySelectorAll('[data-set]').forEach(el => {
    if (focused(el)) return;
    const v = c[el.dataset.set];
    if (el.dataset.kind === 'bool') el.checked = !!v;
    else if (el.dataset.kind === 'time') el.value = toHHMM(v);
    else el.value = v;
  });
  reveal($('schedOpts'), c.autoSchedule);
  $('schedHint').textContent = c.autoSchedule
    ? `On ${fmtT(c.progStart)}–${fmtT(c.progEnd)}${c.fadeMin ? `, fading up from ${fmtT(c.progStart - c.fadeMin)}` : ''}${c.weekdaysOnly ? ', Monday to Friday' : ', every day'}. Switched on from ${fmtT(c.progStart - c.earlyHrs * 60)} it stays on through the workday. Any other time it ${c.afterHoursMin ? 'turns itself off after ' + (c.afterHoursMin >= 60 ? (c.afterHoursMin / 60) + ' h' : c.afterHoursMin + ' min') : 'stays on until you turn it off'}.`
    : 'The lamp only turns on and off when you tell it to.';
  const cct = c.stripType === 1;
  reveal($('cctOpts'), cct); reveal($('rgbwOpts'), !cct);
  $('pixelsLabel').textContent = cct ? 'Segments (cuttable sections)' : 'Pixels (ICs)';
  showLen();
  $('pixelsHint').textContent = cct ? 'Set automatically from the length above, or type it. Each segment is the piece between two cut marks. If the end of the strip stays bright, this is too low.' : 'One chip per pixel. Match the LED count from WLED if you used it there.';
  $('testHint').textContent = cct
    ? 'Shows red, green, blue, then warm white, then cool white, one second each. If a colour is wrong, change the RGB chip order; if the whites are swapped or one shows a colour, change the white chip order or tick "white chip comes first".'
    : 'Shows red, green, blue, then white for one second each. If the order looks wrong, change it and run again.';
  $('glowVal').textContent = c.glow + '%';
  $('redBriVal').textContent = c.redBri + '%';
  $('redEndVal').textContent = c.redEndPct + '%';
  $('redPulseMinVal').textContent = (c.redPulseMin ?? 10) + '%';
  reveal($('pulseOpts'), c.redPulse);
  reveal($('redEndWrap'), c.redStyle !== 1);
  $('maxBriVal').textContent = c.maxBri + '%';
  if (!document.querySelector('#curveRows :focus')) buildCurveRows();

  // Teams
  const red = st.teams.red || st.teams.test || st.teams.manual;
  $('busyBtn').setAttribute('aria-pressed', String(!!st.teams.manual));
  $('busyTitle').textContent = st.teams.manual ? "Busy. Tap to clear" : "I'm busy";
  $('fwSub').textContent = live ? `Version ${st.v}` : 'Updates';
  $('fwInstalled').textContent = live ? 'v' + st.v : '—';
  $('testCall').setAttribute('aria-pressed', String(st.teams.test));
  $('testTitle').textContent = st.teams.test ? 'Test call running. Tap to end' : 'Simulate a Teams call';
  $('teamsChip').textContent = red ? (st.teams.test ? 'Test call' : st.teams.red ? 'Busy · Teams' : 'Busy · manual') : 'Normal';
  $('teamsChip').classList.toggle('red', red);
  $('teamsCard').classList.toggle('alert', red);
  $('presText').textContent = st.teams.seen
    ? `${st.teams.avail || '—'}${st.teams.act ? ' · ' + st.teams.act : ''} · ${ago(st.teams.ago)}`
    : 'No update received yet';
  const host = usb ? `localhost:${(br && br.port) || 8765}` : live ? (st.net.sta ? st.net.ip : st.net.apIp) : 'rombylamp.local';
  $('presUrl').textContent = `http://${host}/api/presence`;

  // USB bridge
  reveal($('micLine'), br && br.mic);
  if (br && br.mic) $('micText').textContent = br.micBusy ? 'In use: busy light on' : 'Not in use';
  reveal($('fwUsbNote'), usb);
  reveal($('fwFileWrap'), !usb); keep($('fwUpload'), !usb);
  reveal($('wifiOpts'), !c.wifiOff);

  // Network
  $('wifiSub').textContent = c.wifiOff ? 'Off · USB only' : st.net.sta ? `Connected to ${st.net.ssid}` : (st.net.ssid ? `Trying ${st.net.ssid}` : 'Not set up');
  $('netStatus').innerHTML = st.net.sta
    ? `<span class="chip">Online</span><span class="mono">${st.net.ip}</span><span class="muted">${st.net.rssi} dBm</span>`
    : `<span class="chip">Offline</span><span class="muted">Join the hotspot below to set up WiFi</span>`;
  $('apHint').textContent = `Setup hotspot: ${st.net.apSsid}, password rombylamp, then open 192.168.4.1.` + (st.net.ap ? ' Hotspot is on now.' : '');
  if (!focused($('ssid')) && !$('ssid').value) $('ssid').value = st.net.ssid || '';
  keep($('mirrorWrap'), live);
  paintTrack();
}
function ago(s) { if (s < 0) return ''; if (s < 60) return 'just now'; if (s < 3600) return Math.round(s / 60) + ' min ago'; return Math.round(s / 3600) + ' h ago'; }

function buildCurveRows() {
  const c = st.cfg, box = $('curveRows');
  const names = ['Night → morning', 'Morning peak', 'Midday', 'Late afternoon', 'Evening'];
  box.innerHTML = c.curveMin.map((m, i) => `
    <div class="curve-row">
      <div class="field"><label for="cm${i}">${names[i]} at</label><input type="time" id="cm${i}" value="${toHHMM(m)}"></div>
      <div class="field"><label for="ck${i}">Colour temp (K)</label><input type="number" id="ck${i}" min="1800" max="7500" step="50" value="${c.curveK[i]}"></div>
    </div>`).join('');
  c.curveMin.forEach((_, i) => {
    $('cm' + i).addEventListener('change', e => sendSettings({ ['cm' + i]: fromHHMM(e.target.value) }));
    $('ck' + i).addEventListener('change', e => sendSettings({ ['ck' + i]: +e.target.value }));
  });
}

/* ---------- talking to the lamp (or the demo) ---------- */
async function post(path, params) {
  if (!live) return demoApply(path, params);
  try {
    const r = await fetch(path, { method: 'POST', body: new URLSearchParams(params) });
    const j = await r.json();
    if (j && j.cfg) { st = j; gotAt = performance.now(); syncUI(); }
    return j;
  } catch (e) { toast('Lamp did not answer. Check it is powered and on WiFi.'); }
}
function demoApply(path, p) {
  const c = st.cfg;
  if (path === '/api/set') {
    if ('on' in p) demoSetPower(p.on == 1);
    if ('bri' in p) st.bri = +p.bri;
    if ('mode' in p) st.mode = +p.mode;
    if ('k' in p) st.manualK = +p.k;
    if ('test' in p) { st.teams.test = p.test == 1; testUntil = performance.now() + 300000; }
    if ('busy' in p) { st.teams.manual = p.busy == 1; st.teams.test = false; }
    if ('clearTeams' in p) { st.teams.red = false; st.teams.test = false; st.teams.manual = false; }
    if ('identify' in p) identifyAt = performance.now();
  } else if (path === '/api/settings') {
    for (const [k, v] of Object.entries(p)) {
      const mm = k.match(/^c([mk])(\d)$/);
      if (mm) (mm[1] === 'm' ? c.curveMin : c.curveK)[+mm[2]] = +v;
      else c[k] = typeof c[k] === 'boolean' ? (v == 1) : +v;
    }
    const pairs = c.curveMin.map((m, i) => [m, c.curveK[i]]).sort((a, b) => a[0] - b[0]);
    c.curveMin = pairs.map(x => x[0]); c.curveK = pairs.map(x => x[1]);
    if ('autoSchedule' in p) demoLastIn = null;
  } else if (path === '/api/wifi') { toast('Demo: the lamp would save this and restart'); return; }
  else if (path === '/api/reboot') { toast('Demo: the lamp would restart'); return; }
  syncUI();
}
let setTimer = null, pending = {};
function sendSet(params, immediate) {
  if (!live) return post('/api/set', params);
  Object.assign(pending, params);
  clearTimeout(setTimer);
  setTimer = setTimeout(() => { const p = pending; pending = {}; post('/api/set', p); }, immediate ? 0 : 120);
}
function sendSettings(params) { post('/api/settings', params).then(() => toast('Saved')); }

const realFetch = window.fetch.bind(window);
/* ---------- online updates: builds are published to GitHub, the dashboard installs them ---------- */
const FW_BASE = 'https://raw.githubusercontent.com/Romby6/romby-lamp/main/firmware/';
const ESPTOOL_URL = 'https://cdn.jsdelivr.net/npm/esptool-js@0.6.1/bundle.js';
let latest = null, updBusy = false;
function verNum(v) { return String(v || '0').split('.').map(n => parseInt(n, 10) || 0).reduce((a, n) => a * 1000 + n, 0); }
/* MD5 (for checking the download and the flash write) */
function md5hex(bytes) {
  const K = new Uint32Array(64), S = [7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21];
  for (let i = 0; i < 64; i++) K[i] = Math.floor(Math.abs(Math.sin(i + 1)) * 4294967296);
  const n = bytes.length, total = (((n + 8) >> 6) + 1) * 64, m = new Uint8Array(total);
  m.set(bytes); m[n] = 0x80;
  const bits = n * 8, dv = new DataView(m.buffer);
  dv.setUint32(total - 8, bits >>> 0, true); dv.setUint32(total - 4, Math.floor(bits / 4294967296), true);
  let a0 = 0x67452301, b0 = 0xefcdab89, c0 = 0x98badcfe, d0 = 0x10325476;
  const M = new Uint32Array(16);
  for (let off = 0; off < total; off += 64) {
    for (let i = 0; i < 16; i++) M[i] = dv.getUint32(off + i * 4, true);
    let A = a0, B = b0, C = c0, D = d0;
    for (let i = 0; i < 64; i++) {
      let F, g;
      if (i < 16) { F = (B & C) | (~B & D); g = i; }
      else if (i < 32) { F = (D & B) | (~D & C); g = (5 * i + 1) % 16; }
      else if (i < 48) { F = B ^ C ^ D; g = (3 * i + 5) % 16; }
      else { F = C ^ (B | ~D); g = (7 * i) % 16; }
      F = (F + A + K[i] + M[g]) >>> 0; A = D; D = C; C = B;
      B = (B + ((F << S[i]) | (F >>> (32 - S[i])))) >>> 0;
    }
    a0 = (a0 + A) >>> 0; b0 = (b0 + B) >>> 0; c0 = (c0 + C) >>> 0; d0 = (d0 + D) >>> 0;
  }
  return [a0, b0, c0, d0].map(x => [0, 8, 16, 24].map(sh => ((x >>> sh) & 255).toString(16).padStart(2, '0')).join('')).join('');
}
async function checkUpdate(quiet) {
  try {
    const r = await realFetch(FW_BASE + 'latest.json?t=' + Date.now(), { cache: 'no-store' });
    if (!r.ok) throw new Error('HTTP ' + r.status);
    latest = await r.json();
  } catch (e) {
    $('fwOnline').textContent = 'Could not check (no internet?)';
    if (!quiet) toast('Could not reach the update server. Check your internet connection.');
    return;
  }
  const have = live ? st.v : null, newer = have && verNum(latest.version) > verNum(have);
  $('fwOnline').textContent = 'v' + latest.version + (newer ? ' · update available' : have ? ' · up to date' : '');
  $('updTitle').textContent = `Update available: v${have} → v${latest.version}`;
  $('updNotes').textContent = latest.notes ? ' · ' + latest.notes : '';
  if (!updBusy) reveal($('updNotice'), !!newer && canUpdate());
  if (!quiet && !newer && have) toast('The lamp is up to date');
}
function canUpdate() { return live && (!!usb.port || st.via === 'wifi'); }
function updSet(pct, msg) {
  keep($('updProg'), pct != null); $('updBar').style.width = ((pct || 0) * 100) + '%';
  if (msg != null) $('updMsg').textContent = msg;
  fwPct = pct;   // also shows on the strip preview
}
async function installUpdate() {
  if (!latest || updBusy) return;
  updBusy = true; $('updBtn').disabled = true;
  try {
    updSet(0, 'Downloading v' + latest.version + '…');
    const r = await realFetch(FW_BASE + latest.update + '?v=' + encodeURIComponent(latest.version), { cache: 'no-store' });
    if (!r.ok) throw new Error('download failed (HTTP ' + r.status + ')');
    const data = new Uint8Array(await r.arrayBuffer());
    if (latest.md5 && md5hex(data) !== latest.md5) throw new Error('the download was damaged, try again');
    if (usb.port) await installOverUsb(data);
    else await installOverWifi(data);
    updSet(1, 'Installed v' + latest.version + '. The lamp is restarting…');
    toast('Lamp updated to v' + latest.version);
    setTimeout(() => { updSet(null, ''); reveal($('updNotice'), false); checkUpdate(true); }, 9000);
  } catch (e) {
    updSet(null, 'Update stopped: ' + e.message + '. The lamp keeps its current version; you can try again.');
  } finally {
    updBusy = false; $('updBtn').disabled = false;
  }
}
async function installOverUsb(data) {
  const port = usb.port;
  usbAuto = false;
  await usbClose();                                     // hand the cable to the flasher
  updSet(0, 'Loading the flasher…');
  const { ESPLoader, Transport } = await import(ESPTOOL_URL);
  const transport = new Transport(port, false);
  const log = [];
  const terminal = { clean() {}, writeLine(l) { log.push(l); }, write(t) { log.push(t); } };
  const loader = new ESPLoader({ transport, baudrate: 115200, terminal, debugLogging: false });
  try {
    updSet(0, 'Connecting to the lamp…');
    await loader.main();
    updSet(0, 'Installing v' + latest.version + '…');
    await loader.writeFlash({
      fileArray: [{ data, address: 0x10000 }], flashMode: 'keep', flashFreq: 'keep', flashSize: 'keep',
      eraseAll: false, compress: true,
      reportProgress: (i, w, t) => updSet(t ? w / t : 0, `Installing v${latest.version}… ${Math.round(100 * w / (t || 1))}%`),
      calculateMD5Hash: img => md5hex(img),
    });
    // restart into the new firmware: hold reset (RTS) for 100 ms with IO0 released (DTR off), then let go.
    // (esptool-js's own hard reset only releases RTS, which doesn't restart the chip on this board.)
    await transport.setDTR(false);
    await transport.setRTS(true);
    await new Promise(r => setTimeout(r, 120));
    await transport.setRTS(false);
  } finally {
    try { await transport.disconnect(); } catch (e) {}
    usbAuto = true;
    setTimeout(() => usbReopen(port), 2500);            // reconnect the dashboard
  }
}
function installOverWifi(data) {
  return new Promise((resolve, reject) => {
    const fd = new FormData(); fd.append('firmware', new Blob([data]), 'RombyLamp.bin');
    const xhr = new XMLHttpRequest();
    xhr.open('POST', '/api/update');
    xhr.upload.onprogress = e => { if (e.lengthComputable) updSet(e.loaded / e.total, `Installing v${latest.version}… ${Math.round(100 * e.loaded / e.total)}%`); };
    xhr.onload = () => { let j = {}; try { j = JSON.parse(xhr.responseText); } catch (e) {} j.ok ? resolve() : reject(new Error(j.error || 'the lamp rejected the file')); };
    xhr.onerror = () => reject(new Error('lost the connection to the lamp'));
    xhr.send(fd);
  });
}

$('updBtn').addEventListener('click', installUpdate);
$('fwCheck').addEventListener('click', () => checkUpdate(false));

/* ---------- direct USB (Chrome/Edge Web Serial): talk to the lamp over its cable, no bridge needed ---------- */
const usb = { port: null, writer: null, reader: null, buf: '', pending: null, queue: Promise.resolve() };
function usbLine(line) {
  if (usb.pending && usb.pending.match(line)) { const p = usb.pending; usb.pending = null; p.done(line); }
}
async function usbReadLoop() {
  const dec = new TextDecoder();
  try {
    usb.reader = usb.port.readable.getReader();
    for (;;) {
      const { value, done } = await usb.reader.read();
      if (done) break;
      usb.buf += dec.decode(value, { stream: true });
      let i;
      while ((i = usb.buf.indexOf('\n')) >= 0) { const line = usb.buf.slice(0, i).replace(/\r$/, ''); usb.buf = usb.buf.slice(i + 1); usbLine(line); }
      if (usb.buf.length > 8000) usb.buf = usb.buf.slice(-2000);
    }
  } catch (e) {}
  usbLost();
}
function usbSend(text, timeout = 4000, kind = 'reply') {
  const run = () => new Promise(res => {
    const match = kind === 'pong' ? l => l.startsWith('pong RombyLamp') : l => l.startsWith('@');
    const t = setTimeout(() => { if (usb.pending && usb.pending.done === done) usb.pending = null; res(null); }, timeout);
    function done(l) { clearTimeout(t); res(l); }
    usb.pending = { match, done };
    if (!usb.writer) { clearTimeout(t); usb.pending = null; res(null); return; }
    usb.writer.write(new TextEncoder().encode(text + '\n')).catch(() => { clearTimeout(t); usb.pending = null; res(null); });
  });
  const p = usb.queue.then(run, run); usb.queue = p.catch(() => {}); return p;
}
window.fetch = async (url, opts = {}) => {
  if (usb.port && typeof url === 'string' && url.startsWith('/api/')) {
    let [path, q = ''] = url.split('?');
    if (opts.body instanceof URLSearchParams) q = [q, opts.body.toString()].filter(Boolean).join('&');
    if (path === '/api/update') return { ok: true, json: async () => ({ ok: false, error: 'Over USB, flash with the web flasher instead' }) };
    const line = await usbSend(q ? path + '?' + q : path);
    if (!line) throw new Error('Lamp not answering over USB');
    const txt = line.slice(1);
    return { ok: true, status: 200, json: async () => JSON.parse(txt), text: async () => txt };
  }
  return realFetch(url, opts);
};
async function usbConnect() {
  let port;
  try { port = await navigator.serial.requestPort(); } catch (e) { return; }   // picker closed
  try { await port.open({ baudRate: 115200 }); }
  catch (e) { toast('Could not open the port. Close the web flasher or any other app using it.'); return; }
  try { await port.setSignals({ dataTerminalReady: false, requestToSend: false }); } catch (e) {}
  usb.port = port; usb.writer = port.writable.getWriter(); usb.buf = '';
  usbReadLoop();
  $('usbBtn').textContent = 'Connecting…'; $('usbBtn').disabled = true;
  // opening the port can restart the ESP32: keep asking for up to 8 seconds
  let ok = false;
  for (let i = 0; i < 10 && !ok; i++) ok = !!(await usbSend('ping', 800, 'pong'));
  $('usbBtn').disabled = false;
  if (!ok) { toast('No answer from the lamp. Check the cable, then try again.'); await usbClose(); return; }
  usbSend('/api/time?epoch=' + Math.floor(Date.now() / 1000) + '&force=1');
  $('usbBtn').textContent = 'Disconnect USB';
  bridgeInfo = null;
  reveal($('demoBanner'), false);
  live = false;                       // let poll() treat this as a fresh connection
  if (!polling) polling = setInterval(poll, 1500);
  await poll();
}
async function usbClose() {
  const p = usb.port; usb.port = null;
  try { if (usb.reader) await usb.reader.cancel(); } catch (e) {}
  try { if (usb.writer) usb.writer.releaseLock(); } catch (e) {}
  try { if (p) await p.close(); } catch (e) {}
  usb.writer = null; usb.reader = null;
  $('usbBtn').textContent = 'Connect USB';
}
function usbLost() {
  if (updBusy) return;
  if (!usb.port && !usb.writer) return;
  const port = usb.port;
  try { if (usb.writer) usb.writer.releaseLock(); } catch (e) {}
  try { if (usb.reader) usb.reader.releaseLock(); } catch (e) {}
  usb.port = null; usb.writer = null; usb.reader = null;
  $('usbBtn').textContent = 'Connect USB';
  if (live) { setConn('lost'); }
  if (usbAuto && port) setTimeout(() => usbReopen(port), 1500);
}
/* reconnect by itself when the lamp's USB comes back (for example after it restarts when the 24V is plugged in) */
let usbAuto = false;
async function usbReopen(port) {
  if (usb.port) return;
  for (let tries = 0; tries < 6 && !usb.port; tries++) {
    try {
      try { await port.close(); } catch (e) {}          // clear the old, broken session first
      await port.open({ baudRate: 115200 });
      try { await port.setSignals({ dataTerminalReady: false, requestToSend: false }); } catch (e) {}
      usb.port = port; usb.writer = port.writable.getWriter(); usb.buf = '';
      usbReadLoop();
      let ok = false;
      for (let i = 0; i < 10 && !ok; i++) ok = !!(await usbSend('ping', 800, 'pong'));
      if (!ok) { await usbClose(); return; }
      usbSend('/api/time?epoch=' + Math.floor(Date.now() / 1000) + '&force=1');
      $('usbBtn').textContent = 'Disconnect USB';
      live = false;
      if (!polling) polling = setInterval(poll, 1500);
      await poll();
      return;
    } catch (e) { await new Promise(r => setTimeout(r, 700)); }
  }
}
if ('serial' in navigator) {
  if (navigator.serial.addEventListener) {
    navigator.serial.addEventListener('connect', e => { if (usbAuto) usbReopen(e.target); });
    navigator.serial.addEventListener('disconnect', () => { if (usb.port) usbLost(); });
  }
  $('usbBtn').addEventListener('click', async () => {
    if (usb.port) { usbAuto = false; await usbClose(); live = false; setConn('demo'); reveal($('demoBanner'), true); syncUI(); }
    else { usbAuto = true; usbConnect(); }
  });
}

let connMode = 'connecting', bridgeInfo = null, polling = null;
async function poll() {
  if (updBusy) return false;
  try {
    const r = await fetch('/api/state', { cache: 'no-store' });
    const j = await r.json();
    if (j && j.cfg) {
      const fresh = !live || connMode !== 'live';
      st = j; gotAt = performance.now(); live = true;
      if (fresh) {
        reveal($('demoBanner'), false);
        if (!st.time.valid) post('/api/time', { epoch: Math.floor(Date.now() / 1000) });
        if (connMode !== 'connecting') toast('Lamp connected');
      }
      setConn('live'); syncUI();
      if (fresh) checkUpdate(true);
      return true;
    }
    if (j && j.bridge && j.bridge.waiting) { bridgeInfo = j.bridge; setConn(live ? 'lost' : 'waiting'); return false; }
    throw 0;
  } catch (e) {
    setConn(live ? 'lost' : connMode === 'waiting' ? 'waiting' : 'demo');
    return false;
  }
}
const CONN = {
  connecting: ['pulse idle', 'Connecting'],
  waiting:    ['pulse idle', 'Waiting for lamp'],
  live:       ['pulse', 'Connected'],
  lost:       ['err', 'Disconnected'],
  demo:       ['idle', 'Demo'],
};
function setConn(m) {
  if (m !== connMode && m === 'lost') toast('Lamp disconnected. Check the USB cable or WiFi.');
  connMode = m;
  $('connDot').className = 'status-dot ' + CONN[m][0];
  $('connText').textContent = CONN[m][1];
  $('connText').classList.toggle('live', m === 'live');
  $('connText').classList.toggle('bad', m === 'lost');
  updateStatusBar();
}
function fmtUp(sec) {
  if (!(sec >= 0)) return '—';
  if (sec < 60) return Math.floor(sec) + ' s';
  const m = Math.floor(sec / 60); if (m < 60) return m + ' min';
  const h = Math.floor(m / 60); if (h < 48) return `${h} h ${m % 60} min`;
  return Math.floor(h / 24) + ' days';
}
function sbSet(id, text, cls) {
  const el = $(id);
  if (el._t !== text) { el._t = text; el.textContent = text; }
  el.classList.toggle('dim', cls === 'dim'); el.classList.toggle('bad', cls === 'bad');
}
function updateStatusBar() {
  const m = connMode, b = st.bridge || bridgeInfo;
  if (m === 'demo') {
    sbSet('sbLink', 'Demo', 'dim'); sbSet('sbAddr', 'No lamp connected', 'dim'); sbSet('sbFw', '—', 'dim');
    sbSet('sbUp', '—', 'dim'); sbSet('sbId', '—', 'dim'); $('sbLastL').textContent = 'Clock'; sbSet('sbLast', 'This device', 'dim');
    return;
  }
  if (m === 'connecting' || m === 'waiting') {
    sbSet('sbLink', b ? 'USB bridge' : 'Searching', b ? '' : 'dim');
    sbSet('sbAddr', b ? 'No lamp on USB' : '—', 'dim'); sbSet('sbFw', '—', 'dim'); sbSet('sbUp', '—', 'dim'); sbSet('sbId', '—', 'dim');
    $('sbLastL').textContent = 'Next step'; sbSet('sbLast', b ? 'Plug in the USB cable' : '—', b ? '' : 'dim');
    return;
  }
  // live, or lost (keep the last known details, link shows offline)
  const usb = st.via === 'usb', lost = m === 'lost';
  sbSet('sbLink', lost ? 'Offline' : usb ? 'USB' : st.net.sta ? 'WiFi' : 'Hotspot', lost ? 'bad' : '');
  sbSet('sbAddr', usb ? ((b && b.com) || 'USB') : st.net.sta ? st.net.ip : st.net.apIp, lost ? 'dim' : '');
  sbSet('sbFw', st.v ? 'v' + st.v : '—', lost ? 'dim' : '');
  sbSet('sbUp', lost ? '—' : fmtUp(st.up + (performance.now() - gotAt) / 1000), lost ? 'dim' : '');
  sbSet('sbId', st.id ? 'RombyLamp-' + st.id : '—', lost ? 'dim' : '');
  if (!usb && st.net.sta) { $('sbLastL').textContent = 'Signal'; sbSet('sbLast', st.net.rssi + ' dBm', lost ? 'dim' : ''); }
  else { $('sbLastL').textContent = 'Clock'; sbSet('sbLast', st.time.valid ? 'Synced' : 'Not set', lost ? 'dim' : st.time.valid ? '' : 'bad'); }
}

/* ---------- wiring up controls ---------- */
$('power').addEventListener('change', e => sendSet({ on: e.target.checked ? 1 : 0 }, true));
$('bri').addEventListener('input', e => { st.bri = +e.target.value; $('briVal').textContent = st.bri + '%'; sendSet({ bri: st.bri }); });
document.querySelectorAll('.seg button').forEach(b => b.addEventListener('click', () => { st.mode = +b.dataset.mode; syncUI(); sendSet({ mode: st.mode }, true); }));
$('mk').addEventListener('input', e => { st.manualK = +e.target.value; $('mkVal').textContent = st.manualK + ' K'; sendSet({ k: st.manualK }); });
$('testCall').addEventListener('click', () => { const on = !st.teams.test; st.teams.test = on; syncUI(); sendSet({ test: on ? 1 : 0 }, true); });
$('clearTeams').addEventListener('click', () => { st.teams.red = false; st.teams.test = false; st.teams.manual = false; syncUI(); sendSet({ clearTeams: 1 }, true); });
$('busyBtn').addEventListener('click', () => { const on = !st.teams.manual; st.teams.manual = on; st.teams.test = false; syncUI(); sendSet({ busy: on ? 1 : 0 }, true); });

/* firmware update */
function fwSet(pct, msg) {
  keep($('fwProg'), pct != null); $('fwBar').style.width = ((pct || 0) * 100) + '%';
  if (msg != null) $('fwMsg').textContent = msg;
}
$('fwUpload').addEventListener('click', async () => {
  const f = $('fwFile').files[0];
  if (!f) { fwSet(null, 'Choose the .bin file first.'); return; }
  if (!/\.bin$/i.test(f.name)) { fwSet(null, 'That isn\'t a .bin file. Use Sketch → Export Compiled Binary in Arduino IDE.'); return; }
  if (/bootloader|partitions|merged/i.test(f.name)) { fwSet(null, 'Pick RombyLamp.ino.bin, not the bootloader, partitions or merged file.'); return; }
  $('fwUpload').disabled = true;
  if (!live) {   // demo: pretend
    for (let i = 0; i <= 20; i++) { fwPct = i / 20; fwSet(fwPct, `Uploading… ${Math.round(fwPct * 100)}%`); await new Promise(r => setTimeout(r, 120)); }
    fwPct = null; fwSet(null, 'Demo: the lamp would restart on the new version now.'); $('fwUpload').disabled = false; return;
  }
  const fd = new FormData(); fd.append('firmware', f, f.name);
  const xhr = new XMLHttpRequest();
  xhr.open('POST', '/api/update');
  xhr.upload.onprogress = e => { if (e.lengthComputable) { fwPct = e.loaded / e.total; fwSet(fwPct, `Uploading… ${Math.round(fwPct * 100)}%`); } };
  xhr.onload = () => {
    fwPct = null;
    let j = {}; try { j = JSON.parse(xhr.responseText); } catch (e) {}
    if (j.ok) { fwSet(1, 'Installed. The lamp is restarting. This page reloads when it is back.'); waitForLamp(); }
    else { fwSet(null, 'Update failed: ' + (j.error || 'the lamp rejected the file') + '. The lamp is still on the old version.'); $('fwUpload').disabled = false; }
  };
  xhr.onerror = () => { fwPct = null; fwSet(null, 'Upload interrupted. The lamp is still on the old version. Try again.'); $('fwUpload').disabled = false; };
  xhr.send(fd);
});
async function waitForLamp() {
  await new Promise(r => setTimeout(r, 4000));
  for (let i = 0; i < 40; i++) {
    try { const r = await fetch('/api/state', { cache: 'no-store' }); if (r.ok) { location.reload(); return; } } catch (e) {}
    await new Promise(r => setTimeout(r, 1500));
  }
  fwSet(null, 'The lamp has not come back yet. Check it is powered, then reload this page.');
}
$('identify').addEventListener('click', () => { identifyAt = performance.now(); sendSet({ identify: 1 }, true); });
document.querySelectorAll('[data-set]').forEach(el => {
  const evt = el.type === 'range' ? 'change' : 'change';
  if (el.type === 'range') el.addEventListener('input', () => {
    st.cfg[el.dataset.set] = +el.value;
    $('glowVal').textContent = st.cfg.glow + '%'; $('redBriVal').textContent = st.cfg.redBri + '%'; $('redEndVal').textContent = st.cfg.redEndPct + '%'; $('redPulseMinVal').textContent = st.cfg.redPulseMin + '%'; $('maxBriVal').textContent = st.cfg.maxBri + '%';
  });
  el.addEventListener(evt, () => {
    const k = el.dataset.set, kind = el.dataset.kind;
    let v = kind === 'bool' ? (el.checked ? 1 : 0) : kind === 'time' ? fromHHMM(el.value) : +el.value;
    if (k === 'pixels') v = Math.min(600, Math.max(1, v | 0));
    sendSettings({ [k]: v });
  });
});
document.querySelectorAll('[data-copy]').forEach(b => b.addEventListener('click', async () => {
  const el = $(b.dataset.copy), text = el.textContent;
  try { await navigator.clipboard.writeText(text); toast('Copied'); }
  catch (e) { const r = document.createRange(); r.selectNodeContents(el); const s = getSelection(); s.removeAllRanges(); s.addRange(r); toast('Selected. Press Ctrl+C to copy'); }
}));
$('reboot').addEventListener('click', () => post('/api/reboot', {}).then(() => live && toast('Restarting…')));
$('saveWifi').addEventListener('click', async () => {
  const ssid = $('ssid').value.trim();
  if (!ssid) { toast('Enter the network name first'); $('ssid').focus(); return; }
  await post('/api/wifi', { ssid, pass: $('pass').value });
  if (live) toast('Saved. The lamp is restarting and joining ' + ssid);
});
$('scanBtn').addEventListener('click', scan);
async function scan() {
  const box = $('nets'); box.innerHTML = '<span class="hint">Scanning…</span>';
  let nets;
  if (!live) { await new Promise(r => setTimeout(r, 700)); nets = [{ ssid: 'Home WiFi', rssi: -48 }, { ssid: 'Home WiFi 5G-ext', rssi: -61 }, { ssid: 'Neighbor', rssi: -80 }]; }
  else {
    for (let i = 0; i < 12; i++) {
      try { const j = await (await fetch('/api/scan', { cache: 'no-store' })).json(); if (!j.scanning) { nets = j.nets; break; } } catch (e) {}
      await new Promise(r => setTimeout(r, 900));
    }
  }
  if (!nets) { box.innerHTML = '<span class="hint">Scan timed out. Try again.</span>'; return; }
  const seen = new Set();
  nets = nets.filter(n => n.ssid && !seen.has(n.ssid) && seen.add(n.ssid)).sort((a, b) => b.rssi - a.rssi);
  box.innerHTML = nets.length ? '' : '<span class="hint">No networks found</span>';
  nets.forEach(n => {
    const b = document.createElement('button'); b.className = 'net'; b.type = 'button';
    b.innerHTML = `<span></span><span class="mono muted">${n.rssi} dBm</span>`; b.firstChild.textContent = n.ssid;
    b.addEventListener('click', () => { $('ssid').value = n.ssid; $('pass').focus(); });
    box.appendChild(b);
  });
}

/* strip length: type the length, the dashboard works out the segments (remembered on this device) */
const LEN = { unit: 'in', cut: 62.5 };
try { const j = JSON.parse(localStorage.getItem('rombyLampLen') || '{}'); if (j.unit) LEN.unit = j.unit; if (j.cut > 0) LEN.cut = j.cut; } catch (e) {}
const mmPer = { in: 25.4, ft: 304.8, cm: 10 };
function saveLen() { try { localStorage.setItem('rombyLampLen', JSON.stringify(LEN)); } catch (e) {} }
function showLen() {
  const segs = st.cfg.pixels, mm = segs * LEN.cut, u = LEN.unit;
  const v = mm / mmPer[u];
  if (document.activeElement !== $('lenVal')) $('lenVal').value = u === 'cm' ? v.toFixed(1) : (Math.round(v * 4) / 4).toString();
  $('lenUnit').value = u;
  if (document.activeElement !== $('cutMm')) $('cutMm').value = LEN.cut;
  const ft = Math.floor(mm / 304.8), inch = (mm - ft * 304.8) / 25.4;
  $('lenHint').textContent = `${segs} segments ≈ ${ft ? ft + ' ft ' : ''}${inch.toFixed(1)} in (${(mm / 1000).toFixed(2)} m)`;
}
$('lenVal').addEventListener('change', () => {
  const v = parseFloat($('lenVal').value);
  if (!(v > 0)) { showLen(); return; }
  const segs = Math.max(1, Math.min(600, Math.round(v * mmPer[LEN.unit] / LEN.cut)));
  st.cfg.pixels = segs; showLen();
  sendSettings({ pixels: segs });
});
$('lenUnit').addEventListener('change', () => { LEN.unit = $('lenUnit').value; saveLen(); showLen(); });
$('cutMm').addEventListener('change', () => { const c = parseFloat($('cutMm').value); if (c >= 5 && c <= 500) { LEN.cut = c; saveLen(); } showLen(); });

/* knob test: fast readout while the Knob section is open */
const knobDrawer = [...document.querySelectorAll('details.drawer')].find(d => d.querySelector('#encSteps'));
let knobBase = null;
async function knobTick() {
  if (!knobDrawer.open || !live || updBusy) { knobBase = null; return; }
  try {
    const k = await (await fetch('/api/knob', { cache: 'no-store' })).json();
    if (knobBase == null) knobBase = k.edges;
    $('kA').textContent = k.a ? '1 (idle)' : '0'; $('kB').textContent = k.b ? '1 (idle)' : '0';
    $('kBtn').textContent = k.btn ? 'released' : 'PRESSED';
    $('kEdges').textContent = k.edges - knobBase;
    $('kHint').textContent = (k.edges - knobBase) > 0 ? 'The lamp sees the knob turning.' : '';
  } catch (e) { $('kHint').textContent = live && st.v && verNum(st.v) < verNum('1.3.7') ? 'Update the lamp to 1.3.7 to use the knob test.' : ''; }
}
setInterval(knobTick, 300);

/* scrubber */
const track = $('track');
let dragging = false, mirrorT = 0;
function setView(m) {
  viewMin = Math.min(1439, Math.max(0, m));
  if (live && $('mirror').checked && performance.now() - mirrorT > 200) { mirrorT = performance.now(); fetch('/api/set', { method: 'POST', body: new URLSearchParams({ preview: Math.round(viewMin) }) }).catch(() => {}); }
}
function fromPointer(e) { const r = track.getBoundingClientRect(); return (e.clientX - r.left) / r.width * 1440; }
track.addEventListener('pointerdown', e => { dragging = true; stopPlay(); track.setPointerCapture(e.pointerId); setView(fromPointer(e)); });
track.addEventListener('pointermove', e => { if (dragging) setView(fromPointer(e)); });
track.addEventListener('pointerup', () => { dragging = false; });
track.addEventListener('keydown', e => {
  const step = e.shiftKey ? 60 : 15, cur = viewMin ?? nowMin();
  if (e.key === 'ArrowRight') { setView(cur + step); e.preventDefault(); }
  if (e.key === 'ArrowLeft') { setView(cur - step); e.preventDefault(); }
  if (e.key === 'Escape') backToNow();
});
function backToNow() {
  stopPlay(); viewMin = null;
  if (live) fetch('/api/set', { method: 'POST', body: new URLSearchParams({ preview: -1 }) }).catch(() => {});
}
$('nowBtn').addEventListener('click', backToNow);
$('mirror').addEventListener('change', e => { if (!e.target.checked && live) fetch('/api/set', { method: 'POST', body: new URLSearchParams({ preview: -1 }) }).catch(() => {}); else if (viewMin != null) { mirrorT = 0; setView(viewMin); } });
let playStart = 0, playFrom = 0;
function stopPlay() { playing = false; $('playBtn').setAttribute('aria-pressed', 'false'); $('playBtn').textContent = 'Play a day'; }
$('playBtn').addEventListener('click', () => {
  if (playing) { stopPlay(); return; }
  playing = true; playStart = performance.now(); playFrom = viewMin ?? 300;
  $('playBtn').setAttribute('aria-pressed', 'true'); $('playBtn').textContent = 'Pause';
});

let toastT;
function toast(msg) { const t = $('toast'); t.textContent = msg; t.classList.add('show'); clearTimeout(toastT); toastT = setTimeout(() => t.classList.remove('show'), 2200); }

/* main loop */
function tick() {
  if (playing) {
    const m = playFrom + (performance.now() - playStart) / 20000 * 1440;   // a day in 20 seconds
    if (m >= playFrom + 1440) { stopPlay(); }
    setView(m % 1440);
  }
  demoSchedule();
  render();
  requestAnimationFrame(tick);
}
window.addEventListener('resize', () => drawCurve());
document.addEventListener('keydown', e => {
  const t = e.target;
  if (e.metaKey || e.ctrlKey || e.altKey) return;
  if (t && (/^(INPUT|SELECT|TEXTAREA)$/.test(t.tagName) || t === track)) return;
  const k = e.key.toLowerCase();
  if (k === 'arrowleft' || k === 'arrowright') { setView((viewMin ?? nowMin()) + (k === 'arrowright' ? 1 : -1) * (e.shiftKey ? 60 : 15)); e.preventDefault(); }
  else if (k === 'p') $('playBtn').click();
  else if (k === 'n' || k === 'escape') backToNow();
  else if (k === 'b') $('busyBtn').click();
  else if (k === 't') $('testCall').click();
});
document.querySelectorAll('details').forEach(d => d.addEventListener('toggle', () => drawCurve()));
if (matchMedia) matchMedia('(prefers-color-scheme: dark)').addEventListener?.('change', drawCurve);

/* boot: are we on the lamp (or the USB bridge), or just a preview? */
async function boot() {
  $('usbBtn').hidden = !('serial' in navigator) || !window.isSecureContext;
  let ok = false, reachable = false;
  if (/^https?:$/.test(location.protocol)) {
    try {
      const ctl = new AbortController(); const to = setTimeout(() => ctl.abort(), 2500);
      const r = await fetch('/api/state', { cache: 'no-store', signal: ctl.signal });
      clearTimeout(to);
      const j = await r.json();
      reachable = true;
      if (j && j.cfg) { st = j; gotAt = performance.now(); live = true; ok = true; }
      else if (j && j.bridge && j.bridge.waiting) bridgeInfo = j.bridge;
      else reachable = false;
    } catch (e) { reachable = false; }
  }
  if (ok) {
    if (!st.time.valid) post('/api/time', { epoch: Math.floor(Date.now() / 1000) });
    setConn('live');
  } else {
    live = false;
    st.on = progActive(nowMin(), nowWday(), st.cfg);   // preview starts the way the schedule would
    if (reachable) setConn('waiting');                 // bridge is running, lamp not plugged in yet
    else { setConn('demo'); reveal($('demoBanner'), true); }
  }
  if (ok || reachable) polling = setInterval(poll, 1500);
  syncUI();
}
syncUI();
requestAnimationFrame(tick);
boot();
})();
</script>
</body></html>)rawliteral";
