<?php
require_once __DIR__ . '/config.php';

function get_db(): PDO {
    static $pdo = null;
    if ($pdo !== null) {
        return $pdo;
    }

    // 1. 優先嘗試連接 MySQL
    try {
        $dsn = "mysql:host=" . DB_HOST . ";dbname=" . DB_NAME . ";charset=utf8mb4";
        $pdo = new PDO($dsn, DB_USER, DB_PASS, [
            PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
            PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
        ]);
        return $pdo;
    } catch (Throwable $e) {
        // MySQL 連接失敗，自動切換至 SQLite 備用，確保存檔與讀檔功能正常運作
    }

    // 2. 本地 SQLite 備用資料庫
    $sqlitePath = __DIR__ . '/project_boom.sqlite';
    $needInit = !file_exists($sqlitePath);

    $pdo = new PDO("sqlite:" . $sqlitePath, null, null, [
        PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
        PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
    ]);

    if ($needInit) {
        $pdo->exec("
            CREATE TABLE IF NOT EXISTS game_sessions (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                session_key TEXT NOT NULL,
                yb TEXT NOT NULL,
                modules_json TEXT NOT NULL,
                public_json TEXT NOT NULL,
                answers_json TEXT NOT NULL,
                solved_json TEXT NOT NULL,
                strikes INTEGER DEFAULT 0,
                max_strikes INTEGER DEFAULT 3,
                time_limit INTEGER DEFAULT 300,
                remaining INTEGER DEFAULT 300,
                trislator_state_json TEXT,
                status TEXT DEFAULT 'playing',
                started_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
            );

            CREATE TABLE IF NOT EXISTS game_history (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                session_key TEXT,
                result TEXT,
                time_used INTEGER,
                strikes INTEGER,
                created_at DATETIME DEFAULT CURRENT_TIMESTAMP
            );
        ");
    }

    return $pdo;
}
