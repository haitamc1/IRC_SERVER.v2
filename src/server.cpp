#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../headers/server.hpp"

void    exitMsg(std::string s)
{
    std :: cerr << s << std :: endl;
    exit (EXIT_FAILURE);
}

void	errMsg(std :: string s)
{
	std :: cerr << s << std :: endl;
}

server  ::  ~server()
{
    shutdown(serverSock, SHUT_RDWR);
}

server  ::  server(std::string pass, std::string port)
{
    int opt = 1;

    this->password = pass;
    this->port = atoi(port.c_str());

    this->serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock == -1)
        exitMsg("FAILED SOCKET CREATION");
	if (fcntl(serverSock, F_SETFL, O_NONBLOCK) == -1)
		exitMsg("fcntl failed");
    if (setsockopt(this->serverSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        exitMsg("SETSOCKOPT FAILED");

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(this->port);
    addrlen = sizeof(addr);

    if (bind(this->serverSock, (struct sockaddr*)&addr, addrlen)  == -1)
        exitMsg("BINDING FAILED");

    if (listen(serverSock, SOMAXCONN) == -1)
        exitMsg("LISTENING FAILED");
	connectedFds = 1;
}

std::string server  ::  getPass()  const  {return password;}

int server  ::  getFd() const {return serverSock;}

int server  ::  getPort()  const {return port;}

void    server  ::  Connect()
{
	int pollRet = 0;

	poller.addSock(serverSock);
    while (true)
    {
		pollRet = poll(poller.getPointer(), connectedFds, -1);
		if (pollRet == -1)
			exitMsg("POLL FAILED");
		for (int i = 0; (unsigned int)i < connectedFds; i++)
		{
			if (!(poller.getRevent(i) & POLLIN))
				continue;
			if ((poller.getRevent(i) == 17))
			{
				int fd = poller.getFd(i);
				erase_clientData(get_client(fd)->getClientFd());
				close(fd);
				connectedFds--;
				puts("client has been disconnected");
				break ;
			}
			if (poller.getFd(i) == serverSock)
				newConnection(poller);
			else{
				try {
            	    communicateWClient(poller, i);
            	} catch (std::exception& e) {
            	    _fd_pool[poller.getFd(i)]->send(e.what());
            	}
			}				
		}
	}
}


void	server	::	newConnection(polldt &poller)
{
	struct sockaddr_in	clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);
	int	newSock;

	std::memset(&clientAddr, 0, clientAddrLen);
	newSock = accept(serverSock, (struct sockaddr*)&clientAddr, &clientAddrLen);
	std :: cout << "newSock : " << newSock  << std :: endl;
	if (newSock == -1)
		exitMsg("Accept  failed");
	if (fcntl(newSock, F_SETFL, O_NONBLOCK) == -1)
		exitMsg("fcntl failed");
	poller.addSock(newSock);
	Client* tmp = new Client(newSock);
	tmp->hostname=std::string(inet_ntoa(clientAddr.sin_addr));
	std::cout << "ip : " << tmp->hostname<< std::endl;
	_fd_pool[newSock] = tmp;
	connectedFds++;
}

std::string server::get_order_from_client(Client* client)
{
	std::string line = "";

	client->ft_read();
	if (client->is_cmd_ready()){
		line = client->get_buffer();
		client->empty_buffer();
	}
	return line;
}

void	server	::	communicateWClient(polldt &poller, int i)
{
	int fd  = poller.getFd(i);
	std::string line;
	Client* client = get_client(fd);

	line = get_order_from_client(client);
	if(line.empty())
		return ;
	std::cout<<"input : |>"<<line<<std::endl;
	Parser::Command cmd = _parser(line);
	if (cmd.authenticate != Parser::PASS && !client->getPass())
		throw ircExceptions::ERR_PASSNEEDED();
	
	authenticate(*client, cmd);
	if (cmd.authenticate == Parser::NOT_REGISTRED && client->getProfileStatus() == 0)
		throw ircExceptions::ERR_NOTREGISTERED();
	execCmd(cmd, *client);
}

