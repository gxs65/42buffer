#include "../h_files/ft_minishell.h"

int ft_builtin1_exit(t_data *data, t_node *current)
{
	(void)current;
	rl_clear_history();
	ft_free_command(data);
	ft_free_exit(data);
	exit(0);
}