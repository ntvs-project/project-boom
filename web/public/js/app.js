/**
 * Project BOOM - 拆彈手冊實境模擬前端核心邏輯
 * 依據 main.pdf 規範重構：支援底圖熱區點擊放大、右上角常駐計時器與YB指示燈
 */

// ================= 全域狀態 =================
const state = {
  running: false,
  timeLimit: 300,
  remaining: 300,
  maxStrikes: 3,
  strikes: 0,
  yb: '00',
  selectedModules: ['fourColor', 'greyCaesar', 'trislator', 'tune', 'sort'],
  solved: {},
  currentFocusedModule: null,
  trislatorState: { round: 0, phase: 'numeric' },
  timerHandle: null,
  publicModules: null,
  soundFx: window.soundFx || null
};

const $ = (sel) => document.querySelector(sel);
const $$ = (sel) => Array.from(document.querySelectorAll(sel));

// ================= API 工具函式 =================
async function apiPost(url, data = {}) {
  const tryPost = async (target) => {
    return await fetch(target, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(data)
    });
  };

  try {
    let res = await tryPost(url);
    if (res.status === 404 && !url.startsWith('http') && !url.startsWith('/')) {
      const alt = url.startsWith('../') ? url.replace(/^\.\.\//, '') : '../' + url;
      res = await tryPost(alt);
    }
    return await res.json();
  } catch (err) {
    console.error('API Error:', err);
    return { ok: false, error: '伺服器通訊異常，請確認後端是否已啟動' };
  }
}

// ================= 7 段顯示器 SVG 產生器 =================
function create7SegmentSvg(activeSegments, width = 36, height = 54) {
  const svg = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
  svg.setAttribute('viewBox', '0 0 42 64');
  svg.setAttribute('width', width);
  svg.setAttribute('height', height);
  svg.classList.add('seven-seg-svg');

  const segPoints = {
    a: '8,6 34,6 30,10 12,10',
    b: '34,8 38,12 36,29 32,27',
    c: '36,35 38,37 34,56 30,52',
    d: '12,54 30,54 26,58 16,58',
    e: '8,37 12,35 10,52 6,56',
    f: '8,12 12,14 10,27 6,29',
    g: '11,31 31,31 29,33 13,33'
  };

  const g = document.createElementNS('http://www.w3.org/2000/svg', 'g');
  for (const [seg, pts] of Object.entries(segPoints)) {
    const poly = document.createElementNS('http://www.w3.org/2000/svg', 'polygon');
    poly.setAttribute('points', pts);
    const isOn = activeSegments && activeSegments.includes(seg);
    poly.setAttribute('fill', isOn ? '#ff2a2a' : '#220808');
    if (isOn) poly.setAttribute('filter', 'drop-shadow(0 0 4px #ff2a2a)');
    g.appendChild(poly);
  }
  svg.appendChild(g);
  return svg;
}

// ================= 畫面切換 =================
function switchScreen(screenId) {
  $$('.screen').forEach(s => s.classList.add('hidden'));
  const target = $(`#${screenId}`);
  if (target) target.classList.remove('hidden');
}

// ================= 倒數計時器與即時同步 =================
function startTimer() {
  if (state.timerHandle) clearInterval(state.timerHandle);
  updateTimerUI();

  state.timerHandle = setInterval(() => {
    if (!state.running) return;
    state.remaining--;
    updateTimerUI();

    if (state.remaining <= 0) {
      clearInterval(state.timerHandle);
      triggerGameOver(false, '時間已耗盡，炸彈引爆！');
    }
  }, 1000);
}

function updateTimerUI() {
  const m = Math.max(0, Math.floor(state.remaining / 60)).toString().padStart(2, '0');
  const s = Math.max(0, state.remaining % 60).toString().padStart(2, '0');
  const timeStr = `${m}:${s}`;

  // 1. 底圖主板計時器
  const mbTimer = $('#mainboardTimer');
  if (mbTimer) mbTimer.textContent = timeStr;

  // 2. 右上角常駐 HUD 計時器
  const hudTimer = $('#hudTimerDisplay');
  if (hudTimer) hudTimer.textContent = timeStr;

  // 時間低於 30 秒播放緊急警報滴答
  if (state.remaining > 0 && state.remaining <= 30 && state.soundFx) {
    state.soundFx.tick();
  }
}

// ================= 失誤燈號同步 =================
function updateStrikesUI() {
  const n = state.strikes;

  // 1. 底圖主板上的 3 個失誤圓點
  for (let i = 1; i <= 3; i++) {
    const dot = $(`#mbStrike${i}`);
    if (dot) dot.classList.toggle('lit', i <= n);
  }

  // 2. 右上角 HUD 上的 3 個失誤 X 燈
  $$('.hud-strike-x').forEach(el => {
    const idx = parseInt(el.dataset.n, 10);
    el.classList.toggle('lit', idx <= n);
  });
}

