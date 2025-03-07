/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_readspecs.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:25:16 by abedin            #+#    #+#             */
/*   Updated: 2025/01/16 21:47:47 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// (Re)initializes values inside a specifiers struct to base values
void	init_spec(t_spec *spec)
{
	int	ind;

	ind = 0;
	while (ind < 5)
	{
		spec->flags[ind] = 0;
		ind++;
	}
	spec->fw = 0;
	spec->precision = 0;
	spec->precision_defined = 0;
	spec->conv_id = -1;
	spec->step = 0;
	spec->is_null = 0;
}

/*
// ! Log function
void	log_spec(t_spec *spec)
{
	int		*f;
	char	*str = "0-+ #";
	int		ind;

	f = spec->flags;
	printf("SPECIFIERS :\n");
	printf("Step          : %i\n", spec->step);
	printf("Flags         :");
	ind = 0;
	while (ind < 5)
	{
		if (f[ind])
			printf(" [%c]", str[ind]);
		ind++;
	}
	printf("\n");
	printf("Field width   : %i\n", spec->fw);
	printf("Precision     : %i\n", spec->precision);
	printf("Precision def : %i\n", spec->precision_defined);
	printf("Conversion id : %i\n", spec->conv_id);
}
*/

// Modifies the given <spec> instance to include the new specifier :
//		- for a flag : sets the corresponding cell in <flags> to 1
//		- for field width / precision : increases the number like an <atoi>,
//			except if the character was the leading "." of precision
//		- for conversion specifier : simply stores its id from 0 to 8
void	store_valid_specifier(t_spec *spec, int cs_ind)
{
	if (spec->step == 0)
		spec->flags[cs_ind - 9] = 1;
	else if (spec->step == 1)
		spec->fw = (spec->fw * 10) + (cs_ind + 1) % 10;
	else if (spec->step == 2 && cs_ind == 14)
		spec->precision_defined = 1;
	else if (spec->step == 2 && cs_ind != 14)
		spec->precision = (spec->precision * 10) + (cs_ind + 1) % 10;
	else if (spec->step == 3)
		spec->conv_id = cs_ind - 15;
}

// Function verifying if the given character is a valid specifier
//		depending on the step reached by spec->step,
//		eg. a flag (step 0) can't follow the precision specification (step 3)
// -> return value :
//		- 0 : the specifier is valid, we can continue scanning the input string
//		- 1 : the specifier is the conversion character, we can print the arg
//		- (-1) : the specifier is invalid
// Reasons for invalidity of specifier :
//		- not belonging to the characters allowed (string given to strchrp)
//		- belonging to a step lower than the current one
//		- {repeated flag (eg. a second '+' flag)} REMOVED
// /!\ LAST INVALIDITY COND HAS BEEN REMOVED,
//		it seems it was only UB for gcc
//		`(pot_step == 0 && spec->flags[cs_ind - 9])`
int	store_specifier(t_spec *spec, char cs)
{
	int	cs_ind;
	int	pot_step;

	cs_ind = ft_strchrp_asprt("1234567890-+ #.cspdiuxX%", cs);
	pot_step = -1;
	if (spec->step == 0 && (cs_ind >= 9 && cs_ind < 14))
		pot_step = 0;
	else if ((spec->step == 2 && (cs_ind >= 0 && cs_ind < 10))
		|| (spec->step != 2 && cs_ind == 14))
		pot_step = 2;
	else if (cs_ind >= 0 && cs_ind < 10)
		pot_step = 1;
	else if (cs_ind >= 15 && cs_ind < 24)
		pot_step = 3;
	if (pot_step == -1 || pot_step < spec->step)
		return (-1);
	else
	{
		spec->step = pot_step;
		store_valid_specifier(spec, cs_ind);
		return (spec->step == 3);
	}
}
