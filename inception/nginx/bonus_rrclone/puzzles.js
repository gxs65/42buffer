// --- VARIABLES

var indEnigme,posCible; // mémoire des positions des cibles
var positions,positions_initiales; // les positions des 4 robots en format <ligne x,colonne y>
var robotCourant; // le robot sélectionné actuellement
var xo,yo,co; // la position de la cible
var nbCoups,nbCoupsRecord; // la mémoire du nombre de coups sur chaque énigme
var murs; // les descriptions du plateau
var memoireDirsPotentielles,memoireFlechesPlateau;
   // la mémoire des :  <memoireDirsPotentielles> directions parmi 4 dans lesquelles le robot courant peut aller
   //                   <memoireFlechesPlateau> tableau associatif associant la direction à l'indice d'une case contenant une flèche
var memoireMeilleuresSeqs,memoireSeqActuelle; // enregistrement des séquences de coups menant à la cible
const listeCouleurs = ["blue","red","green","yellow"]; // couleurs des robots pour le CSS
const listeCouleursClaires = ["#8888ff","#ff8888","#88ff88","#ffff88"]; // pareil en plus clair
const codeDirection = ["Up","Down","Left","Right"]; // les chaînes décrivant les 4 directions de déplacement
const codeDirectionFr = ["Haut","Bas","Gauche","Droite"];
const indDirection = {"Up":0,"Down":1,"Left":2,"Right":3}; // les chaînes décrivant les 4 directions de déplacement
const charFlecheHTML = {"Up":"&uarr;","Down":"&darr;","Left":"&larr;","Right":"&rarr;"}; // les caractères spéciaux HTML pour des flèches
const codageSeqRobot = ["b","r","g","y"];                            // lettres pour le codage des séquences dans une chaîne de caractères
const codageSeqDir = {"Up":"u","Down":"d","Left":"l","Right":"r"}; // (lettre décrivant le robot, lettre décrivant la direction)
const descDir = [[-1,0,1,"Up"],[1,0,0,"Down"],[0,-1,3,"Left"],[0,1,2,"Right"]]; // toutes les caracs d'un déplacement dans une direction
var enCours = false;
var nbEnigmesResolues;


// --- CONVENTIONS

/*
0123 -> BRVJ
0123 -> HBGD
*/


// --- FONCTIONS UTILITAIRES

function randInt(min,max) {
  return Math.floor(min + Math.random() * (max - min));
}

function include(arr,subarr) {
   // pour vérifier si le tableau de tableaux <arr> contient <subarr>
   var contient = false;
   for(var i=0;i<arr.length;i++) {
      var sujet = arr[i];
      if (sujet.length == subarr.length) {
         contient = true;
         for(var j=0;j<arr.length;j++) {
            if(sujet[j] !== subarr[j]) {
               contient = false;
            }
         }
      }
   }
   return contient;
}

function adj(x,y) {
   // renvoie les indices des cases du carré autour de la case <x,y>
   return [[x-1,y-1],[x-1,y],[x-1,y+1],[x,y-1],[x,y+1],[x+1,y-1],[x+1,y],[x+1,y+1]];
}

function ajouterMur(direction,x,y) {
   murs[x * 16 + y][direction] = 1;
   
   if(direction == 0 && x>0) {
      murs[(x-1) * 16 + y][1] = 1;
   }
   if(direction == 1 && x<15) {
      murs[(x+1) * 16 + y][0] = 1;
   }
   if(direction == 2 && y>0) {
      murs[x * 16 + (y-1)][3] = 1;
   }
   if(direction == 3 && y<15) {
      murs[x * 16 + (y+1)][2] = 1;
   }
}

function symetrie(x,y,direction,quadrant) {
   var indicDirx = [1,0];
   var indicDiry = [0,0,3,2];
   if(quadrant >= 2) {
      x = 15 - x;
      if(direction == 0 || direction == 1) {
         direction = indicDirx[direction];
      }
   }
   if(quadrant%2 == 1) {
      y = 15 - y;
      if(direction == 3 || direction == 2) {
         direction = indicDiry[direction];
      }
   }
   return [x,y,direction];
}


// INITIALISATION

