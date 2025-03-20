/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_images_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 16:15:03 by abedin            #+#    #+#             */
/*   Updated: 2025/03/19 18:06:35 by abedin           ###   ########.fr       */
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

// Removes the '\n' ending a string, if there is one
void	ft_delete_ending_linebreak(char *line)
{
	int	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
}

// Frees a null-terminated string array (all strings, then the array itself)
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

// Frees all the lines of an array of doubles up to line <upto>,
// then frees the array itself
void	ft_free_2dim_doublearray_upto(double **arr, int upto)
{
	int	ind;

	ind = 0;
	while (ind < upto)
	{
		free(arr[ind]);
		ind++;
	}
	free(arr);
}

// Allocates a 2-dimensional array of doubles :
// an array of size <height> containing all the arrays of size <width>
double	**ft_allocate_2dim_doublearray(int height, int width)
{
	int		y;
	double	**res;

	res = malloc(height * sizeof(double *));
	if (!res)
		return (NULL);
	y = 0;
	while (y < height)
	{
		res[y] = malloc(width * sizeof(double));
		if (!(res[y]))
		{
			ft_free_2dim_doublearray_upto(res, y);
			return (NULL);
		}
		y++;
	}
	return (res);
}

// Frees all the lines of an array of <t_vec> up to line <upto>,
// then frees the array itself
void	ft_free_2dim_vecarray_upto(t_vec **arr, int upto)
{
	int	ind;

	ind = 0;
	while (ind < upto)
	{
		free(arr[ind]);
		ind++;
	}
	free(arr);
}

// Allocates a 2-dimensional array of <t_vec> :
// an array of size <height> containing all the arrays of size <width>
t_vec	**ft_allocate_2dim_vecarray(int height, int width)
{
	int		y;
	t_vec	**res;

	res = malloc(height * sizeof(t_vec *));
	if (!res)
		return (NULL);
	y = 0;
	while (y < height)
	{
		res[y] = malloc(width * sizeof(t_vec));
		if (!(res[y]))
		{
			ft_free_2dim_vecarray_upto(res, y);
			return (NULL);
		}
		y++;
	}
	return (res);
}

// Reads the header of an already opened PPM file at file descriptor <fd>
// 		- line 0 : checks that the file's magic number is "P3"
// 				   (<=> a plain text non-compressed PPM image)
// 		- lines 1.x : skips commentary lines (beginning with '#')
// 		- line 2 : registers dimensions of image
// 				   thanks to pointers <width> and <height>
// 		- line 3 : skips the max value for RGB channels (assumed to be 255)
int	ft_read_ppm_header(int fd, int *width, int *height)
{
	char	*line;
	char	**words;

	line = get_next_line(fd);
	if (!(ft_strlen(line) == 3 && line[0] == 'P' && line[1] == '3'))
		return (1);
	free(line);
	line = get_next_line(fd);
	while (ft_strlen(line) > 0 && line[0] == '#')
	{
		free(line);
		line = get_next_line(fd);
	}
	words = ft_split(line, ' ');
	if (ft_count_words(words) != 2)
		return (free(line), 1);
	*width = ft_atoi(words[0]);
	*height = ft_atoi(words[1]);
	ft_free_strarray_nullterm(words);
	free(line);
	line = get_next_line(fd);
	free(line);
	return (0);
}
