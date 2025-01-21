#include "../h_files/ft_minishell.h"

// Displays the result of line tokenisation
void	ft_parse1_display_tokens(t_data *data)
{
	int		ind;
	char	*descriptors[] = {"(null)", "input", "heredoc",
							"trunc", "append", "cmd",
							"pipe", "and", "or", "par", "par", "equal"};

	ft_printf(LOGS, "[PARSE1] Tokenisation finished\n");
	ind = 0;
	while (data->tokens->name[ind])
	{
		ft_printf(LOGS, "         token %d: Type %-12s Value %s\n", ind,
			descriptors[data->tokens->type[ind]], data->tokens->name[ind]);
		ind++;
	}
}
