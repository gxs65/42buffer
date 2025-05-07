/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:08:40 by ilevy             #+#    #+#             */
/*   Updated: 2025/05/07 13:30:51 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hpp_files/webserv.hpp"

/////////////////////////////
// CANONICAL ORTHODOX FORM //
/////////////////////////////

Request::Request(t_mainSocket& mainSocket) : _mainSocket(mainSocket)
{
	std::cout << "Constructor for Request\n";
}

Request::~Request()
{
	std::cout << "Destructor for Request\n";
}

///////////////
// ACCESSORS //
///////////////

////////////////////
// STATIC METHODS //
////////////////////

// Checks whether the request if <buffer> is complete,
// which means the double linebreak '\r\n\r\n' and either :
// 		\ there is no "Content-Length" (no body)
// 		\ there is a "Content-Length" and the part after double linebreak (the body)
// 		  has size exactly equal to that content length
bool	Request::requestIsComplete(const std::string& buffer)
{
	size_t	header_end;
	size_t	content_length;

	header_end = buffer.find("\r\n\r\n");
	if (header_end == std::string::npos)
		return (false);
	content_length = buffer.find("Content-Length:");
	// This detects the POST method, which contains a body
	if (content_length != std::string::npos)
	{
		size_t		value_start;
		size_t		value_end;
		std::string	length_string;
		int			length;

		// This will extract the numeric value of Content-length
		value_start = buffer.find_first_not_of(" ", content_length + 15);
		value_end = buffer.find("\r\n", value_start);
		length_string = buffer.substr(value_start, value_end - value_start);
		length = std::atoi(length_string.c_str());

		// If we have received enough data (the whole body), then this is true.
		if (buffer.size() >= header_end + 4 + length)
			return (true);
		return (false);
	}
	//For GET and DELETE methods
	return (true);
}

/////////////////////
// GENERAL PARSING //
/////////////////////

// Parses the request in <raw_request> :
// 		\ parse first line (method name, resource URL, HTTP version)
// 		\ parse header lines before the double linebreak
// 		\ retrieve from resource URL the extension/pathinfo/querystring if they exist
// 		\ retrieve from 'host field' the server name and port number used as destination of request
// 		\ retrieve request body (if there is one) after double linebreak
bool	Request::parse(const std::string& raw_request)
{
	size_t	header_end;

	header_end = raw_request.find("\r\n\r\n");
	if (header_end == std::string::npos)
	{
		std::cout << "[Req::parse] parseError : Request is not complete\n";
		return (false);
	}
	if (this->parse_request(raw_request, header_end) == false)
	{
		std::cout << "[Req::parse] Error : Invalid request first line\n";
		return (false);
	}
	if (this->parse_headers(raw_request, header_end) == false)
	{
		std::cout << "[Req::parse] Error : Invalid request headers\n";
		return (false);
	}
	this->extractFromURL();
	this->extractFromHost();
	if (this->_headers.count("Content-Length"))
	{
		int contentLength = std::atoi(this->_headers["Content-Length"].c_str());
		std::string body_section = raw_request.substr(header_end + 4);
		if ((int)body_section.size() < contentLength)
		{
			std::cout << "[Req::parse] Error : Body size differs from value given by Content-Length\n";
			return (false);
		}
		this->_body = body_section.substr(0, contentLength);
	}
	this->_parsed = true;
	std::cout << "[Req::parse] Request successfully parsed\n";
	return (true);
}

// Retrieves from the request's first line : the method name, resource URI, and HTTP version
bool	Request::parse_request(const std::string& raw_request, size_t header_end)
{
	size_t			line_end;
	size_t			method_end;
	size_t			path_end;
	std::string		header_string;
	std::string		body_string;
	std::string		request_line;

	header_string = raw_request.substr(0, header_end);
	body_string = raw_request.substr(header_end + 4);
	line_end = header_string.find("\r\n");
	if (line_end == std::string::npos)
		return (false);
	request_line = header_string.substr(0,line_end);

	method_end = request_line.find(' ');
	if (method_end == std::string::npos)
		return (false);
	this->_method = request_line.substr(0, method_end);

	path_end = request_line.find(' ', method_end + 1);
	if (path_end == std::string::npos)
		return (false);

	this->_path = request_line.substr(method_end + 1, path_end - method_end - 1);
	this->_httpVersion = request_line.substr(path_end + 1);
	return (true);
}

// Checks that mandatory headers are present
// #f : add checks for Content-Length and Content-Type when there is a body (POST)
bool	Request::checkHeaders()
{
	if (this->_headers.count("Host") == 0)
	{
		std::cout << "[Req::parseHeaders] Error : Request lacks 'host' header\n";
		return (false);
	}
	return (true);
}

// For each header line, inserts it into map <_headers>
bool	Request::parse_headers(const std::string& raw_request, size_t header_end)
{
	size_t		line_start;
	size_t		line_end;
	size_t		colon_pos;
	std::string	header_string;
	std::string	line_content;

	header_string = raw_request.substr(0, header_end);
	line_start = header_string.find("\r\n") + 2;
	while (line_start < header_end)
	{
		line_end = header_string.find("\r\n", line_start);
		if (line_end == std::string::npos)
			break;
		line_content = header_string.substr(line_start, line_end - line_start);
		colon_pos = line_content.find(':');
		if (colon_pos != std::string::npos)
		{
			std::string key = line_content.substr(0, colon_pos);
			std::string value = line_content.substr(colon_pos + 1);
			key.erase(key.find_last_not_of(" ") + 1);
			value.erase(0, value.find_first_not_of(" "));
			value.erase(value.find_last_not_of(" ") + 1);
			this->_headers[key] = value;
		}
		line_start = line_end + 2;
	}
	if (this->checkHeaders() == false)
		return (false);
	return (true);
}

//////////////////////
// SPECIFIC PARSING //
//////////////////////

// Retrieves from requested resource URL in <_path> the non-mandatory elements
// 		\ file extension (after a '.')
// 		\ path info (following the file name)
// 		\ query string (last element, after a '?')
// that will be needed if the request is for a CGI script
// #f : check if query string can contain a '.', this would create pbs
void	Request::extractFromURL()
{
	unsigned long		indExtension;
	unsigned long		indEndExtension;
	unsigned long		indEndPathInfo;

	this->_extension = "";
	this->_pathInfo = "";
	this->_queryString = "";

	indExtension = this->_path.find('.');
	if (indExtension == std::string::npos) // No filename ending with an extension (so it cannot be a CGI)
		return ;
	indEndExtension = this->_path.find_first_of("/?", indExtension);
	if (indEndExtension == std::string::npos) // nothing after the filename (no path info, no query string)
	{
		this->_extension = this->_path.substr(indExtension);
		this->_filePath = this->_path;
		return ;
	}
	this->_extension = this->_path.substr(indExtension, indEndExtension - indExtension);
	this->_filePath = this->_path.substr(0, indEndExtension);

	indEndPathInfo = this->_path.find('?', indEndExtension);
	if (indEndPathInfo == indEndExtension) // filename immediately followed by query string
		this->_queryString = this->_path.substr(indEndPathInfo + 1);
	else if (indEndPathInfo != std::string::npos) // filename followed by path info, then query string
	{
		this->_pathInfo = this->_path.substr(indEndExtension, indEndPathInfo - indEndExtension);
		this->_queryString = this->_path.substr(indEndPathInfo + 1);
	}
	else // filename followed by only a path info
	{
		this->_pathInfo = this->_path.substr(indEndExtension);
		this->_queryString = "";
	}
}

// Retrieves from 'host' header field the information that the client used
// to send its request to this server
// 		\ a server name OR an IP address, before the ':'
// 		\ a port number (mandatory if different from scheme's default port,
// 			here scheme is HTTP so default port if not mentioned is 80)
void	Request::extractFromHost()
{
	std::string			host = this->_headers[std::string("Host")];
	std::string			hostPort;
	unsigned long		posSeparator;
	in_addr				addr;

	this->_hostName = "";
	hostPort = "";
	posSeparator = host.find(':');
	if (posSeparator == std::string::npos)
		this->_hostName = host;
	else
	{
		this->_hostName = host.substr(0, posSeparator);
		hostPort = host.substr(posSeparator + 1);
	}
	if ((this->_hostName.compare("localhost") == 0 && inet_aton("127.0.0.1", &addr))
		|| inet_aton(this->_hostName.c_str(), &addr))
	{
		this->_hostPortaddr.first = ntohl(addr.s_addr);
		this->_hostName = "";
	}
	else
		this->_hostPortaddr.first = this->_mainSocket.portaddr.first;
	if (hostPort.size() > 0)
		this->_hostPortaddr.second = static_cast<uint16_t>(strtol(hostPort.c_str(), NULL, 10));
	else
		this->_hostPortaddr.second = 80;
}

///////////
// UTILS //
///////////

// Logs request components
void	Request::log_request(void)
{
	std::map<std::string, std::string>::iterator	headerIt;

	std::cout << "\033[31m< REQUEST PARSED\n";
	std::cout << "Method: " << this->_method << "\n";
	std::cout << "Path: " << this->_path << "\n";
	std::cout << "Headers:\n";
	for (headerIt = this->_headers.begin(); headerIt != this->_headers.end(); ++headerIt)
		std::cout << "\t" << headerIt->first << ": " << headerIt->second << "\n";
	std::cout << "Extracted from host header :\n"
		<< "\thostName " << this->_hostName << "\n"
		<< "\tportaddr " << this->_hostPortaddr << "\n";
	std::cout << "Extracted from path " << this->_path << " :\n"
		<< "\tfilePath " << this->_filePath << "\n"
		<< "\textension " << this->_extension << "\n"
		<< "\tpathInfo " << this->_pathInfo << "\n"
		<< "\tqueryString " << this->_queryString << "\n";
	std::cout << ">\033[0m\n";
}
