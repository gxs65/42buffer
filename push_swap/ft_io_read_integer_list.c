#include "ft_push_swap.h"

// Searches the given cyclical linked list for integer <n>,
// returns -1 if not found or its index in the list if found
int	find_int_in_rlist(t_rlist *lst, int n)
{
	t_rlist	*current;
	int		ind;

	if (!lst)
		return (-1);
	ind = 0;
	current = lst;
	while (current != lst || ind == 0)
	{
		if (current->n == n)
			return (ind);
		ind++;
		current = current->next;
	}
	return (-1);
}

// Converts the given string to a long int
long	ft_atol(const char *str)
{
	int		sign_n;
	int		pos;
	long	n;

	n = 0;
	pos = 0;
	sign_n = 1;
	while (str[pos] == ' ' || (str[pos] >= 8 && str[pos] <= 13))
		pos++;
	if (str[pos] == '+' || str[pos] == '-')
	{
		if (str[pos] == '-')
			sign_n *= -1;
		pos++;
	}
	while ((str[pos] >= '0' && str[pos] <= '9'))
	{
		n = 10 * n + (str[pos] - 48);
		pos++;
	}
	return (sign_n * n);
}


// Checks that the given input string is not empty,
// has only numeric characters, and at most one sign in 1st position
int invalid_input(char *str)
{
	int	ind;
	int	size;

	size = ft_strlen(str);
	ind = 0;
	if (size == 0 || (size == 1 && (str[ind] == '-' || str[ind] == '+')))
		return (1);
	if (str[ind] == '-' || str[ind] == '+')
		ind++;
	while (str[ind])
	{
		if (!(str[ind] >= '0' && str[ind] <= '9'))
			return (1);
		ind++;
	}
	return (0);
}

// Loops through the input strings, checks that each string is an integer,
// converts it to integer, checks that it is not a duplicate,
// then finally adds it to stack A
// (strings added in reverse, so that 1st string of input is on top of stack)
int	read_integer_list(t_stax *stax, char **input, int size)
{
	int		ind;
	long	n;

	if (size == 0)
		return (1);
	ind = size - 1;
	while (ind >= 0)
	{
		if (invalid_input(input[ind]))
			return (1);
		n = ft_atol(input[ind]);
		if (n < INT_MIN || n > INT_MAX)
			return (1);
		if (find_int_in_rlist(stax->start[0], (int)n) != -1)
			return (1);
		ft_lstadd_before(&(stax->start[0]), ft_lstnew((int)n));
		stax->size[0]++;
		ind--;
	}
	return (0);
}