function creerPlateau() {
   // appelée pour créer le tableau HTML représentant la grille
   var table = document.getElementById("plateau");
   table.innerHTML = "";
   for(var indLigne = 0;indLigne < 16;indLigne ++) {
      table.innerHTML += "<tr id=ligne" + indLigne.toString() + "></tr>";
   }
   var indCase = 0;
   var chaine = "";
   var ligne;
   for(var indLigne = 0;indLigne < 16;indLigne ++) {
      ligne = document.getElementById("ligne"+indLigne.toString());
      for(var indCol = 0;indCol < 16;indCol ++) {
         chaine = "<td class='casePlateau' id='case" + indCase.toString() + "'>" + "</td>";
         ligne.innerHTML += chaine;
         indCase += 1;
      }
   }
   var tdCible;
   for(var indCase = 0;indCase < 256;indCase++) { // rendre chaque case réceptive au click
      tdCible = document.getElementById("case" + indCase.toString());
      tdCible.onclick = function (e) {
         analyserClick(e);
      }
   }
}

function retourInitial() {
   // appelée par l'utilisateur pour réinitialiser la résolution de l'énigme
   console.log("réinitialisation");
   demagnify();
   var x,y;
   for(var i=0;i<4;i++) {
      effacerRobot(i);
      x = positions[i][0];
      y = positions[i][1];
      murs[x * 16 + y][4] = -1;
      x = positions_initiales[i][0];
      y = positions_initiales[i][1];
      murs[x * 16 + y][4] = i;
      positions[i] = [x,y];
      affRobot(i);
   }
   robotCourant = -1;
   nbCoups = 0;
   memoireSeqActuelle = [];
   affCompteur();
   affCible();
}


// --- FONCTIONS DE CHANGEMENT D'ÉNIGME

function chargerEnigme(ind,effacement) {
   if(effacement) {
      effacerCible();
   }
   indEnigme = ind;
   var descEnigme = posCible[ind];
   xo = descEnigme[0];
   yo = descEnigme[1];
   co = descEnigme[2];
   nbCoups = 0;
   affCompteur();
   affRecord();
   affCible();
   affIndEnigme();
}

function clickLevel(indLevel) {
   chargerEnigme(indLevel - 1,1);
   retourInitial();
}

// --- FONCTIONS DE SÉLECTION DE ROBOT

function clickRobot(indRobot) {
   // clic de sélection de robot
   console.log("sélection de robot :",indRobot);
   demagnify();
	robotCourant = indRobot;
	magnify();
}

function magnify() {
   // Mettre le fond du robot sélectionné en gris (plu maintenant, à la place on met les flèches autour)
   console.log("magnifier le robot",robotCourant);
   /*
   robot = document.getElementById('robot' + robotCourant.toString());
   tdParent = robot.closest("td"); // recherche l'élément td le plus proche en grimpant le DOM
   tdParent.style.backgroundColor = "lightgrey";
   */
   
   // Placer les flèches sur les cases accessibles autour
   memoireFlechesPlateau = new Object(); // (c'est un dictionnaire)
   var chaineDir,casesAcc;
   for(var direction = 0;direction < 4;direction++) {
      casesAcc = casesAccessibles(robotCourant,direction);
      chaineDir = codeDirection[direction];
      if(casesAcc.length == 0) { // le robot ne peut se déplacer même d'une seule case dans cette direction -> on n'affiche rien
         console.log("direction",direction,": aucune case accessible");
      }
      else { // le robot peut se déplacer d'au moins une case -> on affiche cette direction
         for(var indCase of casesAcc) { // affichage de la flèche sur chacune des cases dans cette direction
            affDir(indCase,direction,robotCourant); // on donne le robot en para pour la couleur de l'indicateur
            memoireFlechesPlateau[indCase] = chaineDir; // ajout au dictionnaire
         }
         memoireDirsPotentielles.push(chaineDir); // conservation en mémoire de cette direction dans les directions accessibles
      }
   }
   console.log("flèches directionnelles après magnify de robot",robotCourant,":",memoireFlechesPlateau);
   console.log("-> les directions accessibles sont :",memoireDirsPotentielles);
}

function demagnify() {
   var indCase;
   if(robotCourant !== -1) {
      /*
      robot = document.getElementById('robot' + robotCourant.toString());
      tdParent = robot.closest("td");
      tdParent.style.backgroundColor = "";
      */
      var chaineDir;
      for(var indCase in memoireFlechesPlateau) { // on parcourt les clés du dictionnaire
         chaineDir = memoireFlechesPlateau[indCase];
         console.log("suppression de la flèche de direction",chaineDir,"sur la case :",indCase);
         effacerDir(indCase);
      }
      memoireFlechesPlateau = new Object();
      memoireDirsPotentielles = [];
   }
}

// FONCTIONS DE DÉPLACEMENT DE ROBOT

