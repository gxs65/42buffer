#ifndef WEBSERV_HPP
# define WEBSERV_HPP

// Generic modules
# include <cctype>
# include <string>
# include <iostream>
# include <stdlib.h>
# include <strings.h>
# include <unistd.h>
# include <limits>
# include <errno.h>
# include <vector>
# include <map>
# include <stack>
# include <set>
# include <fstream>
# include <fcntl.h>
# include <sys/stat.h>
# include <csignal>


// Sockets modules
# include <sys/socket.h>
# include <arpa/inet.h>
# include <poll.h>

// Classes
class	Server;
class	ClientHandler;
# include "Server.hpp"
# include "ClientHandler.hpp"
# include "cfgParsing.hpp"

# define GET 1
# define POST 2
# define PUT 3
# define DELETE 4

// Global variable for communication of signal SIGINT
extern volatile int	g_global_signal;

int	logError(std::string msg, bool displayErrno);

#endif