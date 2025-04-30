#include "ClientHandler.hpp"

/////////////////////////////
// CANONICAL ORTHODOX FORM //
/////////////////////////////

// <clientSocketFd> is the fd of the socket connecting the server to this client
ClientHandler::ClientHandler(int clientSocketFd)
{
	std::cout << "Parameter constructor for ClientHandler\n";
	this->_socketFd = clientSocketFd;
	this->_state = RECEIVING;
}

ClientHandler::~ClientHandler(void)
{
	std::cout << "Destructor for ClientHandler\n";
	close(this->_socketFd);
}

///////////////
// ACCESSORS //
///////////////

int	ClientHandler::getSocketFd()
{
	return (this->_socketFd);
}

////////////////////
// MAIN INTERFACE //
////////////////////

// Fills the <poll> struct with the expected event information
// 		\ fd of the socket dedicated to this client
// 		\ event flags depending on whether this client must send a request,
// 			or data must be sent to this client
void	ClientHandler::preparePollFd(struct pollfd& pfd)
{
	pfd.fd = this->_socketFd;
	if (this->_state == RECEIVING)
		pfd.events = POLLIN;
	else if (this->_state == SENDING)
		pfd.events = POLLOUT;
}

// Redirects an event received on the socket of this client to the correct method
// (apparently event POLLHUP is never received for sockets)
int		ClientHandler::handleEvent(struct pollfd& pfd)
{
	if ((pfd.revents & POLLIN) && this->_state == RECEIVING)
		return (this->receiveHTTP());
	if ((pfd.revents & POLLOUT) && this->_state == SENDING)
		return (this->sendHTTP());
	if (pfd.revents & POLLHUP && this->_state != RECEIVING)
	{
		std::cout << "\t[CH::handleEvent] hangup from client with socket at fd " << this->_socketFd << "\n";
		return (2);
	}
	return (0);
}

// Uses function <send> to send the data in <_response> through the socket
// The socket may be too small to accept the full <_response>,
// 		so the progress in number of bytes sent is registered in <nBytesSent>
// 		in order to send the next part of <_response> at the next call of <sendHTTP>
// -> state is changed from "SENDING" to "RECEIVING" only when the full <_response> was sent
int		ClientHandler::sendHTTP(void)
{
	unsigned int	nBytesWritten;

	std::cout << "\t[CH::sendHTTP] sending to client with socket at fd " << this->_socketFd
		<< " ; bytes sending progress : " << this->_nbytesSent << " / " << this->_response.size() << "\n";
	nBytesWritten = send(this->_socketFd, this->_response.c_str() + this->_nbytesSent,
											this->_response.size() - this->_nbytesSent, 0);
	if (nBytesWritten <= 0)
		return (logError("\t[CH::receiveHTTP] error when writing to socket", 1));
	std::cout << "\t[CH::sendHTTP] sent " << nBytesWritten << "bytes\n";
	this->_nbytesSent += nBytesWritten;
	if (this->_nbytesSent >= this->_response.size())
	{
		std::cout << "\t[CH::sendHTTP] full response sent\n";
		this->_state = RECEIVING;
	}
	return (0);
}

// #f : check if there is still stuff to read, pass to SENDING only when fully read
// Reads the client's request message in the socket,
// 		then generates a <Request> instance that will parse it
// 		and determine if a <_response> must be sent
int		ClientHandler::receiveHTTP(void)
{
	char			buffer[1024];
	unsigned int	nBytesRead;

	std::cout << "\t[CH::receiveHTTP] reading from client with socket at fd " << this->_socketFd << "\n";
	bzero(buffer, 1024);
	nBytesRead = recv(this->_socketFd, buffer, 1024, 0);
	if (nBytesRead < 0)
		return (logError("\t[CH::receiveHTTP] error when reading from socket", 1));
	if (nBytesRead == 0)
	{
		std::cout << "\t[CH::receiveHTTP] POLLIN while nothing to read from socket -> end of connection\n";
		return (2);
	}
	std::cout << "\t[CH::receiveHTTP] Content read from socket"
		<< " :\n\033[32m< HTTP ---\n" << std::string(buffer) << "\n--- HTTP >\033[0m\n";
	
	// #f : determine based on request if there is something to send and its size 
	this->_state = SENDING;
	this->_nbytesSent = 0;
	this->_response = "Ceci est la reponse\n";
	return (0);
}
