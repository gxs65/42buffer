<?php session_start(); ?>

<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title>Robot Reboot 2 - Daily Challenge</title>
<script type="text/javascript" src="daily_js.js"></script>
<script type="text/javascript" src="puzzles.js"></script>
<link rel="stylesheet" href="bootstrap/css/bootstrap.min.css">
<link rel="stylesheet" href="daily_css.css">
<link rel="stylesheet" href="puzzles_styles.css">
</head>

<body>

   <script type="text/javascript">
      window.onload = (event) => {
         
         // rendre uniquement le bouton "générer" cliquable
         console.log("page chargée");
         document.getElementById("generer").onclick = generer;

         // vérifier si on a des données de session à utiliser
         <?php if(isset($_SESSION["pseudo"])): ?>
            console.log("DONNÉES DE SESSION : existantes");
            var checkSession = true;
            <?php if($_SESSION["pseudo"] != ""): ?>
               var pseudoSession = <?php echo "'" . $_SESSION["pseudo"] . "'"; ?>;
            <?php else: ?>
               var pseudoSession = "rien";
            <?php endif; ?>
            <?php if($_SESSION["seqs"] != ""): ?>
               var seqsSession = <?php echo "'" . $_SESSION["seqs"] . "'"; ?>;
            <?php else: ?>
               var seqsSession = "[[],[],[],[],[],[]]";
            <?php endif; ?>
         <?php else: ?>
            console.log("DONNÉES DE SESSION : absentes");
            var checkSession = false;
            var pseudoSession = -1;
            var seqsSession = -1;
         <?php endif; ?>
         stockerSession(checkSession,pseudoSession,seqsSession); // une fonction qui stockera ces données, dans le script de <daily_js>
         console.log("window.onload : données de session = ",pseudoSession,seqsSession,checkSession);

         // si on a des données de session, générer immédiatement la grille (d'aujourd'hui)
         if(checkSession) {
            generer();
         }
      };
   </script>

   <?php include("navigation.php"); ?>

   <div class="row" id="rowSelectionJour">
      <div class = "col-md-4">
         <div class="texte">
            <?php
               echo "Nous sommes le : " . date("d/m/Y") . ". Sélectionner le challenge :";
               // " hier " . date("d/m/Y",strtotime("yesterday"))
            ?>
         </div>
      </div>
      <div class = "col-md-2">
         <div id="divSelectionJour">
            <select id="selectionJour" name="Jour">
               <option value="0">
                  Aujourd'hui
               </option>
               <option value="1">
                  Hier
               </option>
               <!--
               <option value="0">
                  Avant-hier
               </option>
               -->
            </select>
         </div>
      </div>
      <div class = "col-md-1">
         <div id="generer">
            <button id="generer">Générer</button>
         </div>
      </div>
   </div>
   <div class="row" id="rowEnregistrement">
      <div class = "col-md-4">
         <div class="texte">
            Enregistrement du score (seulement pour le challenge d'aujourd'hui) :
         </div>
      </div>
      <div class = "col-md-2">
         <form id="entreePseudo">
            <input type="text" id="inputPseudo" name="pseudo">
            <input type="hidden" id="inputCoups" name="nbCoups">
            <input type="hidden" id="inputSeqs" name="seqs">
         </form>
      </div>
      <div class = "col-md-1">
         <div id="divSubmit">
            <button id="submitButton">Enregistrer</button>
         </div>
      </div>
      <div class = "col-md-4">
         <div class="texte" id="retourSubmit">
            ---
         </div>
      </div>
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
         <div id="compteurRecord" class="texte">Nombre de coup minimal de l'énigme : ?</div>
      </div>
      <div class="col-md-4">
         <div id="descSeqRecord">---</div>
      </div>
   </div>
   
   <div class="row" id="rowButtons">
      
      <div id="divLevelButtons" class="col-md-2 offset-md-1">
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

      <div id="gen" class="col-md-2">
         <div id="reinit" class="petiteMarge">
            <button id="retourInitial">Replacer les robots</button>
         </div>
         <div id="ancienGenerer" class="petiteMarge">
            <button id="reinitSession">Oublier les solutions</button>
         </div>
      </div>
   </div>
   
   <div class="row">
      <div class="offset-md-1">
         <table id="plateau"></table>
      </div>
   </div>

   <!--- zone de formulaire cachée pour l'envoi des données de session --->
   <div>
      <form id="formSession">
         <input type="hidden" id="sessionPseudo" name="pseudo">
         <input type="hidden" id="sessionSeqs" name="seqs">
      </form>
   </div>
   
</body>
</html>