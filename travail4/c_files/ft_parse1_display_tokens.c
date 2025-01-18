#include "../h_files/ft_minishell.h"

void	ft_parse1_display_tokens(t_data *data)
{
	int		ind;
	char	*descriptors[] = {"(null)", "input", "heredoc",
							"trunc", "append", "leaf",
							"pipe", "and", "or", "par", "par", "equal"};

	ind = 0;
	while (data->tokens->name[ind])
	{
		ft_printf(LOGS, "[%d]: Value: %s Type: %s\n", ind,
			data->tokens->name[ind], descriptors[data->tokens->type[ind]]);
		ind++;
	}
}
