/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: administyrateur <administyrateur@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:19:39 by ilevy             #+#    #+#             */
/*   Updated: 2025/01/12 14:55:05 by administyra      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MINISHELL_H
# define FT_MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "../libft/h_files/libft.h"

# define INPUT		1 // <
# define HEREDOC	2 // <<
# define TRUNC		3 // >
# define APPEND		4 // >>
# define CMD		5 // string containing command + Arg + option
# define PIPE		6 // |
# define AND		7 // &&
# define OR			8 // ||
# define OPEN		9 // (
# define CLOSE		10 // )

# define MALLOC_ERROR 4242
# define CMD_ERROR 8484

# define LOGS 1
# define LOGSV -1

typedef struct s_token
{
	char	**name;
	int		*type;
}				t_token;

typedef struct s_node 
{
	int				type;
	struct s_node	*back;
	struct s_node	*child_1;
	struct s_node 	*child_2;

	int				*cmd_tokens_inds;
	size_t			nb_cmd_tokens;
	int				*redir_tokens_inds;
	size_t			nb_redir_tokens;
	
	char			**cmd_words;
	size_t			nb_cmd_words;

	int				ind_start_token;
	int				ind_end_token;
}				t_node;

typedef struct s_data
{
	char	**env;

	t_token	*tokens;
	int		nb_tokens;

	int		filefd_in;
	int		filefd_out;
	
	t_node	*root;
	t_node	*current;
}				t_data;

// PARSE2
void	ft_parse2_display_cmd_tree(t_data *data, t_node *current, int depth);

int		ft_parse2_tokens_ind_par_retrieve(t_data *data, t_node *current, int opar, int cpar);
int		ft_parse2_tokens_ind_par(t_data *data, t_node *current, int opar, int cpar);
int		ft_parse2_tokens_ind_leaf_count(t_data *data, t_node *current);
int		ft_parse2_tokens_ind_leaf_retrieve(t_data *data, t_node *current);
int		ft_parse2_tokens_ind_leaf(t_data *data, t_node *current);

int		ft_parse2_build_tree(t_data *data);
int		ft_parse2_build_tree_recurr(t_data *data, t_node *current, int start, int end);
int		ft_parse2_recurr_logical_pipe(t_data *data, t_node *current, int ind);
int		ft_parse2_recurr_par(t_data *data, t_node *current, int opar, int cpar);
int		ft_parse2_endrecurr_leaf(t_data *data, t_node *current);

int		ft_parse2_search_logical(t_data *data, t_node *current);
int		ft_parse2_search_pipe(t_data *data, t_node *current);
int		ft_parse2_search_par(t_data *data, t_node *current);

int		ft_is_redir_token(int token_type);
t_node	*ft_node_new(void);

// PARSE3
char	*ft_parse3_find_shellvar(t_data *data, char *name, int len_shellvar);
int		ft_parse3_expand_shellvar(t_data *data, int pos_tok);
int		ft_parse3_expand(t_data *data);
int		ft_parse3_expand_recurr(t_data *data, t_node *current);
char	*ft_strndup(const char *src, int n);
int		ft_strchrp(char *str, char to_find);

// File: ft_initialize_data.c
int	ft_initialize_data(t_data *data, char **env);

// File: ft_utils_1.c
int	ft_isspace(char c);
int	ft_same_char(char first, char second);

// File: ft_empty_line.c
int	ft_empty_line(char *line);

// File: ft_parsing.c
int ft_open_quotes(char *line);

// File: ft_line_to_data.c
char	**ft_create_tokens(char *line);
int		*ft_create_types(char **cmd);
int		ft_token_amount(char *line);
char	*ft_get_token(char *line, unsigned int *i);
int		ft_get_type(char **cmd, unsigned int i);
#endif
