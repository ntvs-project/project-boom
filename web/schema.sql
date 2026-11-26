-- Project BOOM 網頁版資料庫結構（無帳號版本）
-- 匯入方式：用 phpMyAdmin 開新資料庫 project_boom，再貼這份 SQL 執行
-- 說明：不使用帳號登入，改以瀏覽器的 PHP session id 當作識別依據，
--       所以只要是同一個瀏覽器（沒清 cookie），存檔/讀檔就找得到。

CREATE TABLE game_sessions (
  id INT AUTO_INCREMENT PRIMARY KEY,
  session_key VARCHAR(128) NOT NULL,    -- PHP session id，用來辨識「這是哪個瀏覽器」
  yb VARCHAR(2) NOT NULL,               -- '00' '01' '10' '11'
  modules_json TEXT NOT NULL,           -- 這局選了哪些模組 ["fourColor","tune",...]
  public_json TEXT NOT NULL,            -- 可以給前端看的題目資訊
  answers_json TEXT NOT NULL,           -- 正確答案，只有 PHP 會讀
  solved_json TEXT NOT NULL,            -- 各模組是否已解開 {fourColor:false,...}
  strikes INT DEFAULT 0,
  max_strikes INT DEFAULT 3,            -- 幾次失誤結束遊戲
  time_limit INT DEFAULT 300,           -- 秒
  remaining INT DEFAULT 300,            -- 存檔當下剩餘秒數
  trislator_state_json TEXT NULL,       -- 三角翻譯目前進行到第幾題/第幾階段
  started_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  status VARCHAR(10) DEFAULT 'playing', -- playing / won / lost
  updated_at DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  INDEX (session_key)
);

CREATE TABLE game_history (
  id INT AUTO_INCREMENT PRIMARY KEY,
  session_key VARCHAR(128) NOT NULL,
  result VARCHAR(10) NOT NULL,   -- won / lost
  time_used INT NOT NULL,
  strikes INT NOT NULL,
  finished_at DATETIME DEFAULT CURRENT_TIMESTAMP,
  INDEX (session_key)
);
