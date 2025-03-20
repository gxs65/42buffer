/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_images.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:16:15 by abedin            #+#    #+#             */
/*   Updated: 2025/03/19 18:20:36 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Stores pixel heights given by the words (numbers) in a PPM file line :
// 		there must be a number of words divisible by 3,
// 			where in each group of 3 the numbers give the RGB values of pixel
// 			(so the values are stored as <t_vec> in <img.values_albedo>
// After each group is processed, <x>,<y> (the position of the current pixel)
// are updated to progress line by line
int	ft_load_ppm_albedo_words(t_img_ppm *img, char *line, int *x, int *y)
{
	int		nb_words;
	int		ind_word;
	char	**words;

	words = ft_split(line, ' ');
	if (!words)
		return (1);
	nb_words = ft_count_words(words);
	if (nb_words == 0 || (nb_words) % 3 != 0)
		return (ft_free_strarray_nullterm(words), 1);
	ind_word = 0;
	while (ind_word < nb_words && *y < img->height_albedo)
	{
		img->values_albedo[*y][*x] = ft_vec_from_words(&(words[ind_word]));
		(*x)++;
		if (*x == img->width_albedo)
		{
			*x = 0;
			(*y)++;
		}
		ind_word += 3;
	}
	ft_free_strarray_nullterm(words);
	return (0);
}

// Allocates an 2dim array for, then loads
// all lines of a PPM file describing pixel colors
// (since lines in PPM file may be not aligned to lines in described image,
//  the position of current pixel in image is stored in <x>,<y>
//  and passed as reference to <ft_load_albedo_words>)
int	ft_load_ppm_albedo(char *filename, t_img_ppm *img)
{
	int			fd;
	char		*line;
	int			x;
	int			y;

	fd = open(filename, O_RDONLY);
	if (ft_read_ppm_header(fd, &(img->width_albedo), &(img->height_albedo)))
		return (1);
	img->values_albedo = ft_allocate_2dim_vecarray(
			img->height_albedo, img->width_albedo);
	if (!(img->values_albedo))
		return (close(fd), 1);
	y = 0;
	x = 0;
	while (y < img->height_albedo)
	{
		line = get_next_line(fd);
		ft_delete_ending_linebreak(line);
		if (ft_load_ppm_albedo_words(img, line, &x, &y))
			return (free(line), ft_free_2dim_vecarray_upto(
					img->values_albedo, img->height_albedo), close(fd), 1);
		free(line);
	}
	return (0);
}

// Stores pixel heights given by the words (numbers) in a PPM file line :
// 		there must be a number of words divisible by 3,
// 			where in each group of 3 all numbers are equal
// 			(they all give the height for this pixel, with 255 as max height)
// After each group is processed, <x>,<y> (the position of the current pixel)
// are updated to progress line by line
int	ft_load_ppm_bump_words(t_img_ppm *img, char *line, int *x, int *y)
{
	int		nb_words;
	int		ind_word;
	char	**words;

	words = ft_split(line, ' ');
	if (!words)
		return (1);
	nb_words = ft_count_words(words);
	if (nb_words == 0 || (nb_words) % 3 != 0)
		return (ft_free_strarray_nullterm(words), 1);
	ind_word = 0;
	while (ind_word < nb_words && *y < img->height_bump)
	{
		img->values_bump[*y][*x] = (double)ft_atoi(words[ind_word]) / 256.0;
		(*x)++;
		if (*x == img->width_bump)
		{
			*x = 0;
			(*y)++;
		}
		ind_word += 3;
	}
	ft_free_strarray_nullterm(words);
	return (0);
}

// Allocates an 2dim array for, then loads
// all lines of a PPM file describing pixel heights
// (since lines in PPM file may be not aligned to lines in described image,
//  the position of current pixel in image is stored in <x>,<y>
//  and passed as reference to <ft_load_bump_words>)
int	ft_load_ppm_bump(char *filename, t_img_ppm *img)
{
	int			fd;
	char		*line;
	int			x;
	int			y;

	fd = open(filename, O_RDONLY);
	if (ft_read_ppm_header(fd, &(img->width_bump), &(img->height_bump)))
		return (1);
	img->values_bump = ft_allocate_2dim_doublearray(
			img->height_bump, img->width_bump);
	if (!(img->values_bump))
		return (close(fd), 1);
	y = 0;
	x = 0;
	while (y < img->height_bump)
	{
		line = get_next_line(fd);
		ft_delete_ending_linebreak(line);
		if (ft_load_ppm_bump_words(img, line, &x, &y))
			return (free(line), ft_free_2dim_doublearray_upto(
					img->values_bump, img->height_bump), close(fd), 1);
		free(line);
	}
	return (0);
}

// Allocates a structure storing all pixels of a PPM image,
// then loads the albedo and bump values
t_img_ppm	*ft_load_ppm(char *basename)
{
	t_img_ppm	*ppm;

	ppm = malloc(1 * sizeof(t_img_ppm));
	printf("Loading texture albedo for : %s\n", basename);
	if (ft_load_ppm_albedo(ft_strjoin(basename, ".ppm"), ppm))
		return (NULL);
	printf("Loading texture bump for : %s\n", basename);
	if (ft_load_ppm_bump(ft_strjoin(basename, "bump.ppm"), ppm))
		return (NULL);
	return (ppm);
}
