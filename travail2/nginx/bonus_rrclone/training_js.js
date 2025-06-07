// --- INITIALISATION

function generer() {
   // appelée par l'utilisateur pour créer la grille et commencer la résolution
   
   // S'il y avait une grille en cours avant, on enlève sa cible et son bouton d'enregistrement de score
   if(!enCours) {
      enCours = true;
   }
   else {
      effacerCible();
   }

   // Initialisation des variables globales
   indEnigme = 0; // sélection de la première énigme par défaut
   robotCourant = -1;
   positions = [0,0,0,0];
   positions_initiales = [0,0,0,0];
   nbCoups = 0;
   nbCoupsRecord = [0,0,0,0,0,0];
   genererMurs();
   creerPlateau();
   memoireFlechesPlateau = [];
   memoireDirsPotentielles = [];
   memoireMeilleuresSeqs = [[],[],[],[],[],[]];
   memoireSeqActuelle = [];
   nbEnigmesResolues = 0;
   
   // placement de l'objectif pour les 6 énigmes
   posCible = [];
   var dejaPris = [];
   var indCoin,continuer,xc,yc,cc;
   for(var i=0;i<6;i++) {
      continuer = true;
      while(continuer) {
         indCoin = randInt(0,coins.length);
         continuer = false;
         if(dejaPris.includes(indCoin)) {
            continuer = true;
         }
      }
      dejaPris.push(indCoin);
      xc = coins[indCoin][0];
      yc = coins[indCoin][1];
      cc = randInt(0,4);
      console.log("positionnement de la cible",i,":",xc,yc,"couleur",cc);
      posCible.push([xc,yc,cc]);
   }
   
   // choix de la position initiale des robots 
   var positions_interdites = [7 * 16 + 7,7 * 16 + 8,8 * 16 + 7,8 * 16 + 8];
   var x = 0;
   var y = 0;
   var continuer = true;
   for(var i=0;i<4;i++) {
      continuer = true;
      while(continuer) {
         x = randInt(0,16);
         y = randInt(0,16);
         continuer = false;
         if(positions_interdites.includes(x * 16 + y)) {
            continuer = true;
         }
         else {
            positions_interdites.push(x * 16 + y);
            positions[i] = [x,y];
         }
      }
   }
   for(var i=0;i<4;i++) {
      x = positions[i][0];
      y = positions[i][1];
      positions_initiales[i] = [x,y];
      murs[x * 16 + y][4] = i;
      affRobot(i);
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
   
   // Affichage initial
   chargerEnigme(0,0);
   affMurs();
}

// FONCTIONS PROPRE À "TRAINING"

function genererMurs() {
   murs = [];
   coins = [];
   casesInterdites = [];
   for(var i=0;i<256;i++) {
      murs.push([0,0,0,0,-1]);
   }
   
   // 0 HG, 1 HD, 2 BG, 3 BD
   
   var x,y,pos,x1,y1,x2,y2,direction1,direction2,xb1,xb2,yb1,yb2,base,nbCoins,aPlacer,continuer;
   var orientCoin,orientMur,casesAdjacentes,coordsx,coordsy;
   for(var i=0;i<4;i++) {
      
      // POSITION DES 2 MURS DU BORD
      x1 = randInt(1,6);
      y1 = 0;
      x2 = 0;
      y2 = randInt(1,6);
      direction1 = 1;
      direction2 = 3;
      pos1 = symetrie(x1,y1,direction1,i);
      xb1 = pos1[0];
      yb1 = pos1[1];
      direction1 = pos1[2];
      pos2 = symetrie(x2,y2,direction2,i);
      xb2 = pos2[0];
      yb2 = pos2[1];
      direction2 = pos2[2];
      ajouterMur(direction1,xb1,yb1);
      ajouterMur(direction2,xb2,yb2);
      
      base = [[x1,1],[(x1 + 1),1],[1,y2],[1,y2 + 1],[6,6],[7,6],[6,7],[7,7]];
      base.forEach( (elem,j) => {
         xbase = elem[0];
         ybase = elem[1];
         posSym = symetrie(xbase,ybase,0,i);
         casesInterdites.push(posSym[0] * 16 + posSym[1]);
      });
      
      nbCoins = randInt(4,6);
      aPlacer = [[0,2],[0,3],[1,2],[1,3]]; // orientation des murs à placer selon l'orientation du coin
      console.log("quadrant",i,"murs isolés horizontal",x1,y1,"/",xb1,yb1,"vertical",x2,y2,"/",xb2,yb2,"- coins à placer",nbCoins);
      
      for(var j=0;j<nbCoins;j++) {
         
         // POSITION DU COIN SUR 8*8
         continuer = true;
         while(continuer) {
            y = randInt(1,8);
            x = randInt(1,8);
            continuer = false;
            pos = symetrie(x,y,0,i);
            if(casesInterdites.includes(pos[0] * 16 + pos[1])) {
               continuer = true;
               // condition pour retenter : la case fait partie des 4 cases interdites à côté des murs isolés
            }
         }
         
         pos = symetrie(x,y,0,i);
         x = pos[0];
         y = pos[1];
         
         // ORIENTATION ALÉATOIRE
         orientCoin = randInt(0,4);
         //console.log("-> quadrant",i,"-> coin",j,"à",x,y,"orienté",orientCoin);
         for(k=0;k<2;k++) {
            orientMur = aPlacer[orientCoin][k];
            ajouterMur(orientMur,x,y);
         }
         
         casesInterdites.push(x * 16 + y);
         casesAdjacentes = adj(x,y);
         for(var k=0;k<casesAdjacentes.length;k++) {
            xa = casesAdjacentes[k][0];
            ya = casesAdjacentes[k][1];
            coins.push([x,y]);
            casesInterdites.push(xa * 16 + ya);
         }
      }
      //console.log("cases interdites à la fin du quadrant",i,":",casesInterdites);
   }
   
   // MURS DU CARRÉ CENTRAL
   coordsx = [7,7,6,9];
   coordsy = [6,9,7,7];
   //orientMur = [1,0,3,2];
   for(var i=0;i<4;i++) {
      ajouterMur(i,7,7);
      ajouterMur(i,8,7);
      ajouterMur(i,7,8);
      ajouterMur(i,8,8);
   }

   console.log("final <murs> :",murs);
}

function terminerEnigme() {
   // appelée par la fonction <deplacer> quand l'utilisateur vient de résoudre une énigme avec moins de coups qu'avant
   console.log("Passage par <terminerEnigme>");
}