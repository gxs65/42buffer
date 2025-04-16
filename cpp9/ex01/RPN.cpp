#include "RPN.hpp"

// /!\ RPN manages priority without need of parenthesis

// Logs an error, then returns 1
int		logError(std::string msg)
{
	std::cout << "Error : " << msg << "\n";
	return (1);
}

// Checks if a character in the digits range of the ASCII table
bool	isDigit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

// Checks that a literal representing a double (a RPN operand) is valid
// Constraints on string : only digit characters ('0'-'9'),
// except for one unique decimal point ('.') or one leading '-'/'+' sign
// 		-> in particular, special values ('inf'/'nan') are considered invalid
bool	isValidOperand(std::string valueStr)
{
	unsigned int	ind = 0;
	bool			decimal_point_passed = 0;
	int				nb_digits_passed = 0;

	if (valueStr[0] == '+' || valueStr[0] == '-')
		ind++;
	while (ind < valueStr.length())
	{
		if (valueStr[ind] == '.' && !decimal_point_passed)
			decimal_point_passed = 1;
		else if (isDigit(valueStr[ind]))
			nb_digits_passed++;
		else
			return (0);
		ind++;
	}
	if (nb_digits_passed == 0)
		return (0);
	else
		return (1);
}

// Checks that the operator for a RPN expression is valid
// (it must have length 1 and be an operator among "+-*/")
bool	isValidOperator(std::string operatorStr)
{
	if (operatorStr.length() != 1)
		return (0);
	if (operatorStr[0] == '+' || operatorStr[0] == '-'
		|| operatorStr[0] == '*' || operatorStr[0] == '/')
		return (1);
	return (0);
}

// Applies an operator, which in RPN means unstacking the operands stack twice
// to get the two operands, then computing the result of the operation between them,
// and then pushing the result back into the stack
// 		-> after having applied an operator, the stack size is reduced by exactly 1
// /!\ order of unstacking : the top of the stack is operand2, the element before that is operand1,
// 	   so that eg. RPN expression "3 1 -" yields result "3 - 1 = 2" and not "1 - 3 = -2"
int	applyRPNOperator(std::stack<double>& operandsStack, char _operator)
{
	double		operand1;
	double		operand2;
	double		res;

	if (operandsStack.size() < 2)
		return (1);
	operand2 = operandsStack.top();
	operandsStack.pop();
	operand1 = operandsStack.top();
	operandsStack.pop();
	switch (_operator)
	{
		case '+':
			res = operand1 + operand2;
			break;
		case '-':
			res = operand1 - operand2;
			break;
		case '*':
			res = operand1 * operand2;
			break;
		case '/':
			if (operand2 == 0)
				return (1);
			res = operand1 / operand2;
			break;
	}
	std::cout << "\tApplying operator : " << operand1 << " " << _operator << " " << operand2
		<< " = " << res << " added to stack\n";
	operandsStack.push(res);
	return (0);
}

// Creates an empty RPN stack, then parses the expression according to separator ' ' :
// 		\ operand tokens are pushed into the stack
// 		\ operator tokens reduce the stack size by 1 to perform their operation
// At the end of expression the stack size must be 1 (the last element is the result)
// 		(size of 0 would mean empty expression, size > 1 would mean insufficient operators)
// <=> in the expression, if there are n operands, there must be exactly n-1 operators
int	computeRPNExpression(std::string expressionStr)
{
	std::stack<double>	operandsStack;
	std::stringstream	parserStream;
	std::string			token;
	double				operand;

	parserStream << expressionStr;
	while (std::getline(parserStream, token, ' '))
	{
		std::cout << "Token '" << token << "' :\n";
		if (isValidOperand(token))
		{
			operand = strtod(token.c_str(), NULL);
			std::cout << "\tAdding double " << operand << " to stack\n";
			operandsStack.push(operand);
		}
		else if (isValidOperator(token))
		{
			if (applyRPNOperator(operandsStack, token[0]))
				return (logError("invalid operands for operator"));
		}
		else
			return (logError("invalid operator or operand"));
	}
	if (operandsStack.size() != 1)
		return (logError("insufficient operators"));
	else
	{
		std::cout << "Result : " << operandsStack.top() << "\n";
		return (0);
	}
}
