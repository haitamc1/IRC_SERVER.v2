#ifndef AUTHENTICATE_HPP
#define AUTHENTICATE_HPP


#include "client.hpp"
#include "Parser.hpp"
#include <map>

class	authenticate
{
	public:
		static	bool	justSpaces(std::string line);

		static	bool	checkPass(Client& client, std::string password, std::string	line);

		static	bool 	checkNick(std::map<std::string, Client *> _nickname_pool, std::string line);
		static	bool	dupNick(std::map<std::string, Client *> _nickname_pool ,std::string &name);
};

#endif