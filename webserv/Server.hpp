#ifndef SERVER_H
# define SERVER_H

# include "webserv.hpp"

# define MAX_QUEUED_CONNECTIONS 10

class	Server
{
	public:
		Server(std::string cfgFileName);
		~Server();
		int			startServer(void);
		int			serverLoop(void);
		void		stopServer(void);


	private:
		uint16_t					_port;
		int							_mainSocketFd;
		std::vector<ClientHandler*>	clientHandlers;
		// Mainloop
		int			goThroughEvents(struct pollfd* pfds, int nfds);
		int			handleNewConnection(void);
		int			handleOldConnection(int clientInd, struct pollfd& pfd);
		// Utils
		void		removeClosedConnections(void);
		int			createPollFds(struct pollfd **pfds);
		void		displayEvents(short revents);
};

#endif
