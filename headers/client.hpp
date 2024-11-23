#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>
#include <iostream>
#include "ircExceptions.hpp"
#include <sys/socket.h>
#include "Channel.hpp"

#define MAX_CHANNEL 20

class	Client
{
	private:
		std::vector<std::string>	channels;
		std::string nickName;
		std::string userName;
		std::string servername;
		std::string realname;
		std::string _buffer;
		std::string _password;

		bool	passSet;
		bool	profileStatus;
		int		clientSock;

		std::vector<std::string> Mychannel;
    	int _max_channel;

	public:
		~Client();
		Client(int fd);
		std::string hostname;

		void	setHostname(std::string name);
		void	setNickName(std::string name);
		void	setUserName(std::string name);
		void	setservername(std::string name);
		void	setrealname(std::string name);
		void	setClientFd(int fd);
		void	setProfileStatus(bool status);
		void	setPassword(std::string pass);
		void	setPass(bool flag);

		std :: string	getHostname() const ;
		std :: string	getNickName() const ;
		std :: string	getUserName() const ;
		int				getClientFd() const ;
		std :: string	getPassword() const ;//??

		bool 	getPass() const ;


		bool	getProfileStatus() const;
		bool	is_Nick_set() const;
		bool	is_User_set() const;

		void	addChannel(std :: string channelName);
		void	checkProfile();

		// static	bool	fillProfile(std::vector<Client> &Clients,  std::string pass);

		bool ft_read();
		std::string get_buffer();
		void empty_buffer();
		bool is_cmd_ready() const;
		int send(std::string buffer);

		bool registred();


	void add_myChannsList(std::string const& Channelname);
    std::vector<std::string> get_myChannsList();
    void remove_channel(std::string const& Channelname);

};

#endif