#include "../h_files/ft_minishell.h"

int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_data	data;
	t_token	tokens;

	if (ac > 1)
		return (1);
	(void)av;
	data.tokens = &tokens;
	ft_init1_retrieve_env(&data, envp);
	data.path_dirs = NULL;

	while (1)
	{
		line = readline("\033[1;32m>>>\033[0m ");
		data.tokens->name = ft_create_tokens(line);
		data.tokens->type = ft_create_types(data.tokens->name);
		if (ft_exec4_var_assign(&data)) // PAS LA VERSION DEFINITIVE
			ft_printf(LOGS, "[MAIN] Variable definition done\n");
		else
		{
			if (ft_init1_det_path(&data))
				return (1);
			if (ft_parse2_build_tree(&data))
				return (1);
			ft_parse2_display_cmd_tree(&data, data.root, 0);
			if (ft_parse3_expand(&data))
				return (1);
			ft_parse3_display_cmd_tree(&data, data.root, 0);
			if (ft_exec1_exectree_recurr(&data, data.root, 0))
				ft_printf(LOGS, "[MAIN] At least 1 command exited with an error\n");
		}

		add_history(line);
		free(line);
	}
	return (0);
}
