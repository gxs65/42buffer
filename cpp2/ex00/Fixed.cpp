#include "Fixed.hpp"

Fixed::Fixed()
{
	std::cout << "Default constructor for FPN\n";
	this->n = 0;
}

Fixed::Fixed(const Fixed& orig)
{
	std::cout << "Copy constructor for FPN\n";
	this->n = orig.getRawBits();
}

Fixed& Fixed::operator= (const Fixed& orig)
{
	std::cout << "Assignment operator overload for FPN\n";
	if (this != &orig)
	{
		this->n = orig.getRawBits();
	}
	return (*this);
}

Fixed::~Fixed()
{
	std::cout << "Destructor for FPN\n";
}

int	Fixed::getRawBits(void) const
{
	std::cout << "<getRawBits> of FPN called\n";
	return (this->n);
}

void	Fixed::setRawBits(const int raw)
{
	this->n = raw;
}

const int	Fixed::nbits_fractional = 8;
