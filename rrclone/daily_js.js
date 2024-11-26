// --- DONNÉES

var checkSession = false;
var pseudoSession = "rien";
var seqsSession = "[[],[],[],[],[],[]]";
var jour = 0; // le jour sélectionné pour le challenge (0 aujourd'hui, 1 hier)
var indexJour = ["ajd","hier"];

// Chargement des infos des énigmes d'aujourd'hui
var data = new Object();
const ajd = new Date(Date.now());
const strAjd = ajd.toISOString();
const fichierAjd = "./data/data" + strAjd.slice(8,10) + "_" + strAjd.slice(5,7) + "_" + strAjd.slice(0,4) + ".js" ;
import(fichierAjd).then((module) => {
   var strDataAjd = JSON.stringify(module.data);
   console.log("chaine issue du module d'aujourd'hui",fichierAjd,":",strDataAjd);
   var dataAjd = JSON.parse(strDataAjd);
   data["ajd"] = dataAjd;
});
// Chargement des infos des énigmes d'hier
const hier = new Date();
hier.setDate(ajd.getDate() - 1);
const strHier = hier.toISOString();
const fichierHier = "./data/data" + strHier.slice(8,10) + "_" + strHier.slice(5,7) + "_" + strHier.slice(0,4) + ".js" ;
import(fichierHier).then((module) => {
   var strDataHier = JSON.stringify(module.data);
   console.log("chaine issue du module d'hier",fichierHier,":",strDataHier);
   var dataHier = JSON.parse(strDataHier);
   data["hier"] = dataHier;
});

// --- INITIALISATION

function stockerSession(cs,ps,ss) {
   checkSession = cs;
   pseudoSession = ps;
   seqsSession = ss;
}

function generer() {
   // appelée par l'utilisateur pour créer une grille et commencer la résolution
   
   // S'il y avait une grille en cours avant, on enlève sa cible et son bouton d'enregistrement de score
   if(!enCours) {
      enCours = true;
   }
   else {
      effacerCible();
      document.getElementById('submitButton').removeAttribute("onclick");
   }
   document.getElementById("retourSubmit").innerHTML = "";

   // Initialisation des variables globales
   indEnigme = 0; // sélection de la première énigme par défaut
   robotCourant = -1;
   nbCoupsRecord = [0,0,0,0,0,0];
   creerPlateau();
   memoireFlechesPlateau = [];
   memoireDirsPotentielles = [];
   memoireMeilleuresSeqs = [[],[],[],[],[],[]];
   memoireSeqActuelle = [];
   nbEnigmesResolues = 0;
   
   // Enregistrement du jour (aujourd'hui ou hier) que l'utilisateur a choisi dans la liste déroulante
   var selectJour = document.getElementById("selectionJour");
   var jour = selectJour.value;
   var jourCle = indexJour[jour]; // association : 0 => aujourd'hui, 1 => hier

   // Copie des données (murs de la grille, positions des 4 robots, positions des 6 cibles)
   var xi,yi,ci,descOrig,descCopie;
   console.log("copie des données du jour",jour,"-",jourCle);
   positions = [0,0,0,0];
   positions_initiales = [0,0,0,0];
   for(var i=0;i<4;i++) {
      xi = data[jourCle]["posRobots"][i][0];
      yi = data[jourCle]["posRobots"][i][1];
      positions[i] = [xi,yi];
      positions_initiales[i] = [xi,yi];
   }
   posCible = [0,0,0,0,0,0];
   for(var i=0;i<6;i++) {
      xi = data[jourCle]["posCibles"][i][0];
      yi = data[jourCle]["posCibles"][i][1];
      ci = data[jourCle]["posCibles"][i][2];
      posCible[i] = [xi,yi,ci];
   }
   murs = [];
   for(var i=0;i<256;i++) {
      descOrig = data[jourCle]["murs"][i];
      descCopie = [descOrig[0],descOrig[1],descOrig[2],descOrig[3],descOrig[4]];
      murs.push(descCopie);
   }
   console.log("résultat de la copie des données : robots",positions,"cibles",posCible,"murs",murs);

   // Remplissage éventuel avec les données de session
   if(checkSession && jour == 0) { // <checkSession> = variable booléenne définie dans le <window.onload> au début de <daily.php>
      memoireMeilleuresSeqs = JSON.parse(seqsSession);
      document.getElementById("inputPseudo").value = pseudoSession;
      console.log("DONNÉES DE SESSION détectées pendant la génération : memMeilleur = ",memoireMeilleuresSeqs);
      for(var i = 0;i < 6;i++) { // rétrocalcul des autre variables en fonction des séquences de la session
         seq = memoireMeilleuresSeqs[i];
         if(seq.length >= 1) { // l'énigme <i> a été résolue dans la session
            nbEnigmesResolues += 1;
            nbCoupsRecord[i] = seq.length;
         }
      }
   }
   else {
      console.log("aucune donnée de session communiquée");
   }

   // Activation de tous les boutons qui ne fonctionnent que une fois le plateau généré
   document.getElementById("retourInitial").onclick = retourInitial;
   document.getElementById("clickLevel1").onclick = function p() {clickLevel(1)};
   document.getElementById("clickLevel2").onclick = function p() {clickLevel(2)};
   document.getElementById("clickLevel3").onclick = function p() {clickLevel(3)};
   document.getElementById("clickLevel4").onclick = function p() {clickLevel(4)};
   document.getElementById("clickLevel5").onclick = function p() {clickLevel(5)};
   document.getElementById("clickLevel6").onclick = function p() {clickLevel(6)};
   document.getElementById("button0").onclick = function p() {clickRobot(0)};
   document.getElementById("button1").onclick = function p() {clickRobot(1)};
   document.getElementById("button2").onclick = function p() {clickRobot(2)};
   document.getElementById("button3").onclick = function p() {clickRobot(3)};
   document.getElementById("clickUp").onclick = function p() {clickDir(0)};
   document.getElementById("clickDown").onclick = function p() {clickDir(1)};
   document.getElementById("clickLeft").onclick = function p() {clickDir(2)};
   document.getElementById("clickRight").onclick = function p() {clickDir(3)};
   if(jour==0) { // le bouton d'enregistrement n'est cliquable que pour la grille d'aujourd'hui
      document.getElementById("submitButton").onclick = envoyerScore;
      document.getElementById("reinitSession").onclick = unsetSession();
   }

   // Affichage initial
   for(var i = 0;i < 4;i++) {
      affRobot(i);
   }
   affMurs();
   chargerEnigme(0,0);
}

