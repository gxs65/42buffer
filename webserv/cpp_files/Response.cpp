#include "Response.hpp"

/////////////////////////////
// CANONICAL ORTHODOX FORM //
/////////////////////////////

// Parameters to constructor :
// 		\ <request> is a reference to the instance of Request that parsed the current request
// 		\ <clientPortaddr> is the portaddr used for the connection ON THE CLIENT SIDE
// 				(ie. the IP address of the client device, and the port on which client receives packets)
// 			-> necessary because CGI specification requires this information to be put into env
// 		\ <vservIndexes> is (a reference to) the list of vservers listening
// 		  on the server portaddr on which the connection is occurring (<=> on which the request was received)
// 			-> necessary to identify which vserver should respond to the request
// 		\ <vservers> is (a reference to) a list of structures representing the virtual servers
// 			(the indexes in <mainSocket.vservIndexes> are positions in this list)
Response::Response(Request& request, t_portaddr clientPortaddr,
	std::vector<t_vserver>& vservers, std::set<int>& vservIndexes) : _request(request)
{
	std::cout << "Constructor for Response\n";
	this->_clientPortaddr = clientPortaddr;
	this->_vserv = this->identifyVserver(vservers, vservIndexes);
	this->_location = this->identifyLocation();
}

Response::~Response()
{
	std::cout << "Destructor for Response\n";
}

///////////////
// ACCESSORS //
///////////////

std::string&	Response::getResponseBuffer(void)
{
	return (this->_responseBuffer);
}

/////////////////////////////////////
// VSERVER/LOCATION IDENTIFICATION //
/////////////////////////////////////

// #f : reorganise with intermediary functions
t_vserver*	Response::identifyVserver(std::vector<t_vserver>& vservers, std::set<int>& vservIndexes)
{	
	std::set<int>::iterator					vservInd;
	t_vserver*								vserv;
	std::set<t_portaddr >::iterator			vservPortaddr;
	std::set<std::string>::iterator			vservName;
	bool									portaddrMatch;
	bool									nameMatch;

	// try matching host description in request with virtual servers
	for (vservInd = vservIndexes.begin(); vservInd != vservIndexes.end(); vservInd++)
	{
		vserv = &(vservers[*vservInd]);
		// Check if 1 of virtual server's portaddrs matches host portaddr
		portaddrMatch = 0;
		for (vservPortaddr = vserv->portaddrs.begin();
			vservPortaddr != vserv->portaddrs.end(); vservPortaddr++)
		{
			if (portaddrContains(*vservPortaddr, this->_request._hostPortaddr))
			{
				portaddrMatch = 1;
				break;
			}
		}
		// if no match, it cannot be this virtual server
		if (!portaddrMatch)
			continue;
		// if portaddr match and no hostName, then portaddr match is enough
		if (this->_request._hostName.size() == 0)
			return (vserv);
		// if portaddr match and hostName defined,
		// then check if hostName matches 1 of the virtual server's name
		nameMatch = 0;
		for (vservName = vserv->serverNames.begin();
			vservName != vserv->serverNames.end(); vservName++)
		{
			if (this->_request._hostName == *vservName)
			{
				nameMatch = 1;
				break;
			}
		}
		if (nameMatch)
			return (vserv);
	}
	// when no virtual server matches, return the first server in the list
	return (&(vservers.front()));
}

t_location*	Response::identifyLocation()
{
	t_location*		bestMatch;
	size_t			bestLength = 0;
	unsigned int	ind;
	std::string		locationPath;

	bestMatch = NULL;
	for (ind = 0; ind < this->_vserv->locations.size(); ind++)
	{
		locationPath = this->_vserv->locations[ind].locationPath;
		if (this->_request._path.compare(0, locationPath.size(), locationPath) == 0)
		{
			if (locationPath.size() > bestLength)
			{
				bestLength = locationPath.size();
				bestMatch = &(this->_vserv->locations[ind]);
			}
		}
	}
	return (bestMatch);
}

///////////////////////
// PATH TO RESOURCES //
///////////////////////

// Builds the path to the resource in the server's arborescence
// 		\ if a location is defined, this location may have root/alias directive
// 			~ root : rootPath prepended to path in URL
// 			~ alias : aliasPath replaces the part of path in URL corresponding to locationPath
// 		\ if no location undefined or without root/alias, verser may have a root directive
// 		\ in all other cases, the resource can't be found (404)
int	Response::buildFullPath(std::string& fullPath)
{
	if (this->_location && !(this->_location->rootPath.empty()))
		fullPath = this->_location->rootPath + this->_request._filePath;
	else if (this->_location && !(this->_location->aliasPath.empty()))
	{
		fullPath = this->_location->aliasPath
				 + this->_request._filePath.substr(this->_location->locationPath.size() - 1);
	}
	else if (!(this->_vserv->rootPath.empty()))
		fullPath = this->_vserv->rootPath + this->_request._filePath;
	else
		return (1);
	return (0);
}

// Checks that the resolved full path (after executing links/'..')
// begins with the base path used as a result of root/alias directives
// Necessary for security reasons : ensure that the request accesses resources
// only within the part of the arborescence given by the config file
// After checks succeeded, <fullPath> is indeed transformed into resolved full path
int	Response::checkResolvedFullPath(std::string& fullPath)
{
	char		realBase[PATH_MAX];
	char		realFile[PATH_MAX];
	std::string	basePath;

	if (this->_location && !(this->_location->rootPath.empty()))
		basePath = this->_location->rootPath;
	else if (this->_location && !(this->_location->aliasPath.empty()))
		basePath = this->_location->aliasPath;
	else
		basePath = this->_vserv->rootPath;
	if (!realpath(basePath.c_str(), realBase) || !realpath(fullPath.c_str(), realFile))
		return (1);
	if (std::string(realFile).find(realBase) != 0)
		return (1);
	fullPath = std::string(realFile);
	return (0);
}

