<?php

// Récupération des données envoyées par la méthode POST, ajout des données de date
$nom = $_POST['pseudo'];
$nbCoups = $_POST['nbCoups'];
$seqs = $_POST['seqs'];
$jour = date("Y-m-d",strtotime("+2 hours"));
$horaire = date("H:i:s",strtotime("+2 hours"));

// Connexion à la bse de données MyQSL
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
$afficher = 1;

// Effacement de la ligne précédente avec le même pseudo le cas échéant (et si l'ancien score était moins bon)
$query = "SELECT entry_id,nb_coups FROM scores WHERE pseudo = :pseudo and jour = :jour and afficher = :afficher ORDER BY nb_coups ASC";
$statement = $db->prepare($query);
$statement->execute([
    "pseudo" => $nom,
    "jour" => $jour,
    "afficher" => 1,
]) or die(print_r($db->errorInfo()));
$memePseudos = $statement->fetchall();
if(count($memePseudos) > 0) { // il existe bien déjà des lignes affichées avec ce pseudo
    if($memePseudos[0]["nb_coups"] <= $nbCoups) {    // cas 1 : le minimum affiché est inférieur au score envoyé
        $afficher = 0;                                  // -> on va enregistrer le score envoyé, mais il ne sera pas affiché
    }
    else { // cas 2 : le score envoyé est un nouveau minimum
        foreach($memePseudos as $entree) { // -> on efface tous les scores précédents
            $id = $entree["entry_id"];
            $query = "UPDATE scores SET afficher = 0 WHERE entry_id = :entry_id";
            $statement = $db->prepare($query);
            $statement->execute([
                "entry_id" => $id,
            ]) or die(print_r($db->errorInfo()));
        }
    }
}

// Exécution de la requête SQL d'insertion
$query = "INSERT INTO scores(pseudo,nb_coups,jour,horaire,sequences,afficher) VALUES (:pseudo,:nb_coups,:jour,:horaire,:sequences,:afficher)";
$statement = $db->prepare($query);
$statement->execute([
    "pseudo" => $nom,
    "nb_coups" => $nbCoups,
    "jour" => $jour,
    "horaire" => $horaire,
    "sequences" => $seqs,
    "afficher" => $afficher,
]) or die(print_r($db->errorInfo())); // <afficher> sera 0 si le score envoyé est inférieur à celui déjà présent pour ce pseudo

?>