// FONCTIONS PROPRE À "DAILY"

function envoyerScore() { // envoie les données de score par formulaire
   var texteRetour = document.getElementById("retourSubmit");
   if(enCours && jour == 0) { // il faut que la grille d'aujourd'hui soit générée
      if(nbEnigmesResolues >= 1) { // et que ses 6 énigmes aient été résolues même sous-optimalement
         var champPseudo = document.getElementById("inputPseudo");
         var pseudo = champPseudo.value;
         console.log("tentative d'envoi de score, pseudo entré:",pseudo);
         if(pseudo.length > 0 && pseudo.length < 20) {
            console.log("envoi autorisé");

            // Récupération des éléments du HTML, stockage de nbCoups et des séquences de coups dans les champs cachés du formulaire
            var formulaire = document.getElementById('entreePseudo');
            var nbCoupsInput = document.getElementById('inputCoups');
            var somme = nbCoupsRecord.reduce((acc, b) => acc + b, 0);
            nbCoupsInput.value = somme;
            var seqsInput = document.getElementById('inputSeqs');
            var chaineSeqs = coderSeqs(memoireMeilleuresSeqs); // chaîne qui code les séquences pour les 6 énigmes
            console.log("enregistrement : codage des séquences :",chaineSeqs);
            seqsInput.value = chaineSeqs;
            
            // Envoi des données du formulaire par fetch
            fetch('/enregistrer_score.php', {
               method: 'POST',
               body: new FormData(formulaire)
            })
            .then(response => {
               if (response.ok) {
                  console.log("succès enregistrement score");
                  texteRetour.innerHTML = "Envoi réussi.";
               } else {
                  console.log("erreur pendant le php d'enregistrement score");
                  texteRetour.innerHTML = "Erreur pendant l'envoi au serveur.";
               }});

            // Petite surprise pour Bloup
            if(pseudo == "bloup") {
               texteRetour.innerHTML = "Bonjour Bloup";
            }
         }
         else {
            texteRetour.innerHTML = "Échec de l'envoi (pseudo trop long ou trop court).";
         }
      }
      else {
         texteRetour.innerHTML = "Échec de l'envoi (énigmes non résolues).";
      }
   }
}

function envoyerSession() { // envoie les descriptions de séquence à enregistrer dans les données de session
   if(enCours && jour == 0) { // (on ne garde des données de session que pour le challenge d'aujourd'hui)
      if(nbEnigmesResolues >= 1) { // il faut qu'au moins une énigme ait été résolue'
         var champPseudoInput = document.getElementById("inputPseudo");
         var champPseudoForm = document.getElementById("sessionPseudo");
         var pseudo = champPseudoInput.value;
         console.log("tentative d'envoi des DONNÉES DE SESSION, pseudo entré :",pseudo);
         if(pseudo.length < 20) { // le pseudo peut être une chaîne vide, mais pas trop long
            console.log("envoi autorisé");

            // Récupération des éléments du HTML, stockage de nbCoups et des séquences de coups dans les champs cachés du formulaire
            var formulaire = document.getElementById('formSession');
            var champSeqs = document.getElementById('sessionSeqs');
            champSeqs.value = JSON.stringify(memoireMeilleuresSeqs);
            champPseudoForm.value = pseudo;
            
            // Envoi des données du formulaire par fetch
            fetch('/conservation_session.php', {
               method: 'POST',
               body: new FormData(formulaire)
            })
            .then(response => {
               if (response.ok) {
                  console.log("succès envoi données de session");
               } else {
                  console.log("erreur pendant le php de conservation des données de session");
               }
            });
         }
      }
   }
}

function coderSeqs(seqs) {
   var chaineSeqs = "";
   var chaineAdd,robot,dir;
   for(var i=0;i<6;i++) {
      var seq = memoireMeilleuresSeqs[i];
      console.log("[codageSeq] énigme",i,"séquence à coder :",seq);
      for(var j=0;j<seq.length;j++) {
         robot = seq[j][0];
         dir = seq[j][1];
         chaineAdd = codageSeqRobot[robot] + codageSeqDir[dir] + ","; // "," comme séparateur entre les coups
         chaineSeqs += chaineAdd;
      }
      chaineSeqs += "-"; // "-" comme séparateur entre les énigmes
   }
   return chaineSeqs;
}

function terminerEnigme() {
   // appelée par la fonction <deplacer> quand l'utilisateur vient de résoudre une énigme avec moins de coups qu'avant
   console.log("Passage par <terminerEnigme>");
   envoyerSession();
}

function unsetSession() {
   formulaire = document.getElementById("formSession");
   fetch('/destruction_session.php', {
      method: 'POST',
      body: new FormData(formulaire)
   })
   .then(response => {
      if (response.ok) {
         console.log("succès suppression des données de session");
      } else {
         console.log("erreur pendant suppression des données de session");
      }
   });
}