// ================= 特殊指示燈 (YB) 同步 =================
function updateYbUI() {
  const yLit = state.yb[0] === '1';
  const bLit = state.yb[1] === '1';

  // 1. 底圖主板上的黃藍圓點
  const mbY = $('#mbYbY');
  const mbB = $('#mbYbB');
  if (mbY) mbY.classList.toggle('lit', yLit);
  if (mbB) mbB.classList.toggle('lit', bLit);

  // 2. 右上角 HUD 上的黃藍燈號
  const hudY = $('#hudYbY');
  const hudB = $('#hudYbB');
  if (hudY) hudY.classList.toggle('lit', yLit);
  if (hudB) hudB.classList.toggle('lit', bLit);
}

// ================= 各模組狀態綠燈同步 =================
function updateModuleStatusLeds() {
  for (const [mod, isSolved] of Object.entries(state.solved)) {
    const led = $(`#led-status-fourColor, #led-status-greyCaesar, #led-status-trislator, #led-status-tune, #led-status-sort`.replace('fourColor', mod));
    const target = $(`#led-status-${mod}`);
    if (target) target.classList.toggle('solved', !!isSolved);
  }
}

// ================= 模組放大檢視控制器 =================
const MODULE_TITLES = {
  fourColor: '四色按鈕 (FOUR-COLOR)',
  greyCaesar: '格雷 / 凱撒密碼 (CIPHER)',
  trislator: '三角翻譯 (TRISLATOR)',
  tune: '頻率調變 (RESONANCE)',
  sort: '資料排序 (DATA SORTING)'
};

function zoomModule(modKey) {
  if (!state.running) return;
  state.currentFocusedModule = modKey;

  const overlay = $('#moduleFocusOverlay');
  const title = $('#focusModuleTitle');
  const led = $('#focusModuleStatusLed');
  const content = $('#focusModuleContent');

  if (title) title.textContent = MODULE_TITLES[modKey] || '模組操作';
  if (led) led.classList.toggle('solved', !!state.solved[modKey]);
  if (content) content.innerHTML = '';

  // 渲染放大後的模組內容
  switch (modKey) {
    case 'fourColor':
      renderFocusFourColor(content);
      break;
    case 'greyCaesar':
      renderFocusGreyCaesar(content);
      break;
    case 'trislator':
      renderFocusTrislator(content);
      break;
    case 'tune':
      renderFocusTune(content);
      break;
    case 'sort':
      renderFocusSort(content);
      break;
  }

  overlay.classList.remove('hidden');
  if (state.soundFx) state.soundFx.click();
}

function zoomOut() {
  state.currentFocusedModule = null;
  $('#moduleFocusOverlay').classList.add('hidden');
  if (state.soundFx) state.soundFx.click();
  // 檢查是否全部完成
  checkAllSolvedWin();
}

// ================= 1. 四色按鈕 (Four-Color) 放大操作 =================
function renderFocusFourColor(holder) {
  const modData = state.publicModules.fourColor;
  if (!modData) return;

  const wrap = document.createElement('div');
  wrap.className = 'four-color-console';

  const colors = ['R', 'G', 'B'];
  const runningLeds = [true, true, true, true];
  const currentIndices = [0, 1, 2, 0];
  const intervals = [];

  for (let i = 0; i < 4; i++) {
    const col = document.createElement('div');
    col.className = 'fc-unit';

    const led = document.createElement('div');
    led.className = 'fc-led';
    led.dataset.color = colors[currentIndices[i]];

    const btn = document.createElement('button');
    btn.className = 'fc-btn';
    btn.title = '按一下停止/恢復循環';

    const timer = setInterval(() => {
      if (runningLeds[i]) {
        currentIndices[i] = (currentIndices[i] + 1) % colors.length;
        led.dataset.color = colors[currentIndices[i]];
      }
    }, (modData.speeds && modData.speeds[i]) || (700 + i * 80));
    intervals.push(timer);

    btn.addEventListener('click', async () => {
      if (state.soundFx) state.soundFx.click();
      runningLeds[i] = !runningLeds[i];
      btn.style.borderColor = runningLeds[i] ? '#8e9bb0' : 'var(--hazard-yellow)';

      // 檢查是否 4 顆都停止
      if (runningLeds.every(r => !r)) {
        const finalStops = currentIndices.map(idx => colors[idx]);
        const res = await apiPost('game/check.php', {
          module: 'fourColor',
          payload: { stops: finalStops }
        });
        applyCheckResult(res, 'fourColor');
        if (!res.correct) {
          // 錯誤重啟
          setTimeout(() => {
            for (let j = 0; j < 4; j++) runningLeds[j] = true;
          }, 800);
        }
      }
    });

    col.appendChild(led);
    col.appendChild(btn);
    wrap.appendChild(col);
  }

  holder.appendChild(wrap);
}

