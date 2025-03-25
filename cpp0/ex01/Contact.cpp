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

int			Contact::get_contact_set()
{
	return (this->contact_set);
}

void	Contact::display_table()
{
	if (!(this->contact_set))
		return ;
	std::cout << std::setfill(' ')
		<< std::setw(10) << clamp(this->firstname, 10) << "|"
		<< std::setw(10) << clamp(this->lastname, 10) << "|"
		<< std::setw(10) << clamp(this->nickname, 10) << "\n";
}

void	Contact::display_all()
{
	if (!(this->contact_set))
		return ;
	std::cout << "First name : " << this->firstname << "\n"
		<< "Last name : " << this->lastname << "\n"
		<< "Nickname : " << this->nickname << "\n"
		<< "Phone number : " << this->phone_number << "\n"
		<< "Secret : " << this->secret << "\n";
}

Contact::Contact()
{
	this->contact_set = 0;
}

Contact::~Contact()
{
}

void Contact::update_contact()
{
	get_input_line("First name ?", &(this->firstname));
	get_input_line("Last name ?", &(this->lastname));
	get_input_line("Nickname ?", &(this->nickname));
	get_input_line("Phone number ?", &(this->phone_number));
	get_input_line("Darkest secret ?", &(this->secret));
	this->contact_set = 1;
}
