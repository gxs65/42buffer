<?php session_start() ?>

<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title>Robot Reboot 2 - Scores</title>
<script type="text/javascript" src="scores_js.js"></script>
<link rel="stylesheet" href="bootstrap/css/bootstrap.min.css">
<link rel="stylesheet" href="scores_css.css">
</head>

<body>

   <?php include("navigation.php"); ?>

   <?php

      function compterRepart($strSeqs) { // compte le nombre de coups pour chaque énigme et renvoie la liste correspondante
         $repartCoups = [0,0,0,0,0,0];
         $seqs = explode("-",$strSeqs); // liste de 6 séquences, décrites par des caractères codant les déplacements
         for($indEnigme = 0;$indEnigme < 6;$indEnigme++) {
            $seq = explode(",",$seqs[$indEnigme]); // liste de coups (un caractère pour le robot, un caractère pour la direction)
            $repartCoups[$indEnigme] = sizeof($seq) - 1;
         }
         return $repartCoups;
      }

      // CONNEXION BASE DE DONNÉES
      $urlDb = getenv("CLEARDB_DATABASE_URL");
      $decomp = parse_url($urlDb);
      $host = $decomp["host"];
      $username = $decomp["user"];
      $password = $decomp["pass"];
      $nameDb = substr($decomp["path"],1);
      $dsn = "mysql:host=" . $host . ";dbname=" . $nameDb . ";charset=utf8";
      /* Version locale
      $dsn = 'mysql:host=127.0.0.1;dbname=DBtest1;charset=utf8';
      $username = 'root';
      $password = '';
      $db = new PDO($dsn,$username,$password,[PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION]);
      */
      $db = new PDO($dsn,$username,$password,[PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION]);

      // RÉCUPÉRATION DES DONNÉES DE SCORE D'AUJOURD'HUI DANS LA BASE DE DONNÉES
      $jour = date("Y-m-d");
      $query = "SELECT * FROM scores WHERE jour = :jour AND afficher = :afficher ORDER BY nb_coups,horaire";
      $statement = $db->prepare($query);
      $statement->execute([
          "jour" => $jour,
          "afficher" => 1,
      ]) or die(print_r($db->errorInfo()));
      $res = $statement->fetchall();

      // AFFICHAGE DES SCORES D'AUJOURD'HUI DANS UN TABLEAU HTML
      $cles = ["nb_coups","pseudo","horaire","sequences"];
      echo "<div class='row'>";
      echo "<div class='col-md-3 texteHorsCase'>Aujourd'hui :</div>";
      echo "<div class='emplTableau col-md-8'><table id='scoresAjd'>";
      foreach($res as $ligne) {
         echo "<tr>";
         foreach($cles as $cle) {
            $elem = $ligne[$cle];
            if($cle == "pseudo") { // affichage spécifique pour le pseudo
               echo "<td class='case casePseudo'>" . $elem . "</td>";
            }
            elseif($cle=="sequences") { // affichage pour la décomposition (les valeurs ne viennent pas de la db, elles sont calculées)
               $coups = compterRepart($elem); // <$elem> est la chaîne de caractères décrivant les séquences
               echo "<td class='case spoiler'>";
               echo $coups[0] . "-" . $coups[1] . "-" . $coups[2] . "-" . $coups[3] . "-" . $coups[4] . "-" . $coups[5];
               echo "</td>";
            }
            else { // affichage pour le nombre total de coups et l'horaire
               echo "<td class='case'>" . $elem . "</td>";
            }
         }
         echo "</tr>";
      }
      echo "</table></div>";
      echo "</div>";

      // RÉCUPÉRATION DES DONNÉES DE SCORE D'HIER DANS LA BASE DE DONNÉES
      $jour = date("Y-m-d",strtotime("yesterday")); // donnera la date d'hier
      // $jour = date("Y-m-d");
      $query = "SELECT * FROM scores WHERE jour = :jour AND afficher = :afficher ORDER BY nb_coups,horaire";
      $statement = $db->prepare($query);
      $statement->execute([
          "jour" => $jour,
          "afficher" => 1,
      ]) or die(print_r($db->errorInfo()));
      $res = $statement->fetchall();

      // AFFICHAGE DES SCORES D'HIER DANS UN NOUVEAU TABLEAU HTML
      $cles = ["nb_coups","pseudo","horaire","sequences"];
      echo "<div class='row'>";
      echo "<div class='col-md-3 texteHorsCase'>Hier :</div>";
      echo "<div class='emplTableau col-md-8'><table id='scoresHier'>";
      foreach($res as $ligne) {
         echo "<tr>";
         foreach($cles as $cle) {
            $elem = $ligne[$cle];
            if($cle == "pseudo") {
               echo "<td class='case casePseudo'>" . $elem . "</td>";
            }
            elseif($cle=="sequences") { // affichage pour la décomposition (les valeurs ne viennent pas de la db, elles sont calculées)
               $coups = compterRepart($elem); // <$elem> est la chaîne de caractères décrivant les séquences
               echo "<td class='case'>";
               echo $coups[0] . "-" . $coups[1] . "-" . $coups[2] . "-" . $coups[3] . "-" . $coups[4] . "-" . $coups[5];
               echo "</td>";
            }
            else {
               echo "<td class='case'>" . $elem . "</td>";
            }
         }
         echo "</tr>";
      }
      echo "</table></div>";
      echo "</div>";

      // AFFICHAGE DES SÉQUENCES DU MEILLEUR RÉSULTAT D'HIER
      $couleurs = ["b" => "blue","r" => "red","g" => "green","y" => "yellow"];
      $charFlecheHTML = ["u" => "&uarr;","d" => "&darr;","l" => "&larr;","r" => "&rarr;"];
      $seqsOpti = explode("-",$res[0]["sequences"]); // liste de 6 séquences, décrites par des caractères codant les déplacements
      for($indEnigme = 0;$indEnigme < 6;$indEnigme++) {
         echo "<div class='row'>";
         if($indEnigme == 0) {
            echo "<div class='col-md-3 texteHorsCase'>Séquences de " . $res[0]["pseudo"] . " :</div>";
            echo "<div class='col-md-8'>";
            echo "<div class='indicEnigme' id='margeAjust1'>Énigme " . ($indEnigme + 1) . " :</div>";
         }
         else {
            echo "<div class='emplVide col-md-3'></div>";
            echo "<div class='col-md-8'>";
            echo "<div class='indicEnigme'>Énigme " . ($indEnigme + 1) . " :</div>";
         }
         $seqOpti = explode(",",$seqsOpti[$indEnigme]); // liste de coups (un caractère pour le robot, un caractère pour la direction)
         for($indCoup = 0;$indCoup < sizeOf($seqOpti) - 1;$indCoup++) {
            $strCoup = $seqOpti[$indCoup];
            $robot = substr($strCoup,0,1);
            $dir = substr($strCoup,1,1);
            echo "<div class='descFleche text" . $couleurs[$robot] . "'>" . $charFlecheHTML[$dir] . "</div>";
         }
         echo "</div></div>";
      }

      // Test
      /*
      echo "TEST" . "<br/>";
      $query = "SELECT entry_id,pseudo,nb_coups,afficher FROM scores WHERE pseudo = :pseudo";
      $statement = $db->prepare($query);
      $pseudos = ["gxs1","gxs2"];

      foreach($pseudos as $ps) {
         print_r("recherche du pseudo " . $ps . " dans la db" . "<br/>");
         $statement->execute([
            "pseudo" => $ps,
         ]) or die(print_r($db->errorInfo()));
         $idLigneSuppr = $statement->fetchall();
         if(count($idLigneSuppr) == 0) {
            print_r("rien trouvé" . "<br/>");
         }
         else {
            echo "<pre>";
            print_r($idLigneSuppr);
            echo "</pre>";
         }
      }
      */

      // test des données de session
      /*
      if(isset($_SESSION["var"])) {
         echo $_SESSION["var"];
         $_SESSION["var"] += 1;
      }
      else {
         $_SESSION["var"] = 0;
      }
      */
      /*
      if(isset($_SESSION["pseudo"]) && isset($_SESSION["seqs"])) {
         echo $_SESSION["pseudo"];
         echo $_SESSION["seqs"];
      }
      else {
         echo "données de session indéfinies";
      }
      */
   ?>
   
</body>
</html>