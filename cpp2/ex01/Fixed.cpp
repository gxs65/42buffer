#include "Fixed.hpp"

// CONSTRUCTORS AND CANONICAL ORTHODOX FORM

Fixed::Fixed()
{
	std::cout << "<default constructor> of FPN\n";
	this->n = 0;
}

Fixed::Fixed(const Fixed& orig)
{
	std::cout << "<copy constructor> of FPN\n";
	this->n = orig.getRawBits();
}

Fixed::Fixed(const float orig)
{
	std::cout << "<float constructor> of FPN\n";
	this->convertFromFloat(orig);
}

Fixed::Fixed(const int orig)
{
	std::cout << "<int constructor> of FPN\n";
	this->convertFromInt(orig);
}

Fixed& Fixed::operator= (const Fixed& orig)
{
	std::cout << "<assignment operator overload> of FPN\n";
	if (this != &orig)
	{
		this->n = orig.getRawBits();
	}
	return (*this);
}

Fixed::~Fixed()
{
	std::cout << "<destructor> of FPN\n";
}

// ACCESSORS AND LOGS

int	Fixed::getRawBits(void) const
{
	std::cout << "<getRawBits> of FPN\n";
	return (this->n);
}

void	Fixed::setRawBits(const int raw)
{
	std::cout << "<setRawBits> of FPN\n";
	this->n = raw;
}

// /!\ Log function posits that <Fixed::nbits_fractional> is 8
void	Fixed::logRawBits(void)
{
	int	ind;

	std::cout << "<logRawBits> Current state of FPN :\n";
	std::cout << "--- Sign bit : " << ((this->n >> 31) & 1) << "\n";
	std::cout << "--- Integer part bits : ";
	for (ind = 1; ind < 24; ind++)
		std::cout << ((this->n >> (31 - ind)) & 1) << "";
	std::cout << "\n--- Fractional part bits : ";
	for (ind = 24; ind < 32; ind++)
		std::cout << ((this->n >> (31 - ind)) & 1) << "";
	std::cout << "\n";
}

// /!\ Log function posits that <Fixed::nbits_fractional> is 8
void	Fixed::logFloatRawBits(void)
{
	int	ind;

	std::cout << "[FLOATBITS] Raw bits of float binary representation :\n";
	std::cout << "[FLOATBITS] Sign bit : " << (int)(this->sign_bit) << "\n";
	std::cout << "[FLOATBITS] Exponent bits : ";
	for (ind = 0; ind < 8; ind++)
		std::cout << (int)(this->exponent_bits[ind]) << "";
	std::cout << "\n[FLOATBITS] Mantissa bits : ";
	for (ind = 0; ind < 23; ind++)
		std::cout << (int)(this->mantissa_bits[ind]) << "";
	std::cout << "\n";
}

// CONVERSIONS : INTEGER TO FIXED

// Builds the FPN from an integer, which means simply shifting the integer's bits
// by a number of positions equal to the number of bits in the fractional part
// /!\ Does not handle overflows : if the given int needs more than 23 (= 32 - 8fractional - 1sign)
// 	   to be represented, the left (ie. most significant) bits are just discarded
void	Fixed::convertFromInt(int i)
{
	std::cout << "<convertFromInt> of FPN, getting value from int : " << i << "\n";
	this->n = (i << Fixed::nbits_fractional);
	this->logRawBits();
}

// CONVERSIONS : FLOAT TO FPN

// Extracts and sorts the 32 bits representing float <f>
// into the categories : 1 bit for sign, 8 bits for exponent, 23 bits for mantissa
// To be able to read raw bits from the float, it is casted to an array of 4 chars
// (from 1 * 32 bits to 4 * 8 bits)
void	Fixed::extractFloatRawBits(float f)
{
	unsigned char 	*f_raw;
	unsigned int	ind;

	f_raw = reinterpret_cast<unsigned char *>(&f);
	this->sign_bit = ((f_raw[3] >> 7) & 1);
	//std::cout << ((f_raw[3] >> 7) & 1) << " --> " << (int)(this->sign_bit) << "\n";
	for (ind = 1; ind < 8; ind++)
		this->exponent_bits[ind - 1] = ((f_raw[3] >> (7 - ind)) & 1);
	this->exponent_bits[7] = (f_raw[2] >> 7 & 1);
	for (ind = 0; ind < 24; ind++)
	{
		if (ind != 0)
			this->mantissa_bits[ind - 1] = ((f_raw[2 - (ind / 8)] >> (7 - (ind % 8))) & 1);
	}
}

// (Called when the float to convert is NaN/inf/verybig :)
// Sets FPN at extreme value according to the float's sign
void	Fixed::convertFromFloatExtreme(void)
{
	if (this->sign_bit)
		this->n = -2147483648;
	else
		this->n = 2147483647;
}

// Checks the float exponent bits for special values (NaN, inf, 0)
// If there is none, computes the exponent and returns it (it may be negative)
// (/!\ exponent in float_IEEE754 is shifted up by 127,
// 		eg. computed value of 255 means 128, value of 126 means -1)
int	Fixed::convertFromFloatExponentExceptions(void)
{
	int	exponent_shifted;
	int	exponent;

	exponent_shifted = bitarray_to_int(this->exponent_bits, 8);
	if (exponent_shifted == 255)
	{
		std::cout << "[CONV FROM F - EXPONENT] float f is NaN of infinity (closest extreme used instead)\n";
		this->convertFromFloatExtreme();
		return (128);
	}
	if (exponent_shifted == 0)
	{
		std::cout << "[CONV FROM F - EXPONENT] float f is special case 0\n";
		this->n = 0;
		return (128);
	}
	exponent = exponent_shifted - 127;
	std::cout << "[CONV FROM F - EXPONENT] exponent of float f : " << exponent << "\n";
	return (exponent);
}

