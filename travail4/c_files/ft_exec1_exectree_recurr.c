#include "../h_files/ft_minishell.h"

/* TODO : traduire en anglais, inclure les numeros exec1/2/3
- etapes de l'execution en suivant l'arbre construit
	1. si le noeud est un noeud LEAF ou PARENTHESIS, verifier s'il existe des redirections
		~ pour chaque direction (in / out), ouvrir le dernier fichier donne, et remplacer nos fdio par ces fd
		~ si plusieurs fichiers avaient ete donnes, ouvrir et refermer immediatement les fichiers qui ne sont pas dernier
		~ pour la redirection "out", en faisant attention a suivre le mode (TRUNC / APPEND)
	2. si le noeud est un noeud LEAF ou PARENTHESIS -> execution
		~ LEAF : fork un executeur (qui va dup2 pour remplacer les fd 0 et 1 par les eventuelles redirection, puis execve)
			ensuite attendre waitpid l'executeur, enregistrer son exit status et le renvoyer
		~ PARENTHESIS : assigner a child_1 les memes fdios que nous, et recurrer sur child_1 (et renvoyer son exit status)
	3. si le noeud est un noeud PIPE -> execution parallele
		~ creer un pipe classique
		~ assigner les fdios de child_1 et child_2
			. child_1 a fdin herite de noeud courant et fdout dans pipe
			. child_2 a fdin depuis pipe et fdout herite de noeud courant
		~ fork un process pour child1, qui va recurrer sur child_1
		  fork un process pour child2, qui va recurrer sur child_2
		~ attendre les deux enfants, renvoyer le exit_status de child_2
	4. si le noeud est un noeud OR ou AND -> execution sequentielle
		~ assigner les fdios de child_1 et child_2, simplement herites du noeud courant
		~ recurrer sur child_1, recuperer la valeur de retour (qui est le exit status)
		~ selon la valeur de retour, eventuellement recurrer sur child_2
		~ renvoyer le exit_status de child_2
*/

// Uses the dedicated macros to interpret processes' exit status
// which can be either
// 		- an exit status from normal <exit> or <return> call in forked process
// 			(either before execve 125/126/127, or after execve for <125)
// 		- a signal indicator when the process was terminated by a signal
// 		- something unidentified but definitely non-0 (an error)
int	ft_exec1_interpret_exit_status(int exit_status)
{
	if (WIFEXITED(exit_status))
		exit_status = WEXITSTATUS(exit_status);
	else if (WIFSIGNALED(exit_status))
		exit_status = 128;
	else
		exit_status = 1;
}

// Recursively goes through the nodes of the command tree,
// 		by selecting, ccording to node type, the adequate function,
// 			which either recur (OPEN/PIPE/LOGICAL) or end recursion (CMD/EQUAL)
// /!\ Only PIPE nodes will induce parallel execution of commands through forks
// 		(CMD nodes also use <fork>, but wait for the created child
// 		 immediately after <fork> call)
// /!\ Assignation mode may produce an error : a malloc error
// 		when creating a new node in the variables linked list
int	ft_exec1_exectree_recurr(t_data *data, t_node *current, int depth)
{
	ft_printf(LOGS, "[EXEC1 - MAIN %d] node type %d at %p BEGIN\n",
		depth, current->type, current);
	current->depth = depth;
	if (current->type == EQUAL)
		return (ft_exec4_var_assign(data, current));
	if (current->type == CMD)
		return (ft_exec1_exec_node_leaf(data, current, depth));
	else if (current->type == OPEN)
		return (ft_exec1_exec_node_par(data, current, depth));
	else if (current->type == PIPE)
		return (ft_exec1_exec_node_pipe(data, current, depth));
	else
		return (ft_exec1_exec_node_logical(data, current, depth));
}
