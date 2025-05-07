#include "../hpp_files/webserv.hpp"

/////////////////////////////
// CANONICAL ORTHODOX FORM //
/////////////////////////////

// Parameters to constructor :
// 		\ <clientSocketFd> is the fd of the socket connecting the server to this instance
// 		\ <mainSocket> is (a reference to) the structure representing the main socket
// 		  where this client first reached the server and was accepted
// 			-> necessary because it holds in field <vservIndexes> the indexes of the virtual servers
// 			   listening on a list of portaddr which must include <portaddr>
// 					<=> the indexes of the virtual servers to which the client has access
// 		\ <vservers> is (a reference to) a list of structures representing the virtual servers
// 			(the indexes in <mainSocket.vservIndexes> are positions in this list)
Client::Client(int socketFd, t_portaddr clientPortaddr,
	t_mainSocket& mainSocket, std::vector<t_vserver>& vservers) :
	_socketFd(socketFd), _mainSocket(mainSocket), _vservers(vservers)
{
	std::cout << "Parameter constructor for ClientHandler\n";
	std::cout << "\t(dedicated socket has fd " << this->_socketFd << " ; main socket had fd "
		<< this->_mainSocket.fd << " listening on " << this->_mainSocket.portaddr << ")\n";
	this->_state = RECEIVING;
	this->_response_sent = false;
	this->_clientPortAddr = clientPortaddr;
}

// Destructor closes the dedicated socket connected to the client
Client::~Client(void)
{
	std::cout << "Destructor for Client\n";
	close(this->_socketFd);
}

///////////////
// ACCESSORS //
///////////////

int	Client::getSocketFd() const
{
	return (this->_socketFd);
}

t_mainSocket&	Client::getMainSocket(void)
{
	return (this->_mainSocket);
}

////////////////////
// MAIN INTERFACE //
////////////////////

// Fills the <poll> struct with the expected event information
// 		\ fd of the socket dedicated to this client
// 		\ event flags depending on whether this client must send a request,
// 			or data must be sent to this client
void	Client::preparePollFd(struct pollfd& pfd)
{
	pfd.fd = this->_socketFd;
	if (this->_state == RECEIVING)
		pfd.events = POLLIN;
	else if (this->_state == SENDING)
		pfd.events = POLLOUT;
}

// Redirects an event received on the socket of this client to the correct method
// (apparently event POLLHUP is never received for sockets)
int		Client::handleEvent(struct pollfd& pfd)
{
	if ((pfd.revents & POLLIN) && this->_state == RECEIVING)
		return (this->receiveHTTP());
	if ((pfd.revents & POLLOUT) && this->_state == SENDING)
		return (this->sendHTTP());
	if (pfd.revents & POLLHUP && this->_state != RECEIVING)
	{
		std::cout << "\t\t[CH::handleEvent] hangup from client with socket at fd " << this->_socketFd << "\n";
		return (2);
	}
	return (0);
}

// Uses function <send> to send the data in <_response> through the socket
// The socket may be too small to accept the full <_response>,
// 		so the progress in number of bytes sent is registered in <_nBytesSent>
// 		in order to send the next part of <_response> at the next call of <sendHTTP>
// -> state is changed from "SENDING" to "RECEIVING" only when the full <_response> was sent
// /!\ TCP connections are two-way, like 2 pipes (client->server) and (server->client)
// 	   so this function can send response bytes even while request bytes from the client are still in socket
int		Client::sendHTTP(void)
{
	unsigned int	nBytesWritten;

	std::cout << "\t\t[CH::sendHTTP] sending to client with socket at fd " << this->_socketFd
		<< " ; bytes sending progress : " << this->_nbytesSent_tmp << " / " << this->_response_buffer.size() << "\n";
	nBytesWritten = send(this->_socketFd, this->_response_buffer.c_str() + this->_nbytesSent_tmp,
											this->_response_buffer.size() - this->_nbytesSent_tmp, 0);
	if (nBytesWritten <= 0)
		return (logError("[CH::receiveHTTP] error when writing to socket", 1));
	std::cout << "\t\t[CH::sendHTTP] sent " << nBytesWritten << " bytes\n";
	this->_nbytesSent_tmp += nBytesWritten;
	if (this->_nbytesSent_tmp >= this->_response_buffer.size())
	{
		std::cout << "\t\t[CH::sendHTTP] full response sent\n";
		this->_state = RECEIVING;
		this->_response_sent = 1;
	}
	return (0);
}

// On POLLIN event, reads bytes from the socket, adds them to the <_requestBuffer>,
// then checks if the request in <_requestBuffer> is complete and must be answered
// 		(if not, this instance will remain in RECEIVING mode awaiting further POLLIN)
// UNIX API for tcp connections defines that end of connection with a client
// is signified by raising a POLLIN event for an empty socket
// 		so if the <read> call reads 0 bytes, <receiveHTTP> returns 2 to tell the server
// 		that this connections has ended and this instance of <Client> can be deleted
int		Client::receiveHTTP(void)
{
	char			buffer[READSOCK_BUFFER_SIZE + 1];
	unsigned int	nBytesRead;

	std::cout << "\t\t[CH::receiveHTTP] reading from client with socket at fd " << this->_socketFd << "\n";
	bzero(buffer, READSOCK_BUFFER_SIZE + 1);
	nBytesRead = recv(this->_socketFd, buffer, READSOCK_BUFFER_SIZE, 0);
	if (nBytesRead < 0)
		return (logError("[CH::receiveHTTP] error when reading from socket", 1));
	if (nBytesRead == 0)
	{
		std::cout << "\t\t[CH::receiveHTTP] POLLIN while nothing to read from socket -> end of connection\n";
		return (2);
	}
	std::cout << "\t\t[CH::receiveHTTP] Content read from socket :\n";
	std::cout << "\033[32m< HTTP REQUEST\n" << std::string(buffer) << "\n>\033[0m\n";
	this->_request_buffer.append(buffer, nBytesRead);
	if (Request::requestIsComplete(this->_request_buffer))
		this->handleCompleteRequest();
	return (0);
}

// When a request in <_requestBuffer> is complete :
// 		\ create instance of <Request> to parse the request
// 		\ check request compliance with http standard thanks to return value of <Request.parse>
// 		\ create Instance of <Response> to generate response
// 		\ go into SENDING mode to await POLLOUT events on the socket to be able to send the response
// #f : correct coherence of log and error handling
int	Client::handleCompleteRequest(void)
{
	Request		request(this->_mainSocket);

	if (!request.parse(this->_request_buffer))
	{
		std::cout << "\t\t[CH::handleCompleteRequest] bad Request\n"; // #f : Response static method for bad request
		request.log_request();
		this->_response_buffer = "HTTP/1.1 400 Bad Request\r\n\r\n";
	}
	else
	{
		request.log_request();
		Response	response(request, this->_clientPortAddr,
					this->_vservers, this->_mainSocket.vservIndexes);

		if (response.produceResponse())
		{
			response.logResponseBuffer();
			return (1);
		}
		response.logResponseBuffer();
		this->_response_buffer = response.getResponseBuffer();
	}
	this->_request_buffer = "";
	this->_state = SENDING;
	this->_response_sent = false;
	this->_nbytesSent_tmp = 0;
	return (0);
}
