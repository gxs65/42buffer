#ifndef BUREAUCRAT_HPP
# define BUREAUCRAT_HPP

# include <iostream>
# include <string>
# include <exception>

// Remarks on the implementation :
// 		- "grade too low / grade too high" errors are transmitted by Bureaucrat's methods
// 		  through exceptions (that the calling code should expect with a <try/catch>)
// 		- since only instances of Bureaucrats use GradeTooXXException,
// 			they are defined as "nested classes" inside Bureaucrat
// 				so that no outside code may use them
// 		- Bureaucrats methods throw exceptions as objects of type GradeTooXXException,
// 			but in the <main>, instructions <catch std::exception& e>
// 				(1) succeed to catch those objects
// 					(because <GradeTooXXException> inherits from standard class <exception>)
// 				(2) succeed to call method <GradeTooXXException::what>
// 					(because standard class <exception> has a virtual method <what>)
// 			-> the exception-managing infractructure uses polymorphism
// 		- besides storing a specific log message, exception classes may be used
// 			to record information on the context that created the exception
// 		- throwing an exception is done with line `throw (Bureaucrat::GradeTooHighException());`
// 			which calls the exception class default constructor to have a new instance


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
		class GradeTooHighException : public std::exception // /!\ to be called for grades < 1
		{	public: virtual const char*		what() const throw();};
		class GradeTooLowException : public std::exception // /!\ to be called for grades > 150
		{	public: virtual const char*		what() const throw();};
		
	private:
		const std::string 	name;
		int 				grade;
		
};

#endif