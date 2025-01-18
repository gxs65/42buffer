#include "../h_files/ft_minishell.h"

int	main(int ac, char **av, char **envp)
{
	char	*line;
	t_data	data;
	t_token	tokens;
	int		ret;

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
		ft_parse1_display_tokens(&data);

		ret = ft_init1_det_path(&data);
		if (ret)
			ft_printf(LOGS, "===== Error on det path : %d\n", ret);
		
		else
		{
			ret = ft_parse2_build_tree(&data);
			if (ret)
				ft_printf(LOGS, "===== Error on build tree : %d\n", ret);

			else
			{
				ft_parse2_display_cmd_tree(&data, data.root, 0);
				ret = ft_parse3_expand(&data);
				if (ret)
					ft_printf(LOGS, "===== Error on expand : %d\n", ret);
				
				else
				{
					ft_parse3_display_cmd_tree(&data, data.root, 0);
					ret = ft_exec1_exectree_recurr(&data, data.root, 0);
					if (ret)
						ft_printf(LOGS, "===== Error on exec : %d\n", ret);
				}
			}
		}

		add_history(line);
		free(line);
	}
	return (0);
}
