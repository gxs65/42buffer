#include <iostream>

void	ft_display_upper(char *s)
{
	char	c;
	int		ind;

	ind = 0;
	while (s[ind])
	{
		c = toupper(s[ind]);
		std::cout << c;
		ind++;
	}
}

int	main(int ac, char **av)
{
	int	ind;
	
	if (ac == 1)
	{
		std::cout << "* LOUD AND UNBEARABLE FEEDBACK NOISE *" << std::endl;
		return (0);
	}
	ind = 1;
	while (ind < ac)
	{
		ft_display_upper(av[ind]);
		ind++;
	}
	std::cout << std::endl;
	return (0);
}
