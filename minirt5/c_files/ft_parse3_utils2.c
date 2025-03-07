#include "../h_files/ft_minishell.h"

// Returns 1 if the character <c> is considered as whitespace
int	ft_isspace(char c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (1);
	else
		return (0);
}

// Frees an array of strings, up to and not including index <ind>
// Always returns 1
int	free_strs_tab_upto(char **tab, int lim)
{
	int ind;

	ind = 0;
	while (ind < lim)
	{
		free(tab[ind]);
		ind++;
	}
	return (1);
}

// Frees everything in a null-terminated array of strings
// Always returns 1
int	free_strs_tab_nullterm(char **tab)
{
	int ind;

	ind = 0;
	while (tab[ind])
	{
		free(tab[ind]);
		ind++;
	}
	return (1);
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
