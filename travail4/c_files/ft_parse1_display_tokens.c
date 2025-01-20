#include "../h_files/ft_minishell.h"

// Displays the result of line tokenisation
void	ft_parse1_display_tokens(t_data *data)
{
	int		ind;
	char	*descriptors[] = {"(null)", "input", "heredoc",
							"trunc", "append", "cmd",
							"pipe", "and", "or", "par", "par", "equal"};

	ind = 0;
	while (data->tokens->name[ind])
	{
		ft_printf(LOGS, "[PARSE1] token %d: Value %s Type %s\n", ind,
			data->tokens->name[ind], descriptors[data->tokens->type[ind]]);
		ind++;
	}
}
