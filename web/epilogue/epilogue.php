
<?php

// ?code=t123m4
$code = $_GET["code"];
$time = substr($code, 1, 3);
$minute = (int)($time / 60);
$second = $time % 60;
$miss = substr($code, 5, 1);

?>

<p>
  恭喜你通過 Project BOOM 的考驗，<br>
  這裡只有頂尖的入才可以加入。<br>
  你花了 <?= $minute ?> 分鐘 <?= $second ?> 秒解開這顆炸彈；且失誤了 <?= $miss ?> 次。<br>
</p>
