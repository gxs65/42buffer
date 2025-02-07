/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 11:49:02 by ilevy             #+#    #+#             */
/*   Updated: 2024/11/12 12:04:13 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

/*
int	main(int argc, char **argv)
{
	t_list	*lst;

	lst = (t_list *)malloc(1 * sizeof(t_list));
	lst[0].content = NULL;
	lst[0].next = NULL;
	printf("%c\n", ft_lstlast(lst)->content);
	return (0);
}
*/
