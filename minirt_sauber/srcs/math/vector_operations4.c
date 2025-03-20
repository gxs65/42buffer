/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operations4.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 18:47:22 by abedin            #+#    #+#             */
/*   Updated: 2025/03/19 19:12:09 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Functions performing operations on vectors (3 dimensions)
// Here : vector logging, vector initialization from characters

// Checks if the given vector is too small to be handled correctly
int	ft_vec_is_infinitesimal(t_vec v)
{
	if (ft_fabs(v.x) < 1e-20 && ft_fabs(v.y) < 1e-20 && ft_fabs(v.z) < 1e-20)
		return (1);
	else
		return (0);
}

// Displays the given vector with or without a trailing newline
void	ft_vec_display(t_vec v, int newline)
{
	printf("(%f,%f,%f)", v.x, v.y, v.z);
	if (newline)
		printf("\n");
	else
		printf(" ");
}

// Stores in <vec_dest> the vector described by string <s> of the form `x,y,z`
int	ft_vec_from_str(t_vec *vec_dest, char *s)
{
	char	**components;
	int		nb_components;

	components = ft_split(s, ',');
	if (!components)
		return (1);
	nb_components = 0;
	while (components[nb_components])
		nb_components++;
	if (nb_components != 3)
		return (1);
	vec_dest->x = ft_atof(components[0]);
	vec_dest->y = ft_atof(components[1]);
	vec_dest->z = ft_atof(components[2]);
	return (0);
}

// Creates a vector with coordinates in [0, 1[ from values [0, 256[
// stored in the 3 strings a the string array <words>
// (used specifically when reading a PPM file for pixel colors)
t_vec	ft_vec_from_words(char **words)
{
	t_vec	v;

	v.x = (double)ft_atoi(words[0]) / 256.0;
	v.y = (double)ft_atoi(words[1]) / 256.0;
	v.z = (double)ft_atoi(words[2]) / 256.0;
	return (v);
}
