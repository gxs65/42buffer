#include "ft_push_swap.h"

// Returns 1 for an error in case of :
// 		- invalid arguments (duplicates, arg not integer...)
// 		- sorting functions problem (<=> malloc failure)
int	main(int ac, char **av)
{
	t_stax	stax;
	int		ret;

	initialize_stax(&stax);
	if (read_integer_list(&stax, &(av[1]), ac - 1))
	{
		ft_printf(1, "Error\n");
		return (1);
	}
	display_stacks(stax.start[0], stax.start[1], 1); // LOG
	if (stax.size[0] <= 5)
		ret = sort_smallstack(&stax, 0);
	else
		ret = sort_fcost(&stax);
	if (ret)
	{
		ft_printf(1, "Error\n");
		return (1);
	}
	display_stacks(stax.start[0], stax.start[1], 1); // LOG
	return (0);
}