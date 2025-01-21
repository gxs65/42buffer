#include "../h_files/ft_minishell.h"

// Performs pattern matching between a name string and a pattern,
// 		the only recognized meta-character being '*' (replaces 0 or more chars)
// Goes through the pattern and string until finding no more chars in pattern
// 		\ if the current char in pattern is not a '*',
// 			compares it to the current char in name (and stops if unequal)
// 		\ if the current char in pattern is a '*',
// 			recurrs for each possible number of chars this '*' could replace,
// 				and returns 0 if no number of replaced chars produced a match
int	ft_match_wc(char *name, char *pattern, int name_len)
{
	int	ind_in_name;
	int	ind_in_pattern;

	ind_in_name = 0;
	ind_in_pattern = 0;
	while (pattern[ind_in_pattern])
	{
		if (pattern[ind_in_pattern] == '*')
		{
			while (ind_in_name <= name_len)
			{
				if (ft_match_wc(name + ind_in_name,
						pattern + ind_in_pattern + 1, name_len))
					return (1);
				ind_in_name++;
			}
			return (0);
		}
		else if (name[ind_in_name] != pattern[ind_in_pattern])
			return (0);
		if (ind_in_name < name_len)
			ind_in_name++;
		ind_in_pattern++;
	}
	return ((!(name[ind_in_name]) && !(pattern[ind_in_pattern])));
}

// Checks if the file whose infos are currently available in <v.dir_content>
// is a match for the pathname expansion of <pattern> :
// 		1. the file must not be the "." or ".." directories
// 		2. the file must not be a directory of <v.only_directories> is set to 1
// 		3. the file's name must match the pattern (which may contain '*') 
int	ft_match_pathname(char *pattern, t_wcvars *v)
{
	if (ft_strcmp(v->dir_content->d_name, "..")
		&& ft_strcmp(v->dir_content->d_name, ".")
		&& (v->only_directories == 0 || v->dir_content->d_type == DT_DIR)
		&& ft_match_wc(v->dir_content->d_name, pattern + v->offset,
			ft_strlen(v->dir_content->d_name)))
		return (1);
	else
		return (0);
}

// Prepares the search for pathname matches, by :
// 		- opening the current working directory
// 		- checking if the pattern contains the starting sequence "./",
// 			if yes, offsets by that much characters
// 				(this sequence is useless for Minishell,
// 				 since pathname expansion is in CWD only anyway)
// 		- checking if the pattern contains the ending sequence "/",
// 			if yes, removes it, and sets the flag for directory only search 
int	ft_parse3_pathname_prepare(char *pattern, struct __dirstream **dir_pointer,
	int *offset, int *only_directories)
{
	*dir_pointer = opendir(".");
	if (!(*dir_pointer))
		return (1);
	*offset = 0;
	if (ft_strncmp(pattern, "./", 2) == 0)
		*offset = 2;
	*only_directories = 0;
	if (pattern[ft_strlen(pattern) - 1] == '/')
	{
		*only_directories = 1;
		pattern[ft_strlen(pattern) - 1] = '\0';
	}
	return (0);
}

// Count the number of files in the CWD that match the pattern
// 		/!\ may encounter an error with <opendir>, then returns -1
int	ft_parse3_count_pathname(char *pattern)
{
	t_wcvars	v;

	if (ft_parse3_pathname_prepare(pattern, &(v.dir_pointer),
			&(v.offset), &(v.only_directories)))
		return (-1);
	v.count = 0;
	v.ind = 0;
	v.dir_content = readdir(v.dir_pointer);
	while (v.dir_content)
	{
		if (ft_match_pathname(pattern, &v))
			v.count++;
		v.ind++;
		v.dir_content = readdir(v.dir_pointer);
	}
	closedir(v.dir_pointer);
	if (v.only_directories)
		pattern[ft_strlen(pattern)] = '/';
	return (v.count);
}

// Stores in a previously allocated string array <store> the name of the files
// that match the pattern
// 		/!\ may encounter an error with <opendir> or <malloc>
int	ft_parse3_store_pathname(char *pattern, char **store,
	int *index_to_incr)
{
	t_wcvars	v;

	if (ft_parse3_pathname_prepare(pattern, &(v.dir_pointer),
			&(v.offset), &(v.only_directories)))
		return (1);
	v.ind = 0;
	v.dir_content = readdir(v.dir_pointer);
	while (v.dir_content)
	{
		if (ft_match_pathname(pattern, &v))
		{
			store[(*index_to_incr)] = ft_strdup(v.dir_content->d_name);
			if (!store[(*index_to_incr)])
				return (1);
			*index_to_incr += 1;
		}
		v.ind++;
		v.dir_content = readdir(v.dir_pointer);
	}
	closedir(v.dir_pointer);
	if (v.only_directories)
		pattern[ft_strlen(pattern)] = '/';
	return (0);
}
