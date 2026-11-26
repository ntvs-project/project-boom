<?php
require_once __DIR__ . '/../bootstrap.php';
require_once __DIR__ . '/data.php';

$sessKey = session_key();

$in = json_input();
$selectedModules = $in['modules'] ?? ['fourColor', 'greyCaesar', 'trislator', 'tune', 'sort'];
$timeLimit = (int)($in['timeLimit'] ?? 300);
$maxStrikes = (int)($in['maxStrikes'] ?? 3);

// 邊界檢查
$validModules = ['fourColor', 'greyCaesar', 'trislator', 'tune', 'sort'];
$selectedModules = array_values(array_intersect($selectedModules, $validModules));
if (empty($selectedModules)) {
    respond(['error' => '至少要選一個模組'], 400);
}
if ($timeLimit < 30) $timeLimit = 30;
if ($timeLimit > 3600) $timeLimit = 3600;
if ($maxStrikes < 1) $maxStrikes = 1;
if ($maxStrikes > 20) $maxStrikes = 20;

// ---- 產生特殊指示燈 YB ----
// 00: 白白, 01: 白藍, 10: 黃白, 11: 黃藍
$yb_options = ['00', '01', '10', '11'];
$yb = $yb_options[array_rand($yb_options)];

$answers = [];
$public  = [];
$solved  = [];

$has = fn($m) => in_array($m, $selectedModules, true);

// ---- 模組一：四色按鈕 ----
if ($has('fourColor')) {
    $targets = fourcolor_targets($yb);
    $answers['fourColor'] = $targets;
    $public['fourColor'] = [
        'ledCount' => 4,
        'cycleColors' => ['R', 'G', 'B'],
        'speeds' => [rand(600, 1000), rand(600, 1000), rand(600, 1000), rand(600, 1000)],
    ];
    $solved['fourColor'] = false;
}

// ---- 模組二/三：格雷密碼 or 凱撒密碼 ----
if ($has('greyCaesar')) {
    $morse = morse_table();
    // 依手冊第 3、4 頁：
    // 若特殊指示燈為 10（黃白）或 01（白藍），則此為格雷密碼模組
    // 若特殊指示燈為 00（白白）或 11（黃藍），則此為凱撒密碼模組
    if ($yb === '01' || $yb === '10') {
        $bitLen = 4;
        $sourceBits = [];
        for ($i = 0; $i < $bitLen; $i++) $sourceBits[] = rand(0, 1);

        if ($yb === '10') {
            // Y(黃) B(白)：數值為二進位，請將它轉換成格雷碼
            $target = binary_to_gray($sourceBits);
            $mode = 'bin2gray';
        } else { // '01'
            // Y(白) B(藍)：數值為格雷碼，請將它轉換成二進位
            $target = gray_to_binary($sourceBits);
            $mode = 'gray2bin';
        }

        $answers['greyCaesar'] = $target;
        $public['greyCaesarModule'] = 'grey';
        $public['grey'] = [
            'mode' => $mode,
            'sourceBits' => $sourceBits,
            'bitLen' => $bitLen,
        ];
        $public['greyCaesar'] = [
            'type' => 'grey',
            'mode' => $mode,
            'sourceBits' => $sourceBits,
            'bitLen' => $bitLen,
        ];
    } else {
        // '00' 或 '11'：凱撒密碼模組
        $letters = range('A', 'Z');
        $cipherLetter = $letters[array_rand($letters)];
        $shift = rand(1, 7);
        // 若特殊指示燈為 00 (白白) 則為左移；若為 11 (黃藍) 則為右移
        $direction = ($yb === '00') ? 'left' : 'right';
        $idx = array_search($cipherLetter, $letters);
        $plainIdx = ($direction === 'left') ? (($idx - $shift + 26) % 26) : (($idx + $shift) % 26);
        $plainLetter = $letters[$plainIdx];

        $answers['greyCaesar'] = $morse[$plainLetter];
        $public['greyCaesarModule'] = 'caesar';
        $public['caesar'] = [
            'cipherMorse' => $morse[$cipherLetter],
            'shiftBinary' => str_pad(decbin($shift), 3, '0', STR_PAD_LEFT),
            'direction' => $direction,
            'bitLen' => strlen($morse[$plainLetter]), // 白色 LED 亮起數量代表明文的位元數（摩斯碼點劃數）
        ];
        $public['greyCaesar'] = [
            'type' => 'caesar',
            'morse' => $morse[$cipherLetter],
            'shiftBits' => array_map('intval', str_split(str_pad(decbin($shift), 3, '0', STR_PAD_LEFT))),
            'direction' => $direction,
            'bitLen' => strlen($morse[$plainLetter]),
        ];
    }
    $solved['greyCaesar'] = false;
}

