#include "../h_files/ft_minishell.h"

// Frees Minishell's long-term variables before the program exits
int	ft_free_exit(t_data *data)
{
	ft_var_list_clear(data->env);
	return (1);
}

// Frees medium term variables, that were allocated to handle 1 commande line
int	ft_free_command(t_data *data)
{
	ft_printf(LOGSV, "[FREE] Freeing command line\n");
	ft_printf(LOGSV, "[FREE] Freeing path directories\n");
	if (data->path_dirs)
	{
		free_strs_tab_nullterm(data->path_dirs);
		data->path_dirs = NULL;
	}
	ft_printf(LOGSV, "[FREE] Freeing token arrays\n");
	if (data->nb_tokens > 0)
	{
		free_strs_tab_nullterm(data->tokens->name);
		free(data->tokens->type);
		data->nb_tokens = 0;
	}
	ft_printf(LOGSV, "[FREE] Freeing command tree\n");
	if (data->root)
	{
		ft_free_command_tree_recurr(data, data->root, 0);
		data->root = NULL;
	}
	return (0);
}

// Frees a node's array containing indexes of cmd and redir tokens
void	ft_free_command_tree_inds(t_node *current)
{
	if (current->nb_cmd_tokens > 0)
	{
		free(current->cmd_tokens_inds);
		current->nb_cmd_tokens = 0;
	}
	if (current->nb_redir_tokens > 0)
	{
		free(current->redir_tokens_inds);
		current->nb_redir_tokens = 0;
	}
}

// Recursive function to free all nodes in the command tree,
// and their string arrays according to the node's step of allocation
// 		- step 1 : arrays containing the indexes of the node's token
// 				   in the command line, allocated only if there are >0 tokens
// 		- step 2+3 : arrays containing cmd and redir words,
// 				   always allocated as NULL-terminated arrays
// 						(each non-NULL word being an allocated string itself)
void	ft_free_command_tree_recurr(t_data *data, t_node *current, int depth)
{
	if (current->child_1)
		ft_free_command_tree_recurr(data, current->child_1, depth + 1);
	if (current->child_2)
		ft_free_command_tree_recurr(data, current->child_2, depth + 1);
	ft_printf(LOGSV, "[FREE - RECURR] On node type %d at %p\n",
		current->type, current);
	ft_printf(LOGSV, "                Freeing indexes arrays\n");
	if (current->alloc_step >= 1)
		ft_free_command_tree_inds(current);
	ft_printf(LOGSV, "                Freeing cmd words\n");
	if (current->alloc_step >= 2)
		free_strs_tab_upto(current->cmd_words, current->nb_cmd_words);
	ft_printf(LOGSV, "                Freeing redir words\n");
	if (current->alloc_step >= 3)
		free_strs_tab_upto(current->redir_words, current->nb_redir_words);
	free(current);
}

/*
- variables malloc LONG-TERME (free a la fin de minishell)
	+ <var>
	  <var.value> + <var.name>						de Init1 recuperation de l'env, et Exec4 assignation de variables shell
- variables malloc MOYEN-TERME (free a la fin de la gestion d'une ligne)
	+ <data.path_dirs>								de Init1 recuperation du path
	+ <data.tokens.name> + <data.tokens.type>		de Parse1 tokenisation
	\ <node>										de Parse2 construction de l'arbre
	\ <node.redir_tokens_inds>
	  <node.cmd_tokens_inds>						de Parse2 construction de l'arbre
	\ <node.redir_words>
	  <node.cmd_words>								de Parse3 resplit, remplace dans Parse3 expansion des pathnames
  	\ tous les duplicats issus des forks devant execve mais dont execve echoue / des forks ne devant pas execve (acd pipe)
-  variables COURT-TERME (free dans la fonction qui l'a allouee, ou pas si arrive a execve)
	\ <execfile> (char *)							de Exec3 recuperation du nom de fichier executable
	\ <envp> (char **)								de Exec3 reconstitution de l'env pour le passer a execve
	+ <value> (char *)								de Exec4 valeur de la variable shell
	+ <line>										de readline
*/