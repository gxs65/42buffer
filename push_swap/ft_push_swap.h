#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <limits.h>

# define LOGS 1
# define OPS 1
# define SORTED 1
# define EXEC 1

// Memory used for the sorting : 2 linked lists,
// their size, the position of their minimal element
// 		/!\ <min> variable is not maintained by stack operations PUSH and SWAP
typedef struct s_stax
{
	struct s_rlist	*start[2];
	int				size[2];
	int				min[2];
}		t_stax;

// Cyclical linked list with double direction :
// 		- <prev> and <next> : links to other elements
// 		- <n> : number stored, <bin> : its form in base 2
typedef struct s_rlist
{
	struct s_rlist	*prev;
	struct s_rlist	*next;
	int				n;
	int				bin[32];
}		t_rlist;

// Memory of the cost to move element at indA of stackA
// to its correct position in (sorted) stackB
// 		- <ra>,<rb>,<rra>,<rrb> : number of base stack ops
// 		- total : "real" cost in stack ops (with ra+rb replaced by rr)
// 		  -> total = min(max(ra,rb),max(rra,rrb),ra+rrb,rra+rb)
// 		- bast_mode : from 0 to 3 included, the combination of rotations
// 		  that gives this total	->	0 ra and rb		1 rra and rrb
// 								    2 ra and rrb	3 rra abd rb
// 		- <indA>,<indB> : index of elements in stacks A and B to "fetch",
// 		  ie. tomove to the top of the stacks, with rotate ops
typedef struct s_cost
{
	int	ra;
	int	rb;
	int	rra;
	int	rrb;
	int	best_mode;
	int	total;
	int indA;
	int indB;
}		t_cost;

// Manipulation of cyclical linked lists in 2 directions
t_rlist			*ft_lstnew(int n);
void			ft_lstadd_before(t_rlist **lst, t_rlist *newelem);
t_rlist			*ft_lstdelone(t_rlist *lst);
void			ft_lstclear(t_rlist *lst);
int				ft_lstsize(t_rlist *elem, t_rlist *orig, int passed_first);

// Stack operations wrappers
void			swap(t_stax *stax, int l);
void			rotate(t_stax *stax, int l);
void			revrotate(t_stax *stax, int l);
void			push(t_stax *stax, int l);

// Sorts, taking as parameter a <stax> with all numbers in stack A
// (return 1 in case of malloc failure, 0 otherwise)
int				sort_fcost(t_stax *stax);
int				sort_smallstack(t_stax *stax, int l);

// Input/Output
int				read_integer_list(t_stax *stax, char **input, int size);
int				ft_printf(int mode, const char *s, ...);
void			print_int_tab(int *tab, int size);
void			display_rlist(t_rlist *elem, t_rlist *orig, int looped, int mode);
void			display_stacks(t_rlist *starta, t_rlist *startb, int mode);

// (For FCOST + STACK5 sorts) Initial selection of ascending sequence
int				*ft_rlist_to_array(t_rlist *lst);
int				*longest_ascending_seq(int *tab, int size, int *seq_size);
void			initial_push(t_stax *stax);

// Utils - structures
void			initialize_stax(t_stax *stax);
void			initialize_cost(t_cost *cost);
void			copy_cost(t_cost *src, t_cost *dest);

// Utils - others
int				min_nargs(int n, int a, int b, ...);
int				ind_min_4args(int a, int b, int c, int d);
int				min(int a, int b);
int				max(int a, int b);
size_t			ft_strlen(char *str);

#endif