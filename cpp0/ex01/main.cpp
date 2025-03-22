#include "functions.h"

std::string	get_input_line(std::string prompt)
{
	std::string	s;

	std::cout << prompt << " ";
	std::getline(std::cin, s);
	while (s.empty())
	{
		std::cout << prompt << " ";
		std::getline(std::cin, s);
	}
	return (s);
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
		command = get_input_line("Command ?");
		if (command.compare("EXIT") == 0)
			exiting = 1;
		else if (command.compare("ADD") == 0)
			pb.create_contact();
		else if (command.compare("VIEW") == 0)
			pb.view_contacts();
		else
			std::cout << "Command is not recognized" << std::endl;
	}
	
	return (0);
}