// Executes the critical part of the conversion : copying the float mantissa bits
// (<=> the significant bits, representing the number outside of scaling by `2^(exponent)`)
// at the correct bit position <pos_fpn_bits> in the 32-bit integer <n> storing the FPN
// 		- the correct position (= at which bit of <n> to start copying) is given by the float exponent,
// 			eg. exponent of 2 means starting 2 positions before the end of integer part (-> 0p00),
// 				or equivalently 8+2=10 positions from the end of the 32 bits of the number
// 				when taking into account the 8 bits of the fractional part (-> 0p00.00000000)
// 		- when `exponent > 22`, we will get `pos_fpn_bits <= 0` (unacceptable because pos 0 is for sign bit) :
// 			this is special case of float too big for FPN representation (-> rounded to extreme)
// 		- when `exponent < -8`, we will get `pos_fpn_bits > 31` (unacceptable because <n> has only 32 bits) :
// 			this is special case of float too small for FPN to get any significant bit (-> rounded to 0)
// 		- when copying mantissa bits to <n>,
// 			a) start with the "leading invisible/implied mantissa bit" that is always 1
// 			b) continue along mantissa bits until coming to end of mantissa / to end of <n>
// 				(if not reaching end of mantissa, some precision is lost and we round to upper)
// 			c) finally multiply <n> by -1 if float was negative :
// 				we represent FPN < 0 with 2's complements, exactly like an integer
// 				(the exact effect of `n * -1` is flipping all bits, then adding 1)
// (==> acceptable exponents are within the range [-8; 22])
void	Fixed::convertFromFloatCopyBits(int exponent)
{
	int	pos_fpn_bits;
	int	pos_mantissa_bits;

	this->n = 0;
	pos_fpn_bits = 31 - Fixed::nbits_fractional - exponent;
	if (pos_fpn_bits > 31)
	{
		std::cout << "[CONV FROM F - COPYBITS] Warning : float f is too small for FPN, rounding to 0\n";
		return ;
	}
	if (pos_fpn_bits <= 0)
	{
		std::cout << "[CONV FROM F - EXPONENT] Warning : float too big for FPN on 32 bits, rounding to closest extreme\n";
		this->convertFromFloatExtreme();
		return ;
	}
	this->n = this->n | (1 << (31 - pos_fpn_bits));
	pos_fpn_bits++;
	pos_mantissa_bits = 0;
	while (pos_fpn_bits < 32 && pos_mantissa_bits < 23)
	{
		if (this->mantissa_bits[pos_mantissa_bits] == 1)
			this->n = this->n | (1 << (31 - pos_fpn_bits));
		pos_fpn_bits++;
		pos_mantissa_bits++;
	}
	if (pos_mantissa_bits != 23 && bitarray_to_int(
			this->mantissa_bits + pos_mantissa_bits, 23 - pos_mantissa_bits) > 0)
	{
		if (this->mantissa_bits[pos_mantissa_bits] == 1)
		{
			this->n += 1;
			std::cout << "[CONV FROM F - COPYBITS] Warning : precision bits lost, rounding occurred\n";
		}
		else
			std::cout << "[CONV FROM F - COPYBITS] Warning : precision bits lost, no rounding\n";
	}
	if (this->sign_bit == 1)
		this->n = this->n * -1;
}

// Builds the FPN from a float, by looking into the floats' raw bits
// to manage exceptions (ex. inf/NaN) by hand, and avoid bad rounding
// (return value of 128 stored in <exponent> means one of the exceptions occurred,
//  in this case the FPN will already have been set to the function can end)
void	Fixed::convertFromFloat(float f)
{
	int	exponent;

	std::cout << "<convertFromFloat> of FPN, getting value from float : " << f << "\n";
	this->extractFloatRawBits(f);
	this->logFloatRawBits();
	exponent = this->convertFromFloatExponentExceptions();
	if (exponent != 128)
		this->convertFromFloatCopyBits(exponent);
	this->logRawBits();
}

// CONVERSIONS : FPN TO FLOAT / INT

float	Fixed::toFloat(void)
{
	int			ind;
	uint32_t	f_raw;
	float		f;

	this->sign_bit = 0;
	fill_bitarray(this->exponent_bits, 0, 8);
	fill_bitarray(this->mantissa_bits, 0, 23);
	if (this->n != 0)
		this->convertToFloatBits();
	f_raw = 0;
	f_raw = f_raw | ((uint32_t)(this->sign_bit) << 31);
	for(ind = 0; ind < 8; ind++)
		f_raw = f_raw | ((uint32_t)(this->exponent_bits[ind]) << (30 - ind));
	for(ind = 0; ind < 23; ind++)
		f_raw = f_raw | ((uint32_t)(this->mantissa_bits[ind]) << (22 - ind));
	std::memcpy(&f, &f_raw, sizeof(float));
	return (f);
}

void	Fixed::convertToFloatBits(void)
{
	int	ind;
	int	orig;

	orig = this->n;
	this->sign_bit = (orig >> 31) & 1;
	orig = orig * -1;
	ind = 1;
	while (((orig >> (31 - ind)) & 1) == 0)
		ind++;
	
}

// CONST STATIC CLASS MEMBERS

// <nbits_fractional> = FPN scalar = how much bits form the fractional part,
// initialized here since it is a static const class member
// so it should have a value as soon as program starts
const int	Fixed::nbits_fractional = 8;
