/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:19:39 by ilevy             #+#    #+#             */
/*   Updated: 2025/01/15 03:18:59 by abedin           ###   ########.fr       */
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
# include <sys/stat.h>
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

# define LOGS 2
# define LOGSV 2

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
	int				nb_cmd_tokens;
	int				*redir_tokens_inds;
	int				nb_redir_tokens;
	
	char			**cmd_words;
	int				nb_cmd_words;
	char			**redir_words;
	int				nb_redir_words;

	int				ind_start_token;
	int				ind_end_token;

	int				fd_in;
	int				fd_out;
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

	char	**path_dirs;
}				t_data;

// PARSE1
int		ft_same_char(char first, char second);
char	**ft_create_tokens(char *line);
int		*ft_create_types(char **cmd);
int		ft_token_amount(char *line);
char	*ft_get_token(char *line, unsigned int *i);
int		ft_get_type(char **cmd, unsigned int i);

// PARSE2
void	ft_parse2_display_cmd_tree(t_data *data, t_node *current, int depth);

int		ft_parse2_tokens_ind_par(t_data *data, t_node *current, int opar, int cpar);
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
char	*ft_strncat(char *dest, const char *src, int n);
char	*ft_strcat(char *dest, const char *src);


// PARSE3
void	ft_parse3_display_cmd_tree(t_data *data, t_node *current, int depth);

int		ft_parse3_expand(t_data *data);
int		ft_parse3_stepsBCD_recurr(t_data *data, t_node *current);

int		ft_parse3_expand_shellvar(t_data *data);
int		ft_parse3_expand_sv_on_tok(t_data *data, int pos_tok);

int		ft_parse3_resplit_cmd_tokens(t_data *data, t_node *current);
int		ft_parse3_resplit_redir_tokens(t_data *data, t_node *current);

int		ft_parse3_expand_pathname_cmd(t_data *data, t_node *current);
int		ft_parse3_expand_pathname_redir(t_data *data, t_node *current);

void	ft_parse3_remove_quotes(char *word);
void	ft_parse3_remove_quotes_cmd(t_data *data, t_node *current);
void	ft_parse3_remove_quotes_redir(t_data *data, t_node *current);

char	*ft_strndup(const char *src, int n);
int		ft_strchrp(char *str, char to_find);
int		ft_strchrp_nonquoted(char *str, char to_find, int level, int from);
char	*ft_strncat(char *dest, const char *src, int n);
char	*ft_strcat(char *dest, const char *src);

int		ft_isspace(char c);
int		ft_count_tokens_space(const char *str);
char	**ft_split_space(const char *s);
int		free_strs_tab_upto(char **tab, int lim);
int		free_strs_tab_nullterm(char **tab);
void	ft_display_string_array(char **array, int len);

// PARSEX
int		ft_parseX_count_pathname(t_data *data, char *word);
int		ft_parseX_store_pathname(t_data *data, char *word, char **store, int *index_to_incr);

// EXEC
void	ft_exec3_execcmd(t_data *data, t_node *current);
int		ft_exec1_apply_redirections(t_data *data, t_node *current);
int		ft_exec1_exectree_recurr(t_data *data, t_node *current);



#endif
