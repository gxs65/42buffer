#include "Form.hpp"

/////////////////////////////
// CANONICAL ORTHODOX FORM //
/////////////////////////////

Form::Form() : name("Someform"), minGradeToExecute(150), minGradeToSign(150), isSigned(0)
{
	std::cout << "Default Constructor for Form\n";
}

// Copy constructor copies everything except the <isSigned> status, which is 0 at construction
Form::Form(const Form& orig) : name(orig.getName()),
	minGradeToExecute(orig.getMinGradeToExecute()), minGradeToSign(orig.getMinGradeToSign()),
	isSigned(0)
{
	std::cout << "Copy Constructor for Form named " << this->name << "\n";
}

Form::Form(const std::string& _name, int _minGradeToExecute, int _minGradeToSign) :	name(_name),
	minGradeToExecute(_minGradeToExecute), minGradeToSign(_minGradeToSign),
	isSigned(0)
{
	std::cout << "Parameter Constructor for Form named " << this->name << "\n";
	if (_minGradeToExecute < 1 || _minGradeToSign < 1)
		throw (Form::GradeTooHighException());
	if (_minGradeToExecute > 150 || _minGradeToSign > 150)
		throw (Form::GradeTooLowException());
}

Form::~Form()
{
	std::cout << "Destructor for Form named " << this->name << "\n";
}

// Assignment operator overload can't be used since <Form> has constant members
Form& Form::operator=(const Form &orig)
{
	(void)orig;
	std::cout << "Assignment operator overload (REFUSED) for Form named " << this->name << "\n";
	return (*this);
}

///////////////
// ACCESSORS //
///////////////

std::string	Form::getName() const
{
	return (this->name);
}
int	Form::getMinGradeToExecute() const
{
	return (this->minGradeToExecute);
}
int	Form::getMinGradeToSign() const
{
	return (this->minGradeToSign);
}
bool	Form::getIsSigned() const
{
	return (this->isSigned);
}

/////////////////////
// SUBJECT CONTENT //
/////////////////////

void	Form::beSigned(Bureaucrat& bur)
{
	if (this->isSigned)
	{
		std::cout << "Attempt to sign " << *this << " while it is already signed\n";
		return ;
	}
	if (bur.getGrade() > this->minGradeToSign)
		throw (Form::GradeTooLowException());
	this->isSigned = 1;
}

std::ostream& operator<<(std::ostream& out, const Form& form)
{
	out << "[" << form.getName() << ", Form with grade to execute = "
		<< form.getMinGradeToExecute() << ", grade to sign = "
		<< form.getMinGradeToSign() << ", current signed signature : "
		<< form.getIsSigned() << "]";
	return (out);
}

const char*		Form::GradeTooHighException::what() const throw()
{
	return("Exception (Form) : grade is too high");
}

const char*		Form::GradeTooLowException::what() const throw()
{
	return("Exception (Form) : grade is too low");
}
