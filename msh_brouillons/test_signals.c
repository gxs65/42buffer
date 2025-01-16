#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
//#include <sys/type.h>
#define WAITING_TIME 3000000


volatile int	g_com[2];

void	handler(int signum, siginfo_t *info, void *ucontext)
{
	char	logline[] = "Process x received signal SIGINT\n";
	(void)ucontext;
	(void)info;
	g_com[1] = 1;
	logline[8] = '0' + g_com[0];
	write(2, logline, 34);
}

int	main(int ac, char **av)
{
	int					id;
	struct sigaction	action;
	int					elapsed;

	if (ac > 1)
		id = atoi(av[1]);
	else
		return (1);
	action.sa_sigaction = &handler;
	action.sa_flags = SA_SIGINFO;
	sigemptyset(&action.sa_mask);
	sigaddset(&action.sa_mask, SIGINT);
	if (sigaction(SIGINT, &action, NULL))
	{
		perror(NULL);
		dprintf(2, "Could not resolve sigaction for process %d\n", id);
	}
	g_com[0] = id;
	g_com[1] = 0;
	dprintf(2, "Finished setup for process %d\n", id);
	elapsed = 0;
	while (g_com[1] == 0 && elapsed < WAITING_TIME)
	{
		usleep(100);
		elapsed += 100;
	}
	dprintf(2, "Finished waiting for signal in process %d, elapsed = %d\n", id, elapsed);
	if (elapsed >= WAITING_TIME)
		return (0);
	else
		return (1);
}