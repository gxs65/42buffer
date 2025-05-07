#ifndef CLIENT_H
# define CLIENT_H

# include "webserv.hpp"
# define READSOCK_BUFFER_SIZE 1024

enum ClientState
{
	RECEIVING,
	SENDING,
	CLOSED
};

// Holds the logic handling the interaction with the client on a dedicated socket
// 		\ <preparePollFd>, called by <Server>, defines in the socket's <pollfd> structure
// 		  which events are awaited on that socket (POLLIN/POLLOUT)
// 		\ <handleEvent>, called by <Server>, manages events received on the socket
// 			~ POLLIN events : read from the socket into the <_requestBuffer>,
// 				if the <requestBuffer> holds a complete request
// 				generate its response in <_responseBuffer>
// 			~ POLLOUT events : write as much bytes as possible from <_responseBuffer> into the socket
// Registers in its constructor many informations
// about the socket on which it listens and the server's list of virtual servers
// 		to be able to determine to which virtual server requests received on the socket are directed 
class	Client
{
	public:
		Client(int socketFd, t_portaddr clientPortaddr, t_mainSocket& mainSocket, std::vector<t_vserver>& vservers);
		~Client();
		// Mainloop
		void					preparePollFd(struct pollfd& pfd);
		int						handleEvent(struct pollfd& pfd);
		// Accessors
		int						getSocketFd() const;
		t_mainSocket&			getMainSocket();

	private:
		int						receiveHTTP();
		int						handleCompleteRequest();
		int						sendHTTP();
		// Memory of server's config
		int								_socketFd;
		t_mainSocket&					_mainSocket;
		t_portaddr						_clientPortAddr;
		std::vector<t_vserver>&			_vservers;
		// Internal state
		int								_state;
		unsigned int					_nbytesSent_tmp;
		std::string						_request_buffer;
		std::string						_response_buffer;
		bool							_response_sent;
};

#endif
