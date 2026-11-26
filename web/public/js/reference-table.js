/**
 * Project BOOM - 遊戲查表手冊與參考資料
 * 依據《開發拆彈手冊》（main.pdf）完整製作
 */

const REFERENCE = {
  // 四色按鈕：YB → 四顆 LED 正確停止顏色（順序由左而右）
  // 00: 白白, 10: 黃白, 01: 白藍, 11: 黃藍
  fourColor: {
    '00': ['紅', '紅', '藍', '綠'],
    '10': ['紅', '藍', '紅', '藍'],
    '01': ['綠', '紅', '藍', '綠'],
    '11': ['藍', '綠', '綠', '紅'],
  },
  colorName: { R: '紅', G: '綠', B: '藍' },

  // 標準摩斯電碼表
  morse: {
    A: '.-', B: '-...', C: '-.-.', D: '-..', E: '.', F: '..-.', G: '--.',
    H: '....', I: '..', J: '.---', K: '-.-', L: '.-..', M: '--', N: '-.',
    O: '---', P: '.--.', Q: '--.-', R: '.-.', S: '...', T: '-', U: '..-',
    V: '...-', W: '.--', X: '-..-', Y: '-.--', Z: '--..',
  },

  // 三角翻譯查表（main.pdf 第 6 頁全新 30 題）
  trislator: [
    ['cos 87°', '0.052'],   ['cot 82°', '0.141'],    ['-csc 119°', '-1.143'],
    ['sin 731°', '0.191'],  ['tan 1095°', '0.268'],  ['sec 750°', '1.155'],
    ['-cos 108°', '0.309'], ['-cot 113°', '0.424'],  ['csc 56°', '1.206'],
    ['sin 1107°', '0.454'], ['cot 59°', '0.601'],    ['-csc 220°', '1.556'],
    ['cos 55°', '0.574'],   ['tan 1479°', '0.810'],  ['sec 1483°', '1.367'],
    ['-cos 132°', '0.669'], ['cot 44°', '1.036'],    ['csc 35°', '1.743'],
    ['sin 1491°', '0.777'], ['-cot 144°', '1.376'],  ['-csc 155°', '-2.366'],
    ['cos 31°', '0.857'],   ['cot 29°', '1.804'],    ['sec 1147°', '2.559'],
    ['-cos 204°', '0.914'], ['tan 789°', '2.605'],   ['csc 20°', '2.924'],
    ['cos 17°', '0.956'],   ['-cot 256°', '-0.249'], ['sec 797°', '4.445'],
  ],

  // 頻率調變元件表（main.pdf 第 9 頁）
  tune: [
    { n: 1,  f: '5.03 kHz',  X: '34.77 Ω',   L: '1.1 mH',  C: '910 nF', R: { '00': '6.8 kΩ', '10': '27 Ω',   '01': '220 Ω', '11': '3.3 Ω' } },
    { n: 2,  f: '11 kHz',    X: '25.84 Ω',   L: '374 µH',  C: '560 nF', R: { '00': '4.7 kΩ', '10': '5.6 Ω',  '01': '100 Ω', '11': '47 Ω' } },
    { n: 3,  f: '15 kHz',    X: '8.16 Ω',    L: '86.6 µH', C: '1.3 µF', R: { '00': '1 Ω',    '10': '680 Ω',  '01': '3.3 kΩ','11': '15 Ω' } },
    { n: 4,  f: '16 kHz',    X: '3.02 Ω',    L: '30 µH',   C: '3.3 µF', R: { '00': '470 Ω',  '10': '68 Ω',   '01': '1.5 Ω', '11': '3.9 kΩ' } },
    { n: 5,  f: '19.3 kHz',  X: '1212.68 Ω', L: '10 mH',   C: '6.8 nF', R: { '00': '2.2 kΩ', '10': '390 Ω',  '01': '56 Ω',  '11': '6.8 Ω' } },
    { n: 6,  f: '22 kHz',    X: '72.32 Ω',   L: '523 µH',  C: '100 nF', R: { '00': '7.5 Ω',  '10': '330 Ω',  '01': '10 Ω',  '11': '10 kΩ' } },
    { n: 7,  f: '43 kHz',    X: '5.97 Ω',    L: '22.1 µH', C: '620 nF', R: { '00': '4.7 Ω',  '10': '150 Ω',  '01': '22 Ω',  '11': '1.5 kΩ' } },
    { n: 8,  f: '50.3 kHz',  X: '2876.23 Ω', L: '9.1 mH',  C: '1.1 nF', R: { '00': '2.2 Ω',  '10': '12 Ω',   '01': '5.6 kΩ','11': '270 Ω' } },
    { n: 9,  f: '51 kHz',    X: '342.90 Ω',  L: '1.07 mH', C: '9.1 nF', R: { '00': '1 kΩ',   '10': '33 Ω',   '01': '120 Ω', '11': '8.2 Ω' } },
    { n: 10, f: '91 kHz',    X: '17.15 Ω',   L: '30 µH',   C: '102 nF', R: { '00': '18 Ω',   '10': '820 Ω',  '01': '1.2 kΩ','11': '9.1 Ω' } },
  ],

  // 資料排序題目與七段顯示器符號（main.pdf 第 11~12 頁全新 12 題）
  sortQuestions: [
    {
      symbolName: '≡（三橫線）',
      segments: ['a', 'g', 'd'],
      title: '滅火器操作步驟依序為：',
      options: '(A) 瞄準火源　(B) 左右掃射　(C) 拉插梢　(D) 壓下握把',
      answer: 'C → A → D → B（拉、瞄、壓、掃）'
    },
    {
      symbolName: '0（外圈六段）',
      segments: ['a', 'b', 'c', 'd', 'e', 'f'],
      title: '心肺復甦術操作步驟依序為：呼叫患者、尋求協助、＿、＿、＿、＿',
      options: '(A) 暢通呼吸道　(B) 人工呼吸　(C) 胸外按壓　(D) 使用 AED',
      answer: 'C → A → B → D（叫叫CABD）'
    },
    {
      symbolName: 'F',
      segments: ['a', 'e', 'f', 'g'],
      title: '以下記憶體之傳輸速度由快到慢排序：',
      options: '(A) 主記憶體　(B) 輔助記憶體　(C) 暫存器　(D) 快取記憶體',
      answer: 'C → D → A → B'
    },
    {
      symbolName: 'U',
      segments: ['b', 'c', 'd', 'e', 'f'],
      title: '以下介面之傳輸速度由快到慢排序：',
      options: '(A) SAS　(B) SCSI　(C) IDE　(D) SATA',
      answer: 'A → D → B → C'
    },
    {
      symbolName: '=',
      segments: ['g', 'd'],
      title: '指令執行之基本流程依序為：',
      options: '(A) 解碼　(B) 提取　(C) 執行　(D) 儲存',
      answer: 'B → A → C → D（提取 → 解碼 → 執行 → 儲存）'
    },
    {
      symbolName: 'L',
      segments: ['d', 'e', 'f'],
      title: '以下 C++ 語言變數型態所占之記憶體大小由小到大排序：',
      options: '(A) double　(B) short　(C) char　(D) float',
      answer: 'C → B → D → A（char < short < float < double）'
    },
    {
      symbolName: 'o（下半方框）',
      segments: ['c', 'd', 'e', 'g'],
      title: '以下 C++ 語言連算子的優先順序依序為：',
      options: '(A) &　(B) !　(C) +　(D) <<',
      answer: 'B → C → D → A（! > + > << > &）'
    },
    {
      symbolName: '||（兩垂直雙柱）',
      segments: ['b', 'c', 'e', 'f'],
      title: '以下 80x86 中斷的優先順序依序為：',
      options: '(A) 重置　(B) 程式中斷　(C) 單步中斷　(D) 軟體中斷',
      answer: 'A → B → D → C（重置 > 程式異常 > 軟體中斷 > 單步）'
    },
    {
      symbolName: '5 / S',
      segments: ['a', 'f', 'g', 'c', 'd'],
      title: 'USB 2.0 的四個信號接腳從 1 到 4 依序是：',
      options: '(A) D+　(B) VBUS　(C) D-　(D) GND',
      answer: 'B → C → A → D（1:VBUS, 2:D-, 3:D+, 4:GND）'
    },
    {
      symbolName: '[（左中括號）',
      segments: ['a', 'f', 'e', 'd'],
      title: '組合語言的開發流程依序為：',
      options: '(A) 執行　(B) 載入　(C) 連結　(D) 組譯',
      answer: 'D → C → B → A（組譯 → 連結 → 載入 → 執行）'
    },
    {
      symbolName: '=（頂底雙橫線）',
      segments: ['a', 'd'],
      title: '以下四位元二的補數由小到大排列：',
      options: '(A) 0010　(B) 1101　(C) 1011　(D) 0111',
      answer: 'C → B → A → D（-5 < -3 < +2 < +7）'
    },
    {
      symbolName: ']（右中括號）',
      segments: ['a', 'b', 'c', 'd'],
      title: '假設 char name[] = "kyz";，將下列運算結果由大到小排序：',
      options: '(A) strlen(name)*34 (102)　(B) sizeof(name)*34 (136)　(C) (int)(*name) (107)　(D) (int)(2[name]) (122)',
      answer: 'B → D → C → A'
    },
  ],
};

