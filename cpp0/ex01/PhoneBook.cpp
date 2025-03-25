#include "functions.h"
#include "PhoneBook.hpp"

PhoneBook::PhoneBook()
{
	ind_new_contact = 0;
}

PhoneBook::~PhoneBook()
{
}

Contact *PhoneBook::get_contact(int ind)
{
	return (&(this->contacts[ind]));
}

void	PhoneBook::add_contact()
{
	this->contacts[ind_new_contact].update_contact();
	ind_new_contact = (ind_new_contact + 1) % 8;
}

void	PhoneBook::display_table()
{
	int	ind;

	ind = 0;
	while (ind < 8)
	{
		if (this->contacts[ind].get_contact_set())
		{
			std::cout << "[" << ind << "]       |"; 
			this->contacts[ind].display_table();
		}
		ind++;
	}
}

void	PhoneBook::view_contacts()
{
	int			ind;
	const char	*input_c_str;
	std::string	input;

	if (!(this->contacts[0].get_contact_set()))
	{
		std::cout << "No contacts in phonebook\n";
		return ;
	}
	this->display_table();
	get_input_line("Index of contact to view ?", &input);
	input_c_str = input.c_str();
	ind = atoi(input_c_str);
	std::cout << "Index selected : " << ind << "\n";
	if (ind < 0 || ind > 7
		|| !(this->contacts[ind].get_contact_set()))
		std::cout << "This contact is not set\n";
	else
		this->contacts[ind].display_all();
}
