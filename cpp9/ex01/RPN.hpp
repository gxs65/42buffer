#ifndef RPN_H
# define RPN_H

# include <string>
# include <sstream>
# include <fstream>
# include <iostream>
# include <stdlib.h>
# include <stack>
# include <algorithm>

int		logError(std::string msg);
bool	isDigit(char c);
bool	isValidOperand(std::string valueStr);
bool	isValidOperator(std::string operatorStr);
int		applyRPNOperator(std::stack<double>& operandsStack, char _operator);
int		computeRPNExpression(std::string expressionStr);

#endif
