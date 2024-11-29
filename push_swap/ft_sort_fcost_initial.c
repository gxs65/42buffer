#include "ft_push_swap.h"

int	*ft_calloc_int(size_t nmemb, int val)
{
	int		*res;
	size_t	len;
	size_t	pos;

	if (nmemb >= 2147483648)
		return (NULL);
	len = nmemb * sizeof(int);
	if (len >= 2147483648)
		return (NULL);
	res = malloc(len);
	if (!res)
		return (NULL);
	pos = 0;
	while (pos < nmemb)
	{
		res[pos] = val;
		pos++;
	}
	return (res);
}

int	*rebuild_sequence(int *dyn_result, int *mem_previous, int size, int *seq_size)
{
	int	ind_max;
	int	ind;
	int	*seq;
	int	ind_previous;

	ind = 0;
	ind_max = 0;
	while (ind < size)
	{
		if (dyn_result[ind] > dyn_result[ind_max])
			ind_max = ind;
		ind++;
	}
	*seq_size = dyn_result[ind_max];
	seq = ft_calloc_int(*seq_size, -1);
	if (!seq)
		return (NULL);
	ind = *seq_size - 1;
	seq[ind] = ind_max;
	while (ind >= 0)
	{
		ind_previous = mem_previous[seq[ind]];
		ind--;
		if (ind >= 0)
			seq[ind] = ind_previous;
	}
	return (seq);
}

int	*longest_ascending_seq(int *tab, int size, int *seq_size)
{
	int		inda;
	int		indz;
	int		*mem_previous;
	int		*dyn_result;

	mem_previous = ft_calloc_int(size, -1);
	dyn_result = ft_calloc_int(size, 1);
	if (!mem_previous || !dyn_result)
		return (NULL);
	inda = 1;
	while (inda < size)
	{
		indz = 0;
		while (indz < inda)
		{
			if (tab[inda] >= tab[indz] && dyn_result[inda] < dyn_result[indz] + 1)
			{
				dyn_result[inda] = dyn_result[indz] + 1;
				mem_previous[inda] = indz;
			}
			indz++;
		}
		inda++;
	}
	print_int_tab(dyn_result, size);
	print_int_tab(mem_previous, size);
	return(rebuild_sequence(dyn_result, mem_previous, size, seq_size));
}
