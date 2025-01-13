/* TODO

=== NOTES SUR VIDEO : https://www.youtube.com/watch?v=oIFRiwFRSRY&list=PL7_TuD9ZDMhg5uLHLyd8em13XBKfjzCzR&index=4
- attention aux fonctions non-async safe dans les signal handler !
- autres choses que fait un shell : gerer la duree de vie des programmes (job control, mise en arrière-plan)
- gestion du travail en équipe : dur de ne pas pouvoir faire des essais/erreurs de son côté
	\ il faut que le projet actuel fonctionne tous le temps
	\ avoir toujours un truc à présenter, même si ça ne fait pas grand chose
	\ éviter de se mettre en dépendance les uns des autres -> éviter une séparation parsing/execve
- étapes proposees :
	\ exécution de commandes simples, sans grammaire de shell ==> bloc de base
	\ ensuite on peut commencer à parralléliser les tâches, en gardant le bloc de base fonctionnel
	\ construire un AST (asynchronous tree) de noeuds qui sont des commandes / des méta-caractères
		mais apparemment plutôt avec un noeud ayant n'importe quel nb d'enfant (plutôt qu'un arbre binaire)
- différence syntaxe abstraite / syntaxe concrète :
	\ syntaxe concrète = ce qui est entré par l'utilisateur : "b += 12"
	\ syntaxe abstraite = comment on représente la vraie requête : "b = b + 12"

=== A FAIRE
+ lire le shell command language manuel : https://pubs.opengroup.org/onlinepubs/009604499/utilities/xcu_chap02.html
+ reflechir a la pertinence de la priorite, et trouver une manière de différencier :
	`cat in | (wc -l && cat)`	-> wc mange l'output du premier cat, donc le 2nd cat a un input defini vide et ne renvoie rien
	`(cat in | wc -l) && cat`	-> le second cat n'a pas d'input defini autre que stdin, donc il s'execute sur stdin
	`cat in | wc -l && cat`		-> le second cat n'a pas d'input defini autre que stdin, donc il s'execute sur stdin
		-> en particulier, faut-il commencer l'analyse de la ligne de tokens depuis la fin ?
	=> OUI, bash fonctionne en priorite associative <=> dans `cat in | wc -l && cat` le noeud root est de type &&,
		ce qui signifie qu'on commence la recherche de meta-caracteres par la fin de la ligne de tokens
+ reflechir au probleme de l'execution et du waitpid :
	\ quand on entre `ls | grep truc | cat`, notre implementation a arbre binaire agit comme pour `ls | (grep truc | cat)`
		ce qui ne change rien au resultat, mais pose la question de ce qu'on waitpid au premier noeud :
			en theorie ca devrait etre chacun des 3 processes forkes pour lancer les 3 commandes
		-> faut-il que chaque noeud de type "|"	a) si son parent n'est pas pipe, attende ses pids
												b) si son parent est pipe, renvoie une liste chainee des pids a attendre
			ou alors forker les enfants (meme si ce ne sont pas des leafs) juste pour pouvoir les waitpid ?
	\ quand on entre `cat | (wc -l && cat) | grep truc`, bash va wait que chacun des membres termine
		avant de rendre le prompt, mais le membre du milieu est `(wc -l && cat)`, ie. un ensemble de commandes
			plutot qu'un seul process forke pour executer une seule commandes
		-> faut-il qu'a chaque noeud de type "|" on forke les deux enfants pour pouvoir les waitpid ???
	=> OUI, il faut fork a chaque noeud pipe, meme si les noeuds enfants ne sont pas des leafs
+ se resoudre a une solution sur le probleme de la representation des tokens
		appartenant a la commande d'un noeud leaf / a la redirection d'un noeud leaf ou parenthesis
	=> solution lourde en malloc avec un tableau intermediaire d'indices de tokens, et un tableau final de chaines de caractere
- gerer la segfault dans le cas de ligne vide
- reflechir a shell expansion dans le heredoc
- reflechir a la recuperation de la valeur de retour des commandes qui ont ete pipees
- faire une liste des cas d'invalidite d'une commande
- faire une liste des variables allouees dynamiquement

- DOCUMENTATION SUR LES FONCTIONS DU SUJET
- commande "bash" : option "--posix" pour la version minimale conforme a POSIX
	\ separe une chaine en "tokens", puis traite (1) les pipes (2) les redirections (3) les commandes
		/!\ ne pas oublier qu'une ligne peut commencer par une redirection plutot qu'une commande
	\ les redirections du pipe (creation des fichiers pipes) sont executees avant toute autre commande/redirection
		/!\ en testant des combinations de pipe + redirection (ex "ls > out | shuf > out") on se rend compte
			que les 2 redirections s'appliquent : la sortie de <ls> est a la fois ecrite dans <out> et passee a <shuf>
	\ return value ($?) est le statut de la derniere commande :
		son exit status si elle est sortie proprement, ou 128+ind du signal qui l'a tuee avant qu'elle sorte
	\ pas besoin de gerer ";" (execution sequentielle) ou "&" (execution en arriere-plan),
		par contre possible dans les bonus de gerer "&&" et "||", apparemment ONT PRIORITE sur "|"
	\ partie la plus importante du "man bash" pour le parsing (simplifie pour nous) :
		shell expansion = parameter expansion + [cmd substitution] + pathname expansion + quote removal
	\ rappel sur le here_document : il n'est pas forcement issu de stdin,
		ca peut etre un simple fichier lu jusqu'a la string donnee
	\ pas necessaire pour minishell mais interessant : la redirection avec "<"/">" peut etre precedee d'un nombre
		dans ce cas le shell ouvrira le fichier (en RD ou WR selon "<" our ">") en lui donnant un fd egal au nombre,
			le fichier etant ainsi a la disposition du programme sans qu'il ait a l'open
- commandes "env", "unset", "export"
- <readline> and functions <rl_*>, interface de ligne de commande pour l'utilisateur
		utilisee par la plupart des programmes avec une ligne de commande (ftp, bash...)
	\ readline lit stdin jusque trouver un '\n' ou lire 0 caracteres (<=> EOF),
		si EOF trouve avec 0 caracteres en stock : NULL est renvoyee, sinon : les caracteres en stock sont renvoyes
	\ par defaut, readline autocomplete quand l'user entre <Tab>,
		et cherche dans l'historique quand l'user entre <Up>/<Down> et si les lignes precedentes on ete <add_history>
			/!\ mieux vaut ne pas <add_history> les lignes vides
	\ pas tres clair pour l'instant sur les cas d'utilisation de <rl_replace_line> ou <rl_on_new_line>
- <fstat>
- <tcset/get*>
- <*tty*>

= CAS SPECIFIQUES DE BASH
- multiples redirections : `cat in > out1 > out2`, `cat > out < in1 < in2`, `cat > out < in*`
	-> comportement constate : bash n'applique que la derniere redirection, mais attention :
		~ si l'expansion du caractere '*' donne plusieurs noms de fichiers, bash emet une erreur "ambiguous redirection"
		~ double redirection output : seul le second fichier recoit,
			mais le premier est quand meme ouvert en O_TRUNC (<=> vide)
		~ double redirection heredoc : de meme 2 entrees manuelles de heredoc sont requises,
			mais seule la seconde est utilisee
	SAUF quand une redirection est groupee entre parenthese avec la commande,
		dans ce cas cette redirection est prio (et donc bat la seconde redirection, dont le fichier est qd meme ouvert)
- redirections en meme temps que pipe : `cat in > out1 | cat > out2`, `cat in1 | cat < in2`
	-> comportement constate : bash ne duplique rien et la redirection a la priorite, donc
		~ redirection input -> redirection "fournit tout l'input" et donc pipe "transmet des donnes inutilisees"
		~ redirection output -> redirection "mange tout l'output" et donc pipe "transmet un flux nul"
	SAUF quand des () sont utilisees pour grouper des commandes liees par un pipe, exemples :
		~ `(grep lorem | cat) < in` : le contenu de in est envoye a grep, le resultat de grep est envoye a cat
		~ `(ls | grep lorem) > out` : le resultat de ls est traite par grep, le resultat de grep est place dans "out"
			/!\ dans ce genre de cas a parentheses on ne peut pas placer la redirection en premier, ex. `> out (cmd1)`
- pipe vers des commandes liees par && ou ||
	-> comportement constate : bash agit "logiquement" en distribuant/collectant l'output/input de tout un groupe entre ()
		~ `(ls | grep "lorem") && wc -l` : pas d'echec dans le pipe, donc wc s'execute normalement (sur stdin ici)
			/!\ en fait wc ne s'executera que si grep trouve des matches a "lorem",
				car grep donne un exit status de 1 (erreur) s'il ne trouve aucun match
		~ `(wc -x | grep "1") && wc -l` : echec du premier wc, donc echec de grep, donc second wc n'est pas execute
		~ `(wc -x 2>&1 | grep "opt") && wc -l` : echec du premier wc, mais il donne quand meme des lignes a grep (son stderr)
			donc grep peut s'executer sans echec et donc second wc s'execute aussi
		~ `ls | (wc -l && grep lorem)` : wc mange tout l'output recu de ls, donc grep s'execute mais echoue (aucun input)
		~ `ls | (wc -x || grep lorem)` : wc echoue donc ne mange rien de l'output, grep peut s'executer et trouver des lignes

= PLAN PROVISOIRE
- liste des meta-caracteres a gerer : | || && * $ < > << >> ()
- etapes de parsing a la reception d'une ligne non vide , par exemple `ls | (wc -l && grep 'lorem ipsum') > out` :
	1. Split initial en tokens -> resultat : un tableau de strings
		~ separateur de base : space (ou equivalent)
		~ separateur special : tous les meta-caracteres (sauf $ et *) en dehors de parenthese,
			qui sont a la fois un token et un separateur
		~ tous les caracteres a l'interieur d'une paire de guillemets forment un seul token,
			le guillemets font partie du token (pour l'instant)
		==> [ "ls" "|" "(" "wc" "-l" "&&" "grep" "'lorem ipsum'" ")" ">" "out" ]
	2. Hierarchisation des meta-caracteres pour creer l'arbre de commandes | || && ()
			-> resultat : un arbre binaire d'instance de la structure <node>, se differenciant selon les types
		a) ||	-> 2 enfants
		b) &&	-> 2 enfants
		c) |	-> 2 enfants
		d) ()	-> 1 enfant, 1 tableau de strings redirection
		e) LEAF -> 0 enfant, 1 tableau de strings redirection, 1 tableau de strings commande
		==> node |	------------> node LEAF, commande "ls", stdout redirige vers pipe
					------------> node (), stdin vient de pipe, str redir ">" "out"	--------> node &&	--------> node LEAF, commande "wc" "-l"
																										--------> node LEAF, commande "grep" "'lorem ipsum'"
	3. Gestion des tokens a l'interieur des tableaux de commande et de redirection
		~ dans les nodes LEAF, separer les tableaux de commande et de redirection
		~ expansion des variables shell (meta-caractere $), puis re-split uniquement les espaces
		~ expansion du meta-caractere *
		~ determination de l'executable (ou de la variable a assigner si le token de commande contient "=")
		~ retrait des guillemets
	4. Interpretation des redirections
		~ stockage (PAS OUVERTURE) des noms de fichiers de redirection
		~ lecture de tous les heredoc eventuels, stockes dans des fichiers de /tmp
			/!\ substitution de variables dans les lignes du heredoc
- etapes de l'execution en suivant l'arbre construit
	1. si le noeud est un noeud LEAF ou PARENTHESIS, verifier s'il existe des redirections
		~ pour chaque direction (in / out), ouvrir le dernier fichier donne, et remplacer nos fdio par ces fd
		~ si plusieurs fichiers avaient ete donnes, ouvrir et refermer immediatement les fichiers qui ne sont pas dernier
		~ pour la redirection "out", en faisant attention a suivre le mode (TRUNC / APPEND)
	2. si le noeud est un noeud LEAF ou PARENTHESIS
		~ LEAF : fork un executeur (qui va dup2 pour remplacer les fd 0 et 1 par les eventuelles redirection, puis execve)
			ensuite attendre waitpid l'executeur, enregistrer son exit status et le renvoyer
		~ PARENTHESIS : assigner a child_1 les memes fdios que nous, et recurrer sur child_1 (et renvoyer son exit status)
	3. si le noeud est un noeud PIPE
		~ creer un pipe classique
		~ assigner les fdios de child_1 et child_2
			. child_1 a fdin herite de noeud courant et fdout dans pipe
			. child_2 a fdin depuis pipe et fdout herite de noeud courant
		~ fork un process pour child1, qui va recurrer sur child_1
		  fork un process pour child2, qui va recurrer sur child_2
		~ attendre les deux enfants, renvoyer le exit_status de child_2
	4. si le noeud est un noeud OR ou AND
		~ assigner les fdios de child_1 et child_2, simplement herites du noeud courant
		~ recurrer sur child_1, recuperer la valeur de retour (qui est le exit status)
		~ selon la valeur de retour, eventuellement recurrer sur child_2
		~ renvoyer le exit_status de child_2

- cas d'invalidite de la ligne de commande entree :
	\ guillemet laisse ouvert
		-> check : durant l'etape Parsing1
	\ parentheses non coherentes
		-> check : entre l'etape Parsing1 et Parsing2
	\ noeud leaf sans token commande (meme s'il y a des tokens de redirection)
		-> check : apres l'etape Parsing3
			/!\ pas le fonctionnement exact de bash, mais plutot logique
	\ meta-caractere (ou fin de commande) apres caractere de redirection
		-> check : entre l'etape Parsing1 et Parsing2
	\ resultat d'expansion de redirection qui donne 0 ou >1 chaines de caractere post-split(' ')
		-> check : durant l'etape Parsing3


*/



/*

=== MODIFICATIONS A TRANSMETTRE
	\ ft_parse2_build_tree.c
	\ ft_parse3_expansion.c
	\ ft_parse3_utils.c
	\ 2 macros dans le header : LOGS et LOGSV
	\ 1 variable en plus dans le type data : nb_tokens

*/