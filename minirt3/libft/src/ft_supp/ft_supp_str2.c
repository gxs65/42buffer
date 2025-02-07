/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_supp_str2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 16:21:06 by abedin            #+#    #+#             */
/*   Updated: 2025/01/29 16:21:07 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

char	*ft_strecat(char *dest_end, char *src)
{
	int	pos_src;

	pos_src = 0;
	while (src[pos_src] != '\0')
	{
		dest_end[pos_src] = src[pos_src];
		pos_src++;
	}
	dest_end[pos_src] = '\0';
	return (dest_end + pos_src);
}

int	ft_strjoin_calc_size(int size, char **strs, char *sep)
{
	int	size_res;
	int	pos;

	size_res = 0;
	pos = 0;
	while (pos < size)
	{
		size_res += ft_strlen(strs[pos]);
		pos++;
	}
	size_res = size_res + (size - 1) * ft_strlen(sep) + 1;
	return (size_res);
}

// Allocates a string long enough to hold all strings in <strs>
// with <sep> as separator,
// 		then concatenates them in the new string and returns the result
char	*ft_strjoin_str_array(int size, char **strs, char *sep)
{
	int		pos;
	int		size_res;
	char	*joined_str;
	char	*end_ptr;

	if (size <= 0)
	{
		joined_str = malloc(1 * sizeof(char));
		joined_str[0] = '\0';
		return (joined_str);
	}
	size_res = ft_strjoin_calc_size(size, strs, sep);
	pos = 0;
	joined_str = malloc(size_res * sizeof(char));
	end_ptr = joined_str;
	while (pos < size - 1)
	{
		end_ptr = ft_strecat(end_ptr, strs[pos]);
		end_ptr = ft_strecat(end_ptr, sep);
		pos++;
	}
	end_ptr = ft_strecat(end_ptr, strs[pos]);
	return (joined_str);
}
