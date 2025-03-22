#ifndef PHONEBOOK_H
# define PHONEBOOK_H

# include <iostream>
# include "functions.h"
# include "Contact.hpp"

class PhoneBook
{
	private:
		Contact		*contacts[8];
		int			ind_new_contact;
	
	public:
		PhoneBook();
		Contact		*get_contact(int ind);
		void		create_contact();
		void		view_contacts();
};

#endif