@echo off
chcp 65001 >nul
title Project BOOM 遊戲伺服器
echo ==============================================
echo        Project BOOM 拆彈專案已啟動！
echo ==============================================
echo.
echo 正在瀏覽器中打開遊戲畫面...
echo 伺服器運作於: http://127.0.0.1:8080
echo （請保持此視窗開啟，關閉此視窗將停止伺服器）
echo.

start "" "http://127.0.0.1:8080/public/index.html"
C:\xampp\php\php.exe -S 127.0.0.1:8080 -t .
