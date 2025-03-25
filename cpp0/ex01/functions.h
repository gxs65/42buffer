#ifndef FUNCTIONS_H
# define FUNCTIONS_H

# include <iostream>
# include <iomanip>
# include <string>
# include <stdlib.h>

# include "PhoneBook.hpp"
# include "Contact.hpp"

void			get_input_line(const char *prompt, std::string *buffer);
std::string		clamp(std::string& src, unsigned int max_size);

#endif