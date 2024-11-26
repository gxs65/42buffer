#!/usr/bin/env python2
# -*- coding: utf-8 -*-

from random import randint,shuffle
import json
import datetime

# UTILITAIRES

def adj(x,y):
   """Renvoie la liste des positions <x,y> des cases adjacentes à celle indiquée."""
   return [[x-1,y-1],[x-1,y],[x-1,y+1],[x,y-1],[x,y+1],[x+1,y-1],[x+1,y],[x+1,y+1]]

def murOppose(x,y,direction):
   """Indique comment placer le mur opposé à celui en <x,y>."""
   if direction == 0 and x>0:
      return [x-1,y,1]
   elif direction == 1 and x<15:
      return [x+1,y,0]
   elif direction == 2 and y>0:
      return [x,y-1,3]
   elif direction == 3 and y<15:
      return [x,y+1,2]
   else:
      return [7,7,3] # un endroit "poubelle" où il y aura un mur de toute façon

def symetrie(x,y,direction,quadrant):
   """Indique la position dans le quadrant <quadrant> du symétrique de <x,y> dans le quadrant 1."""
   indicDirx = [1,0]
   indicDiry = [0,0,3,2]
   if quadrant >= 2:
      x = 15 - x
      if direction == 0 or direction == 1:
         direction = indicDirx[direction]
   if quadrant%2 == 1:
      y = 15 - y
      if direction == 3 or direction == 2:
         direction = indicDiry[direction]
   return [x,y,direction]

def indtoxy(ind):
   """Calcule <x,y> à partir d'un indice sur 256."""
   x = ind // 16
   y = ind % 16
   return x,y

def affMurs(murs):
   """Fonction d'affichage en console, représentant une case par 5 nombres."""
   print("\n")
   for i in range(256):
      if i%16 == 0:
         print()
      case = murs[i]
      strCase = [str(case[_]) for _ in range(5)]
      chaine = "".join(strCase)
      print(chaine,end=" ")
   print()

def affMursGraphique(murs,casesInterdites):
   """Fonction d'affichage en console, représentant une case graphiquement."""
   print("\n")
   print("Affichage des murs générés, casesInterdites :",casesInterdites)
   for x in range(16):
      for y in range(16):
         ind = 16 * x + y
         case = murs[ind]
         print("+",end="")
         if case[0] or x == 0:
            print("--",end="")
         else:
            print("  ",end="")
      print("+")
      for y in range(16):
         ind = 16 * x + y
         case = murs[ind]
         if case[2] or y == 0:
            print("|",end="")
         else:
            print(" ",end="")
         if ind in casesInterdites:
            print("  ",end="")
         else:
            print("  ",end="")
      print("|")
   for i in range(16):
      print("+--",end="")
   print("+")

# COEUR

def creerBase(n):
   """Utilise <generer> pour créer les énigmes pour <n> jours à partir de la date actuelle."""
   
   d = datetime.datetime.now()
   for i in range(1,n):
      nd = d + datetime.timedelta(i)
      nomFich = "data" + nd.strftime("%d") + "_" + nd.strftime("%m") + "_" + nd.strftime("%Y") + ".js"
      generer(nomFich)

def generer(nomFich):
   """Génère une nouvelle série d'énigme avec une grille aléatoire, et l'enregistre dans un fichier nommé <nomFich>."""
   
   # Création des 3 variables à renvoyer : <posRobots>, <posCibles>, <murs>
   posRobots = []
   posCibles = []
   coins,murs,casesInterdites = genererMurs()
   
   # Placement des 6 cibles
   print("=== PLACEMENT DES CIBLES")
   shuffle(coins)
   print("liste des coins pour le placement des cibles :",coins)
   for indEnigme in range(6):
      xc,yc = coins.pop()
      cc = randint(0,3)
      posCibles.append([xc,yc,cc])
      print("placement d'une cible en",xc,yc,"de couleur",cc)
   print("---> 6 cibles placées :",posCibles)
   
   # Placement des 4 robots
   print("=== PLACEMENT DES ROBOTS")
   cases = [i for i in range(256) if i not in (119, 120, 135, 136)]
   shuffle(cases)
   print("liste des cases pour le placement des robots :",cases)
   for indRobot in range(4):
      ind = cases.pop()
      xc,yc = indtoxy(ind)
      posRobots.append([xc,yc])
      murs[ind][4] = indRobot
      print("placement d'un robot en",xc,yc,"-> case de <murs> modifiée :",murs[ind])
   affMurs(murs)
   affMursGraphique(murs,casesInterdites)
   
   # Écriture dans le fichier
   grosDict = {"posRobots":posRobots,
               "posCibles":posCibles,
               "murs":murs}
   chaineJSON = json.dumps(grosDict)
   chaineComplete = "export var data = " + chaineJSON + ";"
   print(">>> enregistrement des données dans le fichier",nomFich)
   f = open(nomFich,"w")
   f.write(chaineComplete)
   f.close()
   
   return posRobots,posCibles,murs

