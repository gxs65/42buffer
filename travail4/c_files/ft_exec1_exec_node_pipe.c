#include "../h_files/ft_minishell.h"

// Waits both children of the pipe, which are executing in parallel,
// and returns the exit_status of child_2
int	ft_exec1_pipe_final_wait(t_data *data, t_node *current,
	int *pipe_fds, int *pipe_pids)
{
	int	exit_status;

	close(pipe_fds[0]);
	close(pipe_fds[1]);
	waitpid(pipe_pids[0], &exit_status, 0);
	exit_status = ft_exec1_interpret_exit_status(exit_status);
	ft_printf(LOGSV, "[EXEC1 - MAIN %d] node type %d at %p - \
end of wait on processes for child_1, exit_status = %d\n",
		current->depth, current->type, current, exit_status);
	waitpid(pipe_pids[1], &exit_status, 0);
	exit_status = ft_exec1_interpret_exit_status(exit_status);
	ft_printf(LOGSV, "[EXEC1 - MAIN %d] node type %d at %p - \
end of wait on processes for child_2, exit_status = %d\n",
		current->depth, current->type, current, exit_status);
	return (exit_status);
}

// Forks a process for the child_2, and sends
// 		- the child process on recursion
// 		- the parent on the function that will wait both children
int	ft_exec1_pipe_fork_child2(t_data *data, t_node *current,
	int *pipe_fds, int *pipe_pids)
{
	int	ret;

	pipe_pids[1] = fork();
	if (pipe_pids[1] == -1)
		return (KILL_FORK_ERROR);
	else if (pipe_pids[1] == 0)
	{
		close(pipe_fds[1]);
		ret = ft_exec1_exectree_recurr(data, current->child_2,
			current->depth + 1);
		exit(ret);
	}
	else
	{
		ft_printf(LOGSV, "[EXEC1 - MAIN %d] node type %d at %p - \
successfully forked process for pipe child_2\n",
			current->depth, current->type, current);
		return (ft_exec1_pipe_final_wait(data, current, pipe_fds, pipe_pids));
	}
}

// Forks a process for the child_1, and sends
// 		- the child process on recursion
// 		- the parent on the function that will fork for child_2
int	ft_exec1_pipe_fork_child1(t_data *data, t_node *current,
	int *pipe_fds, int *pipe_pids)
{
	int	ret;

	pipe_pids[0] = fork();
	if (pipe_pids[0] == -1)
		return (KILL_FORK_ERROR);
	else if (pipe_pids[0] == 0)
	{
		close(pipe_fds[0]);
		ret = ft_exec1_exectree_recurr(data, current->child_1,
			current->depth + 1);
		exit(ret);
	}
	else
	{
		ft_printf(LOGSV, "[EXEC1 - MAIN %d] node type %d at %p - \
successfully forked process for pipe child_1\n",
			current->depth, current->type, current);
		return (ft_exec1_pipe_fork_child2(data, current, pipe_fds, pipe_pids));
	}
}

// When recursion tree comes to a PIPE node : 2 recursions
// The pipe transmission is created, and file descriptors of children
// are set adequately : child_1 sends to pipe, child_2 receives from pipe
// Then calls to functions that will accomplish the forks of child_1/2,
// 		and return the exit status of child_2
int	ft_exec1_exec_node_pipe(t_data *data, t_node *current, int depth)
{
	int		pipe_fds[2];
	pid_t	pipe_pids[2];

	if (pipe(pipe_fds))
		return (KILL_PIPE_ERROR);
	ft_printf(LOGSV, "[EXEC1 - MAIN %d] node type %d at %p - PIPE < %d > %d\n",
		depth, current->type, current, pipe_fds[0], pipe_fds[1]);
	current->child_1->fd_in = current->fd_in;
	current->child_1->fd_out = pipe_fds[1];
	current->child_2->fd_in = pipe_fds[0];
	current->child_2->fd_out = current->fd_out;
	ft_printf(LOGSV, "[EXEC1 - MAIN %d] node type %d at %p - \
child_1 at %p and child_2 at %p, now forking\n",
		depth, current->type, current, current->child_1, current->child_2);
	return (ft_exec1_pipe_fork_child1(data, current, pipe_fds, pipe_pids));
}
