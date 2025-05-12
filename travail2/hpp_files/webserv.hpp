/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: administyrateur <administyrateur@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 15:19:26 by ilevy             #+#    #+#             */
/*   Updated: 2025/05/09 20:48:12 by administyra      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

// Libraries
# include "libraryUsed.hpp"

// Global variable for logs
extern int	LOGSV;
// Global variable for communication of signal SIGINT
extern volatile int	g_global_signal;

# define GET 1
# define POST 2
# define PUT 3
# define DELETE 4

// Classes and structures
class	Server;
class	Client;
class	Request;
class	Response;
typedef struct s_vserver t_vserver;
typedef struct s_location t_location;
typedef struct s_mainSocket t_mainSocket;
typedef std::pair<uint32_t, uint16_t> t_portaddr; // an IP address (int) + a port number (short)

// Other headers
# include "Client.hpp"
# include "Server.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "cfgParsing.hpp"
# include "utilsTemplates.tpp"

// Misc functions
int			logError(std::string msg, bool displayErrno);
void		logTime(int linebreak);
void		free2dimArray(char** arr, int size);
void		trimString(std::string& s);
int			splitString(std::string& orig, std::vector<std::string>& tokens, std::string seps);
char*		ft_strjoinDefsize(char* s1, char* s2, size_t sizes2);
// Template functions
template <class T>	std::string	itostr(T number);

#endif
