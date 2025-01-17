#include "../h_files/ft_minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	ind;

	ind = 0;
	while (s1[ind] && s2[ind] && s1[ind] == s2[ind])
		ind++;
	return (s1[ind] - s2[ind]);
}
