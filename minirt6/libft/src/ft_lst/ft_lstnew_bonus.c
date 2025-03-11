/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 11:30:30 by ilevy             #+#    #+#             */
/*   Updated: 2024/11/08 12:20:51 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new;

	new = (t_list *)malloc(1 * sizeof(t_list));
	if (!new)
		return (0);
	new -> content = content;
	new -> next = NULL;
	return (new);
}

/*
int	main(int argc, char **argv)
{
	char	*content;

	content = "hello world";
	printf("%s\n", (char *)ft_lstnew(content)->content);
	return (0);
}
*/
