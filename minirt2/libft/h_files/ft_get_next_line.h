/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: administyrateur <administyrateur@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:35:08 by abedin            #+#    #+#             */
/*   Updated: 2025/01/27 14:23:17 by administyra      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_GET_NEXT_LINE_H
# define FT_GET_NEXT_LINE_H

# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 16
# endif

// Structure for a chained list of strings, used for :
// - storing the chunks of a line being read and not yet ended
// - storing the <prest> of each different file passed to <get_next_line>
// 		(only this part uses "fd", to identify the associated file)
typedef struct s_cstr
{
	char			data[BUFFER_SIZE + 1];
	struct s_cstr	*next;
	int				fd;
}				t_cstr;

// Lecture ligne a ligne du fichier
t_cstr	*read_to_ln(int fd, int *total_to_ln, char *buffer);
char	*get_next_line(int fd, int mode);
void	cstr_to_str(t_cstr *cstr, char *prest, char *ret, int pos_end);
int		manage_line_in_rest(char **ret, char *prest, int pos_ln);
int		manage_line_in_file(char **ret, char *prest, int fd, char *buffer);

// Utils
int		ft_strchrp_gnl(char *str, char to_find);
void	ft_strncpy(char *dest, char *src, int n);

// Allocation
t_cstr	*ft_create_cstr(char *data, int fd);
t_cstr	*free_cstrs(t_cstr *cstr, int fd);
t_cstr	*find_cstr_by_fd(t_cstr **prest, int fd);

#endif
