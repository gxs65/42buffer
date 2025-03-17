/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_images.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 16:16:15 by abedin            #+#    #+#             */
/*   Updated: 2025/03/17 17:51:27 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Counts the number of strings in a null-terminated string array
int	ft_count_words(char **words)
{
	int	count;

	count = 0;
	while (words[count])
		count++;
	return (count);
}

void	ft_free_strarray_nullterm(char **strs)
{
	int	ind;

	ind = 0;
	while (strs[ind])
	{
		free(strs[ind]);
		ind++;
	}
	free(strs);
}

void	ft_delete_ending_linebreak(char *line)
{
	int	len;

	len = ft_strlen(line);
	if (line[len - 1] == '\n')
		line[len - 1] = '\0';
}

t_img_ppm	*ft_load_ppm_image(char *filename)
{
	int			fd;
	int			ind_line;
	char		*line;
	char		**words;
	int			nb_words;
	int			ind_word;
	t_img_ppm	*img;
	int			x;
	int			y;

	img = malloc(1 * sizeof(t_img_ppm));
	fd = open(filename, O_RDONLY);

	line = get_next_line(fd); // "magic number" line, must be "P3"
	//printf("magic number line : %s", line);
	if (!(ft_strlen(line) == 3 && line[0] == 'P' && line[1] == '3'))
		return (NULL);
	free(line);

	line = get_next_line(fd); // commentary lines (skipped)
	while (ft_strlen(line) > 0 && line[0] == '#')
	{
		free(line);
		line = get_next_line(fd);
	}

	//printf("dimensions line : %s", line);
	words = ft_split(line, ' '); // width and height line
	if (ft_count_words(words) != 2)
		return (NULL);
	img->width = ft_atoi(words[0]);
	img->height = ft_atoi(words[1]);
	ft_free_strarray_nullterm(words);
	free(line);

	line = get_next_line(fd); // max pixel R/G/B value line (skipped)
	//printf("max pix line : %s", line);
	free(line);
	
	// Allocating array for pixel values
	img->values = malloc(img->height * sizeof(t_vec *));
	y = 0;
	while (y < img->height)
	{
		img->values[y] = malloc(img->width * sizeof(t_vec));
		y++;
	}
	//printf("allocation of array successful\n");
	// -----

	y = 0; // pixel values lines
	x = 0;
	ind_line = 0;
	while (y < img->height)
	{
		line = get_next_line(fd);
		ft_delete_ending_linebreak(line);
		words = ft_split(line, ' ');
		nb_words = ft_count_words(words);
		//printf("with x = %d, y = %d ; next values line has %d words : %s", x, y, nb_words, line);
		if (nb_words == 0 || (nb_words) % 3 != 0)
			return (NULL);
		ind_word = 0;
		while (ind_word < nb_words && y < img->height) // second condition is protection against ill-formatted ppms
		{
			//printf("with x = %d, y = %d ; next words at ind %d are line : %s, %s, %s\n", x, y,
			//	ind_word, words[ind_word], words[ind_word + 1], words[ind_word + 2]);
			img->values[y][x] = ft_vec_init((double)ft_atoi(words[ind_word]) / 256.0,
				(double)ft_atoi(words[ind_word + 1]) / 256.0,
				(double)ft_atoi(words[ind_word + 2]) / 256.0);
			x++;
			if (x == img->width)
			{
				x = 0;
				y++;
			}
			ind_word += 3;
		}
		ft_free_strarray_nullterm(words);
		free(line);
	}

	return (img);
}