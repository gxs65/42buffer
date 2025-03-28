#ifndef __FIXED_H__
# define __FIXED_H__

# include <iostream>
# include <string>
# include <stdlib.h>

class	Fixed
{
	public:
		Fixed(void);
		Fixed(const Fixed& orig);
		Fixed& operator= (const Fixed& orig);
		~Fixed(void);
		int					getRawBits(void) const; // the ending `const` indicates that this method won't modify the object it's called on
		void				setRawBits(const int raw);
	
	private:
		int					n;
		static const int	nbits_fractional;
};

#endif
