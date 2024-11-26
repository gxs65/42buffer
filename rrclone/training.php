<?php session_start(); ?>

<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title>Robot Reboot 2 - Training</title>
<script type="text/javascript" src="puzzles.js"></script>
<script type="text/javascript" src="training_js.js"></script>
<link rel="stylesheet" href="bootstrap/css/bootstrap.min.css">
<link rel="stylesheet" href="training_css.css">
<link rel="stylesheet" href="puzzles_styles.css">
</head>

<body>
   
   <script type="text/javascript">
      window.onload = (event) => {
         console.log("page is fully loaded");
         document.getElementById("generer").onclick = generer;
      };
   </script>

   <?php include("navigation.php"); ?>
   
   <div class="row" id="rowPlaceholder">
   </div>
   <div class="row" id="rowCoups">
      <div id="titreEnigme" class="col-md-1">Énigme 1</div>
      <div class="col-md-4">
         <div id="compteurCoups" class="texte">Nombre de coups de cette tentative : 0</div>
      </div>
      <div class="col-md-4">
         <div id="descSeq">---</div>
      </div>
   </div>
   <div class="row" id="rowRecords">
      <div id="placeHolder" class="col-md-1"></div>
      <div class="col-md-4">
         <div id="compteurRecord" class="texte">Nombre de coup minimal sur cette énigme : ?</div>
      </div>
      <div class="col-md-4">
         <div id="descSeqRecord">---</div>
      </div>
   </div>
   
   <div class="row" id="rowButtons">
      <div id="gen" class="col-md-1">
         <div id="divGenerer"><button class="impButton" id="generer">Générer</button></div>
         <div id="divReinit"><button class="impButton" id="retourInitial">Réinitialiser</button></div>
      </div>
      
      <div id="divLevelButtons" class="col-md-2">
         <table id="tableLevelButtons" class="tableauInvisible">
            <tr>
               <td><div><button class="levelButton" id="clickLevel1">--1--</div></td>
               <td id="indicRecord0">-</td>
               <td><div><button class="levelButton" id="clickLevel2">--2--</div></td>
               <td id="indicRecord1">-</td>
               <td><div><button class="levelButton" id="clickLevel3">--3--</div></td>
               <td id="indicRecord2">-</td>
            </tr>
            <tr>
               <td><div><button class="levelButton" id="clickLevel4">--4--</div></td>
               <td id="indicRecord3">-</td>
               <td><div><button class="levelButton" id="clickLevel5">--5--</div></td>
               <td id="indicRecord4">-</td>
               <td><div><button class="levelButton" id="clickLevel6">--6--</div></td>
               <td id="indicRecord5">-</td>
            </tr>
         </table>
      </div>
      
      <div id="divRobotButtons" class="col-md-1">
         <table id="tableRobotButtons" class="tableauInvisible">
            <tr>
               <td><div class="robotButton"><button id="button0">--o--</button></div></td>
               <td><div class="robotButton"><button id="button1">--o--</button></div></td>
            </tr>
             <tr>
               <td><div class="robotButton"><button id="button2">--o--</button></div></td>
               <td><div class="robotButton"><button id="button3">--o--</button></div></td>
            </tr>
         </table>
      </div>
      
      <div id="divDirButtons" class="col-md-1">
         <table id="tableDirButtons" class="tableauInvisible">
            <tr>
               <td></td>
               <td><div class="dirButton"><button id="clickUp"><img src="images/arrowUp.jpeg"></button></div></td>
               <td></td>
            </tr>
            <tr>
               <td><div class="dirButton"><button id="clickLeft"><img src="images/arrowLeft.jpeg"></button></div></td>
               <td><div class="dirButton"><button id="clickDown"><img src="images/arrowDown.jpeg"></button></div></td>
               <td><div class="dirButton"><button id="clickRight"><img src="images/arrowRight.jpeg"></button></div></td>
            </tr>
         </table>
      </div>
   </div>
   
   <div class="row">
      <div class="offset-md-1">
         <table id="plateau"></table>
      </div>
   </div>
   
</body>
</html>