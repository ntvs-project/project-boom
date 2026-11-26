<?php
// ==== 資料庫連線設定 ====
// 1. 本機預設設定 (XAMPP / Local)：
define('DB_HOST', '127.0.0.1');
define('DB_NAME', 'project_boom');
define('DB_USER', 'root');
define('DB_PASS', '');

// 2. InfinityFree 線上主機設定範例：
// 若要使用 InfinityFree 的 MySQL 資料庫，請填入控制面板 (cPanel) 提供的資料：
// define('DB_HOST', 'sqlXXX.infinityfree.com');
// define('DB_NAME', 'epiz_XXXXXXX_projectboom');
// define('DB_USER', 'epiz_XXXXXXX');
// define('DB_PASS', '你的控制面板密碼');

// 💡 容錯機制：即使未設定 MySQL，系統也會自動切換至 SQLite 輕量資料庫，遊戲遊玩與存檔功能完全正常！