void    server::erase_clientData(int fd)
{
	poller.removeSock(fd);
	Client *client =  get_client(fd);
	if (client->getNickName().empty() == 0)
	{
		_nickname_pool.erase(client->getNickName());
		if (client->get_myChannsList().size() != 0){
			std::vector<std::string> list= client->get_myChannsList();
			PART(list, *client);
		}
	}
	delete _fd_pool[fd];
	_fd_pool.erase(fd);
}

Client * server::get_client(int fd) {
	Client	*clt;
	try
	{
		clt = _fd_pool.at(fd);
		return clt;
	}
	catch(const std::exception& e)
	{
		throw ircExceptions::ERR_NOSUCHNICK("fd");
	}
	
}

Client * server::get_client(std::string nickname) {
	Client	*clt;
	try
	{
		if (nickname == "/Bot")
			throw ircExceptions::ERR_NOSUCHNICK(nickname);
		clt = _nickname_pool.at(nickname);
		return clt;
	}
	catch(const std::exception& e)
	{
		throw ircExceptions::ERR_NOSUCHNICK(nickname);
	}
}

void server::authenticate(Client& client, const Parser::Command& cmd)
{
	if (cmd.authenticate == Parser::NICK)
		NICK(client, cmd);
	else if (client.getProfileStatus() && cmd.authenticate != Parser::NOT_REGISTRED)
		throw ircExceptions::ERR_ALREADYREGISTRED();
	if (cmd.authenticate == Parser::PASS)
		PASS(client, cmd);
	else if (cmd.authenticate == Parser::USER)
		USER(client, cmd);
	if (!client.getProfileStatus() && client.registred()){
		client.setProfileStatus(true);
		IRCReplies::RPL_WELCOME(client);
		IRCReplies::RPL_YOURHOST(client);
		IRCReplies::RPL_CREATED(client);
		IRCReplies::RPL_MYINFO(client);
	}
}

void	server	::	execCmd(Parser::Command& cmd, Client &client)
{


	if (cmd.authenticate != Parser::NOT_REGISTRED)
		return ;
	if (cmd.command == Parser::PRIVMSG)
		PRIVMSG(client,cmd);
	else if (cmd.command == Parser::JOIN)
		JOIN(client,cmd);
	else if (cmd.command == Parser::KICK)
		KICK(client, cmd);
	else if (cmd.command == Parser::INVITE)
		INVITE(client, cmd);
	else if (cmd.command == Parser::TOPIC)
		TOPIC(client, cmd);
	else if (cmd.command == Parser::MODE)
		MODE(client, cmd);
	else if (cmd.command == Parser::QUIT)
		QUIT(client);
	else if (cmd.command == Parser::KICK)
		KICK(client, cmd);
	else if(cmd.command == Parser::BOT)
		BOT(client,cmd);
	else if (cmd.command == Parser::NOT_FOUND)
	{
		throw ircExceptions::ERR_UNKNOWNCOMMAND(cmd.args[0]);
	}
}

bool	server	::	nickExist(std::string name, std::map<std::string, Client *> _nickname_pool)
{
	try{
		_nickname_pool.at(name);
		return true;
	}
	catch(const std::exception& e){
		return false;}
}

void server::RPL_NAMREPLY(Client& client,
                         std::string channelname,
                         ChannelsHandler& channels) {

    std::vector<std::string> userlist = channels.get_Users(channelname);
     for (size_t i = 0; i < userlist.size(); i++)
        get_client(userlist[i])->send(":" +client.getNickName() + "!" + client.getUserName() + "@" +client.getHostname() + " JOIN :" + channelname + "\r\n");

    client.send(":irc.1337.com 353 " + client.getNickName() + " = " + channelname + " :");
    for (size_t i = 0; i < userlist.size(); i++) {
        if (channels.is_channel_admin(channelname, userlist[i]))
            client.send( "@");
        client.send(userlist[i] + " ");
    }
    client.send("\r\n");
}

void    server::QUIT(Client &client)
{
		int fd = client.getClientFd();
		erase_clientData(fd);
		connectedFds--;
		close(fd);
		std::cout<<"Client disconnected"<<std::endl;
}