function analyserClick(event) {
   // détermine si un click sur la case <indCase> de la grille veut dire quelque chose (délectionner robot / déplacer robot)
   
   // Sélection du bon élément HTML (un <td> du tableau représentant le plateau)
   console.log("click sur :",event.target.id,event.target.localName);
   var targ1 = event.target;
   var targ2;
   if(targ1.localName == "td") {
      targ2 = targ1;
   }
   else {
      targ2 = targ1.closest("td")
   }
   console.log("td cliqué :",targ2.id);
   var strid = targ2.id.slice(4);
   var indCase = parseInt(strid);
   var descCase = murs[indCase];

   // Traitement : renvoi à une fonction de sélection de robot
   console.log("check s'il y a un robot sur cette case",strid,"décrite",descCase);
   if(descCase[4] !== -1) { // cas où l'utilisateur a cliqué sur une case contenant un robot
      var indRobot = descCase[4];
      console.log("-> robot détecté sur la case :",descCase[4]);
      clickRobot(indRobot);
   }

   // Traitement : renvoi à une fonction de déplacement
   var casesFleches = Object.keys(memoireFlechesPlateau); // on récupère les clés du dictionnaire
   console.log("check si la case fait partie des cases avec une flèche :",casesFleches);
   if(casesFleches.includes(strid)) {  // cas où l'utilisateur a cliqué sur une flèche directionnelle
                                       // (pour une raison inconnue les indices dans <casesFleches> sont des str au lieu de int
      var chDir = memoireFlechesPlateau[indCase];
      console.log("-> flèche",chDir,"détectée sur case :",indCase);
      var indDir = indDirection[chDir];
      clickDir(indDir);
   }

   // Traitement : renvoi à la fonction de réinitialisation (click sur l'une des quatre cases intérieures)
   var casesInterieures = [119,120,135,136];
   if(casesInterieures.includes(indCase)) {
      console.log("réinitialisation par click central");
      retourInitial();
   }
}

function clickDir(indDir) {
   // clic de sélection de direction pour le robot
   var chaineDir = codeDirection[indDir];
   console.log("commande de déplacement en direction :",chaineDir);
   deplacement(indDir);
}

function deplacement(indDir) {
   // appelée pour déplacer le robot courant dans la direction indiquée <indDir>


   if(robotCourant !== -1) {
      demagnify();
      var x = positions[robotCourant][0],y = positions[robotCourant][1];
      var chaineDir = codeDirection[indDir];
      effacerRobot(robotCourant);
      
      // Récupérer la dernière des cases accessibles dans la direction donnée, ie. la case où arrive le robot
      var casesTraversees = casesAccessibles(robotCourant,indDir);
      if(casesTraversees.length == 0) {
         return -1
      }
      else {
         var derniereCase = casesTraversees[casesTraversees.length -1]; // derniereCase est un <indCase> sur 256
         var xp = (derniereCase / 16 >> 0),yp = derniereCase % 16; // conversion en coords <x,y>
         positions[robotCourant][0] = xp,positions[robotCourant][1] = yp;
         affRobot(robotCourant);

         // Vérifier (au cas où) que la case d'arrivée est bien différente de cette de départ, pour compter un nouveau coup
         if(xp !== x || yp !== y) {
            nbCoups += 1;
            memoireSeqActuelle.push([robotCourant,chaineDir]);
            affCompteur();
            murs[x * 16 + y][4] = -1;
            murs[xp * 16 + yp][4] = robotCourant;
            magnify();
            console.log("déplacement final en",xp,yp,"-> case d'origine",murs[x * 16 + y],"- case destination",murs[xp * 16 + yp]);

            // Compter l'énigme comme résolue si on est arrivé à la cible
            if(xp == xo && yp == yo && robotCourant == co) {
               console.log("objectif atteint à nb de coups",nbCoups);
               ncr = nbCoupsRecord[indEnigme];

               // Vérifier s'il faut mettre à jour le record pour cette énigme
               if(nbCoups < ncr || ncr == 0) {
                  if(ncr == 0) { // <ncr> à 0 signifie que l'énigme n'a jamais été résolue
                     nbEnigmesResolues++;
                  }
                  nbCoupsRecord[indEnigme] = nbCoups;
                  memoireMeilleuresSeqs[indEnigme] = [];
                  var coup;
                  for(var c = 0;c < nbCoups;c++) { // copie de <memoireSeqActuelle> dans <memoireMeilleureSeq[indEnigme]>
                     coup = memoireSeqActuelle[c];
                     memoireMeilleuresSeqs[indEnigme].push([coup[0],coup[1]]);
                  }
                  terminerEnigme(); // pour exécuter finalement la fonction <envoyerSession> de <training_js.js>
                  console.log("-> record battu, modification de nbCoupsRecord",nbCoupsRecord,"et memoire :",memoireMeilleuresSeqs[indEnigme]);
                  console.log("-> et nb énigmes résolues :",nbEnigmesResolues);
                  affRecord();
               }
               retourInitial();
               if(indEnigme !== 5) { // passage automatique à la prochaine énigme
                  chargerEnigme(indEnigme + 1,1);
               }
            }
         }
         return 1;
      }
   }
}

