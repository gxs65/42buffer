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
		ft_strcat(filename, basename);
		if (access(filename, F_OK) == 0 && access(filename, X_OK) == 0)
			return (0);
		ind++;
	}
	return (1);
}

void	ft_exec3_execcmd(t_data *data, t_node *current)
{
	char	exec_file[200]; // TODO : trouver une methode plus elegante de connaitre le nom de l'executable,
							// peut-etre que <ft_exec3_find_execfile> peut renvoyer un pointeur vers une string

	if (ft_exec3_find_execfile(data, current->cmd_words[0], exec_file))
	{
		//ft_printf(LOGSV, "[EXEC1 - EXECUTOR] node type %d at %p - no executable file found for cmd %s\n", current->type, current, current->cmd_words[0]);
		exit(1);
	}
	free(current->cmd_words[0]);
	current->cmd_words[0] = ft_strdup(exec_file);
	if (!(current->cmd_words[0]))
		exit(1);
	//ft_printf(LOGSV, "[EXEC1 - EXECUTOR] node type %d at %p - found executable named %s\n", current->type, current, exec_file);

	if (current->fd_in != 0)
		dup2(current->fd_in, 0);
	if (current->fd_out != 1)
		dup2(current->fd_out, 1);

	if (execve(current->cmd_words[0], current->cmd_words, NULL) == -1)
		exit(1);
}
