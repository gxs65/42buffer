/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:48:28 by ilevy             #+#    #+#             */
/*   Updated: 2025/05/06 17:06:56 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "webserv.hpp"

// Request takes in the request from Client when static method 'requestIsComplete()'
// has verified that it is complete (ie. has a double linebread and a body of the right size)
// Request buffer should be given to 'parse_request()' method
// Parse will check the compliance of the HTTP request to the http/tcp standard
// and return false if the request is not compliant

class Request
{
	public:
		Request(t_mainSocket& _mainSocket);
		~Request();
		bool				parse(const std::string& raw_request);
		void				log_request();
		// Static method to check completeness of request
		static bool			requestIsComplete(const std::string& buffer);
		// Memory of server's config
		t_mainSocket&						_mainSocket;
		// Results of general parsing
		std::string							_method;
		std::string							_path;
		std::string							_filePath; // the path, without path info and query string
		std::string							_httpVersion;
		std::map<std::string, std::string>	_headers;
		std::string							_body;
		bool								_parsed;
		// Results of specific parsing
		std::string							_pathInfo;
		std::string							_queryString;
		std::string							_extension;
		t_portaddr							_hostPortaddr;
		std::string							_hostName;

	private:
		bool			parse_request(const std::string& raw_request, size_t header_end);
		bool			parse_headers(const std::string& raw_request, size_t header_end);
		bool			checkHeaders();
		void			extractFromURL();
		void			extractFromHost();
		
};

#endif