// ---- 模組四：三角翻譯 ----
if ($has('trislator')) {
    $trisTable = trislator_table();
    shuffle($trisTable);
    $trisQuestions = array_slice($trisTable, 0, 5);
    $words = trislator_words();
    shuffle($words);
    $trisWords = array_slice($words, 0, 5);

    $answers['trislator'] = [
        'numeric' => array_map(fn($q) => $q['a'], $trisQuestions),
        'letters' => array_map(fn($w) => $w['word'][$w['blank']], $trisWords),
    ];
    $public['trislator'] = [
        'questions' => array_map(fn($q) => $q['q'], $trisQuestions),
        'words' => array_map(function($w) {
            $chars = str_split($w['word']);
            $chars[$w['blank']] = '_';
            return implode('', $chars);
        }, $trisWords),
    ];
    $solved['trislator'] = false;
    $trislatorState = ['round' => 0, 'phase' => 'numeric'];
}

// ---- 模組五：頻率調變 ----
if ($has('tune')) {
    $tuneTable = tune_table();
    $rowIndex = array_rand($tuneTable);
    $row = $tuneTable[$rowIndex];
    $correctR = $row['R'][$yb];
    $correctRUnit = $row['Runit'][$yb];

    $answers['tune'] = [
        'f' => $row['f'], 'fUnit' => $row['unit'],
        'L' => $row['L'], 'LUnit' => $row['Lunit'],
        'C' => $row['C'], 'CUnit' => $row['Cunit'],
        'R' => $correctR, 'RUnit' => $correctRUnit,
    ];
    // rowNumber: 1 ~ 10，對應指示條由上而下亮起位置
    $public['tune'] = ['rowNumber' => $rowIndex + 1];
    $solved['tune'] = false;
}

// ---- 模組六：資料排序 ----
if ($has('sort')) {
    $sortQs = sort_questions();
    $sIndex = array_rand($sortQs);
    $sq = $sortQs[$sIndex];
    // 手冊第 8 頁：
    // 00 或 11：由左至右依序插入 (ltr)
    // 10 或 01：由右至左依序插入 (rtl)
    $direction = ($yb === '00' || $yb === '11') ? 'ltr' : 'rtl';
    $correctOrder = $sq['order'];
    $altOrder = $sq['alt_order'] ?? null;

    if ($direction === 'rtl') {
        $correctOrder = array_reverse($correctOrder);
        if ($altOrder) $altOrder = array_reverse($altOrder);
    }

    $answers['sort'] = $correctOrder;
    if ($altOrder) {
        $answers['sort_alt'] = $altOrder;
    }

    $public['sort'] = [
        'id' => $sq['id'],
        'symbol' => $sq['symbol'],
        'segments' => $sq['segments'],
        'title' => $sq['title'],
        'items' => $sq['items'],
        'direction' => $direction,
    ];
    $solved['sort'] = false;
}

// ---- 存進 session ----
$_SESSION['game'] = [
    'yb' => $yb,
    'modules' => $selectedModules,
    'answers' => $answers,
    'solved' => $solved,
    'strikes' => 0,
    'maxStrikes' => $maxStrikes,
    'startedAt' => time(),
    'timeLimit' => $timeLimit,
    'trislatorState' => $trislatorState ?? null,
];
$_SESSION['game_public'] = $public;

respond([
    'ok' => true,
    'yb' => $yb,
    'timeLimit' => $timeLimit,
    'maxStrikes' => $maxStrikes,
    'selectedModules' => $selectedModules,
    'modules' => $public,
    'trislatorState' => $trislatorState ?? null,
]);
