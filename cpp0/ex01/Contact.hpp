#ifndef CONTACT_H
# define CONTACT_H

# include <iostream>
# include <iomanip>
# include <string>

# include "functions.h"
# include "PhoneBook.hpp"

class Contact
{
	private:
		std::string		firstname;
		std::string		lastname;
		std::string		nickname;
		std::string		phone_number;
		std::string		secret;
		int				contact_set;
	
	public:
		Contact();
		~Contact();
		void			update_contact();
		int				get_contact_set();
		std::string		get_firstname();
		std::string		get_lastname();
		std::string		get_nickname();
		std::string		get_phone_number();
		std::string		get_secret();
		void			display_table();
		void			display_all();
};

#endif