# convention d'orientation des coins : 0 HG, 1 HD, 2 BG, 3 BD
def genererMurs():
   """Choisit aléatoirement la position des murs."""
   
   coins = []
   casesInterdites = []
   murs = [[0,0,0,0,-1] for i in range(256)]
   for quadrant in range(4):
      
      print("=== PLACEMENT DES MURS, QUADRANT",quadrant)
      # Placer les deux murs isolés
      print("- placement murs isolés")
      x1 = randint(1,5) # mur horizontal (la ligne est importante)
      y1 = 0
      x2 = 0
      y2 = randint(1,5) # mur vertical (la colonne est importante)
      dir1 = 1 # pré-symétrie le mur horizontal est placé en BAS de sa ligne
      dir2 = 3 # pré-symétrie le mur vertical est placé à DROITE de sa colonne
      print("mur horizontal présym :",x1,y1,dir1)
      print("mur vertical présym :",x2,y2,dir2)
      baseInterdit = [[x1,1],[(x1 + 1),1],[1,y2],[1,y2 + 1],[6,6],[7,6],[6,7],[7,7]] # cases interdites pré-symétrie
      x1,y1,dir1 = symetrie(x1,y1,dir1,quadrant) # calcul symétrie des murs isolés
      x2,y2,dir2 = symetrie(x2,y2,dir2,quadrant)
      xb1,yb1,dirb1 = murOppose(x1,y1,dir1)
      murs[x1 * 16 + y1][dir1] = 1
      murs[xb1 * 16 + yb1][dirb1] = 1
      xb2,yb2,dirb2 = murOppose(x2,y2,dir2)
      murs[x2 * 16 + y2][dir2] = 1
      murs[xb2 * 16 + yb2][dirb2] = 1
      print("mur horizontal postsym :",x1,y1,dir1,"soit pour mur opposé :",xb1,yb1,dirb1)
      print("mur vertical postsym :",x2,y2,dir2,"soit pour mur opposé :",xb2,yb2,dirb2)
      
      # Placer les coins
      print("- placement coins")
      for xbase,ybase in baseInterdit: # calcul symétrie des cases interdites
         xsym,ysym,mull = symetrie(xbase,ybase,0,quadrant)
         casesInterdites.append(xsym * 16 + ysym)
      print("cases interdites avant placement des coins :",baseInterdit,"\nsoit ajouté au reste après symétrie",casesInterdites)
      nbCoins = randint(4,5) # nombre de coins à placer dans le quadrant
      print(nbCoins,"coins à placer")
      murSelonOrientCoin = [[0,2],[0,3],[1,2],[1,3]] # orientation des murs à placer selon l'orientation du coin
      for j in range(nbCoins):
         print("/ coin",j,"sur",nbCoins)
         continuer = True
         while continuer: # boucle tant qu'on a pas trouvé une case autorisée
            xBase = randint(1,7)
            yBase = randint(1,7)
            indBase = xBase * 16 + yBase
            xSym,ySym,mull = symetrie(xBase,yBase,0,quadrant)
            indSym = xSym * 16 + ySym
            continuer = False
            print("\ttrouvé case :",xSym,ySym,"->",end = " ")
            if indSym in casesInterdites:
               continuer = True
               print("interdite")
            else:
               print("pas interdite")
         orientCoin = randint(0,3) # orientation aléatoire du coin
         print("\tchoix d'orientation :",orientCoin,"soit des murs en",murSelonOrientCoin[orientCoin])
         for orientMur in murSelonOrientCoin[orientCoin]: # ajout des deux murs du coins
            xb,yb,dirb = murOppose(xSym,ySym,orientMur)
            murs[xSym * 16 + ySym][orientMur] = 1
            murs[xb * 16 + yb][dirb] = 1
            print("\tplacement de mur en :",xSym,ySym,orientMur,"; et opposé :",xb,yb,dirb)
         
         casesInterdites.append(indSym) # actualisation de la liste des cases interdites avec celle du coin et ses adjacentes
         casesAdjacentes = adj(xSym,ySym)
         for xa,ya in casesAdjacentes:
            casesInterdites.append(xa * 16 + ya)
         coins.append([xSym,ySym])
      
      print("FIN DE QUADRANT",quadrant,"-> cases interdites",casesInterdites,"coins placés",coins)
   
   # Indépendamment des quadrants, placer les murs du carré central
   for xCentre,yCentre in [[8,8],[7,7],[7,8],[8,7]]:
      for orientMur in range(4):
         xbCentre,ybCentre,orientbMur = murOppose(xCentre,yCentre,orientMur)
         murs[xCentre * 16 + yCentre][orientMur] = 1
         murs[xbCentre * 16 + ybCentre][orientbMur] = 1
   
   print("\n\nétat final de <murs> :")
   affMurs(murs)
   
   return coins,murs,casesInterdites

# EXÉCUTION

if __name__ == "__main__":
   creerBase(365)