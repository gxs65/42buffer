/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <ilevy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:33:28 by ilevy             #+#    #+#             */
/*   Updated: 2025/01/08 13:51:00 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/ft_minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_data	*data; // Data contenant l'env, les input de l'utilisateur, et les informations necessaires pour gerer les file descriptors.
	char	*line;

	if (!ft_initialize_data(&data, env) == -1) // Prendre les donnees de l'environement de depart et les ajouter a un struct data.
	{
		printf("minishell: %s\n", strerror(ENOENT));
		return (ENOENT); // Rendre un message derreur si on ne trouve pas l'environment.
	}
	while (1)
	{
		line = readline("minishell> "); //Cree un prompte affichant "minishell >"
		if (!line)
		{
			ft_free_all(data, line);
			printf("exiting minishell.\n"); //Si on a pas de place pour la ligne vu que readline est associe a malloc
			return (MALLOC_ERROR);
		}
		else if (ft_empty_line(line) == 1)
		{
			free(line); // on libere la ligne pour une prochaine exec.
			continue ; //On skip les commandes nulles.
		}
		add_history(line); //On ajoute la ligne a l'histoire des lignes precedentes donnees par l'utilisateur
		if (ft_line_to_data(data, line) == -1); //On prend l'input de l'utilisateur, et on le tokenize pour faciliter l'execution.
				continue ; // Si la ligne est incoherente genre "wgfwijghw"
		if (!ft_exec(data))
		{
			ft_free_all(data, line);
			printf("couldn't execute command.\n");
			return (CMD_ERROR);
		}
		ft_erase_tokens(data); //On efface les infos donnees par la ligne qu'on vient d'executer.
		free(line); // on libere la ligne pour une prochaine exec.
	}
	rl_clear_history();
	ft_free_all(data, line);
	return (0);
}