function casesAccessibles(indRobot,indDir) {
   // renvoie l'ensemble des cases traversées pour aller dans la direction <chDir> avec le robot <robot>
   // (rappel convention indDir : 0->H 1->B 2->G 3->D)

   // Définition des variables ; récupération des deltax et deltay correspondant à la direction
   var pos = positions[indRobot];
   var x = pos[0],y = pos[1];
   var xp = x,yp = y;
   console.log("début exécution casesAccessibles, robot",indRobot,"orig",x,y,"direction",codeDirection[indDir]);
   var desc = descDir[indDir];
   var deltax = desc[0],deltay = desc[1],orientationMur = desc[2]; // <orientationMur> est l'orientation où il faut vérifier si pas de mur
   var casesTraversees = [],indCase,descCase,continuer = true;
   
   // boucle de passage dans les cases :tout droit jusqu'à ce que le robot soit bloqué
   while(continuer) {
      xp += deltax; // récupération de la prochaine case où potentiellement se déplacer
      yp += deltay;
      continuer = false;
      if(xp <= 15 && xp >= 0 && yp <= 15 && yp >= 0) { // on reste dans le plateau
         indCase = xp * 16 + yp;
         descCase = murs[indCase];
         //console.log("-> case où on tente de se déplacer :",descCase);
         if(descCase[4] == -1 && descCase[orientationMur] == 0) { // la case est vide et sans murs sur le chemin
            continuer = true;
            casesTraversees.push([indCase]); // on garde le passage en mémoire
         }
      }
   }
   console.log("retour de casesAccessibles :",casesTraversees);
   return casesTraversees;
}

// --- FONCTIONS D'AFFICHAGE

// (sur le plateau)

function affRobot(identifiant_robot) {
   
   // Créer une image de robot qui est aussi un bouton pour le sélectionner
   var x = positions[identifiant_robot][0];
   var y = positions[identifiant_robot][1];
   var pos256 = x * 16 + y;
   var tdRobot = document.getElementById('case' + pos256.toString());
   var strid = identifiant_robot.toString();
   var chaineImg = "<img class='imgRobot' id='robot" + strid + "' src='images/robot" + strid + ".png'>";
   tdRobot.innerHTML = chaineImg;
}

function effacerRobot(identifiant_robot) {
   var x = positions[identifiant_robot][0];
   var y = positions[identifiant_robot][1];
   var pos256 = x * 16 + y;
   var tdRobot = document.getElementById('case' + pos256.toString());
   tdRobot.innerHTML = "";
}

function affMurs() {
   var placementMur = ["murHaut","murBas","murGauche","murDroite"];
   var descCase;
   for(var i=0;i<256;i++) {
      descCase = murs[i];
      //console.log(i,descCase);
      for(var j=0;j<4;j++) {
         if(descCase[j] == 1) {
            document.getElementById("case" + i.toString()).classList.add(placementMur[j]);
         }
      }
   }
}

function affCible() {
   // La cible sur le plateau
   var pos256 = xo * 16 + yo;
   var tdObj = document.getElementById('case' + pos256.toString());
   tdObj.style.backgroundColor = listeCouleursClaires[co];
   // La couleur du bouton d'énigme
   var strid = "clickLevel" + (indEnigme + 1).toString();
   var b2 = document.getElementById(strid);
   b2.style.backgroundColor = "#BBBBBB";
}
function effacerCible() {
   // La cible sur le plateau
   var pos256 = xo * 16 + yo;
   var tdObj = document.getElementById('case' + pos256.toString());
   tdObj.style.backgroundColor = "";
   // La couleur du bouton d'énigme
   var strid = "clickLevel" + (indEnigme + 1).toString();
   var b2 = document.getElementById(strid);
   b2.style.backgroundColor = "#E0E0E0";
}

function affDir(indCase,indDir,indRobot) { // place un indicateur de déplacement potentiel sur la case <indCase>
   var tdCible = document.getElementById("case" + indCase.toString());
   //var chaineDir = codeDirection[indDir];
   //var chaineImg = "<img class='imgDir dir" + chaineDir + "' src='images/arrow" + chaineDir + ".jpeg'>";
   var chaineImg = "<div class='indicDir indicDir" + codeDirectionFr[indDir] + " indicDir" + indRobot.toString() + "' ></div>";
   tdCible.innerHTML = chaineImg;
}
function effacerDir(indCase) { // efface l'indicateur de déplacement potentiel sur la case <indCase>
   tdCible = document.getElementById("case" + indCase.toString());
   tdCible.innerHTML = "";
}

