/**
 * Project BOOM 音效系統
 * 全部用 Web Audio API 即時合成，不依賴任何外部音檔素材
 */
const SFX = (function () {
  let ctx = null;
  function getCtx() {
    if (!ctx) ctx = new (window.AudioContext || window.webkitAudioContext)();
    if (ctx.state === 'suspended') ctx.resume();
    return ctx;
  }

  function tone({ freq = 440, duration = 0.12, type = 'sine', gain = 0.15, freqEnd = null, delay = 0 }) {
    const c = getCtx();
    const osc = c.createOscillator();
    const g = c.createGain();
    osc.type = type;
    osc.frequency.setValueAtTime(freq, c.currentTime + delay);
    if (freqEnd) osc.frequency.linearRampToValueAtTime(freqEnd, c.currentTime + delay + duration);
    g.gain.setValueAtTime(gain, c.currentTime + delay);
    g.gain.exponentialRampToValueAtTime(0.001, c.currentTime + delay + duration);
    osc.connect(g); g.connect(c.destination);
    osc.start(c.currentTime + delay);
    osc.stop(c.currentTime + delay + duration + 0.02);
  }

  // 單一嗶聲，回傳 Promise，播完（含間隔）才 resolve，方便 await 排隊播放
  function beep(freq, durationMs, gapMs) {
    return new Promise((resolve) => {
      tone({ freq, duration: durationMs / 1000, type: 'sine', gain: 0.2 });
      setTimeout(resolve, durationMs + (gapMs ?? durationMs * 0.4));
    });
  }

  const DIH = 100;       // 摩斯電碼「點」的基本單位（毫秒）
  const DAH = DIH * 3;   // 摩斯電碼「劃」

  // 依序播放一串摩斯碼字串（例如 ".-.."），高音表示密文
  async function playMorseString(morse, freq = 700) {
    for (const ch of morse) {
      await beep(freq, ch === '-' ? DAH : DIH);
    }
  }

  // 依序播放 0/1 位元陣列，長聲=1、短聲=0（用於格雷密碼模組）
  async function playBits(bits, freq = 500) {
    for (const b of bits) {
      await beep(freq, b === 1 ? DAH : DIH);
    }
  }

  return {
    beep, playMorseString, playBits,
    // 一般按鈕點擊
    click() { tone({ freq: 720, duration: 0.05, type: 'square', gain: 0.08 }); },
    // 旋鈕轉動喀一聲（有段旋鈕的卡點感）
    knobTick() { tone({ freq: 300, duration: 0.03, type: 'square', gain: 0.06 }); },
    // 蜂鳴器：長音（摩斯/格雷碼的「劃」或 1）
    buzzLong() { tone({ freq: 480, duration: 0.32, type: 'sine', gain: 0.18 }); },
    // 蜂鳴器：短音（摩斯/格雷碼的「點」或 0）
    buzzShort() { tone({ freq: 480, duration: 0.1, type: 'sine', gain: 0.18 }); },
    // 答對
    correct() {
      tone({ freq: 660, duration: 0.1, type: 'triangle', gain: 0.15 });
      tone({ freq: 880, duration: 0.15, type: 'triangle', gain: 0.15, delay: 0.09 });
    },
    // 答錯 / 觸發機關
    strike() {
      tone({ freq: 180, duration: 0.25, type: 'sawtooth', gain: 0.2, freqEnd: 90 });
    },
    // 模組解開
    solved() {
      tone({ freq: 523, duration: 0.1, type: 'triangle', gain: 0.16 });
      tone({ freq: 659, duration: 0.1, type: 'triangle', gain: 0.16, delay: 0.1 });
      tone({ freq: 784, duration: 0.2, type: 'triangle', gain: 0.16, delay: 0.2 });
    },
    // 拆彈成功
    win() {
      [523, 659, 784, 1046].forEach((f, i) => tone({ freq: f, duration: 0.18, type: 'triangle', gain: 0.18, delay: i * 0.13 }));
    },
    // 爆炸／失敗
    lose() {
      tone({ freq: 220, duration: 0.6, type: 'sawtooth', gain: 0.25, freqEnd: 40 });
      tone({ freq: 110, duration: 0.8, type: 'square', gain: 0.2, freqEnd: 30, delay: 0.05 });
    },
    // 計時器低於 30 秒時的嗶嗶聲（呼叫端自行控制頻率）
    timerBeep() { tone({ freq: 900, duration: 0.06, type: 'square', gain: 0.1 }); },
  };
})();
