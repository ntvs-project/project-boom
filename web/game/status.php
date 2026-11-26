<?php
require_once __DIR__ . '/../bootstrap.php';

session_key();

if (empty($_SESSION['game'])) {
    respond(['active' => false]);
}

$game = $_SESSION['game'];
$elapsed = time() - $game['startedAt'];
$remaining = max(0, $game['timeLimit'] - $elapsed);

respond([
    'active' => true,
    'yb' => $game['yb'],
    'strikes' => $game['strikes'],
    'solved' => $game['solved'],
    'remaining' => $remaining,
]);
