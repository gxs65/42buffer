/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 11:54:06 by ilevy             #+#    #+#             */
/*   Updated: 2024/11/12 12:06:04 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	if (!lst || !new)
		return ;
	if (*lst == NULL)
		*lst = new;
	else
		ft_lstlast(*lst)->next = new;
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
	ft_lstadd_back(&lst, new);
	printf("%s\n%s\n", lst->content, ft_lstlast(lst)->content);
	return (0);
}
*/