// ================= 2. 格雷 / 凱撒密碼 (Grey / Caesar) 放大操作 =================
function renderFocusGreyCaesar(holder) {
  // 手冊規則：在其它所有模組都解開後，此模組才會啟用
  const otherModules = state.selectedModules.filter(m => m !== 'greyCaesar');
  const allOthersSolved = otherModules.every(m => state.solved[m]);

  if (!allOthersSolved) {
    const lockedBox = document.createElement('div');
    lockedBox.className = 'cipher-locked-box';
    lockedBox.innerHTML = `
      <div class="cipher-lock-icon">🔒</div>
      <div class="cipher-locked-title">模組尚未通電啟用</div>
      <div class="cipher-locked-desc">
        依據拆彈手冊第 3 頁與第 4 頁規範：<br>
        <strong>必須在其它所有模組都解開後，此密碼模組才會啟用。</strong><br><br>
        請先點擊並解開其他 4 個模組！
      </div>
    `;
    holder.appendChild(lockedBox);
    return;
  }

  // 已解鎖，顯示輸入控制台
  const modData = state.publicModules.greyCaesar;
  if (!modData) return;

  const wrap = document.createElement('div');
  wrap.className = 'cipher-active-console';

  const bitLen = modData.bitLen || 4;
  const ledsRow = document.createElement('div');
  ledsRow.className = 'cipher-leds-row';

  const leds = [];
  for (let i = 0; i < bitLen; i++) {
    const l = document.createElement('div');
    l.className = 'cipher-bit-led white';
    ledsRow.appendChild(l);
    leds.push(l);
  }
  wrap.appendChild(ledsRow);

  // 密碼聲音播放器
  const soundControls = document.createElement('div');
  soundControls.innerHTML = `
    <button class="cipher-sound-toggle-btn" id="playCipherAudioBtn">🔊 聆聽訊號密碼 (PLAY)</button>
  `;
  wrap.appendChild(soundControls);

  let playTimer = null;
  function playSignals() {
    if (playTimer) clearTimeout(playTimer);
    let delay = 300;

    if (modData.type === 'grey') {
      const bits = modData.sourceBits || [];
      bits.forEach(b => {
        setTimeout(() => {
          if (state.soundFx) state.soundFx.beep(b === 1 ? 700 : 250, 800);
        }, delay);
        delay += (b === 1 ? 700 : 250) + 350;
      });
    } else {
      // 凱撒密碼：高音字母摩斯碼 + 低音位移
      const morsePattern = modData.morse || '';
      for (const char of morsePattern) {
        setTimeout(() => {
          if (state.soundFx) state.soundFx.beep(char === '-' ? 600 : 200, 1000);
        }, delay);
        delay += (char === '-' ? 600 : 200) + 250;
      }
      delay += 500;
      const shiftBits = modData.shiftBits || [];
      shiftBits.forEach(b => {
        setTimeout(() => {
          if (state.soundFx) state.soundFx.beep(b === 1 ? 600 : 200, 400);
        }, delay);
        delay += (b === 1 ? 600 : 200) + 250;
      });
    }
  }

  wrap.querySelector('#playCipherAudioBtn').addEventListener('click', () => {
    playSignals();
  });

  // 大號按鈕操作 (長按短按)
  const bigBtn = document.createElement('button');
  bigBtn.className = 'cipher-big-btn';
  bigBtn.innerHTML = '長按: 1<br>短按: 0';

  let pressStart = 0;
  const userBits = [];
  const userMorse = [];

  bigBtn.addEventListener('mousedown', () => {
    pressStart = Date.now();
  });

  bigBtn.addEventListener('mouseup', async () => {
    const duration = Date.now() - pressStart;
    const isLong = duration >= 450; // 手冊 Hint: 長按直到 LED 變靛色再放開
    const bit = isLong ? 1 : 0;
    const currentIdx = userBits.length;

    if (currentIdx < bitLen) {
      userBits.push(bit);
      userMorse.push(isLong ? '-' : '.');
      leds[currentIdx].className = 'cipher-bit-led indigo';
      if (state.soundFx) state.soundFx.click();

      // 當輸入位元數足夠時自動驗證
      if (userBits.length === bitLen) {
        if (modData.type === 'grey') {
          const res = await apiPost('game/check.php', {
            module: 'grey',
            payload: { bits: userBits }
          });
          applyCheckResult(res, 'greyCaesar');
        } else {
          const res = await apiPost('game/check.php', {
            module: 'caesar',
            payload: { morse: userMorse.join('') }
          });
          applyCheckResult(res, 'greyCaesar');
        }

        // 若錯誤重設輸入
        if (!state.solved.greyCaesar) {
          userBits.length = 0;
          userMorse.length = 0;
          leds.forEach(l => l.className = 'cipher-bit-led white');
        }
      }
    }
  });

  wrap.appendChild(bigBtn);
  holder.appendChild(wrap);
}

