/**
 * Project BOOM - 純 CSS 炸彈實境機箱互動邏輯
 * 按照 main.pdf 規範：開局直接 5 分鐘倒數、點擊模組平滑放大、右上角常駐即時 HUD
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
  solved: {
    fourColor: false,
    greyCaesar: false,
    trislator: false,
    tune: false,
    sort: false
  },
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
    return { ok: false, error: '伺服器通訊異常' };
  }
}

// ================= 7 段顯示器 SVG 產生器 =================
function create7SegmentSvg(activeSegments, width = 36, height = 48) {
  const svg = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
  svg.setAttribute('viewBox', '0 0 42 64');
  svg.setAttribute('width', width);
  svg.setAttribute('height', height);

  const segPoints = {
    a: '8,6 34,6 30,10 12,10',
    b: '34,8 38,12 36,29 32,27',
    c: '32,35 36,33 38,50 34,54',
    d: '12,52 30,52 34,56 8,56',
    e: '4,33 8,35 6,54 2,50',
    f: '2,12 6,8 8,27 4,29',
    g: '9,31 33,31 31,33 11,33'
  };

  for (const [seg, pts] of Object.entries(segPoints)) {
    const poly = document.createElementNS('http://www.w3.org/2000/svg', 'polygon');
    poly.setAttribute('points', pts);
    const isLit = activeSegments.includes(seg);
    poly.setAttribute('fill', isLit ? '#ff9100' : '#261b0c');
    if (isLit) {
      poly.setAttribute('filter', 'drop-shadow(0 0 4px #ff9100)');
    }
    svg.appendChild(poly);
  }
  return svg;
}

// ================= 計時器管理 =================
function startTimer() {
  if (state.timerHandle) clearInterval(state.timerHandle);
  updateTimerUI();

  state.timerHandle = setInterval(() => {
    if (!state.running) return;
    state.remaining--;
    updateTimerUI();

    if (state.remaining <= 0) {
      clearInterval(state.timerHandle);
      state.remaining = 0;
      updateTimerUI();
      triggerGameOver(false, '時間耗盡，炸彈引爆！');
    }
  }, 1000);
}

function updateTimerUI() {
  const m = Math.floor(state.remaining / 60);
  const s = state.remaining % 60;
  const timeStr = `${String(m).padStart(2, '0')}:${String(s).padStart(2, '0')}`;

  const mbTimer = $('#mainboardTimer');
  if (mbTimer) mbTimer.textContent = timeStr;

  const hudTimer = $('#hudTimerDisplay');
  if (hudTimer) hudTimer.textContent = timeStr;

  if (state.remaining > 0 && state.remaining <= 30 && state.soundFx) {
    state.soundFx.tick();
  }
}

// ================= 失誤燈號同步 =================
function updateStrikesUI() {
  const n = state.strikes;
  for (let i = 1; i <= 3; i++) {
    const dot = $(`#mbStrike${i}`);
    if (dot) dot.classList.toggle('lit', i <= n);
    const hudDot = $(`#hudStrike${i}`);
    if (hudDot) hudDot.classList.toggle('lit', i <= n);
  }
}

// ================= 黃藍指示燈 (YB) 同步 =================
function updateYbUI() {
  const yLit = state.yb[0] === '1';
  const bLit = state.yb[1] === '1';

  const mbY = $('#mbYbY');
  const mbB = $('#mbYbB');
  if (mbY) mbY.classList.toggle('lit', yLit);
  if (mbB) mbB.classList.toggle('lit', bLit);

  const hudY = $('#hudYbY');
  const hudB = $('#hudYbB');
  if (hudY) hudY.classList.toggle('lit', yLit);
  if (hudB) hudB.classList.toggle('lit', bLit);
}

// ================= 模組綠色狀態燈 (Solved LED) =================
function updateModuleStatusLeds() {
  for (const [mod, isSolved] of Object.entries(state.solved)) {
    const led = $(`#led-status-${mod}`);
    if (led) led.classList.toggle('solved', !!isSolved);
  }
}

// ================= 驗證結果處理 =================
function applyCheckResult(res, modKey) {
  if (res.correct) {
    state.solved[modKey] = true;
    updateModuleStatusLeds();
    if (state.soundFx) state.soundFx.defuse();
    checkAllSolvedWin();
  } else {
    state.strikes = res.strikes;
    updateStrikesUI();
    if (state.soundFx) state.soundFx.strike();
    if (res.gameOver || state.strikes >= state.maxStrikes) {
      triggerGameOver(false, '失誤次數達上限，炸彈引爆！');
    }
  }
}

function checkAllSolvedWin() {
  const allSolved = state.selectedModules.every(m => state.solved[m]);
  if (allSolved && state.running) {
    triggerGameOver(true, '所有安全機關已全數拆除，炸彈成功拆解！');
  }
}

function triggerGameOver(won, msg) {
  state.running = false;
  if (state.timerHandle) clearInterval(state.timerHandle);

  // 關閉放大狀態
  closeZoom();

  const modal = $('#resultModal');
  const title = $('#resultTitle');
  const desc = $('#resultDesc');
  const icon = $('#resultIcon');

  if (won) {
    if (state.soundFx) state.soundFx.win();
    icon.textContent = '🎉';
    title.textContent = '任務成功！';
    title.style.color = 'var(--led-green)';
    desc.textContent = msg || '成功解除所有模組，世界安全了！';
  } else {
    if (state.soundFx) state.soundFx.explode();
    icon.textContent = '💥';
    title.textContent = '孩子，你炸了！';
    title.style.color = 'var(--led-red)';
    desc.textContent = msg || '失誤累積達 3 次或時間耗盡，炸彈引爆。';
  }

  modal.classList.remove('hidden');
}

// ================= 放大與縮小 (ZOOM) 機制 =================
let currentZoomedBay = null;

function zoomBay(bayEl) {
  if (!bayEl || bayEl.classList.contains('mainboard-bay')) return;
  if (currentZoomedBay) closeZoom();

  bayEl.classList.add('is-zoomed');
  currentZoomedBay = bayEl;

  // 顯示右上角 HUD
  const hud = $('#zoomedTopHud');
  if (hud) hud.classList.remove('hidden');

  updateTimerUI();
  updateStrikesUI();
  updateYbUI();

  if (state.soundFx) state.soundFx.click();
}

function closeZoom() {
  if (currentZoomedBay) {
    currentZoomedBay.classList.remove('is-zoomed');
    currentZoomedBay = null;
  }
  const hud = $('#zoomedTopHud');
  if (hud) hud.classList.add('hidden');
}

// ================= 1. 四色按鈕 (FOUR-COLOR) 互動 =================
let fcIntervals = [null, null, null, null];
let fcRunning = [true, true, true, true];
let fcIndices = [0, 0, 0, 0];
const fcColors = ['R', 'G', 'B'];
const fcHex = { R: '#ff1744', G: '#00e676', B: '#00b0ff' };

function initFourColorModule(modData) {
  const speeds = modData?.speeds || [600, 750, 900, 700];

  for (let i = 0; i < 4; i++) {
    if (fcIntervals[i]) clearInterval(fcIntervals[i]);
    fcRunning[i] = true;
    fcIndices[i] = Math.floor(Math.random() * 3);

    const bulb = $(`#fc-led-${i}`);
    if (bulb) bulb.style.background = fcHex[fcColors[fcIndices[i]]];

    fcIntervals[i] = setInterval(() => {
      if (!fcRunning[i] || state.solved.fourColor) return;
      fcIndices[i] = (fcIndices[i] + 1) % 3;
      if (bulb) bulb.style.background = fcHex[fcColors[fcIndices[i]]];
    }, speeds[i]);

    const btn = $(`#fc-btn-${i}`);
    if (btn) {
      btn.onclick = async (e) => {
        e.stopPropagation();
        if (state.solved.fourColor || !state.running) return;
        fcRunning[i] = !fcRunning[i];
        btn.style.borderColor = fcRunning[i] ? '#78909c' : '#ffea00';
        if (state.soundFx) state.soundFx.click();

        // 4 個全停即發送驗證
        if (fcRunning.every(r => !r)) {
          const finalStops = fcIndices.map(idx => fcColors[idx]);
          const res = await apiPost('game/check.php', {
            module: 'fourColor',
            payload: { stops: finalStops }
          });
          applyCheckResult(res, 'fourColor');
          if (!res.correct) {
            // 稍後重新啟動旋轉
            setTimeout(() => {
              for (let k = 0; k < 4; k++) {
                fcRunning[k] = true;
                const b = $(`#fc-btn-${k}`);
                if (b) b.style.borderColor = '#78909c';
              }
            }, 1000);
          }
        }
      };
    }
  }
}

// ================= 3. 複雜密碼 (GREY / CAESAR) 互動 =================
let cipherUserBuffer = [];

function initCipherModule(modData) {
  const isGrey = modData.type === 'grey';
  const typeTag = $('#cipherTypeTag');
  const shiftTag = $('#cipherShiftTag');
  const codeDisp = $('#cipherCodeDisplay');
  const inputDisp = $('#cipherInputDisplay');
  const keypad = $('#cipherKeypad');

  if (typeTag) typeTag.textContent = isGrey ? 'GREY CODE' : 'CAESAR MORSE';
  if (shiftTag) {
    const dir = modData.direction === 'left' ? '←' : '→';
    const bits = Array.isArray(modData.shiftBits) ? modData.shiftBits.join('') : (modData.shiftBinary || '1');
    shiftTag.textContent = `${dir} ${bits}`;
  }
  if (codeDisp) {
    codeDisp.textContent = isGrey ? (modData.bits || []).join('') : (modData.morse || '.-');
  }

  cipherUserBuffer = [];
  if (inputDisp) inputDisp.textContent = 'INPUT: _';

  if (keypad) {
    keypad.innerHTML = '';
    const keys = isGrey ? ['0', '1', 'DEL', 'OK'] : ['.', '-', 'DEL', 'OK'];

    keys.forEach(k => {
      const btn = document.createElement('button');
      btn.className = 'cipher-btn' + (k === 'OK' ? ' action' : '');
      btn.textContent = k;
      btn.onclick = async (e) => {
        e.stopPropagation();
        if (state.solved.greyCaesar || !state.running) return;
        if (state.soundFx) state.soundFx.click();

        if (k === 'DEL') {
          cipherUserBuffer.pop();
        } else if (k === 'OK') {
          if (cipherUserBuffer.length === 0) return;
          if (isGrey) {
            const res = await apiPost('game/check.php', {
              module: 'grey',
              payload: { bits: cipherUserBuffer.map(Number) }
            });
            applyCheckResult(res, 'greyCaesar');
          } else {
            const res = await apiPost('game/check.php', {
              module: 'caesar',
              payload: { morse: cipherUserBuffer.join('') }
            });
            applyCheckResult(res, 'greyCaesar');
          }
          cipherUserBuffer = [];
        } else {
          if (cipherUserBuffer.length < 8) {
            cipherUserBuffer.push(k);
          }
        }
        if (inputDisp) {
          inputDisp.textContent = 'INPUT: ' + (cipherUserBuffer.join('') || '_');
        }
      };
      keypad.appendChild(btn);
    });
  }
}

// ================= 4. 三角翻譯 (TRISLATOR) 互動 =================
let triBuffer = '';

function initTrislatorModule(modData) {
  const round = state.trislatorState?.round || 0;
  const questions = modData?.questions || ['cos 87°', 'sec 1147°'];
  const words = modData?.words || ['_LOCK', 'SPA_E'];

  const qLine = $('#trislatorQLine');
  const inputLine = $('#trislatorInputLine');

  if (qLine) {
    qLine.textContent = questions[round] || 'cos 87°';
  }

  // 數字階段預先填入小數點 .
  triBuffer = '';
  updateTrislatorDisplay();

  $$('.tri-btn').forEach(btn => {
    btn.onclick = async (e) => {
      e.stopPropagation();
      if (state.solved.trislator || !state.running) return;
      if (state.soundFx) state.soundFx.click();
      const k = btn.dataset.k;

      if (k === '*') {
        // 退格
        triBuffer = triBuffer.slice(0, -1);
        updateTrislatorDisplay();
      } else if (k === '#') {
        // 確認送出
        if (state.trislatorState.phase === 'numeric') {
          const valToSend = '.' + triBuffer;
          const res = await apiPost('game/check.php', {
            module: 'trislator_step',
            payload: { type: 'numeric', value: valToSend }
          });
          applyCheckResult(res, 'trislator');
          if (res.correct) {
            state.trislatorState.phase = 'letter';
            triBuffer = '';
            if (qLine) qLine.textContent = words[state.trislatorState.round] || 'WORD';
            updateTrislatorDisplay();
          } else {
            triBuffer = '';
            updateTrislatorDisplay();
          }
        } else {
          // 字母確認
          const res = await apiPost('game/check.php', {
            module: 'trislator_step',
            payload: { type: 'letter', letter: triBuffer }
          });
          applyCheckResult(res, 'trislator');
          if (res.correct) {
            if (res.solved && res.solved.trislator) {
              state.solved.trislator = true;
              updateModuleStatusLeds();
            } else {
              state.trislatorState.round++;
              state.trislatorState.phase = 'numeric';
              triBuffer = '';
              if (qLine) qLine.textContent = questions[state.trislatorState.round] || '';
              updateTrislatorDisplay();
            }
          } else {
            triBuffer = '';
            updateTrislatorDisplay();
          }
        }
      } else if (['A', 'B', 'C', 'D'].includes(k)) {
        if (state.trislatorState.phase === 'numeric') {
          apiPost('game/check.php', { module: 'trislator_step', payload: { type: 'letter', letter: k } })
            .then(res => applyCheckResult(res, 'trislator'));
        } else {
          triBuffer = k;
          updateTrislatorDisplay();
        }
      } else {
        // 數字 0-9
        if (state.trislatorState.phase === 'letter') {
          apiPost('game/check.php', { module: 'trislator_step', payload: { type: 'numeric', value: k } })
            .then(res => applyCheckResult(res, 'trislator'));
        } else {
          if (triBuffer.length < 4) {
            triBuffer += k;
            updateTrislatorDisplay();
          }
        }
      }
    };
  });
}

function updateTrislatorDisplay() {
  const inputLine = $('#trislatorInputLine');
  if (!inputLine) return;
  if (state.trislatorState.phase === 'numeric') {
    inputLine.textContent = 'INPUT: .' + (triBuffer ? triBuffer.padEnd(4, '_') : '____');
  } else {
    inputLine.textContent = 'LETTER: ' + (triBuffer || '_');
  }
}

// ================= 5. 頻率調變 (RESONANCE / TUNE) 互動 =================
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
  { C: 910, unit: 'n' }, { C: 560, unit: 'p' }, { C: 1.2, unit: 'n' }, { C: 3.3, unit: 'n' },
  { C: 6.8, unit: 'p' }, { C: 100, unit: 'n' }, { C: 68, unit: 'n' }, { C: 1.1, unit: 'u' },
  { C: 9.1, unit: 'n' }, { C: 100, unit: 'n' }
];
const rOptions = [
  { R: 7.5, unit: '' }, { R: 330, unit: '' }, { R: 10, unit: '' }, { R: 10, unit: 'k' },
  { R: 4.7, unit: '' }, { R: 150, unit: '' }, { R: 22, unit: '' }, { R: 1.5, unit: 'k' },
  { R: 2.2, unit: '' }, { R: 12, unit: '' }, { R: 5.6, unit: 'k' }, { R: 270, unit: '' },
  { R: 1, unit: 'k' }, { R: 33, unit: '' }, { R: 120, unit: '' }, { R: 8.2, unit: '' },
  { R: 18, unit: '' }, { R: 820, unit: '' }, { R: 1.2, unit: 'k' }, { R: 9.1, unit: '' }
];

let tuneIndices = { f: 0, L: 0, C: 0, R: 0 };

function initTuneModule(modData) {
  const targetRow = modData?.rowNumber || 1;
  const targetVal = $('#tuneTargetVal');
  if (targetVal) targetVal.textContent = `TARGET: ROW ${targetRow}`;

  function refreshTuneDisplays() {
    const curF = fOptions[tuneIndices.f];
    const curL = lOptions[tuneIndices.L];
    const curC = cOptions[tuneIndices.C];
    const curR = rOptions[tuneIndices.R];

    const valF = $('#tuneVal-f'); if (valF) valF.textContent = `${curF.f} ${curF.unit}Hz`;
    const valL = $('#tuneVal-L'); if (valL) valL.textContent = `${curL.L} ${curL.unit}H`;
    const valC = $('#tuneVal-C'); if (valC) valC.textContent = `${curC.C} ${curC.unit}F`;
    const valR = $('#tuneVal-R'); if (valR) valR.textContent = `${curR.R} ${curR.unit}Ω`;
  }

  $$('.tune-dial-row').forEach(row => {
    const param = row.dataset.param;
    const prevBtn = row.querySelector('.dial-btn.prev');
    const nextBtn = row.querySelector('.dial-btn.next');

    const maxLen = param === 'f' ? fOptions.length : (param === 'L' ? lOptions.length : (param === 'C' ? cOptions.length : rOptions.length));

    if (prevBtn) {
      prevBtn.onclick = (e) => {
        e.stopPropagation();
        tuneIndices[param] = (tuneIndices[param] - 1 + maxLen) % maxLen;
        refreshTuneDisplays();
        if (state.soundFx) state.soundFx.click();
      };
    }
    if (nextBtn) {
      nextBtn.onclick = (e) => {
        e.stopPropagation();
        tuneIndices[param] = (tuneIndices[param] + 1) % maxLen;
        refreshTuneDisplays();
        if (state.soundFx) state.soundFx.click();
      };
    }
  });

  const submitBtn = $('#tuneSubmitBtn');
  if (submitBtn) {
    submitBtn.onclick = async (e) => {
      e.stopPropagation();
      if (state.solved.tune || !state.running) return;
      const curF = fOptions[tuneIndices.f];
      const curL = lOptions[tuneIndices.L];
      const curC = cOptions[tuneIndices.C];
      const curR = rOptions[tuneIndices.R];

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
    };
  }

  refreshTuneDisplays();
}

// ================= 6. 資料排序 (SORTING / TROS) 互動 =================
let sortOrder = [null, null, null, null];
let selectedChip = null;

function initSortModule(modData) {
  const segContainer = $('#sortSeg7Svg');
  if (segContainer) {
    segContainer.innerHTML = '';
    const activeSegs = modData?.segments || ['a', 'd'];
    segContainer.appendChild(create7SegmentSvg(activeSegs, 40, 52));
  }

  sortOrder = [null, null, null, null];
  selectedChip = null;
  updateSortSlotsUI();

  // 晶片點選
  $$('.chip-item').forEach(btn => {
    btn.onclick = (e) => {
      e.stopPropagation();
      const chip = btn.dataset.chip;
      if (selectedChip === chip) {
        selectedChip = null;
        btn.classList.remove('active');
      } else {
        $$('.chip-item').forEach(b => b.classList.remove('active'));
        selectedChip = chip;
        btn.classList.add('active');
      }
      if (state.soundFx) state.soundFx.click();
    };
  });

  // 插座點擊
  $$('.ic-slot').forEach(slot => {
    slot.onclick = (e) => {
      e.stopPropagation();
      const idx = parseInt(slot.dataset.slot, 10);
      if (selectedChip) {
        // 若該晶片已經在別的插座，先拔除
        const prevIdx = sortOrder.indexOf(selectedChip);
        if (prevIdx !== -1) sortOrder[prevIdx] = null;
        sortOrder[idx] = selectedChip;
        selectedChip = null;
        $$('.chip-item').forEach(b => b.classList.remove('active'));
      } else {
        // 清空該插槽
        sortOrder[idx] = null;
      }
      updateSortSlotsUI();
      if (state.soundFx) state.soundFx.click();
    };
  });

  const checkBtn = $('#sortSubmitBtn');
  if (checkBtn) {
    checkBtn.onclick = async (e) => {
      e.stopPropagation();
      if (state.solved.sort || !state.running) return;

      const res = await apiPost('game/check.php', {
        module: 'sort',
        payload: { order: sortOrder }
      });

      const lamp = $('#sortOrangeLamp');
      if (res.correct) {
        if (lamp) lamp.classList.remove('lit');
      } else {
        // 橘燈錯誤反饋 (main.pdf 第 11 頁)
        if (lamp) {
          if (res.sortError === 'wrong_position') {
            lamp.classList.add('lit');
          } else {
            lamp.classList.remove('lit');
          }
        }
      }
      applyCheckResult(res, 'sort');
    };
  }
}

function updateSortSlotsUI() {
  for (let i = 0; i < 4; i++) {
    const el = $(`#chip-slot-${i}`);
    if (el) el.textContent = sortOrder[i] ? `IC-${sortOrder[i]}` : '-';
  }
}

// ================= 開局任務啟動 =================
async function launchMission(selMods = ['fourColor', 'greyCaesar', 'trislator', 'tune', 'sort'], timeLimit = 300, maxStrikes = 3) {
  // 前端瞬間預載 05:00
  state.running = true;
  state.timeLimit = timeLimit;
  state.remaining = timeLimit;
  state.maxStrikes = maxStrikes;
  state.strikes = 0;
  state.yb = state.yb || '00';
  state.selectedModules = selMods;
  state.solved = Object.fromEntries(selMods.map(m => [m, false]));
  state.trislatorState = { round: 0, phase: 'numeric' };

  closeZoom();
  $('#resultModal').classList.add('hidden');

  updateTimerUI();
  updateStrikesUI();
  updateYbUI();
  updateModuleStatusLeds();
  startTimer();

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

    // 初始化各個模組
    if (res.modules) {
      if (res.modules.fourColor) initFourColorModule(res.modules.fourColor);
      if (res.modules.greyCaesar) initCipherModule(res.modules.greyCaesar);
      if (res.modules.trislator) initTrislatorModule(res.modules.trislator);
      if (res.modules.tune) initTuneModule(res.modules.tune);
      if (res.modules.sort) initSortModule(res.modules.sort);
    }
  }
}

// ================= 事件監聽與掛載 =================
document.addEventListener('DOMContentLoaded', () => {

  // 1. 點擊模組插槽放大
  $$('.module-bay').forEach(bay => {
    bay.addEventListener('click', (e) => {
      // 避免點擊模組內部元件觸發縮放
      if (e.target.closest('button, .ic-slot, .chip-item')) return;
      zoomBay(bay);
    });
  });

  // 2. 縮小 / 返回全貌
  const hudCloseBtn = $('#hudCloseBtn');
  if (hudCloseBtn) {
    hudCloseBtn.addEventListener('click', (e) => {
      e.stopPropagation();
      closeZoom();
    });
  }

  // 鍵盤 ESC 快捷鍵縮小
  window.addEventListener('keydown', (e) => {
    if (e.key === 'Escape') {
      closeZoom();
      $('#manualModal').classList.add('hidden');
    }
  });

  // 點擊背景縮小
  document.addEventListener('click', (e) => {
    if (currentZoomedBay && !e.target.closest('.module-bay.is-zoomed, #zoomedTopHud')) {
      closeZoom();
    }
  });

  // 3. 拆彈手冊
  const manualFloatingBtn = $('#manualFloatingBtn');
  if (manualFloatingBtn) {
    manualFloatingBtn.addEventListener('click', () => {
      if (window.renderManualContent) {
        window.renderManualContent($('#manualBody'));
      }
      $('#manualModal').classList.remove('hidden');
      if (state.soundFx) state.soundFx.click();
    });
  }
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

  // 4. 再玩一次
  const restartGameBtn = $('#restartGameBtn');
  if (restartGameBtn) {
    restartGameBtn.addEventListener('click', () => {
      launchMission();
    });
  }

  // 5. 點開網頁立即武裝並開始 5 分鐘倒數！
  launchMission();
});
