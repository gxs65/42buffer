/* Notes generales

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

=== PROBLEMES D'IMPLEMENTATION
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

=== RAFFINEMENTS A EFFECTUER
+ lire le shell command language manuel : https://pubs.opengroup.org/onlinepubs/009604499/utilities/xcu_chap02.html
+ gerer le cas de guillemets a l'interieur du token ex. <<echo"$var"'$var'>>
- gerer la segfault dans le cas de ligne vide
- reflechir a shellvar (mais pas pathname) expansion dans le heredoc
- reflechir a la transmission des erreurs rencontrees (un errno avec une valeur par defaut pour malloc ?)
- dans la shellvar expansion, rajouter le cas des variables shell speciales $?
- stockage des shellvar avec une liste chainee plutot qu'un tableau, pour pouvoir en rajouter
- reflechir a la recuperation de la valeur de retour des commandes qui ont ete pipees
- faire une liste des cas d'invalidite d'une commande
- faire une liste des variables allouees dynamiquement
- permettre les return en 1 ligne en cas d'erreur avec une fonction de log d'erreur
- reflechir a la gestion des pipes dans l'execution : vu qu'ils ne transmogrifient pas le process avec execve,
	possible de se contenter d'un thread que l'on join ? a-t-on le droit d'utiliser un pipe pour connecter des threads ?
- raccorder le split du path avant de process une nouvelle ligne de commande au initialize_data general
	/!\ on doit faire une requete pour la variable d'environnement PATH
	/!\ si elle n'existe pas, on ne cherchera les executables que dans le repertoire courant
- verifier et rendre une erreur si on trouve 2 fois le meme fichier dans les redirections, dans des sens opposes
- passer la norminette dans la libft : corrections a faire dans le ft_printf modifie
- implementer le fait que apparemment quand la commande exit est dans un pipe, elle ne s'execute pas...
- verifier comment gerer path_max_len
- rectifier l'erreur de norme dans le header de get_next_line (acd le ifndef supplementaire ?)
- se renseigner sur la question de la gestion du mode interactif ?
	-> utiliser <isatty> pour se proteger contre les lancements de minishell avec une redirection de l'input !
- interagir avec l'environnement : modifier la var d'environnement SHELL, PWD, OLDPWD
	(quand on lance bash dans zsh, utilise `cd` puis `exit`, zsh n'a pas change de dossier :
		donc la variable d'environnement PWD d'est pas un proxy, det reellement ou on est pour par ex `ls` qui prendra envp en arg a l'execve)
- avoir un prompt qui donne aussi le cwd (demande un malloc de plus)
- dans la pathname expansion :	\ ne pas tenir compte des '*' entre guillemets
								\ faire un quote removal sur les tokens avant expansion
- le builtin <export> doit accepter les commandes du type `export s=1`
- verifier qu'on produit bien une erreur de syntaxe quand on a un noeud parenthese avec un token de redir qui est "="
- correction de l'etape parse1 :
	\ proteger le malloc du ft_substr dans la fonction get_token
	\ assurer le bon fonctionnement sur les guillemets (pas des separateurs)
	\ verification syntax error d'incoherence de parentheses ou guillemets
	\ verification strncmp des identifications de tokens

=== DOCUMENTATION SUR LES FONCTIONS DU SUJET
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
- pathname expansion : bash skip les '*' entre simples ou doubles guillemets,
	mais quand il y a des '*' hors-guillemets il peut y avoir d'autres morceaux entre guillemets,
		qui seront pattern-matches sans les guillemets : d"*$var* avec var=r renvoie `dir1 dir2 dir3`
	-> methode d'implementation pas exactement equivalente mais fonctionnelle serait lors du split post-expansion :
		si le token n'aurait qu'un seul morceau suite au split, et qu'il contient un '*' hors-guillemets,
			on applique la pathname expansion dessus qui retire les guillemets puis tente le pattern matching
				(soit elle ne trouve rien et laisse le token en l'etat, soit elle trouve des trucs et les renvoie)
		sinon on le split sur les espaces de facon classique

=== CAS PARTICULIERS TESTES AVEC BASH
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

=== COMPREHENSION DE CWD ET CD
- quand on lance bash dans zsh, utilise `cd ..` puis `exit`, zsh n'a pas change de dossier :
	il semble que bash a son propre CWD, herite de zsh initialement mais qu'il modifie sans impacter zsh
		-> idee initiale : la substance de CWD est le contenu de la variable d'environnement "PWD",
			qu'on passe aux commandes qu'on execute avec <execve> pour qu'elles en heritent
		et il n'y a aucune autre representation du CWD d'un process
- mais battu en breche par l'exemple de modifier la var d'environnement CWD directement dans bash, ex `PWD="/"` :
	(le prompt de bash sera modifie car apparemment il utilise la valeur de PWD) mais appeler ensuite `pwd` ou `ls` montre qu'on a pas bouge !
		-> en realite il existe une autre representation du CWD d'un process : dans les systemes UNIX, dans le dossier "/proc/",
			il y a un dossier pour chaque process (identifie par son PID) contenant les infos sur la representation de ce process dans le kernel,
				notamment "cwd" = un lien symbolique vers le current working directory de ce process
		(en fait les proc files d'un process contiennent toutes les infos issues de l'initialisation et des system calls dans un process :
			command line arguments argv, environnement envp, open file descriptors, memoire utilisee...)
- donc meilleure interpretation de l'exemple initial : dans le process zsh, on a envoye la commande `bash`,
	donc zsh a forke et le process enfant a execve sur l'executable de bash (sachant que execve maintient le PID et le CWD) :
		bash a donc son propre process avec ses infos dans "/proc/<bashPID>/",
			et son propre CWD ("/proc/<bashPID>/cwd") initialement herite de zsh mais qu'il peut modifier sans impacter zsh
- dans cet heritage de CWD par les process forkes, la variable d'environnement "PWD" ne joue aucun role :
	les variables d'environnement appartiennent au "User space", alors que la duplication de process par fork
	manipule des structures de donnees decrivant le "process state" dans le "kernel space"
		(d'ailleurs le lien "/proc/<bashPID>/cwd" ne joue pas de role non plus, il est lui aussi un relais et pas la representation ultime du CWD)
- consequence pour Minishell : la variable d'environnement "PWD" n'est qu'une "user-space convenience" pour representer le vrai CWD,
		et donc il faut s'assurer manuellement de sa bonne valeur si une action devait la modifier/initialiser :
	\ modification, par un `cd` : notre version builtin de cd, apres avoir utilise <chdir>,
		doit modifier la variable "PWD" (sans doute avec <getcwd>)
	\ initialisation, par un fork (ou juste le lancement du programme par fork+execve) :
		fork copie et donc conserve les variables d'environnement, donc rien a faire si la variable "PWD" du process qui a forke avait la bonne valeur,
			(mais au cas ou on peut toujours verifier avec <getcwd>)

=== PLAN PROVISOIRE
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

=== LISTES
- erreurs de syntaxe :
	I guillemets incoherents, parentheses incoherentes
		-> check : durant l'etape Parsing1
	I probleme de taille de fichier executable / de redirection (PATH_MAX_LEN)
		-> check : durant l'etape Exec2 (redir) et Exec3 (executable)
	+ redirection incoherente <=> meta-caractere de redir non suivi d'un mot
		-> check : apres l'etape Parsing2 sur un noeud
	+ noeud leaf sans command words (meme s'il y a des redir words)
		-> check : apres l'etape Parsing2 sur un noeud
			(et non apres Parsing3, parce que si l'absence de cmd words est le resultat d'une expansion, c'est OK)
	+ noeud PAR contenant un "=" / noeud LEAF contenant un "=" mal place
		-> check : durant l'etape Parsing2
	+ ligne sans tokens
		-> check : durant l'etape Parsing2 (apres avoir compte les tokens)
	+ resultat de resplit sur un redir word qui donne 0 ou >1 chaines de caractere
	  resultat de pathname expansion sur un redir word qui donne 0 ou >1 chaines de caractere
		-> check : durant l'etape Parsing3
- erreurs sur une commande en particulier pendant l'execution (n'empeche pas l'execution des autres commandes)
	\ pas d'executable trouve pour une commande
		-> intervient : durant l'etape Exec3, avec des access
	\ pas de fichier de redirection avec le nom donne
		-> intervient : durant l'etape Exec2, quand un open ne reussit pas
	\ system calls malloc et fork
- erreurs de system calls
	\ sigaction			-> Init1 pendant mise en place des gestionnaires de signaux
	\ malloc			-> (cf liste des variables allouees)
	\ pipe				-> Exec1 sur noeud pipe
	\ open				-> Exec2 fichiers de redirections, Exec0 heredoc
	\ opendir			-> Parse3 pendant pathname expansion
	\ fork				-> Exec1 sur noeud leaf, Exec1 sur noeud pipe
	\ execve			-> Exec3 par definition
	\ systems calls des builtins
- variables malloc LONG-TERME (free a la fin de minishell)
	\ <var>
	  <var.value> + <var.name>						de Init1 recuperation de l'env, et Exec4 assignation de variables shell
- variables malloc MOYEN-TERME (free a la fin de la gestion d'une ligne)
	\ <data.path_dirs>								de Init1 recuperation du path
	\ <data.tokens.name> + <data.tokens.type>		de Parse1 tokenisation, certains morceaux remplaces dans Parse3 shellvar expansion
	\ <node>										de Parse2 construction de l'arbre
	\ <node.redir_tokens_inds>
	  <node.cmd_tokens_inds>						de Parse2 construction de l'arbre
	\ <node.redir_words>
	  <node.cmd_words>								de Parse3 resplit, remplace dans Parse3 expansion des pathnames
	\ tous les duplicats issus des forks devant execve mais dont execve echoue / des forks ne devant pas execve (acd pipe)
-  variables COURT-TERME (free dans la fonction qui l'a allouee, ou pas si arrive a execve)
	\ <execfile> (char *)							de Exec3 recuperation du nom de fichier executable
													/!\ pas besoin de free dans Exec3, car fait partie des cmd_words du noeud
	\ <envp> (char **)								de Exec3 reconstitution de l'env pour le passer a execve
	\ <value> (char *)								de Exec4 valeur de la variable shell
	\ <line>										de Parse1 appelant readline

=== COMMUNICATION ENTRE VERSIONS
- modifications par rapport aux fichiers precedents
	\ tous les fichiers de la forme ft_parse[23] ou ft_exec[123]
	\ 2 macros dans le header : LOGS et LOGSV
	\ 1 include dans le header : <sys/stat.h>
	\ plusieurs variables supplementaires dans les types data et node
	\ chaine <readline> de main pour ajouter une couleur au prompt
	\ ajout des fonctions de parse et d'execution dans les tests du main
	\ remplacement de la fonction <isspace> qui etait dans <ft_utils_1>
	\ remplacement de ft_printf classique par une version qui appelle <write> 1 seule fois
		/!\ apres avoir mis en conformite avec la norminette
- aspects de bash non respectes pour le moment
	\ pathname expansion qui renvoie des mots (noms de fichier) avec des guillemets :
		ces guillemets seront enleves pendant quote removal, alors qu'ils devraient rester
	\ shellvar expansion qui renvoie des mots  avec des guillemets :
		ces guillemets seront enleves pendant quote removal, alors qu'ils devraient rester
			(ex `var="a 'b c'"` puis `echo $var` donne `a b c` mais doit donner `a 'b c'`)
	\ l'expansion est faite pour tous les noeuds avant le passage en exec,
		de sorte que la commande `var=5 ; echo $var` affiche la valeur de var avant `var=5`
			+ les checks de syntaxe seront aussi effectues avant l'exec, ex checks de non ambiguite des redirs
	\ on considere les signes "=" comme des meta-caracteres separateurs de tokens,
		donc on acceptera `a = 6` (alors que bash verrait ca comme une commande a deux arguments)
			ce qui empeche les programmes d'avoir des arguments pouvant etre un "="
				(sauf si on quote cet argument quand on le donne au programme)
	\ la valeur de la variable $? n'est mise a jour qu'a la fin de l'execution de toute une ligne,
		de sorte que la commande `rien || echo $?` affiche 0 au lieu de 127 (exec not found)
	\ pathname expansion prend aussi en compte les fichiers caches
	\ en cas de pipe ou l'un des membres rencontre une erreur avant execve (par ex OPEN ou NOEXEC),
		ce membre va fermer sa moitie du pipe et donc le membre de l'autre cote recoit un SEGPIPE,
			ce qui empeche son execution (qui pourrait etre plusieurs commandes liees par un &&)
		alors que dans bash le membre valide peut s'executer en entier normalement,
			ex. `(cat file1 && touch file2) | wc -x` cree le fichier file2, meme si `wc -x` fait fail membre droit

*/

