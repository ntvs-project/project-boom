<?php
// 每支 API 檔案開頭都會 require 這支
require_once __DIR__ . '/db.php';

session_start();

header('Content-Type: application/json; charset=utf-8');

function json_input(): array {
    $raw = file_get_contents('php://input');
    $data = json_decode($raw, true);
    return is_array($data) ? $data : [];
}

function respond(array $data, int $code = 200): void {
    http_response_code($code);
    echo json_encode($data, JSON_UNESCAPED_UNICODE);
    exit;
}

function session_key(): string {
    return session_id();
}
