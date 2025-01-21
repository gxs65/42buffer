#include "../h_files/ft_minishell.h"

// Waits for the child executor,
// then closes potential redirections that were opened at this recursion level,
// and returns the executor's exit status
int	ft_exec1_leaf_final_wait(t_data *data, t_node *current,
	pid_t pid, int depth)
{
	int	exit_status;

	ft_printf(LOGSV, "[EXEC1 - MAIN %d] node type %d at %p - \
exec process forked\n", depth, current->type, current);
	waitpid(pid, &exit_status, 0);
	exit_status = ft_exec1_interpret_exit_status(exit_status);
	ft_printf(LOGSV, "[EXEC1 - MAIN %d] node type %d at %p - \
end of wait on exec process, exit status = %d\n",
			depth, current->type, current, exit_status);
	ft_exec2_close_redirections(data, current);
	return (exit_status);
}

// Checks if a given command name is a minishell builtin
int	ft_exec1_is_builtin(char *name)
{
	if (ft_strncmp(name, "exit", 5) == 0)
		return (1);
	return (0);
}

// Calls a minishell builtin function for the current node's command
int	ft_exec1_builtin(t_data *data, t_node *current)
{
	ft_printf(LOGS, "[EXEC1] builtin detected : %s\n", current->cmd_words[0]);
	if (ft_strncmp(current->cmd_words[0], "exit", 5) == 0)
	{
		return (ft_builtin1_exit(data, current));
	}
	return (0); // TODO : other builtins
}

// When recursion tree comes to a leaf (command) node : ends recursion
// Redirections are applied, then process is forked to get
// 		- child = an "executor" that will transform into the cmd with <execve>
// 			(the line "return (0) after call to <ft_exec3_execcmd>
// 			 will never be reached, since <ft_exec3_execcmd> always terminates
// 			 the child process' execution, either by <execve> or <exit>)
// 		- parent = the main process that will wait on the child executor
// 			to return the child's exit status
// /!\ if a leaf node has no cmd words (which could happen after expansion),
// 	   it should still open its redirection files, before exiting as success
int	ft_exec1_exec_node_leaf(t_data *data, t_node *current, int depth)
{
	pid_t	pid;
	int		builtin_ret;

	if (ft_exec2_apply_redirections(data, current))
		return (CMD_OPEN_ERROR);
	if (current->nb_cmd_words == 0)
	{
		ft_exec2_close_redirections(data, current);
		return (0);
	}
	if (ft_exec1_is_builtin(current->cmd_words[0]))
	{
		builtin_ret = ft_exec1_builtin(data, current);
		ft_exec2_close_redirections(data, current);
		return (builtin_ret);
	}
	pid = fork();
	if (pid == -1)
	{
		ft_exec2_close_redirections(data, current);
		return (KILL_FORK_ERROR);
	}
	else if (pid == 0)
	{
		ft_exec3_execcmd(data, current);
		return (0);
	}
	else
		return (ft_exec1_leaf_final_wait(data, current, pid, depth));
}