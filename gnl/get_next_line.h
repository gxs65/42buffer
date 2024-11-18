/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   largest_square.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andrean <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:53:40 by andrean           #+#    #+#             */
/*   Updated: 2024/07/16 10:07:19 by andrean          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>

# define M_BUFFER_SIZE 64

typedef struct s_cstr
{
	char			data[M_BUFFER_SIZE + 1];
	struct s_cstr	*next;
}				t_cstr;

// Lecture ligne a ligne du fichier
t_cstr	*read_to_ln(int fd, int *total_to_ln);
char	*get_next_line(int fd);
t_cstr	*read_to_ln(int fd, int *total_to_ln);
void	cstr_to_str(t_cstr *cstr, char *str, char *rest);

// Utils
int		ft_strlen(char *str);
int		ft_char_in_str(char *str, char to_find);
void	ft_strncpy(char *dest, char *src, int n);
char	*ft_strdup(char *str);

// Allocation
t_cstr	*ft_create_cstr(char *data);
void	free_cstrs(t_cstr *cstr);

#endif
