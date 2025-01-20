/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: administyrateur <administyrateur@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:19:39 by ilevy             #+#    #+#             */
/*   Updated: 2025/01/20 15:57:45 by administyra      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MINISHELL_H
# define FT_MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include "../libft/h_files/libft.h"

// Macros identifying the types of tokens in a command line
// (and the type of nodes in the command tree)
# define INPUT		1 // <
# define HEREDOC	2 // <<
# define TRUNC		3 // >
# define APPEND		4 // >>
# define CMD		5 // command words
# define PIPE		6 // |
# define AND		7 // &&
# define OR			8 // ||
# define OPEN		9 // (
# define CLOSE		10 // )
# define EQUAL		11 // =

// Custom error codes
// 		- 0  < KILL codes < 32  : fatal errors, cause minishell to exit
// 		- 31 < STOP codes < 128 : non-fatal errors, cause minishell to reprompt
# define KILL_MALLOC_ERROR			16
# define KILL_SIGACTION_ERROR		17
# define KILL_ENVFORMAT_ERROR		18
# define KILL_OPENCWD_ERROR			19
# define KILL_FORK_ERROR			20
# define KILL_PIPE_ERROR			21
# define STOP_OPEN_ERROR			32
# define STOP_SYNTAX_ERROR			33
// Like in Bash, processes forked so that they execve a specific command
// may also exit with error codes that donot affect shell (no stop/reprompt)
// 		- code = 125 : error of system call during execve preparation
// 		- code = 126/127 : executable not found or no exec right
// 		- code >=128 : process received a terminating signal
// 		- 0 < code < 125 : error code defined by program invoked with execve
# define CMD_SYSCALL_ERROR			125
# define CMD_NOXRIGHT_ERROR			126
# define CMD_NOEXEC_ERROR			127

// Maximal length of a path to a certain file
// (not certain wether linux kernel really limits path length,
//  but glibc seems to work with this maximal length)
# define PATH_MAX_LEN 4095
// Maximal length of file name
# define FILENAME_MAX_LEN 255

// Macros controlling the display of logs (given as fds to <ft_printf>) :
// 		LOGS to 2 for simple logs, LOGSV to 2 for verbose logs
# define LOGS 2
# define LOGSV 2

// Structure for the sole purpose of putting together
// variables used when matching filenames to a pattern
// 		(file "ft_parse3_match_pathname.c")
typedef struct s_wcvars
{
	int					ind;
	int					count;
	int					offset;
	int					only_directories;
	struct __dirstream	*dir_pointer;
	struct dirent		*dir_content;
}				t_wcvars;

// Structure binding the two arrays that describe the command line tokens :
// 		- <name> = the token strings as found in the command line
// 		- <type> = the token type, among the type macros from 1 to 10
typedef struct s_token
{
	char	**name;
	int		*type;
}				t_token;

// A node in the command tree, with
// 		- a type, among the type macros CMD/PIPE/OPEN/AND/OR
// 		- reference to the two potential children nodes
// 		- cmd and redir tokens (+ words) string arrays, null-terminated
// 		- input/output file descriptors
typedef struct s_node 
{
	int				type;
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
	int				fd_in_orig;
	int				fd_out;
	int				fd_out_orig;
	int				depth;
}				t_node;

// Elements of a linked list to store shell variables :
// 		name, value, and a boolean for wether the variable belongs to env
// 			(because it was there at initialization / it was exported)
typedef	struct s_var
{
	char			*value;
	char			*name;
	int				is_in_env;
	struct s_var	*next;
}				t_var;

// Main structure holding all general informations
// 		- <tokens>, the result of the command line's tokenisation
// 		- <root>, reference to the root node of the command tree
// 		- <path_dirs>, string array containing the paths of dirs for binaries
// 		- <env>, reference to the first element of a linked list
// 				 of shell variables of type <t_var>
typedef struct s_data
{
	t_token	*tokens;
	int		nb_tokens;
	
	t_node	*root;
	t_node	*current;

	char	**path_dirs;
	t_var	*env;
}				t_data;

// VARIABLE LINKED LIST
t_var	*ft_var_new(char *name, char *value, int is_in_env);
t_var	*ft_var_push_back(t_var *start, t_var *new_var);
void	ft_var_delone(t_var *var);
void	ft_var_list_clear(t_var *current);
t_var	*ft_var_find_by_name(t_var *current, char *name);
int		ft_strcmp(char *s1, char *s2);

// INIT
int		ft_init1_retrieve_env(t_data *data, char **envp);
int		ft_init2_setup_sighandling(t_data *data);
int		ft_init3_det_path(t_data *data);

// PARSE1
int		ft_parse1_tokenize(t_data *data);
void	ft_parse1_display_tokens(t_data *data);
int		ft_same_char(char first, char second);
char	**ft_create_tokens(char *line);
int		*ft_create_types(char **cmd);
int		ft_token_amount(char *line);
char	*ft_get_token(char *line, unsigned int *i);
int		ft_get_type(char **cmd, unsigned int i);

// PARSE2
void	ft_parse2_display_cmd_tree(t_data *data, t_node *current, int depth);

int		ft_parse2_tokens_ind_par(t_data *data, t_node *current, int opar, int cpar);
int		ft_parse2_tokens_ind_cmd(t_data *data, t_node *current);

int		ft_parse2_build_tree(t_data *data);
int		ft_parse2_build_tree_recurr(t_data *data, t_node *current, int start, int end);
int		ft_parse2_recurr_logical_pipe(t_data *data, t_node *current, int ind);
int		ft_parse2_recurr_par(t_data *data, t_node *current, int opar, int cpar);
int		ft_parse2_endrecurr_leaf(t_data *data, t_node *current);

int		ft_parse2_search_logical(t_data *data, t_node *current);
int		ft_parse2_search_pipe(t_data *data, t_node *current);
int		ft_parse2_search_par(t_data *data, t_node *current);

int		ft_parse2_has_correct_assign(t_data *data, t_node *current);
int		ft_parse2_tokens_ind_equal(t_data *data, t_node *current);

int		ft_is_redir_token(int token_type);
t_node	*ft_node_new(void);
char	*ft_strncat(char *dest, const char *src, int n);
char	*ft_strcat(char *dest, const char *src);

// PARSE3
void	ft_parse3_display_cmd_tree(t_data *data, t_node *current, int depth);

int		ft_parse3_expand(t_data *data);
int		ft_parse3_stepsBCD_recurr(t_data *data, t_node *current);

int		ft_parse3_stepA_expand_shellvar(t_data *data);
int		ft_parse3_expand_sv_on_tok(t_data *data, int pos_tok);

int		ft_parse3_resplit_cmd_tokens(t_data *data, t_node *current);
int		ft_parse3_resplit_redir_tokens(t_data *data, t_node *current);

int		ft_parse3_expand_pathname_cmd(t_data *data, t_node *current);
int		ft_parse3_expand_pathname_redir(t_data *data, t_node *current);
int		ft_parse3_count_pathname(char *pattern);
int		ft_parse3_store_pathname(char *pattern, char **store, int *index_to_incr);

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

// EXEC
void	ft_exec3_execcmd(t_data *data, t_node *current);
int		ft_exec2_apply_redirections(t_data *data, t_node *current);
void	ft_exec2_close_redirections(t_data *data, t_node *current);
int		ft_exec1_exectree_recurr(t_data *data, t_node *current, int depth);
int		ft_exec1_interpret_exit_status(int exit_status);

int		ft_exec1_exec_node_pipe(t_data *data, t_node *current, int depth);
int		ft_exec1_exec_node_par(t_data *data, t_node *current, int depth);
int		ft_exec1_exec_node_logical(t_data *data, t_node *current, int depth);
int		ft_exec1_exec_node_leaf(t_data *data, t_node *current, int depth);

int		ft_exec4_var_assign(t_data *data, t_node *current);
char	*ft_strjoin_str_array(int size, char **strs, char *sep);

#endif
