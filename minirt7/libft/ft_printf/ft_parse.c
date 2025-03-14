/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 13:44:55 by alex              #+#    #+#             */
/*   Updated: 2024/06/01 21:06:14 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_parse(const char *format, va_list args)
{
	if (*format == 'c')
		return (ft_putchar(va_arg(args, int)));
	else if (*format == 's')
		return (ft_parse_str(args));
	else if (*format == 'p')
		return (ft_parse_pointer(args));
	else if (*format == 'd' || *format == 'i')
		return (ft_parse_int(args));
	else if (*format == 'u')
		return (ft_parse_nbase(args, "0123456789"));
	else if (*format == 'x')
		return (ft_parse_nbase(args, "0123456789abcdef"));
	else if (*format == 'X')
		return (ft_parse_nbase(args, "0123456789ABCDEF"));
	else if (*format == '%')
		return (ft_putchar('%'));
	return (0);
}

int	ft_parse_str(va_list args)
{
	char	*s;

	s = va_arg(args, char *);
	if (!s)
	{
		ft_putstr_fd("(null)", 1);
		return (6);
	}
	else
	{
		ft_putstr_fd(s, 1);
		return (ft_strlen(s));
	}
}

int	ft_parse_pointer(va_list args)
{
	void	*ptr;
	int		count;

	ptr = va_arg(args, void *);
	count = 0;
	if (!ptr)
	{
		ft_putstr_fd("(nil)", 1);
		return (5);
	}
	ft_putstr_fd("0x", 1);
	count += ft_putnbr_base((long)ptr, "0123456789abcdef");
	return (count + 2);
}

int	ft_parse_int(va_list args)
{
	long int	n;
	int			count;

	n = (long)va_arg(args, int);
	count = 0;
	if (n < 0)
	{
		count += ft_putchar('-');
		n = -n;
	}
	count += ft_putnbr_base(n, "0123456789");
	return (count);
}

int	ft_parse_nbase(va_list args, char *base)
{
	unsigned int	n;
	int				count;

	n = va_arg(args, unsigned int);
	count = ft_putnbr_base(n, base);
	return (count);
}