/* Plan des taches a realiser

- définir les cas d'erreur de syntaxe de la commande,
	leur assigner une valeur, un message d'erreur,
		et faire en sorte qu'ils n'interrompent pas Minishell mais produisent un nouveau prompt
- définir les cas d'erreur d'appels système dans parsing+exec (malloc/open/execve),
	leur assigner une valeur, un message d'erreur,
		et faire en sorte qu'ils interrompent toute l'exécution de Minishell
- gestion de la mémoire dans parsing+exec
	\ libération de toutes les variables allouées à la fin de l'exécution
	\ en cas d'erreur d'appel système / de syntaxe de la commande,
		libérer ce qui a été alloué jusqu'ici

- redefinition du path avant de commencer l'execution d'une ligne
	(au cas ou la variable d'environnement aurait change / ete unset)
- recherche d'un fichier exécutable selon le premier mot de la commande
	\ premier mot sans "/" : membre des BUILTIN, sinon dans le PATH
	\ premier mot avec "/" en premier caractère : chemin absolu
	\ premier mot avec "/" à l'intérieur : chemin relatif
- recherche d'un fichier de redirection
	\ nom sans "/" : dans le répertoire courant
	\ nom avec "/" en premier caractère : chemin absolu
	\ nom avec "/" à l'intérieur : chemin relatif
+ pattern-matching des expansions de "*"
	et cas particuliers de "*" suivi de "/", et "./" suivi de "*"

- gestion des signaux, avec une seule variable globale
	\ que doivent faire exactement Ctrl+C et Ctrl+D ?
	\ doit-on se contenter de la gestion par défaut des signaux par les programmes en cours d'exécution,
		ou les kill (SIGKILL) en plus si on reçoit un Ctrl+C ?

- conversion de l'environnement initial en une liste chainee
	\ cas d'un environnement initial minimal
	\ chaque élément de la liste chaînée à un bolléen indiquant s'il fait partie de l'env
	\ ajout de variables sans export <=> ajout d'éléments à la liste chaînée
	\ export et unset <=> modification du booléen dans la liste chaînée
	\ fonction transformant la liste chaînée en un tableau de chaînes de caracteres,
		pour pouvoir donner ce tableau en paramètre à execve
+ détection des noeuds ou le premier mot contient un "=" :
	elles définissent la valeur d'une variable,
		donc erreur si la ligne n'a pas exactement un noeud avec exactement un mot
			/!\ pas le comportement exact de Bash, mais plutot logique
			/!\ l'assignation de variable passe quand meme par shellvar et pathname expansion
- gestion de la variable spéciale $? = exit status de la dernière exécution
	-> documentation sur les valeurs des exit status, et comment les interpréter

- dans Parsing1, modifier la gestion des guillemets,
	pour que par exemple `"a"'b'c` soit considéré comme un seul token
- gestion du heredoc : tous les heredoc sont lus AVANT de commencer l'exécution,
	donc leur contenu doit être stocké dans un fichier temporaire,
		dont le nom remplace la string EOF dans le tableau <node.redir_words>

- inclure des cas d'erreur de syntaxe / erreurs de fichiers dans les tests,
	pour verifier que la memoire est quand meme bien liberee
- comprendre a quoi servent toutes les fonctions dispos et qu'on a pas encore utilisees
- passer en revue les cas particuliers de
https://docs.google.com/spreadsheets/d/1uJHQu0VPsjjBkR4hxOeCMEt3AOM1Hp_SmUzPFhAH-nA/edit#gid=0
	vérifier si on les gère sans segfault + sans fuite de mémoire + de manière justifiable

*/