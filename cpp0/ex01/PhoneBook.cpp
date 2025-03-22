#include "functions.h"
#include "PhoneBook.hpp"

PhoneBook::PhoneBook()
{
	int		ind;

	this->ind_new_contact = 0;
	ind = 0;
	while (ind < 8)
	{
		this->contacts[ind] = NULL;
		ind++;
	}
}

Contact *PhoneBook::get_contact(int ind)
{
	return (this->contacts[ind]);
}

void	PhoneBook::create_contact()
{
	Contact		*new_contact;

	new_contact = new Contact;
	this->contacts[ind_new_contact] = new_contact;
	ind_new_contact = (ind_new_contact + 1) % 8;
}

void	PhoneBook::view_contacts()
{
	int			ind;

	ind = 0;
	while (ind < 8)
	{
		if (this->contacts[ind])
			(this->contacts[ind])->display(0);
		ind++;
	}
}
