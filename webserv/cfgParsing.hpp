#ifndef CFGPARSING_HPP
# define CFGPARSING_HPP

# include "webserv.hpp"

typedef struct s_location
{
	std::string							locationPath;
	std::string							redirection; // cannot be set if anything else is set
	std::string							rootPath;
	std::set<std::string>				acceptedMethods;
	bool								acceptedMethodsSet;
	std::map<std::string, std::string>	cgiExtensions;
	std::string							uploadPath;
	std::string							defaultFileWhenDir;
	bool								autoIndex; // cannot be True if <defaultFileWhenDir> is set
	bool								autoIndexSet;
}				t_location;

typedef struct s_vserver
{
	uint32_t					listenAddress; // stored in host byte order, ie. little-endian
	uint16_t					listenPort;
	bool						listenSet;
	std::set<std::string>		serverNames;
	std::map<int, std::string>	errorPages;
	long						maxRequestBodySize;
	std::vector<t_location>		locations;
}				t_vserver;

int	parseCfgFile(std::string& cfgFileName, std::vector<t_vserver>& vservers);

#endif