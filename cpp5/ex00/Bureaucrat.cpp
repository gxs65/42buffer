#include "Bureaucrat.hpp"

/////////////////////////////
// CANONICAL ORTHODOX FORM //
/////////////////////////////

Bureaucrat::Bureaucrat() : name("Anonymous"), grade(150)
{
	std::cout << "Default Constructor for Bureaucrat\n";
}

Bureaucrat::Bureaucrat(const Bureaucrat& orig) : name(orig.getName()), grade(orig.getGrade())
{
	std::cout << "Copy Constructor for Bureaucrat named " << this->name << "\n";
}

Bureaucrat::Bureaucrat(const std::string& _name, int _grade) : name(_name)
{
	std::cout << "Parameter Constructor for Bureaucrat named " << this->name << "\n";
	if (_grade < 1)
		throw (Bureaucrat::GradeTooHighException());
	if (_grade > 150)
		throw (Bureaucrat::GradeTooLowException());
	this->grade = _grade;
}

Bureaucrat::~Bureaucrat()
{
	std::cout << "Destructor for Bureaucrat named " << this->name << "\n";
}

// Assignment operator overload can't be used since <Bureaucrat> has constant members
Bureaucrat& Bureaucrat::operator=(const Bureaucrat &orig)
{
	(void)orig;
	std::cout << "Assignment operator overload (REFUSED) for Bureaucrat named " << this->name << "\n";
	return (*this);
}

///////////////
// ACCESSORS //
///////////////

std::string Bureaucrat::getName() const
{
	return (this->name);
}
int Bureaucrat::getGrade() const
{
	return (this->grade);
}

/////////////////////
// SUBJECT CONTENT //
/////////////////////

void	Bureaucrat::promote(void)
{
	if (this->grade <= 1)
		throw (Bureaucrat::GradeTooHighException());
	this->grade--;
	std::cout << *this << " got promoted\n";
}

void	Bureaucrat::demote(void)
{
	if (this->grade >= 150)
		throw (Bureaucrat::GradeTooLowException());
	this->grade++;
	std::cout << *this << " got demoted\n";
}

std::ostream& operator<<(std::ostream& out, const Bureaucrat& bur)
{
	out << "[" << bur.getName() << ", bureaucrat grade " << bur.getGrade() << "]";
	return (out);
}

const char*		Bureaucrat::GradeTooHighException::what() const throw()
{
	return("Exception : bureaucrat's grade is too high (can't be better than 1)");
}

const char*		Bureaucrat::GradeTooLowException::what() const throw()
{
	return("Exception : bureaucrat's grade is too low (can't be worse than 150)");
}
