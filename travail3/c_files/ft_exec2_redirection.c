#include "../h_files/ft_minishell.h"

int	ft_exec1_apply_redirections(t_data *data, t_node *current)
{
	int	ind;
	int	type;

	ft_printf(LOGSV, "[EXEC1 - MAIN] node type %d at %p - applying redirections\n", current->type, current);
	ind = 0;
	while (ind < current->nb_redir_words)
	{
		type = data->tokens->type[current->redir_tokens_inds[ind * 2]];
		if (type == INPUT)
		{
			if (current->fd_in > 2) // TODO : not sure about what exactly to close at this point
				close(current->fd_in);
			current->fd_in = open(current->redir_words[ind], O_RDONLY);
			ft_printf(LOGSV, "[EXEC1 - MAIN] node type %d at %p - \tinput is now from fd %d\n", current->type, current, current->fd_in);
			if (current->fd_in == -1)
			{
				ft_printf(LOGS, "! error when opening file for input redirection\n");
				return (1);
			}
		}
		if (type == APPEND || type == TRUNC)
		{
			if (current->fd_out > 2) // TODO : not sure about what exactly to close at this point
				close(current->fd_out);
			if (type == APPEND)
				current->fd_out = open(current->redir_words[ind], O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
			else
				current->fd_out = open(current->redir_words[ind], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
			ft_printf(LOGSV, "[EXEC1 - MAIN] node type %d at %p - \toutput is now to fd %d\n", current->type, current, current->fd_out);
			if (current->fd_out == -1)
			{
				ft_printf(LOGS, "! error when opening file for output redirection\n");
				return (1);
			}
		}
		ind++;
	}
	return (0);
}