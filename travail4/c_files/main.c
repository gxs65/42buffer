#include "../h_files/ft_minishell.h"

void	ft_log_error(int errcode)
{
	if (errcode == KILL_MALLOC_ERROR)
		ft_printf(2, "Error : malloc call\n");
	else if (errcode == KILL_SIGACTION_ERROR)
		ft_printf(2, "Error : sigaction call\n");
	else if (errcode == KILL_OPENCWD_ERROR)
		ft_printf(2, "Error : opening current working directory\n");
	else if (errcode == KILL_ENVFORMAT_ERROR)
		ft_printf(2, "Error : badly formatted env\n");
	else if (errcode == KILL_FORK_ERROR)
		ft_printf(2, "Error : fork call\n");
	else if (errcode == KILL_PIPE_ERROR)
		ft_printf(2, "Error : pipe call\n");
	else if (errcode == STOP_SYNTAX_ERROR)
		ft_printf(2, "Error : command line syntax\n");
	else if (errcode == STOP_OPEN_ERROR)
		ft_printf(2, "Error : opening files for redirection\n");
	else if (errcode == CMD_NOEXEC_ERROR)
		ft_printf(2, "Cmd spec error : no executable found\n");
	else if (errcode == CMD_NOXRIGHT_ERROR)
		ft_printf(2, "Cmd spec error : executable lacks exec right\n");
	else if (errcode == CMD_SYSCALL_ERROR)
		ft_printf(2, "Cmd spec error : system call during execve preparation\n");
	else if (errcode >= 128)
		ft_printf(2, "Cmd spec error : terminated by signal\n");
	else if (errcode != 0)
		ft_printf(2, "Error : unidentified\n");
}

int	ft_is_fatal_error(int errcode)
{
	ft_log_error(errcode);
	if (errcode > 0 && errcode < 32)
		return (1);
	else
		return (0);
}

int	ft_handle_one_line(t_data *data)
{
	int	ret;

	ret = ft_init3_det_path(data); // potential errors : malloc
	if (ft_is_fatal_error(ret))
		return (ret);
	else if (ret)
		return (0);

	ret = ft_parse1_tokenize(data); // potential errors : malloc
	if (ft_is_fatal_error(ret))
		return (ret);
	else if (ret)
		return (0);
	ft_parse1_display_tokens(data);
	
	ret = ft_parse2_build_tree(data); // potential errors : malloc
	if (ft_is_fatal_error(ret))
		return (ret);
	else if (ret)
		return (0);
	ft_parse2_display_cmd_tree(data, data->root, 0);

	ret = ft_parse3_expand(data); // potential errors : malloc, opendir
	if (ft_is_fatal_error(ret))
		return (ret);
	else if (ret)
		return (0);
	ft_parse3_display_cmd_tree(data, data->root, 0);
				
	ret = ft_exec1_exectree_recurr(data, data->root, 0);
	if (ft_is_fatal_error(ret)) // TODO : gestion particuliere car la valeur de retour de exectree
		return (ret);			// 		  est aussi le exit status du dernier process
	else if (ret)
		return (0);
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_data	data;
	t_token	tokens;
	int		ret;

	if (ac > 1)
		return (1);
	(void)av;
	data.tokens = &tokens;
	if (ft_init1_retrieve_env(&data, envp)) // potential errors : malloc
		return (KILL_MALLOC_ERROR);
	if (ft_init2_setup_sighandling(&data)) // potential errors : sigaction
		return (KILL_SIGACTION_ERROR);
	data.path_dirs = NULL;
	while (1)
	{
		ret = ft_handle_one_line(&data);
		if (ret)
		{
			ft_printf(2, "Fatal error, exiting\n");
			return (ret);
		}
	}
	return (0);
}