// 渲染拆彈手冊內容
window.renderManualContent = function(holder) {
  if (!holder) return;
  holder.innerHTML = `
    <h3>【炸彈組成與勝負條件】</h3>
    <p>
      炸彈包含五個模組與一個主板。主板上有計時器、三個紅色失誤指示燈和一組黃藍特殊指示燈（Y B）。每個模組都有一個綠色狀態指示燈。<br>
      <strong>勝負條件：</strong>當所有模組綠色狀態指示燈全部亮起，代表拆彈成功；若累積 3 次失誤使三個紅色指示燈全亮，代表：<strong>孩子，你炸了！</strong>
    </p>

    <h3>【四色按鈕】(Four-Color)</h3>
    <p>
      模組內有四顆 LED 循環切換紅、綠、藍三色。按下方按鈕停止切換，再次按下恢復循環。<br>
      請在正確時機讓四顆 LED 停在正確顏色上：
    </p>
    <table>
      <tr><th>特殊指示燈狀態 (Y B)</th><th>由左至右正確停止顏色</th></tr>
      <tr><td>白白 (00)</td><td>紅、紅、藍、綠</td></tr>
      <tr><td>黃白 (10)</td><td>紅、藍、紅、藍</td></tr>
      <tr><td>白藍 (01)</td><td>綠、紅、藍、綠</td></tr>
      <tr><td>黃藍 (11)</td><td>藍、綠、綠、紅</td></tr>
    </table>

    <h3>【格雷密碼 / 凱撒密碼】</h3>
    <p>
      <strong>在其它所有模組都解開後，此模組才會啟用。</strong><br>
      長聲代表 1；短按為 0（長按需按住直到 LED 變為靛色再放開）。<br>
      • <strong>YB 為 00 或 11</strong>：凱撒密碼（00 字母左移，11 字母右移）。<br>
      • <strong>YB 為 10</strong>：二進制轉格雷碼（第一位不變，其餘未轉換前一位與當前位元做 XOR）。<br>
      • <strong>YB 為 01</strong>：格雷碼轉二進制（第一位不變，其餘已轉換前一位與當前位元做 XOR）。
    </p>

    <h3>【三角翻譯】(Trislator)</h3>
    <p>
      第一行顯示題目；第二行顯示等號並輸入答案（<strong>小數點「.」已固定顯示於模組螢幕上</strong>，請直接依序輸入數值，負數亦直接輸入數字）。<br>
      答案正確後第一行轉為挖空單字，依前後字母猜出正確字母，用鍵盤 A B C D 輸入並按 # 送出。共 5 組題目。
    </p>
    <table>
      <tr><th>題目</th><th>答案</th><th>題目</th><th>答案</th><th>題目</th><th>答案</th></tr>
      ${REFERENCE.trislator.reduce((acc, [q, a], i) => {
        if (i % 3 === 0) acc += '<tr>';
        acc += `<td><strong>${q}</strong></td><td style="color:#b91c1c;">${a}</td>`;
        if (i % 3 === 2 || i === REFERENCE.trislator.length - 1) acc += '</tr>';
        return acc;
      }, '')}
    </table>

    <h3>【頻率調變】(Resonance)</h3>
    <p>
      1. 轉動 f，直到兩顆紫色 LED 閃爍頻率一致（諧振）。<br>
      2. 由上而下數指示條亮起位置（1~10）為對應資料組別。<br>
      3. 轉動 L 和 C 使數值與表格一致。<br>
      4. 依照 Y B 指示燈對照表格轉動 R 電阻值，最後按下鎖定送出。
    </p>
    <table>
      <tr><th rowspan="2">#</th><th rowspan="2">f</th><th rowspan="2">L</th><th rowspan="2">C</th><th colspan="4">R</th></tr>
      <tr><th>白白(00)</th><th>黃白(10)</th><th>白藍(01)</th><th>黃藍(11)</th></tr>
      ${REFERENCE.tune.map(t => `
        <tr>
          <td>${t.n}</td><td>${t.f}</td><td>${t.L}</td><td>${t.C}</td>
          <td>${t.R['00']}</td><td>${t.R['10']}</td><td>${t.R['01']}</td><td>${t.R['11']}</td>
        </tr>
      `).join('')}
    </table>

    <h3>【資料排序】(Data Sorting)</h3>
    <p>
      依照七段顯示器符號判斷題目，將四片晶片拔起後依敘述排列並插入：<br>
      • <strong>YB 00 或 01</strong>：由左至右依序插入。<br>
      • <strong>YB 10 或 11</strong>：由右至左依序插入。<br>
      <em>提示：有晶片未插時橘燈常亮；插錯位置時橘燈不亮；正確插滿時橘燈變綠燈！</em>
    </p>
    <table>
      <tr><th>符號</th><th>題目與選項</th><th>正確順序</th></tr>
      ${REFERENCE.sortQuestions.map(s => `
        <tr>
          <td><strong>${s.symbolName}</strong></td>
          <td style="text-align:left;">${s.title}<br><small style="color:#555;">${s.options}</small></td>
          <td style="color:#b91c1c;font-weight:700;">${s.answer}</td>
        </tr>
      `).join('')}
    </table>
  `;
};
