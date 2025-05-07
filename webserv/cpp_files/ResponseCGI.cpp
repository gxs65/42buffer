#include "../hpp_files/webserv.hpp"

//////////////////////////
// HANDLING CGI - CHILD //
//////////////////////////

char**	Response::createEnvArray(std::map<std::string, std::string>& env)
{
	std::map<std::string, std::string>::iterator	envIt;
	char**											envArray;
	unsigned int									ind;
	char*											envVar;
	std::string										envVarStr;

	ind = 0;
	envArray = new char*[env.size()];
	for (envIt = env.begin(); envIt != env.end(); envIt++)
	{
		envVarStr = (*envIt).first + std::string("=") + (*envIt).second;
		envVar = strdup(envVarStr.c_str());
		envArray[ind] = envVar;
		ind++;
	}
	return (envArray);
}

// #f : chdir
// #f : check existence of content length and content type for requests with a body
char**	Response::setupCGIEnv(std::string& fullPath)
{
	std::map<std::string, std::string>&				headers = this->_request._headers;
	std::map<std::string, std::string>				env;
	in_addr											clientAddress;

	clientAddress.s_addr = htonl(this->_clientPortaddr.first);
	env["PATH_INFO"] = this->_request._pathInfo.c_str();
	env["QUERY_STRING"] = this->_request._queryString.c_str();
	env["REQUEST_METHOD"] = this->_request._method.c_str();
	if (this->_request._method == "POST")
	{
		env["CONTENT_LENGTH"] = itostr(this->_request._body.size()).c_str();
		env["CONTENT_TYPE"] = headers["Content-Type"].c_str();
		// std::cout << "CGI CHILD env for POST request :\n"
		// 	<< "\tCONTENT_LENGTH to " << this->_request._body.size() << "\n"
		// 	<< "\tCONTENT_TYPE to " << headers["Content-Type"] << "\n"
		// 	<< "\tREQUEST_METHOD to " << this->_request._method << "\n";
	}
	env["GATEWAY_INTERFACE"] = "CGI/1.1";
	env["REMOTE_ADDR"] = std::string(inet_ntoa(clientAddress));
	env["SCRIPT_NAME"] = fullPath.c_str();
	env["SERVER_NAME"] = headers["Host"].c_str();
	env["SERVER_PORT"] = itostr(this->_clientPortaddr.second).c_str();
	env["SERVER_PROTOCOL"] = "HTTP/1.1";
	env["SERVER_SOFTWARE"] = "Webserv";
	return (this->createEnvArray(env));
}

void	Response::childExecuteCGI(std::string& fullPath, int fdInput, int fdOutput)
{
	char**								argv;
	char**								envArray;

	std::cout << "CGI CHILD : executes with input from fd " << fdInput
		<< ", output to fd " << fdOutput << "\n";
	if (dup2(fdInput, 0) == -1 || dup2(fdOutput, 1) == -1)
	{
		logError("CGI CHILD : Error: 'dup2' failure", 1);
		exit(127);
	}
	argv = static_cast<char **>(malloc(1 * sizeof(char *)));
	if (!argv)
	{
		logError("CGI CHILD : Error: 'malloc' failure", 1);
		exit(127);
	}
	argv[0] = strdup(fullPath.c_str());
	if (!argv[0])
	{
		free(argv);
		logError("CGI CHILD : Error: 'malloc' failure", 1);
		exit(127);
	}
	envArray = this->setupCGIEnv(fullPath);
	if (execve(argv[0], argv, envArray))
	{
		free(argv);
		logError("CGI CHILD : Error: call to 'execve' failed", 1);
		exit(127);
	}
}

///////////////////////////////////////
// HANDLING CGI - PROCESS CGI OUTPUT //
///////////////////////////////////////

unsigned long	Response::findCgiOutputHeader()
{
	unsigned long	ind = 0;

	while (ind + 1 < this->_cgiOutput.size())
	{
		if (this->_cgiOutput[ind] == '\n' && this->_cgiOutput[ind + 1] == '\n')
			return (ind);
	}
	return (0);
}

int	Response::parseCGIStatusLine(std::string& line, std::string& statusCode, std::string& statusDesc)
{
	unsigned long	indBeginCode;
	unsigned long	indEndCode;
	std::string		codeLit;

	indBeginCode = line.find(':', 0) + 1;
	while (indBeginCode < line.size() && line[indBeginCode] == ' ')
		indBeginCode++;
	if (indBeginCode >= line.size())
		return (1);
	indEndCode = line.find(' ', indBeginCode);
	if (indEndCode == std::string::npos || indEndCode == indBeginCode)
		return (1);
	codeLit = line.substr(indBeginCode, indBeginCode - indEndCode);
	if (invalidReturnCode(codeLit))
		return (1);
	statusCode = codeLit;
	statusDesc = line.substr(indEndCode + 1);
	return (0);
}

int	Response::parseCGIHeaderLine(std::string& line, std::map<std::string, std::string>& cgiHeaders)
{
	;
}

