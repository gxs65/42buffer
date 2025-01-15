#include "../h_files/ft_minishell.h"

/*
- etapes de l'execution en suivant l'arbre construit
	1. si le noeud est un noeud LEAF ou PARENTHESIS, verifier s'il existe des redirections
		~ pour chaque direction (in / out), ouvrir le dernier fichier donne, et remplacer nos fdio par ces fd
		~ si plusieurs fichiers avaient ete donnes, ouvrir et refermer immediatement les fichiers qui ne sont pas dernier
		~ pour la redirection "out", en faisant attention a suivre le mode (TRUNC / APPEND)
	2. si le noeud est un noeud LEAF ou PARENTHESIS -> execution
		~ LEAF : fork un executeur (qui va dup2 pour remplacer les fd 0 et 1 par les eventuelles redirection, puis execve)
			ensuite attendre waitpid l'executeur, enregistrer son exit status et le renvoyer
		~ PARENTHESIS : assigner a child_1 les memes fdios que nous, et recurrer sur child_1 (et renvoyer son exit status)
	3. si le noeud est un noeud PIPE -> execution parallele
		~ creer un pipe classique
		~ assigner les fdios de child_1 et child_2
			. child_1 a fdin herite de noeud courant et fdout dans pipe
			. child_2 a fdin depuis pipe et fdout herite de noeud courant
		~ fork un process pour child1, qui va recurrer sur child_1
		  fork un process pour child2, qui va recurrer sur child_2
		~ attendre les deux enfants, renvoyer le exit_status de child_2
	4. si le noeud est un noeud OR ou AND -> execution sequentielle
		~ assigner les fdios de child_1 et child_2, simplement herites du noeud courant
		~ recurrer sur child_1, recuperer la valeur de retour (qui est le exit status)
		~ selon la valeur de retour, eventuellement recurrer sur child_2
		~ renvoyer le exit_status de child_2
*/

int	ft_exec1_exectree_recurr(t_data *data, t_node *current)
{
	pid_t	pid;
	pid_t	pipe_pids[2];
	int		exit_status;
	int		pipe_fds[2];
	int		ret;

	ft_printf(LOGS, "[EXEC1 - MAIN] node type %d at %p - beginning execution\n", current->type, current);
	if ((current->type == CMD || current->type == OPEN) && ft_exec1_apply_redirections(data, current))
		return (1);

	if (current->type == CMD)
	{
		pid = fork();
		if (pid == -1)
		{
			ft_printf(LOGS, "! error when forking child process for leaf\n");
			return (1);
		}
		else if (pid == 0)
		{
			ft_exec3_execcmd(data, current);
			return (0); // this return should never be reached, because of preceding calls to <execve> or <exit> in <ft_exec1_execcmd>
		}
		else
		{
			ft_printf(LOGSV, "[EXEC1 - MAIN] node type %d at %p - successfully forked process for leaf\n", current->type, current);
			waitpid(pid, &exit_status, 0);
			ft_printf(LOGSV, "[EXEC1 - MAIN] node type %d at %p - end of wait on process for leaf\n", current->type, current);
			return (exit_status);
		}
	}

	else if (current->type == OPEN)
	{
		current->child_1->fd_in = current->fd_in;
		current->child_1->fd_out = current->fd_out;
		ft_printf(LOGSV, "[EXEC1 - MAIN] node type %d at %p - assigned file descriptors of child_1 at %p, now recurring\n", current->type, current, current->child_1);
		return (ft_exec1_exectree_recurr(data, current->child_1));
	}

	else if (current->type == PIPE)
	{
		if (pipe(pipe_fds))
		{
			ft_printf(LOGS, "! error when creating a pipe\n");
			return (1);
		}
		current->child_1->fd_in = current->fd_in;
		current->child_1->fd_out = pipe_fds[1];
		current->child_2->fd_in = pipe_fds[0];
		current->child_2->fd_out = current->fd_out;
		ft_printf(LOGSV, "[EXEC1 - MAIN] node type %d at %p - created pipe, assigned fds of child_1 at %p and child_2 at %p, now forking\n",
			current->type, current, current->child_1, current->child_2);
		pipe_pids[0] = fork();
		if (pipe_pids[0] == -1)
		{
			ft_printf(LOGS, "! error when forking child process for pipe child_1\n");
			return (1);
		}
		else if (pipe_fds[0] == 0)
		{
			ret = ft_exec1_exectree_recurr(data, current->child_1);
			exit(ret);
		}
		else
		{
			ft_printf(LOGSV, "[EXEC1 - MAIN] node type %d at %p - successfully forked process for pipe child_1\n", current->type, current);
			pipe_fds[1] = fork();
			if (pipe_pids[1] == -1)
			{
				ft_printf(LOGS, "! error when forking child process for pipe child_2\n");
				return (1);
			}
			else if (pipe_fds[1] == 0)
			{
				ret = ft_exec1_exectree_recurr(data, current->child_2);
				exit(ret);
			}
			else
			{
				ft_printf(LOGSV, "[EXEC1 - MAIN] node type %d at %p - successfully forked process for pipe child_2\n", current->type, current);
				waitpid(pipe_pids[0], &exit_status, 0);
				waitpid(pipe_pids[1], &exit_status, 0);
				ft_printf(LOGSV, "[EXEC1 - MAIN] node type %d at %p - end of wait on processes for child_1 and child_2\n", current->type, current);
				return (exit_status);
			}
		}
	}

	else if (current->type == AND || current->type == OR)
	{
		current->child_1->fd_in = current->fd_in;
		current->child_1->fd_out = current->fd_out;
		current->child_2->fd_in = current->fd_in;
		current->child_2->fd_out = current->fd_out;
		ft_printf(LOGSV, "[EXEC1 - MAIN] node type %d at %p - OR/AND, assigned fds of child_1 at %p and child_2 at %p, now recurring\n",
			current->type, current, current->child_1, current->child_2);
		exit_status = ft_exec1_exectree_recurr(data, current->child_1);
		ft_printf(LOGSV, "[EXEC1 - MAIN] node type %d at %p - received exit status %d on first part of AND/OR connection\n",
			current->type, current);
		if ((exit_status == 0 && current->type == AND) || (exit_status != 0 && current->type == OR))
		{
			exit_status = ft_exec1_exectree_recurr(data, current->child_2);
			ft_printf(LOGSV, "[EXEC1 - MAIN] node type %d at %p - received exit status %d on second part of AND/OR connection\n",
				current->type, current);
		}
		return (exit_status);
	}

	else
	{
		ft_printf(LOGS, "! error when trying to read current node type\n");
		return (1);
	}
}
