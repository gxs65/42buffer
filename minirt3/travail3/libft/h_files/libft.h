/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 03:46:33 by ilevy             #+#    #+#             */
/*   Updated: 2025/01/29 16:20:26 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include "ft_printf.h"
# include "ft_get_next_line.h"

# define LOGS 1

typedef unsigned int t_ui;

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}		t_list;

void	ft_striteri(char *s, void (*f)(unsigned int, char *));
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstadd_front(t_list **lst, t_list *new);
void	ft_lstiter(t_list *lst, void (*f)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_bzero(void *s, size_t n);
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	*ft_memset(void *b, int c, size_t n);
void	*ft_memchr(const void *s, int c, size_t n);
void	*ft_calloc(size_t count, size_t size);

int		ft_strlcpy(char *dest, const char *src, unsigned int size);
int		ft_strlcat(char *dest, const char *src, unsigned int size);
int		ft_isalnum(int c);
int		ft_isalpha(int c);
int		ft_isascii(int c);
int		ft_isdigit(int c);
int		ft_atoi(const char *nptr);
int		ft_isprint(int c);
int		ft_iswhitespace(int c);
int		ft_memcmp(const void *str1, const void *str2, size_t n);
int		ft_memcmp(const void *str1, const void *str2, size_t n);
int		ft_lstsize(t_list *lst);
int		ft_toupper(int c);
int		ft_tolower(int c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);

char	*ft_strnstr(const char *big, const char *little, unsigned int len);
char	*ft_strchr(const char *s1, int character);
char	*ft_strrchr(const char *string, int character);
char	*ft_substr(const char *s, unsigned int start, size_t len);
char	*ft_strdup(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(const char *s1, const char *set);
char	*ft_itoa(int n);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char	**ft_split(char const *s, char c);
size_t	ft_strlen(const char *str);

t_list	*ft_lstlast(t_list *lst);
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
t_list	*ft_lstnew(void *content);

t_ui	to_base10(int *digits, int size, t_ui base);
int		from_base10(int *digits, int size, t_ui n, t_ui base);
void	fill_tab_with(int *tab, int size, int val);
int		ind_max_in_tab(int *tab, int size);
int		ft_free_strs_tab_upto(char **tab, int lim);
int		ft_free_strs_tab_nullterm(char **tab);
int		ft_isspace(char c);
void	ft_itoa_in_recurr(int n, char *s, int ind);
void	ft_itoa_in(int n, char *s);
void	ft_display_string_array(char **array, int len);
void	ft_display_int_array(char **array, int len);
int		ft_strchrp(char *str, char to_find);
int		ft_strchrp_nonquoted(char *str, char to_find, int level, int from);
char	*ft_strndup(const char *src, int n);
char	*ft_strncat(char *dest, const char *src, int n);
int		ft_strcmp(char *s1, char *s2);

#endif
