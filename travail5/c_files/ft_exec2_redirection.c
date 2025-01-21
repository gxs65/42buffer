#include "../h_files/ft_minishell.h"

void	ft_exec2_close_redirections(t_data *data, t_node *current)
{
	if (current->fd_in != current->fd_in_orig)
	{
		close(current->fd_in);
		current->fd_in = current->fd_in_orig;
	}
	if (current->fd_out != current->fd_out_orig)
	{
		close(current->fd_out);
		current->fd_out = current->fd_out_orig;
	}
}

// Opens the file given by input redir token of index <ind>
// and replaces the current input fd by the new file's fd
// /!\ different opening flags according to redir type (append/truncate)
// /!\ files opened by this node (<=> at this recursion level),
//     but in fact replaced by a later file, should be closed
// 			BUT not files opened at a higher recursion level,
//			hence the condition : `current->fd_in != current->fd_out_in`
int	ft_exec2_replace_fdin(t_data *data, t_node *current, int ind)
{
	if (current->fd_in != current->fd_in_orig) // TODO : be certain what to close there
		close(current->fd_in);
	current->fd_in = open(current->redir_words[ind], O_RDONLY);
	if (current->fd_in == -1)
	{
		ft_printf(2, "%s: could not open file\n", current->redir_words[ind]);
		return (1);
	}
	ft_printf(LOGSV, "[EXEC2] node type %d at %p - \tinputs now from fd %d\n",
		current->type, current, current->fd_in);
	return (0);
}

// Opens the file given by output redir token of index <ind>
// and replaces the current output fd by the new file's fd
// /!\ different opening flags according to redir type (append/truncate)
// /!\ files opened by this node (<=> at this recursion level),
//     but in fact replaced by a later file, should be closed
// 			BUT not files opened at a higher recursion level,
//			hence the condition : `current->fd_out != current->fd_out_orig`
int	ft_exec2_replace_fdout(t_data *data, t_node *current, int ind)
{
	int	type;

	type = data->tokens->type[current->redir_tokens_inds[ind * 2]];
	if (current->fd_out != current->fd_out_orig) // TODO : be certain what to close there
		close(current->fd_out);
	if (type == APPEND)
		current->fd_out = open(current->redir_words[ind],
			O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
	else
		current->fd_out = open(current->redir_words[ind],
			O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (current->fd_out == -1)
	{
		ft_printf(2, "%s: could not open file\n", current->redir_words[ind]);
		return (1);
	}
	ft_printf(LOGSV, "[EXEC2] node type %d at %p - \toutputs now to fd %d\n",
		current->type, current, current->fd_out);
	return (0);
}

// Goes through the node's redirection tokens to open new files if needed
// By default, a node's in/out fds were passed down to it by its parent
// (with root node having default values 0 and 1)
// 		and are initially given in <node.fd_in> and <node.fd_out> ;
// 			but redirection tokens may override this inheritance
// 		so the inherited values are stored in <node.fd_in/out_orig>
// 			and each new redir token met in the loop replaces the last one
// If there are more than 1 redir token of a type (in/out),
// 		only the last file serves as input, but other files are still opened
int	ft_exec2_apply_redirections(t_data *data, t_node *current)
{
	int	ind;
	int	type;

	ft_printf(LOGSV, "[EXEC2] node type %d at %p - applying redirections\n",
		current->type, current);
	current->fd_in_orig = current->fd_in;
	current->fd_out_orig = current->fd_out;
	ind = 0;
	while (ind < current->nb_redir_words)
	{
		type = data->tokens->type[current->redir_tokens_inds[ind * 2]];
		if (type == INPUT && ft_exec2_replace_fdin(data, current, ind))
			return (1);
		if ((type == APPEND || type == TRUNC)
			&& ft_exec2_replace_fdout(data, current, ind))
			return (1);
		ind++;
	}
	ft_printf(LOGSV, "[EXEC2] node type %d at %p - \
after redirections, input from %d and output to %d\n",
		current->type, current, current->fd_in, current->fd_out);
	return (0);
}