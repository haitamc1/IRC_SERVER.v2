#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <map>

#include "Parser.hpp"
#include "ircExceptions.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#include "./polldt.hpp"
#include "client.hpp"
#include "authenticate.hpp"
#include "IRCReplies.hpp"
#include "Channel.hpp"


class	authenticate;

class server
{
    private:
        Parser _parser;
        socklen_t           addrlen;
        struct sockaddr_in  addr;
        polldt	poller;
        
        std::string         password;
        int 				serverSock;
        int 				port;
		nfds_t				connectedFds;
	
        ChannelsHandler channels;

        std::map<int, Client *> _fd_pool;
        std::map<std::string, Client *> _nickname_pool;

        // std::map<std::string, client *> _nickname_pool;


    public:
        ~server();
        server(std::string pass, std::string port);

        int         getPort() const;
        int         getFd() const;
        std::string getPass() const;
        Client * get_client(int fd);
        Client * get_client(std::string nickname);

        void    erase_clientData(int fd);

		void	newConnection(polldt &poller);
		void	communicateWClient(polldt &poller, int i);

        void    Connect();


       std::string get_order_from_client(Client* client);

        void authenticate(Client& client, const Parser::Command& cmd);

		void	execCmd(Parser::Command& cmd, Client &client);

        
		static bool nickExist(std::string name, std::map<std::string, Client *> _nickname_pool);



    void    PASS(Client& client, const Parser::Command& cmd);
    void    USER(Client& client, const Parser::Command& cmd);
    void    NICK(Client &client, const Parser::Command& cmd);
    void    QUIT(Client &client);

    // void PRIVMSG(Client& client, const Parser::Command& cmd);
    void	JOIN(Client& client, const Parser::Command& cmd);
    void	PRIVMSG(Client& client, const Parser::Command& cmd);
    void	MODE(Client& client, Parser::Command& cmd);
    void	TOPIC(Client & client, const Parser::Command& cmd);
    void	INVITE( Client& client,const Parser::Command& cmd);
    void	KICK(Client& client, const Parser::Command& cmd);
	void	BOT(Client& client, const Parser::Command& cmd);
    // void	QUIT(Client& client);
    void	PART(std::vector<std::string> mychannel, Client & client);
    void	broadcastMessage(const std::string& channelName,  const std::string& message, std::string sender);
    void	RPL_NAMREPLY(Client& client,
                         std::string channelname,
                         ChannelsHandler& channels);
};

void ft_send(Client& client, std::vector<std::string> msg);
#endif