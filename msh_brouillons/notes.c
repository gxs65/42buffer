/*

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

- recherche d'un fichier exécutable selon le premier mot de la commande
	\ premier mot sans "/" : dans le PATH
	\ premier mot avec "/" en premier caractère : chemin absolu
	\ premier mot avec "/" à l'intérieur : chemin relatif
- recherche d'un fichier de redirection
	\ nom sans "/" : dans le répertoire courant
	\ nom avec "/" en premier caractère : chemin absolu
	\ nom avec "/" à l'intérieur : chemin relatif
- pattern-matching des expansions de "*"
	+ cas particuliers de "*" suivi de "/", et "./" suivi de "*"

- gestion des signaux, avec une seule variable globale
	\ que doivent faire exactement Ctrl+C et Ctrl+D ?
	\ doit-on se contenter de la gestion par défaut des signaux par les programmes en cours d'exécution,
		ou les kill (SIGKILL) en plus si on reçoit un Ctrl+C ?

- conversion de l'environnement initial en une liste chainee
	\ chaque élément de la liste chaînée à un bolléen indiquant s'il fait partie de l'env
	\ ajout de variables sans export <=> ajout d'éléments à la liste chaînée
	\ export et unset <=> modification du booléen dans la liste chaînée
	\ fonction transformant la liste chaînée en un tableau de chaînes de caracteres,
		pour pouvoir donner ce tableau en paramère à execve
- détection des commandes ou le premier mot contient un "=" :
	ce sont les commandes qui définissent la valeur d'une variable,
		donc erreur si la commande contient plus d'un mot (pas le comportement de Bash, mais plus logique ?)
- gestion de la variable spéciale $? = exit status de la dernière exécution
	-> documentation sur les valeurs des exit status, et comment les interpréter

- dans Parsing1, modifier la gestion des guillemets,
	pour que par exemple `"a"'b'c` soit considéré comme un seul token
- gestion du heredoc : tous les heredoc sont lus AVANT de commencer l'exécution,
	donc leur contenu doit être stocké dans un fichier temporaire,
		dont le nom remplace la string EOF dans le tableau <node.redir_words>

- passer en revue les cas particuliers de
https://docs.google.com/spreadsheets/d/1uJHQu0VPsjjBkR4hxOeCMEt3AOM1Hp_SmUzPFhAH-nA/edit#gid=0
	vérifier si on les gère sans segfault + sans fuite de mémoire + de manière justifiable

*/