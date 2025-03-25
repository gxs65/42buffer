#include <iostream>

void	get_input_line(std::string prompt, std::string *buffer)
{
	std::cout << prompt << " ";
	std::getline(std::cin, *buffer);
	while (buffer->empty())
	{
		std::cout << prompt << " ";
		std::getline(std::cin, *buffer);
	}
	std::cout << "getline - " << *buffer << "\n";
	std::cout << "getline - " << buffer << "\n";
}

int	main(void)
{
	std::string	s1;
	const char	*s2;
	std::string	s3;
	std::string	s4;

	s1 = "coucou";
	std::cout << s1 << "\n";
	std::cout << &s1 << "\n";
	s2 = s1.c_str();
	std::cout << s2 << "\n";
	std::cout << &s2 << "\n";
	getline(std::cin, s3);
	std::cout << s3 << "\n";
	std::cout << &s3 << "\n";
	get_input_line("truc ?", &s4);
	std::cout << s4 << "\n";
	std::cout << &s4 << "\n";
	return (0);
}