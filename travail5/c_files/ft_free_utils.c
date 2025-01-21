#include "../h_files/ft_minishell.h"

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
	free(tab);
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
	free(tab);
	return (1);
}