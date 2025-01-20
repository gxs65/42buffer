#include "../h_files/ft_minishell.h"

// Searches for the executable file of command <basename>
// in the directories defined in <data.path_dirs>
// and stores the resulting filename in buffer <filename>
int	ft_exec3_find_execfile(t_data *data, char *basename, char *filename)
{
	int		ind;

	ind = 0;
	while (data->path_dirs[ind])
	{
		filename[0] = '\0';
		ft_strcat(filename, data->path_dirs[ind]);
		ft_strcat(filename, "/");
		ft_strcat(filename, basename); // TODO : utiliser strlcat, pour eviter de depasser PATH_MAX_LEN
		if (access(filename, F_OK) == 0)
		{
			if (access(filename, X_OK) == 0)
				return (0);
		}
		ind++;
	}
	ft_printf(LOGSV, "[EXEC3] no execfile for cmd basename %s\n", basename);
	ft_printf(2, "No executable file found for '%s'\n", basename);
	return (CMD_NOEXEC_ERROR); // TODO : dans l'incorporation des chemins relatifs/absolus, mep cas d'erreur 126
}

// Function called by child process created when a leaf node forks,
// 		so the child process is being waited by its leaf node parent,
// 			and its sole purpose is to transmogrify into another program
// 			thanks to <execve>
// To set up the transmogrification :
// 		1. determine the executable file for the command
// 			(if none is found, exits with code 127 or 126)
// 		2. change the process' stdin and stdout with <dup>
// 			according to the node's <fdin> and <fdout>
// 		3. call execve and exit with adequate code if it fails
void	ft_exec3_execcmd(t_data *data, t_node *current)
{
	char	exec_file[PATH_MAX_LEN];
	int		ret;

	ret = ft_exec3_find_execfile(data, current->cmd_words[0], exec_file);
	if (ret)
		exit(ret);
	free(current->cmd_words[0]);
	current->cmd_words[0] = ft_strdup(exec_file);
	if (!(current->cmd_words[0]))
		exit(CMD_SYSCALL_ERROR);
	ft_printf(LOGSV, "[EXEC3] node type %d at %p - found execfile named %s\n",
		current->type, current, exec_file);
	if (current->fd_in != 0)
		dup2(current->fd_in, 0);
	if (current->fd_out != 1)
		dup2(current->fd_out, 1);
	if (execve(current->cmd_words[0], current->cmd_words, NULL) == -1)
		exit(CMD_SYSCALL_ERROR);
}
