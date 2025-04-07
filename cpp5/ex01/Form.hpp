#ifndef FORM_HPP
# define FORM_HPP

# include <iostream>
# include <string>
# include <exception>
# include "Bureaucrat.hpp"

class Bureaucrat;
class Form;
std::ostream& operator<<(std::ostream& out, const Form& form);

class Form
{
	public:
		Form();
		Form(const Form& copy);
		Form(const std::string& name, int minGradeToExecute, int minGradeToSign);
		~Form();
		Form & operator=(const Form &assign);
		std::string 		getName(void) const;
		int 				getMinGradeToSign(void) const;
		int 				getMinGradeToExecute(void) const;
		bool				getIsSigned(void) const;
		void				beSigned(Bureaucrat &bur);
		class GradeTooHighException : public std::exception
		{	public: virtual const char*		what() const throw();};
		class GradeTooLowException : public std::exception
		{	public: virtual const char*		what() const throw();};
		
	private:
		const std::string 	name;
		const int 			minGradeToExecute;
		const int			minGradeToSign;
		bool				isSigned;
		
};

#endif