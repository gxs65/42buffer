/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:15:51 by ilevy             #+#    #+#             */
/*   Updated: 2024/11/12 12:26:20 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void(*del)(void *))
{
	t_list	*new_list;
	t_list	*new_block;
	t_list	*current;

	new_list = NULL;
	current = NULL;
	if (!lst || !f || !del)
		return (NULL);
	while (lst)
	{
		new_block = ft_lstnew(f(lst->content));
		if (!new_block)
		{
			ft_lstclear(&new_list, del);
			return (NULL);
		}
		if (!new_list)
			new_list = new_block;
		else
			current->next = new_block;
		lst = lst->next;
		current = new_block;
	}
	return (new_list);
}