// (en dehors du plateau)

function affCompteur() {
   var divCompteur = document.getElementById("compteurCoups");
   divCompteur.innerHTML = "Nombre de coups de cette tentative : " + nbCoups.toString();
   console.log("affichage du compteur de coups, séquence actuelle :",memoireSeqActuelle);
   var desc = chaineDescSeq(memoireSeqActuelle);
   var divDesc = document.getElementById("descSeq");
   divDesc.innerHTML = desc;
}

function affRecord() {
   // ligne indiquant le record pour l'énigme en cours
   var divRecord = document.getElementById("compteurRecord");
   var ncr = nbCoupsRecord[indEnigme];
   var somme = nbCoupsRecord.reduce((acc, b) => acc + b, 0);
   console.log("affichage de record pour énigme",indEnigme,"-> coups",nbCoupsRecord,"somme",somme,"séquence",memoireMeilleuresSeqs[indEnigme]);
   var chaine = "Nombre de coup minimal sur cette énigme : " + ncr.toString();
   chaine += " --- Total : " + somme.toString();
   divRecord.innerHTML = chaine;
   var desc = chaineDescSeq(memoireMeilleuresSeqs[indEnigme]);
   var divDescRecord = document.getElementById("descSeqRecord");
   divDescRecord.innerHTML = desc;
   
   // nombres à côté des boutons de choix d'énigme
   var chInd,tdRecord;
   for(var i=0;i<6;i++) {
      chInd = i.toString();
      tdRecord = document.getElementById("indicRecord" + chInd);
      tdRecord.innerHTML = nbCoupsRecord[i].toString();
   }
}

function chaineDescSeq(seq) {
   // crée une chaine de balises HTML qui décrivent une séquence de coups (un coup : un robot + une direction)
   var chaineTt = "";
   var chaine,robot,chDir;
   if(seq.length <= 30) { // limitation du nombre de coups affichés (contrainte de place)
      var longueur = seq.length;
   }
   else {
      var longueur = 30;
   }
   for(var i = 0;i < longueur;i++) {
      robot = seq[i][0];
      chDir = seq[i][1];
      chaine = "<div class='descFleche text" + listeCouleurs[robot] + "'>" + charFlecheHTML[chDir] + "</div>";
      chaineTt += chaine;
   }
   // console.log("[ fonction chaineDescSeq, donnée",seq,"renvoie",chaineTt,"]");
   return chaineTt;
}

function affIndEnigme() {
   // afficher l'indice de l'énigme que l'on est en train de résoudre
   var divTitre = document.getElementById("titreEnigme");
   var indCorrige = indEnigme + 1
   divTitre.innerHTML = "Énigme " + indCorrige.toString();
}

// --- GESTION DES TOUCHES

document.addEventListener('keydown', (event) => {
   var code = event.code;
   var toucheDir = ["ArrowUp","ArrowDown","ArrowRight","ArrowLeft"];
   var toucheEnigme = ["Digit1","Digit2","Digit3","Digit4","Digit5","Digit6"];
   console.log("touche détectée :",code);
   if(enCours) {

      // Appui sur une touche de flèche pour déplacer le robot
      if(toucheDir.includes(code)) {
         var touche = event.key;
         var descFleche = touche.slice(5);
         console.log("c'est une flèche directionnelle :",descFleche);
         if(robotCourant !== -1 && memoireDirsPotentielles.includes(descFleche)) {
            event.preventDefault();
            console.log("-> déplacement autorisé");
            var indDir = indDirection[descFleche];
            clickDir(indDir);
         }
      }

      // Appui sur une touche de nombre pour sélectionner une énigme
      if(toucheEnigme.includes(code)) {
         // event.preventDefault(); // commenté pour permettre de taper des chffres dans les pseudos
         var indEnigme = parseInt(code.slice(5));
         console.log("c'est un indice d'énigme :",indEnigme);
         clickLevel(indEnigme);
      }

      // Appui sur <Tab> pour changer de robot sélectionné
      if(code == "Tab") {
         event.preventDefault();
         console.log("c'est la touche Tab");
         nvRobot = (robotCourant + 1) % 4; // si <robotCourant> est -1 (indéfini), on sélectionne donc le premier robot
         clickRobot(nvRobot); 
      }
   }
}, false);