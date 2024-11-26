<?php session_start(); ?>

<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title>Robot Reboot 2 - About</title>
<script type="text/javascript" src="about_js.js"></script>
<link rel="stylesheet" href="bootstrap/css/bootstrap.min.css">
<link rel="stylesheet" href="about_css.css">
</head>

<body>

   <?php include("navigation.php"); ?>
   <div id="maintext">
      <p>Ricochets Robots is a board game created by Alex Randolph :
         <a href = "https://boardgamegeek.com/boardgame/51/ricochet-robots">see on BGG</a>
      .</p>
      <p> This website is a replica of the original Robot Reboot website, whose daily challenge is currently not working :
         <a href = "http://www.robotreboot.com/challenge">Robot Reboot</a>
      .</p>
      <?php
         $urlDb = getenv("CLEARDB_DATABASE_URL");
         $decomp = parse_url($urlDb);
         $host = $decomp["host"];
         $username = $decomp["user"];
         $password = $decomp["pass"];
         $nameDb = substr($decomp["path"],1);
         echo "<p>URL : " . $urlDb . " HOST : " . $host . " USER : " . $username . " PWD : " . $password . " PATH : " . $nameDB . "</p>";
      ?>
   </div>
   
</body>
</html>