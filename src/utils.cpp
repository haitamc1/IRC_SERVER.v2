#include "../headers/utils.hpp"

void    upperIt(std::string &s)
{
    for (size_t i = 0; i < s.size(); i++)
            s[i] = std::toupper(s[i]);
}

void	skipSpace(std::string &s, size_t &i)	{for(; i < s.size() && s[i] == ' '; i++);}

void	trimSpace(std::string &s)
{
	std::string ret;

	for(size_t i = 0; i < s.size(); i++)
	{
		if (s[i] != ' ')
			ret += s[i];
	}
	s = ret;
}

size_t	lastSpace(std::string s)
{
	size_t	i = s.size() - 1;
	while (i > 0 && s[i] == ' ')
		i--;
	return i;
}