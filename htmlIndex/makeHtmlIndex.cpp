# include <dirent.h>
# include <stdlib.h>
# include <string.h>
# include <iostream>
# include <fstream>

int	main(int ac, char **av)
{
	std::ofstream		outstream;
	struct __dirstream	*dirPointer;
	struct dirent		*dirContent;

	if (ac < 2)
		return (1);
	dirPointer = opendir(av[1]);
	if (!dirPointer)
		return (1);
	dirContent = readdir(dirPointer);

	outstream.open("index.html");
	outstream << "<!DOCTYPE html>\n<html>\n"
		<< "<head>\n<meta charset=\"UTF-8\">\n<title>Default index</title>\n</head>\n"
		<< "<body>\n<h1>Default index of directory : " << av[2] << "</h1>\n"
		<< "<ul>\n";
	while (dirContent)
	{
		if (dirContent->d_type == DT_REG)
		{
			outstream << "<li><a href=\"" << dirContent->d_name << "\">"
				<< dirContent->d_name << "</a></li>\n";
		}
		else if (dirContent->d_type == DT_DIR
			&& strcmp(dirContent->d_name, ".") != 0 && strcmp(dirContent->d_name, "..") != 0)
		{
			outstream << "<li><a href=\"" << dirContent->d_name << "/\">"
				<< dirContent->d_name << "/</a></li>\n";
		}
		dirContent = readdir(dirPointer);
	}
	outstream << "</ul>\n</body>\n</html>\n";
	outstream.close();
	closedir(dirPointer);
	return (0);
}