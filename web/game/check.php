<?php
require_once __DIR__ . '/../bootstrap.php';

$sessKey = session_key();

if (empty($_SESSION['game'])) {
    respond(['error' => '目前沒有進行中的局，請先開新局'], 400);
}

$in = json_input();
$module = $in['module'] ?? '';
$payload = $in['payload'] ?? [];

$game = &$_SESSION['game'];
$answers = $game['answers'];

function add_strike(&$game): void {
    $game['strikes']++;
}

// 依手冊規則：在其它所有模組都解開後，格雷密碼/凱撒密碼才會啟用
function greyCaesar_unlocked($game): bool {
    $others = array_diff($game['modules'], ['greyCaesar']);
    foreach ($others as $m) {
        if (empty($game['solved'][$m])) return false;
    }
    return true;
}

$correct = false;
$sortError = null; // 'uninserted' (未插入，橘燈常亮) 或 'wrong_position' (位置錯誤，橘燈不會亮)

switch ($module) {

    case 'fourColor':
        $stops = $payload['stops'] ?? [];
        $correct = ($stops === $answers['fourColor']);
        if ($correct) {
            $game['solved']['fourColor'] = true;
        } else {
            add_strike($game);
        }
        break;

    case 'grey':
        if (!greyCaesar_unlocked($game)) respond(['error' => '此模組尚未啟用，需先解開其他模組'], 400);
        $bits = $payload['bits'] ?? [];
        $correct = ($bits === $answers['greyCaesar']);
        if ($correct) {
            $game['solved']['greyCaesar'] = true;
        } else {
            add_strike($game);
        }
        break;

    case 'caesar':
        if (!greyCaesar_unlocked($game)) respond(['error' => '此模組尚未啟用，需先解開其他模組'], 400);
        $morseInput = trim($payload['morse'] ?? '');
        $correct = ($morseInput === $answers['greyCaesar']);
        if ($correct) {
            $game['solved']['greyCaesar'] = true;
        } else {
            add_strike($game);
        }
        break;

    case 'trislator_step':
        $state = $game['trislatorState'] ?? ['round' => 0, 'phase' => 'numeric'];
        $round = $state['round'];
        $phase = $state['phase'];
        $type = $payload['type'] ?? '';

        if ($round >= 5) {
            respond(['error' => '此模組已完成'], 400);
        }
        if ($type !== $phase) {
            // 在不對的時機作答
            add_strike($game);
            $correct = false;
            break;
        }

        if ($phase === 'numeric') {
            $value = trim($payload['value'] ?? '');
            $correctVal = trim($answers['trislator']['numeric'][$round]);
            $isNumMatch = false;
            if ($value === $correctVal) {
                $isNumMatch = true;
            } elseif (is_numeric($value) && is_numeric($correctVal)) {
                $numVal = (float)$value;
                $numCorrect = (float)$correctVal;
                // 允許完全相等，或因鍵盤無負號比對絕對值相等（支援 -1.143 輸入 1.143）
                if (abs($numVal - $numCorrect) < 0.0001 || abs(abs($numVal) - abs($numCorrect)) < 0.0001) {
                    $isNumMatch = true;
                }
            }
            $correct = $isNumMatch;
            if ($correct) {
                $state['phase'] = 'letter';
            } else {
                add_strike($game);
            }
        } else { // phase === 'letter'
            $letter = strtoupper(trim($payload['letter'] ?? ''));
            $correctLetter = $answers['trislator']['letters'][$round];
            $correct = ($letter === $correctLetter);
            if ($correct) {
                $state['round'] = $round + 1;
                $state['phase'] = 'numeric';
                if ($state['round'] >= 5) {
                    $game['solved']['trislator'] = true;
                }
            } else {
                add_strike($game);
            }
        }
        $game['trislatorState'] = $state;
        break;

    case 'tune':
        $ans = $answers['tune'];
        $sameNum = function($a, $b) {
            return abs((float)$a - (float)$b) < 0.001;
        };
        $fOk = $sameNum($payload['f'] ?? 0, $ans['f']) && (($payload['fUnit'] ?? '') === $ans['fUnit']);
        $lOk = $sameNum($payload['L'] ?? 0, $ans['L']) && (($payload['LUnit'] ?? '') === $ans['LUnit']);
        $cOk = $sameNum($payload['C'] ?? 0, $ans['C']) && (($payload['CUnit'] ?? '') === $ans['CUnit']);
        $rOk = $sameNum($payload['R'] ?? 0, $ans['R']) && (($payload['RUnit'] ?? '') === $ans['RUnit']);
        $correct = $fOk && $lOk && $cOk && $rOk;
        if ($correct) {
            $game['solved']['tune'] = true;
        } else {
            add_strike($game);
        }
        break;

    case 'sort':
        // main.pdf 第 11 頁：
        // 有子板未插入，橘色指示燈會常亮 (uninserted)
        // 有子板位置錯誤，橘色指示燈會不會亮 (wrong_position)
        $order = $payload['order'] ?? [];
        if (count($order) < 4 || in_array('', $order, true) || in_array(null, $order, true)) {
            $sortError = 'uninserted';
            $correct = false;
            add_strike($game);
        } else {
            $isMatch = ($order === $answers['sort']) || (!empty($answers['sort_alt']) && $order === $answers['sort_alt']);
            if ($isMatch) {
                $correct = true;
                $game['solved']['sort'] = true;
            } else {
                $sortError = 'wrong_position';
                $correct = false;
                add_strike($game);
            }
        }
        break;

    default:
        respond(['error' => '未知的模組'], 400);
}

// ---- 判斷輸贏 ----
$allSolved = !in_array(false, $game['solved'], true);
$elapsed = time() - $game['startedAt'];
$timeUp = $elapsed >= $game['timeLimit'];
$gameOver = false;
$won = false;
$maxStrikes = $game['maxStrikes'] ?? 3;

if ($game['strikes'] >= $maxStrikes) {
    $gameOver = true; $won = false;
} elseif ($allSolved) {
    $gameOver = true; $won = true;
} elseif ($timeUp) {
    $gameOver = true; $won = false;
}

if ($gameOver) {
    try {
        $db = get_db();
        $stmt = $db->prepare('INSERT INTO game_history (session_key, result, time_used, strikes) VALUES (?, ?, ?, ?)');
        $stmt->execute([$sessKey, $won ? 'won' : 'lost', $elapsed, $game['strikes']]);
    } catch (Throwable $e) {}
}

respond([
    'ok' => true,
    'correct' => $correct,
    'sortError' => $sortError,
    'strikes' => $game['strikes'],
    'maxStrikes' => $maxStrikes,
    'solved' => $game['solved'],
    'trislatorState' => $game['trislatorState'] ?? null,
    'gameOver' => $gameOver,
    'won' => $won,
]);
