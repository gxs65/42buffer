#include "../h_files/ft_minishell.h"

// Searches for a character in the given string
// Does not return a pointer to the first occurrence of <to_find> like strchr,
//		but the index (int) of that character (or -1 if not found)
// + processes the terminating null byte
int	ft_strchrp(char *str, char to_find)
{
	int	pos;

	if (str)
	{
		pos = 0;
		while (str[pos])
		{
			if (str[pos] == to_find)
				return (pos);
			pos++;
		}
		if (to_find == '\0')
			return (pos);
	}
	return (-1);
}

// Duplicates <n> characters from the given string, using <malloc>
char	*ft_strndup(const char *src, int n)
{
	char	*dup;
	int		len_src;
	int		pos;

	len_src = 0;
	while (src[len_src] != '\0')
		len_src++;
	if (len_src < n)
		n = len_src;
	dup = malloc((n + 1) * sizeof(char));
	if (!dup)
		return ((char *) 0);
	pos = 0;
	while (pos < n)
	{
		dup[pos] = src[pos];
		pos++;
	}
	dup[n] = '\0';
	return (dup);
}