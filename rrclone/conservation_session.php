<?php
session_start();

// Connexion à la bse de données MyQSL (pour pouvoir logger)
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

// Récupération des données envoyées par la méthode POST
$nom = $_POST['pseudo'];
$seqs = $_POST['seqs'];

// Enregistrement en données de session
$_SESSION["pseudo"] = $nom;
$_SESSION["seqs"] = $seqs;

?>