// ================= 3. 三角翻譯 (Trislator) 放大操作 =================
function renderFocusTrislator(holder) {
  const modData = state.publicModules.trislator;
  if (!modData) return;

  const round = state.trislatorState.round || 0;
  const phase = state.trislatorState.phase || 'numeric';

  if (round >= 5) {
    holder.innerHTML = `<div style="color:var(--led-green);font-size:1.3rem;font-weight:700;">✓ 三角翻譯 5 道難題已全數解開！</div>`;
    return;
  }

  const wrap = document.createElement('div');
  wrap.className = 'tris-console';

  // CRT 顯示幕
  const crt = document.createElement('div');
  crt.className = 'tris-crt-panel';

  const line1 = document.createElement('div');
  line1.className = 'tris-crt-line1';

  const line2 = document.createElement('div');
  line2.className = 'tris-crt-line2';

  crt.appendChild(line1);
  crt.appendChild(line2);
  wrap.appendChild(crt);

  let buffer = '';

  function updateNumericDisplay() {
    if (!buffer || buffer.length === 0) {
      line2.textContent = '=  .';
    } else if (buffer.length === 1) {
      line2.textContent = `= ${buffer[0]}.`;
    } else {
      line2.textContent = `= ${buffer[0]}.${buffer.slice(1)}`;
    }
  }

  function getNumericValueToSend() {
    if (!buffer || buffer.length === 0) return '';
    if (buffer.length === 1) return `${buffer[0]}.`;
    return `${buffer[0]}.${buffer.slice(1)}`;
  }

  if (phase === 'numeric') {
    line1.textContent = `[${round + 1}/5] ` + modData.questions[round];
    updateNumericDisplay();
  } else {
    line1.textContent = `[${round + 1}/5] ` + modData.words[round];
    line2.textContent = 'INPUT: _';
  }

  // 4x4 實體矩陣鍵盤
  const keypad = document.createElement('div');
  keypad.className = 'tris-keypad-grid';

  const keyLayout = [
    ['1', 'num'], ['2', 'num'], ['3', 'num'], ['A', 'letter'],
    ['4', 'num'], ['5', 'num'], ['6', 'num'], ['B', 'letter'],
    ['7', 'num'], ['8', 'num'], ['9', 'num'], ['C', 'letter'],
    ['*', 'action'], ['0', 'num'], ['#', 'action'], ['D', 'letter']
  ];

  keyLayout.forEach(([k, type]) => {
    const btn = document.createElement('button');
    btn.className = `k-btn ${type}`;
    btn.textContent = k;

    btn.addEventListener('click', async () => {
      if (k === '*') {
        // main.pdf: 按下 * 即可刪除目前的數字或字母
        if (state.trislatorState.phase === 'numeric') {
          buffer = buffer.slice(0, -1);
          updateNumericDisplay();
        } else {
          buffer = '';
          line2.textContent = 'INPUT: _';
        }
      } else if (k === '#') {
        // 確認送出
        if (state.trislatorState.phase === 'numeric') {
          const val = getNumericValueToSend();
          const res = await apiPost('game/check.php', {
            module: 'trislator_step', payload: { type: 'numeric', value: val }
          });
          applyCheckResult(res, 'trislator');
          if (res.correct) {
            renderFocusTrislator(holder);
          } else {
            buffer = '';
            updateNumericDisplay();
          }
        } else {
          const res = await apiPost('game/check.php', {
            module: 'trislator_step', payload: { type: 'letter', letter: buffer }
          });
          applyCheckResult(res, 'trislator');
          if (res.correct) {
            renderFocusTrislator(holder);
          } else {
            buffer = '';
            line2.textContent = 'INPUT: _';
          }
        }
      } else if (type === 'letter') {
        if (state.trislatorState.phase === 'numeric') {
          // main.pdf: 在不對的時機按下 A B C D 將觸發機關
          apiPost('game/check.php', { module: 'trislator_step', payload: { type: 'letter', letter: k } }).then(res => applyCheckResult(res, 'trislator'));
        } else {
          buffer = k;
          line2.textContent = 'INPUT: ' + buffer;
        }
      } else {
        // 數字鍵 0~9
        if (state.trislatorState.phase === 'letter') {
          // main.pdf: 在不對的時機按下 0~9 將觸發機關
          apiPost('game/check.php', { module: 'trislator_step', payload: { type: 'numeric', value: k } }).then(res => applyCheckResult(res, 'trislator'));
        } else {
          // 最多 4 位數字 (1 整數 + 3 小數)
          if (buffer.length < 4) {
            buffer += k;
            updateNumericDisplay();
          }
        }
      }
    });

    keypad.appendChild(btn);
  });

  wrap.appendChild(keypad);
  holder.innerHTML = '';
  holder.appendChild(wrap);
}

