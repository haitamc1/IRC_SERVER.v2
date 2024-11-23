#include "../headers/client.hpp"
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

Client	::	~Client(){}

Client	::	Client(int fd){
			passSet = false;
			profileStatus = false;
			clientSock = fd;
			nickName = "";		
			hostname = "";
			servername = "";
			realname = "";
			_password = "";
			_max_channel = 0;
			
	}

void	Client	::	setClientFd(int fd)	{clientSock = fd;}

void	Client	::	setHostname(std::string name)	{hostname = name;}

void	Client	::	setNickName(std::string name)	{nickName = name;}

void	Client	::	setUserName(std::string name)	{userName = name;}

void	Client	::	setProfileStatus(bool status)	{profileStatus = status;}
void	Client	::	setPassword(std::string pass) 	{_password = pass;}

void	Client	::	setservername(std::string name)	{servername = name;}
void	Client	::	setrealname(std::string name)	{realname = name;}

void	Client	::	setPass(bool flag)	{passSet = flag;}

int	Client	::	getClientFd() const	{return clientSock;}

std::string	Client::getPassword() const {return _password;}

bool	Client	::	getProfileStatus() const	{return profileStatus;}

std :: string	Client	::	getHostname()	const {return hostname;}

std :: string	Client	::	getNickName()	const {return nickName;}

std :: string	Client	::	getUserName()	const {return userName;}

bool 	Client	::	getPass()	 const {return passSet;}

void	Client	::	addChannel(std::string channelName)
{
	std::vector<std::string>::iterator it = std::find(channels.begin(), channels.end(), channelName);

	if (it == channels.end())
		channels.push_back(channelName);
	return ;	
}

bool Client :: registred()
{
	if (nickName != ""  && userName != "" && realname != "")
		return true;
	return false;
}

void	Client	::	checkProfile()
{
	if (userName != "" && realname != "")
		profileStatus = true;
}

bool Client::ft_read(){
	char	buff[512];
	int	size = 0;
	std::memset(buff, 0, 512);
	size = recv(clientSock, buff, sizeof(buff), 0);
	if (size == -1){
		std :: cerr << "recv  failed" << std:: endl;
		return false;
	}
	buff[size] ='\0';
	_buffer.append(std::string(buff));
	return true;
}

bool Client::is_cmd_ready() const {
    return (_buffer.find("\n") != std::string::npos);
};

std::string Client::get_buffer(){
	if (_buffer.size() > 512){
		this->send("ERROR :Message too long\r\n");
		return "";
	}
	size_t found = _buffer.find("\r\n");
	if (_buffer.find("\r\n") != std::string::npos)
    	return _buffer.substr(0, found);
	found = _buffer.find("\n");
		return _buffer.substr(0, found);
}

void Client::empty_buffer() { _buffer = ""; };

int Client::send(std::string buffer){
	int size = ::send(clientSock, buffer.c_str(), buffer.size(), 0);
	if (size == -1)
		std::cerr << "send failed" << std::endl;
	return size;
}

bool	Client::is_Nick_set() const
{
	if (nickName.empty() != 0)
		return 1;
	return 0;
}

bool	Client::is_User_set() const
{
	if (userName.empty() != 0)
		return 1;
	return 0;
}

void Client::add_myChannsList(std::string const& Channelname) {
	
    if (_max_channel + 1 >= MAX_CHANNEL)
	{
		std::cout<<"max_channel "<<_max_channel<<std::endl;
        throw ircExceptions::ERR_TOOMANYCHANNELS(Channelname);
	}
    _max_channel++;
    Mychannel.push_back(Channelname);
}

std::vector<std::string> Client::get_myChannsList() {
    return Mychannel;
}

void Client::remove_channel(std::string const& Channelname)
{
   std::vector<std::string>::iterator it = find(Mychannel.begin(), Mychannel.end(), Channelname);
   if (it != Mychannel.end())
        Mychannel.erase(it);
    else
        throw ircExceptions::ERR_NOTONCHANNEL(Channelname);
	_max_channel--;
}
