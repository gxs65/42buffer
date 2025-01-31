/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 12:26:21 by ilevy             #+#    #+#             */
/*   Updated: 2024/11/08 12:50:28 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!lst || !new)
		return ;
	new->next = *lst;
	*lst = new;
}

/*
int	main(int argc, char **argv)
{
	t_list	*lst;
	t_list	*new;

	new = (t_list *)malloc(sizeof(t_list));
	lst = (t_list *)malloc(sizeof(t_list));
	new->content = argv[1];
	lst->content = argv[2];
	new->next = NULL;
	lst->next = NULL;
	ft_lstadd_front(&lst, new);
	printf("%s\n%s\n", lst->content, ft_lstlast(lst)->content);
	return (0);
}
*/