// ================= 4. 頻率調變 (Resonance / Tune) 放大操作 =================
function renderFocusTune(holder) {
  const modData = state.publicModules.tune;
  if (!modData) return;

  const wrap = document.createElement('div');
  wrap.className = 'tune-console';

  // 左右兩顆紫色 LED 與中間 10 段指示條
  const displayRow = document.createElement('div');
  displayRow.className = 'tune-display-area';

  const purpleL = document.createElement('div');
  purpleL.className = 'purple-neon-lamp';

  const purpleR = document.createElement('div');
  purpleR.className = 'purple-neon-lamp';

  const meterBox = document.createElement('div');
  meterBox.className = 'tune-meter-box';

  const barGraph = document.createElement('div');
  barGraph.className = 'freq-led-bar-graph';
  for (let i = 1; i <= 10; i++) {
    const seg = document.createElement('div');
    seg.className = 'bar-seg';
    seg.dataset.row = i;
    barGraph.appendChild(seg);
  }

  // 電路數值面板
  const circuitDisplay = document.createElement('div');
  circuitDisplay.className = 'circuit-display';
  circuitDisplay.innerHTML = `
    <div class="circuit-val-row"><span>f 頻率:</span><span class="val-num" id="valF">-- kHz</span></div>
    <div class="circuit-val-row"><span>L 電感:</span><span class="val-num" id="valL">-- mH</span></div>
    <div class="circuit-val-row"><span>C 電容:</span><span class="val-num" id="valC">-- nF</span></div>
    <div class="circuit-val-row"><span>R 電阻:</span><span class="val-num" id="valR">-- Ω</span></div>
  `;

  meterBox.appendChild(barGraph);
  meterBox.appendChild(circuitDisplay);

  displayRow.appendChild(purpleL);
  displayRow.appendChild(meterBox);
  displayRow.appendChild(purpleR);
  wrap.appendChild(displayRow);

  // 4 旋鈕數值選項 (main.pdf 第 9 頁表格數據)
  const fOptions = [
    { f: 5.03, unit: 'k' }, { f: 11, unit: 'k' }, { f: 15, unit: 'k' }, { f: 16, unit: 'k' },
    { f: 19.3, unit: 'k' }, { f: 22, unit: 'k' }, { f: 43, unit: 'k' }, { f: 50.3, unit: 'k' },
    { f: 51, unit: 'k' }, { f: 91, unit: 'k' }
  ];
  const lOptions = [
    { L: 1.1, unit: 'm' }, { L: 374, unit: 'u' }, { L: 86.6, unit: 'u' }, { L: 30, unit: 'u' },
    { L: 10, unit: 'm' }, { L: 523, unit: 'u' }, { L: 22.1, unit: 'u' }, { L: 9.1, unit: 'm' },
    { L: 1.07, unit: 'm' }, { L: 30, unit: 'u' }
  ];
  const cOptions = [
    { C: 910, unit: 'n' }, { C: 560, unit: 'n' }, { C: 1.3, unit: 'u' }, { C: 3.3, unit: 'u' },
    { C: 6.8, unit: 'n' }, { C: 100, unit: 'n' }, { C: 620, unit: 'n' }, { C: 1.1, unit: 'n' },
    { C: 9.1, unit: 'n' }, { C: 102, unit: 'n' }
  ];
  const rOptions = [
    { R: 6.8, unit: 'k' }, { R: 27, unit: '' }, { R: 220, unit: '' }, { R: 3.3, unit: '' },
    { R: 4.7, unit: 'k' }, { R: 5.6, unit: '' }, { R: 100, unit: '' }, { R: 47, unit: '' },
    { R: 1, unit: '' }, { R: 680, unit: '' }, { R: 3.3, unit: 'k' }, { R: 15, unit: '' },
    { R: 470, unit: '' }, { R: 68, unit: '' }, { R: 1.5, unit: '' }, { R: 3.9, unit: 'k' },
    { R: 2.2, unit: 'k' }, { R: 390, unit: '' }, { R: 56, unit: '' }, { R: 6.8, unit: '' },
    { R: 7.5, unit: '' }, { R: 330, unit: '' }, { R: 10, unit: '' }, { R: 10, unit: 'k' },
    { R: 4.7, unit: '' }, { R: 150, unit: '' }, { R: 22, unit: '' }, { R: 1.5, unit: 'k' },
    { R: 2.2, unit: '' }, { R: 12, unit: '' }, { R: 5.6, unit: 'k' }, { R: 270, unit: '' },
    { R: 1, unit: 'k' }, { R: 33, unit: '' }, { R: 120, unit: '' }, { R: 8.2, unit: '' },
    { R: 18, unit: '' }, { R: 820, unit: '' }, { R: 1.2, unit: 'k' }, { R: 9.1, unit: '' }
  ];

  let fIdx = 0, lIdx = 0, cIdx = 0, rIdx = 0;

  function updateTuneDisplays() {
    const curF = fOptions[fIdx];
    const curL = lOptions[lIdx];
    const curC = cOptions[cIdx];
    const curR = rOptions[rIdx];

    wrap.querySelector('#valF').textContent = `${curF.f} ${curF.unit}Hz`;
    wrap.querySelector('#valL').textContent = `${curL.L} ${curL.unit}H`;
    wrap.querySelector('#valC').textContent = `${curC.C} ${curC.unit}F`;
    wrap.querySelector('#valR').textContent = `${curR.R} ${curR.unit}Ω`;

    // 10 段指示條亮燈位置 (1~10)
    wrap.querySelectorAll('.bar-seg').forEach(seg => {
      seg.classList.toggle('lit', parseInt(seg.dataset.row, 10) === (fIdx + 1));
    });
  }

  // 紫色 LED 閃爍效果 (諧振時同步)
  const isHarmonic = () => (fIdx + 1) === modData.rowNumber;
  let flashTimer = setInterval(() => {
    if (!wrap.isConnected) { clearInterval(flashTimer); return; }
    const harmonic = isHarmonic();
    purpleL.classList.toggle('lit');
    if (harmonic) {
      purpleR.classList.toggle('lit', purpleL.classList.contains('lit'));
    } else {
      purpleR.classList.toggle('lit', Math.random() > 0.5);
    }
  }, 400);

  // 旋鈕生成
  const knobsRow = document.createElement('div');
  knobsRow.className = 'knobs-row';

  function createKnobUnit(label, getIdx, setIdx, maxLen) {
    const u = document.createElement('div');
    u.className = 'knob-control-unit';
    u.innerHTML = `
      <span class="knob-lbl">${label}</span>
      <div class="knob-disc"><div class="knob-indicator-notch"></div></div>
      <div class="knob-steppers">
        <button class="step-btn minus">-</button>
        <button class="step-btn plus">+</button>
      </div>
    `;
    const disc = u.querySelector('.knob-disc');

    function refreshRotation() {
      const deg = (getIdx() / maxLen) * 300 - 150;
      disc.style.transform = `rotate(${deg}deg)`;
      updateTuneDisplays();
    }

    u.querySelector('.minus').addEventListener('click', () => {
      setIdx((getIdx() - 1 + maxLen) % maxLen);
      refreshRotation();
      if (state.soundFx) state.soundFx.click();
    });
    u.querySelector('.plus').addEventListener('click', () => {
      setIdx((getIdx() + 1) % maxLen);
      refreshRotation();
      if (state.soundFx) state.soundFx.click();
    });

    refreshRotation();
    return u;
  }

  knobsRow.appendChild(createKnobUnit('f', () => fIdx, v => fIdx = v, fOptions.length));
  knobsRow.appendChild(createKnobUnit('L', () => lIdx, v => lIdx = v, lOptions.length));
  knobsRow.appendChild(createKnobUnit('C', () => cIdx, v => cIdx = v, cOptions.length));
  knobsRow.appendChild(createKnobUnit('R', () => rIdx, v => rIdx = v, rOptions.length));
  wrap.appendChild(knobsRow);

  // 確認驗證按鈕
  const checkBtn = document.createElement('button');
  checkBtn.className = 'ktane-btn primary-btn';
  checkBtn.style.marginTop = '15px';
  checkBtn.textContent = '⚡ 鎖定頻率參數送出';
  checkBtn.addEventListener('click', async () => {
    const curF = fOptions[fIdx];
    const curL = lOptions[lIdx];
    const curC = cOptions[cIdx];
    const curR = rOptions[rIdx];

    const res = await apiPost('game/check.php', {
      module: 'tune',
      payload: {
        f: curF.f, fUnit: curF.unit,
        L: curL.L, LUnit: curL.unit,
        C: curC.C, CUnit: curC.unit,
        R: curR.R, RUnit: curR.unit
      }
    });
    applyCheckResult(res, 'tune');
  });
  wrap.appendChild(checkBtn);

  holder.appendChild(wrap);
  updateTuneDisplays();
}

