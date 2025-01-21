#include "../h_files/ft_minishell.h"

// Returns 1 if the character <c> is considered as whitespace
int	ft_isspace(char c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (1);
	else
		return (0);
}

// Displays strings in an array of size <len> 
void	ft_display_string_array(char **array, int len)
{
	int	ind;

	ind = 0;
	while (ind < len)
	{
		ft_printf(LOGS, "%s ", array[ind]);
		ind++;
	}
}
