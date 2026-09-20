<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Analyze</title>
</head>
<body>

<?php $code = $_GET["code"]; ?>

<p>
time:
<?= substr($code, 1, 3); ?>
</p>

<p>
miss:
<?= substr($code, 5, 1); ?>
</p>

</body>
</html>