// ================= 5. 資料排序 (Data Sorting) 放大操作 =================
function renderFocusSort(holder) {
  const modData = state.publicModules.sort;
  if (!modData) return;

  const wrap = document.createElement('div');
  wrap.className = 'sort-console';

  // 頂部 7 段顯示器與橘色指示燈
  const headerRow = document.createElement('div');
  headerRow.className = 'sort-header-row';

  const segBox = document.createElement('div');
  segBox.className = 'sort-7seg-holder';
  const segSvg = create7SegmentSvg(modData.segments || ['a', 'b', 'c', 'd', 'e', 'f'], 42, 64);
  segBox.appendChild(segSvg);

  const orangeLamp = document.createElement('div');
  orangeLamp.className = 'sort-orange-pilot-lamp lit'; // main.pdf: 有子板未插入，橘色燈常亮

  headerRow.appendChild(segBox);
  headerRow.appendChild(orangeLamp);
  wrap.appendChild(headerRow);

  // 題目敘述
  const title = document.createElement('div');
  title.className = 'sort-question-title';
  title.textContent = modData.title;
  wrap.appendChild(title);

  // 插入方向提示
  const dirBadge = document.createElement('div');
  dirBadge.className = 'sort-direction-badge';
  dirBadge.textContent = modData.direction === 'ltr' ? '依手冊 YB 指示：由左至右依序插入' : '依手冊 YB 指示：由右至左依序插入';
  wrap.appendChild(dirBadge);

  // 4 插槽與 4 晶片卡片
  const slotsRow = document.createElement('div');
  slotsRow.className = 'sort-slots-container';

  const letters = ['A', 'B', 'C', 'D'];
  // 隨機初始排序
  const currentOrder = [...letters].sort(() => Math.random() - 0.5);

  function refreshSlots() {
    slotsRow.innerHTML = '';
    currentOrder.forEach((letter, idx) => {
      const slot = document.createElement('div');
      slot.className = 'chip-slot';
      slot.dataset.slotIdx = idx;

      const numLbl = document.createElement('span');
      numLbl.className = 'slot-num-label';
      numLbl.textContent = `#${idx + 1}`;
      slot.appendChild(numLbl);

      if (letter) {
        const chip = document.createElement('div');
        chip.className = 'pcb-chip-card';
        chip.draggable = true;
        chip.dataset.letter = letter;

        chip.innerHTML = `
          <div class="chip-gold-fingers"></div>
          <div class="chip-letter-badge">${letter}</div>
          <div class="chip-text-desc">${modData.items[letter]}</div>
        `;

        chip.addEventListener('dragstart', (e) => {
          e.dataTransfer.setData('text/plain', idx.toString());
        });

        slot.appendChild(chip);
      }

      slot.addEventListener('dragover', (e) => {
        e.preventDefault();
        slot.classList.add('drag-over');
      });
      slot.addEventListener('dragleave', () => {
        slot.classList.remove('drag-over');
      });
      slot.addEventListener('drop', (e) => {
        e.preventDefault();
        slot.classList.remove('drag-over');
        const fromIdx = parseInt(e.dataTransfer.getData('text/plain'), 10);
        const toIdx = idx;
        if (!isNaN(fromIdx) && fromIdx !== toIdx) {
          // 交換位置
          const temp = currentOrder[fromIdx];
          currentOrder[fromIdx] = currentOrder[toIdx];
          currentOrder[toIdx] = temp;
          refreshSlots();
          if (state.soundFx) state.soundFx.click();
        }
      });

      slotsRow.appendChild(slot);
    });
  }

  refreshSlots();
  wrap.appendChild(slotsRow);

  // 驗證按鈕
  const checkBtn = document.createElement('button');
  checkBtn.className = 'sort-confirm-btn';
  checkBtn.textContent = '✔ 插入確認並送出';

  checkBtn.addEventListener('click', async () => {
    const res = await apiPost('game/check.php', {
      module: 'sort',
      payload: { order: currentOrder }
    });

    // 橘色燈反饋 (main.pdf 第 11 頁)
    if (res.correct) {
      orangeLamp.className = 'sort-orange-pilot-lamp solved-green';
    } else {
      if (res.sortError === 'wrong_position') {
        orangeLamp.className = 'sort-orange-pilot-lamp'; // 位置錯誤：燈不亮
      } else {
        orangeLamp.className = 'sort-orange-pilot-lamp lit'; // 未插滿：常亮
      }
    }

    applyCheckResult(res, 'sort');
  });

  wrap.appendChild(checkBtn);
  holder.appendChild(wrap);
}

