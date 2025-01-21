#include "../h_files/ft_minishell.h"

// When recursion tree comes to a parenthesis node : 1 recursion
// Redirection is applied, potentially modifying <node.fd_in/out>,
// then file descriptors of the parenthesis node are passed down
// to the unique child, before recurring on that unique child 
int	ft_exec1_exec_node_par(t_data *data, t_node *current, int depth)
{
	int	ret;

	if (ft_exec2_apply_redirections(data, current))
		return (CMD_OPEN_ERROR);
	current->child_1->fd_in = current->fd_in;
	current->child_1->fd_out = current->fd_out;
	ft_printf(LOGSV, "[EXEC1 - MAIN %d] node type %d at %p - PAR recurring\n",
		depth, current->type, current, current->child_1);
	ret = ft_exec1_exectree_recurr(data, current->child_1, depth + 1);
	ft_exec2_close_redirections(data, current);
	return (ret);
}

// When recursion tree comes to a AND/OR node : 1 or 2 recursions
// File descriptors of the AND/OR node are passed down
// to both children nodes, before recurring on <child_1>,
// then the node may also recurr on <child_2>
// 		- OR node : if <child_1> failed (non-null exit status)
// 		- AND node : if <child_1> succeeded (null exit status)
// Return value is the exit status of last recurred child
int	ft_exec1_exec_node_logical(t_data *data, t_node *current, int depth)
{
	int	exit_status;

	current->child_1->fd_in = current->fd_in;
	current->child_1->fd_out = current->fd_out;
	current->child_2->fd_in = current->fd_in;
	current->child_2->fd_out = current->fd_out;
	ft_printf(LOGSV, "[EXEC1 - MAIN %d] node type %d at %p - \
OR/AND, child_1 at %p and child_2 at %p, now recurring\n",
		depth, current->type, current, current->child_1, current->child_2);
	exit_status = ft_exec1_exectree_recurr(data, current->child_1, depth + 1);
	ft_printf(LOGSV, "[EXEC1 - MAIN %d] node type %d at %p - \
received exit status %d on first part of AND/OR connection\n",
		depth, current->type, current);
	if ((exit_status == 0 && current->type == AND)
		|| (exit_status != 0 && current->type == OR))
	{
		exit_status = ft_exec1_exectree_recurr(data,
			current->child_2, depth + 1);
		ft_printf(LOGSV, "[EXEC1 - MAIN %d] node type %d at %p - \
received exit status %d on second part of AND/OR connection\n",
			depth, current->type, current);
	}
	return (exit_status);
}