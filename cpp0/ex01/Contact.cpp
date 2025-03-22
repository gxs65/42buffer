#include "functions.h"
#include "PhoneBook.hpp"

std::string	Contact::get_firstname()
{
	return (this->firstname);
}

std::string	Contact::get_lastname()
{
	return (this->lastname);
}

std::string	Contact::get_nickname()
{
	return (this->nickname);
}

std::string	Contact::get_phone_number()
{
	return (this->phone_number);
}

std::string	Contact::get_secret()
{
	return (this->secret);
}

void	Contact::display(int include_secret)
{
	std::cout << this->firstname << " " << this->lastname << " " << this->nickname
		<< " " << this->phone_number;
	if (include_secret)
		std::cout << " " << this->secret << std::endl;
	else
		std::cout << std::endl;
}

Contact::Contact()
{
	this->firstname = get_input_line("First name ?");
	this->lastname = get_input_line("Last name ?");
	this->nickname = get_input_line("Nickname ?");
	this->phone_number = get_input_line("Phone number ?");
	this->secret = get_input_line("Darkest secret ?");
}