// ================= 通用檢查結果反饋 =================
function applyCheckResult(res, moduleKey) {
  if (res.strikes !== undefined) {
    state.strikes = res.strikes;
    updateStrikesUI();
  }

  if (res.correct) {
    if (state.soundFx) state.soundFx.success();
    if (moduleKey) {
      state.solved[moduleKey] = true;
      updateModuleStatusLeds();
      const consoleLed = $('#focusModuleStatusLed');
      if (consoleLed) consoleLed.classList.add('solved');
    }
  } else {
    if (state.soundFx) state.soundFx.buzz();
  }

  // 狀態同步
  if (res.trislatorState) {
    state.trislatorState = res.trislatorState;
  }

  // 輸贏判斷
  if (res.gameOver) {
    if (res.won) {
      triggerGameOver(true, '所有安全機關已全數拆除，炸彈成功拆解！');
    } else {
      triggerGameOver(false, '失誤累積達上限，炸彈引爆！');
    }
  } else {
    checkAllSolvedWin();
  }
}

function checkAllSolvedWin() {
  const allSolved = state.selectedModules.every(m => state.solved[m]);
  if (allSolved && state.running) {
    triggerGameOver(true, '所有安全機關已全數拆除，炸彈成功拆解！');
  }
}

// ================= 結算視窗 =================
function triggerGameOver(won, msg) {
  state.running = false;
  if (state.timerHandle) clearInterval(state.timerHandle);

  const modal = $('#resultModal');
  const icon = $('#resultIcon');
  const title = $('#resultTitle');
  const desc = $('#resultDesc');
  const stats = $('#resultStats');

  if (won) {
    if (state.soundFx) state.soundFx.win();
    icon.textContent = '🎉';
    title.textContent = '拆彈成功！任務達成';
    title.style.color = 'var(--led-green)';
    desc.textContent = msg;
  } else {
    if (state.soundFx) state.soundFx.explode();
    icon.textContent = '💥';
    title.textContent = '孩子，你炸了！';
    title.style.color = 'var(--led-red)';
    desc.textContent = msg;
  }

  const elapsed = state.timeLimit - state.remaining;
  const m = Math.floor(elapsed / 60);
  const s = elapsed % 60;
  stats.innerHTML = `
    <div style="font-family:var(--font-mono);font-size:1.05rem;color:#ccd6e8;margin-bottom:8px;">
      用時：${m} 分 ${s} 秒 ｜ 失誤次數：${state.strikes} / ${state.maxStrikes}
    </div>
  `;

  modal.classList.remove('hidden');
}

