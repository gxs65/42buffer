#include "../hpp_files/webserv.hpp"

////////////////////////////
// HANDLING POST REQUESTS //
////////////////////////////

int	Response::writeFullBufferInFile(std::string& fullPath, size_t bufferSize, char *buffer)
{
	int		fd;
	size_t	ind;
	long	nBytesToWrite;
	long	nBytesWritten;

	fd = open(fullPath.c_str(), O_WRONLY | O_CREAT, 0644);
	if (fd < 0)
		return (1);
	ind = 0;
	while (ind < bufferSize)
	{
		nBytesToWrite = std::min(WRITEFILE_BUFFER_SIZE, bufferSize - ind);
		nBytesWritten = write(fd, buffer + ind, nBytesToWrite);
		if (nBytesWritten < 0)
			return (close(fd), 1);
		ind += nBytesWritten;
	}
	close(fd);
	return (0);
}

// Handles POST requests without CGI -> the request contains the content of a file
// that must be stored on the server side at the location given by the path
// Checks that 
int	Response::handlePost()
{
	std::string	fullPath;
	bool		fileExists;

	std::cout << "\tchecks on POST request for uploading a file\n";
	if (this->_request->_toDir)
		return (this->makeErrorResponse("404 Not Found"), 0); // #f : accept POST upload to directory
	if (!(this->_location) || this->_location->uploadPath.empty())
		return (this->makeErrorResponse("403 Upload Forbidden"), 0);
	if (this->pathToFile(fullPath, 0, 0, NULL)) // #f : invalidated
		return (0); // #f : check if <makeErrorResponse> should be at this level
	if (this->_request->_bodySize > FILEUPLOAD_MAXSIZE)
		return (this->makeErrorResponse("413 Content Too Large"), 0);
	fileExists = (access(fullPath.c_str(), F_OK) == 0);
	std::cout << "\tchecks succeeded ; file already existed : " << fileExists << "\n";
	if (this->writeFullBufferInFile(fullPath, this->_request->_bodySize, this->_request->_body))
		return (this->makeErrorResponse("500 Internal Server Error"), 0);
	std::cout << "\tfile upload finished\n";
	if (fileExists)
		this->makeSuccessResponse("200 OK");
	else
		this->makeSuccessResponse("201 Created");
	return (0);
}

//////////////////////////////
// HANDLING DELETE REQUESTS //
//////////////////////////////

int	Response::handleDelete()
{
	std::string	fullPath;
	bool		fileExists;

	std::cout << "\tchecks on DELETE\n";
	if (this->_request->_toDir)
		return (this->makeErrorResponse("403 Deleting Directory Forbidden"), 0);
	if (this->pathToFile(fullPath, 1, 0, NULL)) // #f : invalidated
		return (this->makeErrorResponse("404 Not Found"), 0);
	fileExists = (access(fullPath.c_str(), F_OK) == 0);
	std::cout << "\tchecks ended ; file exists : " << fileExists << "\n";
	if (!fileExists)
		return (this->makeErrorResponse("500 Internal Server Error"), 0);
	if (remove(fullPath.c_str()))
		return (this->makeErrorResponse("500 Internal Server Error"), 0);
	std::cout << "\tfile successfully deleted\n";
	this->makeSuccessResponse("500 Internal Server Error");
	return (0);
}
