#ifndef __FIXED_H__
# define __FIXED_H__

# include <iostream>
# include <string>
# include <cstring>
# include <stdlib.h>

class	Fixed;

int		bitarray_to_int(unsigned char *arr, int size);
void	fill_bitarray(unsigned char *arr, unsigned char filler, int size);

// Represents a fixed-point number (FPN),
// defined by an ordinary 32-bits integer
// and a "scaler" = number of bits dedicated to
// the fractional part of the number (here set at 8 bits)
// 		-> "resolution" (minimal gap between 2 FPN values) of `1 / 2^(scaler)`
// 		-> able to represent min/max values of `(+/-) 2^(32 - scaler)`
// Implements basic arithmetic operations applied to FPNs,
// and conversions to/from floats (in IEEE754 format) by working with raw bits
// /!\ Does not handle overflows : user should make the precautions for it
class	Fixed
{
	public:
		Fixed(void);
		Fixed(const Fixed& orig);
		Fixed(const float orig);
		Fixed(const int orig);
		Fixed& operator= (const Fixed& orig);
		~Fixed(void);
		int					getRawBits(void) const;
		void				setRawBits(const int raw);
		// Conversions to FPN
		void				convertFromFloat(float f);
		void				convertFromInt(int i);
		// Conversions from FPN
		float				toFloat(void);
		int					toInt(void);
	
	private:
		// FPN representation
		int					n;
		static const int	nbits_fractional;
		// Storage for float raw bits during a conversion
		unsigned char		sign_bit;
		unsigned char		exponent_bits[8];
		unsigned char		mantissa_bits[23];
		// Conversions helpers
		void				extractFloatRawBits(float f);
		int					convertFromFloatExponentExceptions(void);
		void				convertFromFloatExtreme(void);
		void				convertFromFloatCopyBits(int exponent);
		void				convertToFloatBits(void);
		// Log
		void				logFloatRawBits(void);
		void				logRawBits(void);
};

#endif