// Transforms the <_path> parsed by the Request instance
// into the real path to the resource in server's arborescence, by
// 		\ identifying empty <_path> / <_path> pointing to directories
// 		\ building the full path according to 'root'/'alias' directives from config file
// 		\ check if the resolved full path is safe (see <checkResolvedFullPath> description)
// 		\ check that the resulting file exists, and is executable if <checkExec> is set
int	Response::pathToResource(std::string& fullPath, bool checkExec)
{
	struct stat	fileStat;

	if (this->_request._filePath.empty() || this->_request._filePath[0] != '/')
		this->_request._filePath = "/";
	std::cout << "\tURL path to resource : " << this->_request._filePath << "\n";
	if (this->buildFullPath(fullPath))
		return (this->makeErrorResponse(404), 1);
	std::cout << "\tfull path built : " << fullPath << "\n";
	if (this->checkResolvedFullPath(fullPath))
		return (this->makeErrorResponse(404), 1);
	std::cout << "\tfull path resolved : " << fullPath << "\n";
	if (stat(fullPath.c_str(), &fileStat) != 0 || !S_ISREG(fileStat.st_mode))
		return (this->makeErrorResponse(404), 1);
	if (checkExec && (fileStat.st_mode & S_IXUSR))
	std::cout << "\tfile exists and meets requirements\n";
	return (0);
}

///////////////////////////
// HANDLING GET REQUESTS //
///////////////////////////

// Generates the response for an error according to error type given by errcode
void	Response::makeErrorResponse(int errcode)
{
	std::stringstream	ss;

	ss << "HTTP/1.1 " << errcode << " Error\r\n"
		<< "Content-Length: 16\r\n"
		<< "Content-Type: text/html\r\n\r\n"
		<< "<html>" << errcode << "</html>";
	this->_responseBuffer = ss.str();
}

// Opens the requested resource, generates the response headers,
// then appends the resource's content to the response as its body
int	Response::makeFileResponse(std::string& fullPath)
{
	std::ifstream		file;
	std::ostringstream	bodyStream;
	std::string			body;
	std::ostringstream	response;


	file.open(fullPath.c_str(), std::ios::in | std::ios::binary);
	if (!file)
		return (this->makeErrorResponse(500), 0);
	bodyStream << file.rdbuf();
	body = bodyStream.str();
	response << "HTTP/1.1 200 OK\r\n"
			 << "Content-Type: " << this->getResponseBodyType() << "\r\n"
			 << "Content-Length: " << body.size() << "\r\n"
			 << "\r\n"
			 << body;
	this->_responseBuffer = response.str().c_str();
	return (0);
}

// Returns a string containing the exact MIME type of the returned resource
// according to its file extension
std::string	Response::getResponseBodyType()
{
	if (this->_request._extension == ".html")
		return ("text/html");
	if (this->_request._extension == ".jpg" || this->_request._extension == ".jpeg")
		return ("image/jpeg");
	if (this->_request._extension == ".png")
		return ("image/png");
	if (this->_request._extension == ".css")
		return ("text/css");
	if (this->_request._extension == ".js")
		return ("application/javascript");
	return ("application/octet-stream");
}

// Checks that the requested resource exists before generating the response containing its content
int	Response::handleGet()
{
	std::string	fullPath;

	if (this->pathToResource(fullPath, 0))
		return (0);
	return (this->makeFileResponse(fullPath));
}

///////////////////////
// GENERATE RESPONSE //
///////////////////////

// Check if the request demands the execution of a CGI script/program, with 2 conditions
// 		\ request method must be GET or POST
// 		\ request must be directed to a location, and the requested file
// 		  must have an extension belonging to the CGI extensions list of that location
// 			(extensions will mostly be '.php' or '.py',
// 			 but any extension can be defined as CGI extension in the config file)
bool	Response::isCGIRequest()
{
	unsigned int	ind;
	
	if (!(this->_location) || this->_location->cgiExtensions.size() == 0
		|| (this->_request._method != "GET" && this->_request._method != "POST"))
		return (0);
	for (ind = 0; ind < this->_location->cgiExtensions.size(); ind++)
	{
		if (this->_request._extension == this->_location->cgiExtensions[ind])
			return (1);
	}
	return (0);
}

// Displays the current state of the response buffer
void	Response::logResponseBuffer()
{
	std::cout << "[Res::Response] Generated response in buffer :\n";
	std::cout << "\033[33m< HTTP\n" << this->_responseBuffer << "\n>\033[0m\n";
}

// Calls the correct handling functions to generate the HTTP response
// according to the request's method and use of CGI
// #f : check POST request with no body and not a CGI / POST request targeting existing static file
// #f : check that request is allowed at the given location
int	Response::produceResponse(void)
{
	std::cout << "[Res::Response] Generating response, virtual server = "
		<< this->_vserv << ", location = " << this->_location << "\n";
	this->_responseBuffer = "";
	if (this->isCGIRequest())
	{
		std::cout << "\tRequest for CGI, extension " << this->_request._extension << "\n";
		return (this->handleCGI());
	}
	else
	{
		std::cout << "\tRequest without CGI, method : " << this->_request._method << "\n";
		if (this->_request._method == "GET")
			return (this->handleGet());
		else
		{
			this->_responseBuffer = "HTTP/1.1 200 OK\r\nContent-Length: 4\r\n\r\nBASE";
			return (0);
		}
	}
}
