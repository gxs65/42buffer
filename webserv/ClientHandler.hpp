#ifndef CLIENTHANDLER_H
# define CLIENTHANDLER_H

# include "webserv.hpp"

# define RECEIVING 1
# define SENDING 2

class	ClientHandler
{
	public:
		ClientHandler(int clientSocketFd);
		~ClientHandler();
		// Mainloop
		void			preparePollFd(struct pollfd& pfd);
		int				handleEvent(struct pollfd& pfd);
		// Accessors
		int				getSocketFd(void);

	private:
		int				receiveHTTP(void);
		int				sendHTTP(void);
		int				_socketFd;
		int				_state;
		unsigned int	_nbytesSent;
		std::string		_response;
};



#endif
