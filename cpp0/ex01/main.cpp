#include "functions.h"

void	get_input_line(const char *prompt, std::string *buffer)
{
	std::cout << prompt << " ";
	std::getline(std::cin, *buffer);
	while (buffer->empty())
	{
		std::cout << prompt << " ";
		std::getline(std::cin, *buffer);
	}
}

std::string	clamp(std::string& src, unsigned int max_size)
{
	std::string	clamped;

	if (src.size() <= max_size)
	{
		clamped = src;
	}
	else
	{
		clamped = src.substr(0, max_size - 1);
		clamped.append(".");
	}
	return (clamped);
}

int	main(int ac, char **av)
{
	int			exiting;
	std::string	command;
	PhoneBook	pb;
	
	(void)av;
	if (ac > 1)
		return (1);
	exiting = 0;
	while (!exiting)
	{
		get_input_line("Command ?", &command);
		if (command.compare("EXIT") == 0)
			exiting = 1;
		else if (command.compare("ADD") == 0)
			pb.add_contact();
		else if (command.compare("VIEW") == 0)
			pb.view_contacts();
		else
			std::cout << "This command is not recognized" << std::endl;
	}
	
	return (0);
}