// ================= 綁定事件監聽器 =================
document.addEventListener('DOMContentLoaded', () => {

  // ================= 任務啟動函式 (5分鐘開局) =================
  async function launchMission(selMods = ['fourColor', 'greyCaesar', 'trislator', 'tune', 'sort'], timeLimit = 300, maxStrikes = 3) {
    // 預先在前端初始化，讓倒數計時器 05:00 立即開始跳動
    state.running = true;
    state.timeLimit = timeLimit;
    state.remaining = timeLimit;
    state.maxStrikes = maxStrikes;
    state.strikes = 0;
    state.yb = state.yb || '00';
    state.selectedModules = selMods;
    state.solved = Object.fromEntries(selMods.map(m => [m, false]));
    state.trislatorState = { round: 0, phase: 'numeric' };

    // 關閉任何開啟中的彈窗
    const focusOverlay = $('#moduleFocusOverlay');
    if (focusOverlay) focusOverlay.classList.add('hidden');
    const resultModal = $('#resultModal');
    if (resultModal) resultModal.classList.add('hidden');

    // 同步 UI 與計時器
    updateTimerUI();
    updateStrikesUI();
    updateYbUI();
    updateModuleStatusLeds();
    startTimer();

    // 請求後端 PHP 初始化局狀態與題目
    const res = await apiPost('game/start.php', {
      modules: selMods,
      timeLimit,
      maxStrikes
    });

    if (res && res.ok) {
      state.yb = res.yb || '00';
      state.selectedModules = res.selectedModules;
      state.publicModules = res.modules;
      state.trislatorState = res.trislatorState || { round: 0, phase: 'numeric' };
      updateYbUI();
      updateModuleStatusLeds();
    }
  }

  // 1. 底圖熱區點擊放大
  $$('.module-bay-hotspot').forEach(hotspot => {
    hotspot.addEventListener('click', () => {
      const mod = hotspot.dataset.module;
      if (mod) {
        zoomModule(mod);
      }
    });
  });

  // 2. 返回炸彈全貌
  const zoomOutBtn = $('#zoomOutBtn');
  if (zoomOutBtn) {
    zoomOutBtn.addEventListener('click', () => {
      zoomOut();
    });
  }

  // 鍵盤 ESC 快捷鍵縮小
  window.addEventListener('keydown', (e) => {
    if (e.key === 'Escape') {
      const focusOverlay = $('#moduleFocusOverlay');
      if (focusOverlay && !focusOverlay.classList.contains('hidden')) {
        zoomOut();
      }
      const manualModal = $('#manualModal');
      if (manualModal && !manualModal.classList.contains('hidden')) {
        manualModal.classList.add('hidden');
      }
    }
  });

  // 3. 拆彈手冊
  function openManual() {
    if (window.renderManualContent) {
      window.renderManualContent($('#manualBody'));
    }
    const manualModal = $('#manualModal');
    if (manualModal) manualModal.classList.remove('hidden');
    if (state.soundFx) state.soundFx.click();
  }

  const openManualBtn = $('#openManualBtn');
  if (openManualBtn) openManualBtn.addEventListener('click', openManual);
  const manualCloseBtn = $('#manualCloseBtn');
  if (manualCloseBtn) {
    manualCloseBtn.addEventListener('click', () => {
      $('#manualModal').classList.add('hidden');
    });
  }
  const manualBackdrop = $('#manualBackdrop');
  if (manualBackdrop) {
    manualBackdrop.addEventListener('click', () => {
      $('#manualModal').classList.add('hidden');
    });
  }

  // 4. 重置 / 再玩一次按鈕
  const resetRoundBtn = $('#resetRoundBtn');
  if (resetRoundBtn) {
    resetRoundBtn.addEventListener('click', () => {
      if (confirm('確定要重新開始一局 5 分鐘拆彈任務嗎？')) {
        launchMission();
      }
    });
  }

  const restartGameBtn = $('#restartGameBtn');
  if (restartGameBtn) {
    restartGameBtn.addEventListener('click', () => {
      launchMission();
    });
  }

  // 5. 點開網頁立即武裝並開始 5 分鐘倒數！
  launchMission();
});
