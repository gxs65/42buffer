/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:48:28 by ilevy             #+#    #+#             */
/*   Updated: 2025/05/05 09:33:12 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "webserv.hpp"
# define READPIPE_BUFFER_SIZE 16UL
# define WRITEPIPE_BUFFER_SIZE 16UL

class Response
{
	private:
		// Memory of server's config
		Request&			_request;
		t_portaddr			_clientPortaddr;
		// Identification of virtual server and location to which request is directed
		t_vserver*			_vserv;
		t_location*			_location;
		// Internal state
		std::string			_responseBuffer;
		std::vector<char>	_cgiOutput;
		// Identify virtual server targeted by request
		t_vserver*			identifyVserver(std::vector<t_vserver>& vservers, std::set<int>& vservIndexes);
		t_location*			identifyLocation();
		// Find path to resource
		int					pathToResource(std::string& fullPath, bool checkExec);
		int					buildFullPath(std::string& fullPath);
		int					checkResolvedFullPath(std::string& fullPath);
		bool				isCGIRequest();
		// Process CGIs output
		int					parseCGIStatusLine(std::string& line, std::string& statusCode, std::string& statusDesc);
		int					parseCGIHeaderLine(std::string& line, std::map<std::string, std::string>& cgiHeaders);
		int					parseCGIHeaders(std::string& headersString,
								std::map<std::string, std::string>& cgiHeaders, std::string& statusCode, std::string& statusDesc);
		unsigned long		findCgiOutputHeader();
		int					processCGIOutput();
		// Handle CGIs
		char**				createEnvArray(std::map<std::string, std::string>& env);
		char**				setupCGIEnv(std::string& fullPath);
		void				childExecuteCGI(std::string& fullPath, int fdInput, int fdOutput);
		int					feedBodyToChild(int fdOutput);
		int					readResponseFromChild(int fdInput);
		int					waitForChildCGI(pid_t pid, int *pipeRequest, int *pipeResponse);
		int					forkCGI(std::string& fullPath);
		int					handleCGI();
		// Handle Non-CGIs
		void				makeErrorResponse(int errcode);
		int					makeFileResponse(std::string& fullPath);
		std::string			getResponseBodyType();
		int					handleGet();

	public:
		Response(Request& request, t_portaddr clientPortaddr, 
			std::vector<t_vserver>& vservers, std::set<int>& vservIndexes);
		~Response();
		int				produceResponse();
		void			logResponseBuffer();
		std::string&	getResponseBuffer();

};

#endif
