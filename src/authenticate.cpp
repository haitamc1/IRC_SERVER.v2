#include "../headers/authenticate.hpp"

bool	authenticate	::	justSpaces(std::string line)
{
	for (size_t i = 0; i < line.size(); i++)
	{
		if (line[i] != ' ')
			return false;
	}
	return true;
}

bool	authenticate	::	checkPass(Client& client, std::string password, std::string	line)
{
	std::string pass = "Bot";
	pass += 1;
		if (justSpaces(line) || line == "")
			throw ircExceptions::ERR_NEEDMOREPARAMS("PASS");
		if (line == password){
			std::cout <<"The password is correct\n";
			return true;
		}
		else if (line == pass)
		{
			std::cout << "Bot is available now" << std::endl;
			client.setProfileStatus(true);
			client.setNickName("/Bot");
			client.setUserName("bot");
			return true;
		}
		throw ircExceptions::ERR_PASSWDMISMATCH();
}


bool	authenticate	::	dupNick(std::map<std::string, Client *> _nickname_pool, std::string &name)
{
	std::map<std::string, Client *>::iterator it;

	it = _nickname_pool.find(name);
  	if (it != _nickname_pool.end()){
			throw ircExceptions::ERR_NICKNAMEINUSE(name);
			return false;
	}
	return true;
}

bool	authenticate	::	checkNick(std::map<std::string, Client *> _nickname_pool, std::string line)
{
	if (line == "")
		throw ircExceptions::ERR_NONICKNAMEGIVEN();
	if (!isalpha(line[0]) && line[0] != '[' && line[0] != ']' &&
			line[0] != '\\' && line[0] != '^' && line[0] != '_' &&
			line[0] != '-') 
			throw ircExceptions::ERR_ERRONEUSNICKNAME(line);
	for (size_t i = 1; i < line.size(); ++i) {
		if (!isalnum(line[i]) && line[i] != '[' && line[i] != ']' &&
			line[i] != '\\' && line[i] != '^' && line[i] != '_' &&
			line[i] != '-') {
			throw ircExceptions::ERR_ERRONEUSNICKNAME(line);
			return false;
        }}
	if (!dupNick(_nickname_pool, line) || (line.size() > 9))
			return false;
	return true;
}
