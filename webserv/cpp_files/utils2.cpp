#include "../hpp_files/webserv.hpp"

// Trims a string, ie. removes all space characters on the left and right
void	trimString(std::string& s)
{
	unsigned int	ind;

	ind = 0;
	while (ind < s.size() && std::isspace(s[ind]))
		ind++;
	if (ind > 0)
		s.erase(0, ind);
	if (s.size() == 0)
		return ;
	ind = s.size() - 1;
	while (ind >= 0 && std::isspace(s[ind]))
		ind--;
	if (ind < s.size() - 1)
		s.erase(ind + 1, s.size() - ind);
}

// Splits strings <orig> by separators in <seps>
// and puts the resulting tokens in vector <tokens>
int	splitString(std::string& orig, std::vector<std::string>& tokens, std::string seps)
{
	unsigned int	ind = 0;
	unsigned int	indLast = 0;

	while (ind < orig.size())
	{
		while (ind < orig.size() && seps.find(orig[ind]) == std::string::npos)
			ind++;
		if (ind != indLast)
			tokens.push_back(orig.substr(indLast, ind - indLast));
		ind++;
		indLast = ind;
	}
	return (0);
}

// Checks if <str> ends with <suffix>
bool ends_with(const std::string& str, const std::string& suffix)
{
	if (suffix.size() > str.size())
		return (false);
	return (std::equal(suffix.rbegin(), suffix.rend(), str.rbegin()));
}
