#include "../h_files/ft_minishell.h"

// Checks if there exists a shell variables with name "PATH"
// (not necessarily a variable belonging to env :
//  bash takes into account a PATH even if it was not exported)
// and splits it into directories, to allow later search for binaries
// /!\ This function is called for each new line handled by Minishell,
// 		since the value of PATH may change
int	ft_init3_det_path(t_data *data)
{
	t_var	*path_var;

	if (data->path_dirs)
		free_strs_tab_nullterm(data->path_dirs);
	data->path_dirs = NULL;
	path_var = ft_var_find_by_name(data->env, "PATH");
	if (!path_var)
		data->path_dirs = ft_split("", ':');
	else
		data->path_dirs = ft_split(path_var->value, ':');
	if (!(data->path_dirs))
		return (KILL_MALLOC_ERROR);
	return (0);
}
