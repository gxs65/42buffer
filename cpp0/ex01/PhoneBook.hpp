#ifndef PHONEBOOK_H
# define PHONEBOOK_H

# include <iostream>
# include <iomanip>
# include <string>

# include "functions.h"
# include "Contact.hpp"

class PhoneBook
{
	private:
		Contact		contacts[8];
		int			ind_new_contact;
	
	public:
		PhoneBook();
		~PhoneBook();
		Contact		*get_contact(int ind);
		void		add_contact();
		void		view_contacts();
		void		display_table();
};

#endif