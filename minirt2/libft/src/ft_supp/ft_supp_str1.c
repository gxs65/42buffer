/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_supp_str1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 16:21:02 by abedin            #+#    #+#             */
/*   Updated: 2025/01/29 16:21:03 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

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

// Searches for a character in the given string, skipping characters in quotes
// 		- if <level> is 0, no characters are skipped
// 		- if <level> is 1, chars enclosed in single quotes skipped
// 						   chars enclosed in double quotes NOT skipped
// 		- if <level> is 2, chars enclosed in single AND double quotes skipped
// Also skips all characters up to and not including index <from>
// 		(the character at <from> is not skipped)
int	ft_strchrp_nonquoted(char *str, char to_find, int level, int from)
{
	char	quoted;
	int		pos;

	if (!str)
		return (-1);
	quoted = ' ';
	pos = 0;
	while (str[pos])
	{
		if (quoted != ' ' && quoted == str[pos])
			quoted = ' ';
		else if (quoted == ' '
			&& ((str[pos] == '\'') || (str[pos] == '\"')))
			quoted = str[pos];
		else if ((quoted == ' '
				|| (quoted == '\"' && level <= 1)
				|| (quoted == '\'' && level <= 0))
			&& pos >= from && str[pos] == to_find)
			return (pos);
		pos++;
	}
	if (to_find == '\0')
		return (pos);
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

// Copies <src> to the end of <dest>, max <n> characters copied
char	*ft_strncat(char *dest, const char *src, int n)
{
	int		ind_src;
	int		ind_dest;

	ind_dest = 0;
	while (dest[ind_dest])
		ind_dest++;
	ind_src = 0;
	while (ind_src < n && src[ind_src])
	{
		dest[ind_dest + ind_src] = src[ind_src];
		ind_src++;
	}
	dest[ind_dest + ind_src] = '\0';
	return (dest);
}

// Compares to string, without length limit
int	ft_strcmp(char *s1, char *s2)
{
	int	ind;

	ind = 0;
	while (s1[ind] && s2[ind] && s1[ind] == s2[ind])
		ind++;
	return (s1[ind] - s2[ind]);
}
