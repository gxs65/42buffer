#include <stdio.h>
#include <stdlib.h>

long	ft_power(int base, int exponent)
{
	if (exponent == 0)
		return (1);
	else if (base == 0 || exponent < 0)
		return (0);
	else
		return (base * ft_power(base, exponent - 1));
}

int	ft_isspace(char c)
{
	if (c == ' ' || (c >= 8 && c <= 13))
		return (1);
	else
		return (0);
}

int	ft_isdigit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

double	ft_atof_digits(char *s, int ind)
{
	double	res;
	int		ind_decimals;

	res = 0;
	while (s[ind] && ft_isdigit(s[ind]))
	{
		printf("On digit int part : %c\n", s[ind]);
		res = res * 10 + (s[ind] - '0');
		ind++;
	}
	if (!(s[ind]) || s[ind] != '.')
		return (res);
	ind++;
	ind_decimals = 0;
	while (s[ind + ind_decimals] && ft_isdigit(s[ind + ind_decimals]))
	{
		printf("On digit dec part : %c\n", s[ind + ind_decimals]);
		res = res * 10 + (s[ind + ind_decimals] - '0');
		ind_decimals++;
	}
	printf("Res before division : %f\n", res);
	return (res / (double)(ft_power(10, ind_decimals)));
}

double	ft_atof(char *s)
{
	int		ind;
	int		sign;

	ind = 0;
	sign = 1;
	while (s[ind] && ft_isspace(s[ind]))
		ind++;
	while (s[ind] && (s[ind] == '-' || s[ind] == '+'))
	{
		if (s[ind] == '-')
			sign = sign * -1;
		ind++;
	}
	return (sign * ft_atof_digits(s, ind));
}

int	main(int ac, char **av)
{
	double	res;

	if (ac != 2)
		return (1);
	res = ft_atof(av[1]);
	printf("Floating point number : %f\n", res);
	return (0);	
}