int	Response::parseCGIHeaders(std::string& headersString,
	std::map<std::string, std::string>& cgiHeaders, std::string& statusCode, std::string& statusDesc)
{
	std::stringstream	parserStream;
	std::string			line;

	while (std::getline(parserStream, line))
	{
		if (line.find("Status:", 0) == 0)
		{
			if (this->parseCGIStatusLine(line, statusCode, statusDesc))
				return (1);
		}
		else if (this->parseCGIHeaderLine(line, cgiHeaders))
			return (1);
	}
	if (statusCode.empty())
		return (1);
	return (0);
}

int	Response::processCGIOutput()
{
	unsigned long						posEndHeaders;
	std::string							headersString;
	std::map<std::string, std::string>	cgiHeaders;
	std::string							statusCode = "";
	std::string							statusDesc;

	posEndHeaders = this->findCgiOutputHeader();
	if (posEndHeaders == 0)
		return (makeErrorResponse(502), 0); // #f : check which errcode to use for "invalid CGI output"
	headersString = std::string(this->_cgiOutput.begin(), this->_cgiOutput.begin() + posEndHeaders);
	std::cout << "\textracted headers string from CGI output :\n" << headersString << "\n";
	if (this->parseCGIHeaders(headersString, cgiHeaders, statusCode, statusDesc))
		return (makeErrorResponse(502), 0);
	this->_responseBuffer = "";
}

///////////////////////////
// HANDLING CGI - PARENT //
///////////////////////////

int		Response::feedBodyToChild(int fdOutput)
{
	long			nBytesWritten;
	unsigned long	nBytesToWrite;
	unsigned long	posInBody = 0;
	unsigned long	bodySize;

	std::cout << "\tfeeding POST request body to CGI child\n";
	bodySize = this->_request._body.size();
	while (posInBody < bodySize)
	{
		nBytesToWrite = std::min(WRITEPIPE_BUFFER_SIZE, bodySize - posInBody);
		nBytesWritten = write(fdOutput,
			this->_request._body.substr(posInBody, nBytesToWrite).c_str(), nBytesToWrite);
		if (nBytesWritten <= 0)
			return (close(fdOutput), 1);
		posInBody += nBytesWritten;
	}
	close(fdOutput);
	return (0);
}

int		Response::readResponseFromChild(int fdInput)
{
	long	nBytesRead = 1;
	char	buffer[READPIPE_BUFFER_SIZE + 1];

	this->_cgiOutput.clear();
	while (nBytesRead > 0)
	{
		bzero(buffer, READPIPE_BUFFER_SIZE + 1);
		nBytesRead = read(fdInput, buffer, READPIPE_BUFFER_SIZE);
		if (nBytesRead == -1)
			return (close(fdInput), 1);
		else if (nBytesRead > 0)
			this->_cgiOutput.insert(this->_cgiOutput.end(), buffer, buffer + nBytesRead);
	}
	close(fdInput);
	return (0);
}

int		Response::waitForChildCGI(pid_t pid, int *pipeRequest, int *pipeResponse)
{
	int		exitStatus;

	std::cout << "\tCGI child successfully forked\n";
	close(pipeRequest[0]);
	close(pipeResponse[1]);
	if (this->_request._method == "POST")
	{
		if (this->feedBodyToChild(pipeRequest[1]))
			return (logError("[Res::CGI] Error: unable to feed POST request body to CGI program", 1));
	}
	else
		close(pipeRequest[1]);
	if (this->readResponseFromChild(pipeResponse[0]))
		return (logError("[Res::CGI] Error: unable to read response from CGI program", 1));
	waitpid(pid, &exitStatus, 0);
	if (WIFEXITED(exitStatus))
		exitStatus = WEXITSTATUS(exitStatus);
	else
		return (logError("[Res::CGI] Error: CGI program did not exit", 1));
	if (exitStatus == 127)
		return (logError("[Res::CGI] Error: CGI program could not be executed", 1));
	std::cout << "\t[Res::CGI] CGI program terminated with exit status " << exitStatus << "\n";
	return (this->processCGIOutput());
}

int		Response::forkCGI(std::string& fullPath)
{
	int		pipeRequest[2];
	int		pipeResponse[2];
	pid_t	pid;

	if (pipe(pipeRequest) == -1)
		return (logError("Error: unable to create pipe for CGI\n", 1));
	if (pipe(pipeResponse) == -1)
		return (logError("Error: unable to create pipe for CGI\n", 1));
	pid = fork();
	if (pid == -1)
		return (logError("Error: unable to fork child process for CGI\n", 1));
	else if (pid == 0)
	{
		close(pipeRequest[1]);
		close(pipeResponse[0]);
		this->childExecuteCGI(fullPath, pipeRequest[0], pipeResponse[1]);
		return (0);
	}
	else
		return (this->waitForChildCGI(pid, pipeRequest, pipeResponse));
}

// #f : check for executability
int	Response::handleCGI()
{
	std::string	fullPath;

	if (this->pathToResource(fullPath, 1))
		return (0);
	return (this->forkCGI(fullPath));
}
