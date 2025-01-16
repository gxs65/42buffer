#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>

int	ft_match_wc(char *name, char *pattern)
{
	int	ind_in_name;
	int	ind_in_pattern;

	//printf("Match Wildcards called on strings : <<%s>> <<%s>>\n", name, pattern);
	ind_in_name = 0;
	ind_in_pattern = 0;
	while (name[ind_in_name] && pattern[ind_in_pattern])
	{
		if (pattern[ind_in_pattern] == '*')
		{
			while(name[ind_in_name])
			{
				if (ft_match_wc(name + ind_in_name, pattern + ind_in_pattern + 1))
					return (1);
				ind_in_name++;
			}
			//printf("After check for any nb of replaced chars, indn %d indp %d\n", ind_in_name, ind_in_pattern);
			if (!(name[ind_in_name]) && !(pattern[ind_in_pattern + 1]))
				return (1);
			else
				return (0);
		}
		else if (name[ind_in_name] != pattern[ind_in_pattern])
			return (0);
		ind_in_name++;
		ind_in_pattern++;
	}
	if (!(name[ind_in_name]) && !(pattern[ind_in_pattern]))
		return (1);
	else
		return (0);
}

int	ft_list_elements_in_dir(char *pattern, char *path, int matching)
{
	int				ind;
	DIR				*dir_pointer;
	struct dirent	*dir_content;
	
	dir_pointer = opendir(path);
	if (!dir_pointer)
		return (1);
	ind = 0;
	dir_content = readdir(dir_pointer);
	while (dir_content)
	{
		if (matching == 0 || ft_match_wc(dir_content->d_name, pattern))
			printf("%d (%d) - %s\n", ind, dir_content->d_type == DT_DIR, dir_content->d_name);
		ind++;
		dir_content = readdir(dir_pointer);
	}
	closedir(dir_pointer);
	return (0);
}

int	ft_expand_wc(char *pattern, char *path)
{
	printf("ELEMENTS FOUND IN DIRECTORY :\n");
	if (ft_list_elements_in_dir(pattern, path, 0))
		return (1);
	printf("MATCHING ELEMENTS ONLY :\n");
	if (ft_list_elements_in_dir(pattern, path, 1))
		return (1);
	return (0);
}



int	main(int ac, char **av)
{
	char	**res;

	if (ac < 2)
		return (1);
	ft_expand_wc(av[1], av[2]);
	return (0);
}
