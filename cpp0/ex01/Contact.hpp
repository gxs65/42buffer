#ifndef CONTACT_H
# define CONTACT_H

# include <iostream>
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
	
	public:
		Contact();
		std::string		get_firstname();
		std::string		get_lastname();
		std::string		get_nickname();
		std::string		get_phone_number();
		std::string		get_secret();
		void			display(int include_secret);
};

#endif