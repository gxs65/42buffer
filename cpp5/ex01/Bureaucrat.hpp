#ifndef BUREAUCRAT_HPP
# define BUREAUCRAT_HPP

# include <iostream>
# include <string>
# include <exception>
# include "Form.hpp"

class Form;
class Bureaucrat;
std::ostream& operator<<(std::ostream& out, const Bureaucrat& bur);

class Bureaucrat
{
	public:
		Bureaucrat();
		Bureaucrat(const Bureaucrat& copy);
		Bureaucrat(const std::string& name, int grade);
		~Bureaucrat();
		Bureaucrat & operator=(const Bureaucrat &assign);
		std::string 		getName(void) const;
		int 				getGrade(void) const;
		void				promote(void);
		void				demote(void);
		void				signForm(Form& form);
		class GradeTooHighException : public std::exception // /!\ to be called for grades < 1
		{	public: virtual const char*		what() const throw();};
		class GradeTooLowException : public std::exception // /!\ to be called for grades > 150
		{	public: virtual const char*		what() const throw();};
		
	private:
		const std::string 	name;
		int 				grade;
		
};

#endif