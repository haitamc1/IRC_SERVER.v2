#include "../headers/Bot.hpp"

void    exitMsg(std::string s)
{
    std :: cerr << s << std :: endl;
    exit (EXIT_FAILURE);
}

Bot	::	Bot(std::string  host, std::string port)
{
    this->host = host;
    this->port = port;

}

Bot	::	~Bot(){}

void	sendErr(int rt)
{
	if (rt == -1)
	{
		std::cerr << "send failed" << std::endl;
		exit(EXIT_FAILURE);
	}
}

int ft_send(int fd, std::string buffer){
	int size = send(fd, buffer.c_str(), buffer.size(), 0);
	if (size == -1)
		std::cerr << "send failed" << std::endl;
	return size;
}

void	authenticateBot(int sock)
{
    std::string msg = "pass Bot";
	msg += 1;
	msg += "\r\n";
    ft_send(sock, msg.c_str());
}

int	connectToServer(std::string const &host, std::string const &port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        std::cerr << "socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(atoi(port.c_str()));
    if (inet_pton(AF_INET, host.c_str(), &sockaddr.sin_addr) <= 0)
    {
        std::cerr << "Failed to setup server address" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (connect(sock, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
    {
        std::cerr << "connect Failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    return (sock);
}

void	Bot::help(int fd, std::string nick)
{

	ft_send(fd,"PRIVMSG " + nick + " :Welcome to your friendly IRC Bot\n");
	usleep(10000);
	ft_send(fd,"PRIVMSG " + nick + " :The available command to mess with the server\n");
	usleep(10000);
	ft_send(fd,"PRIVMSG " + nick + " :to troll the server enter : troll\n");
	usleep(10000);
	ft_send(fd,"PRIVMSG " + nick + " :to make the server sleep : sleep\n");
	usleep(10000);
	ft_send(fd,"PRIVMSG " + nick + " :to introduce the server to Giovani Giorgio : giorgio\n");
	usleep(10000);
	ft_send(fd,"PRIVMSG " + nick + " :watch the olympics on the server : olympics\n");
	usleep(10000);
	ft_send(fd,"PRIVMSG " + nick + " :reavel the secret mistery of mermaids : secret footage\n");
	usleep(10000);
	ft_send(fd,"PRIVMSG " + nick + " :if you want to fly : fly\n");
	usleep(10000);
	ft_send(fd,"PRIVMSG " + nick + " :Thank you for tuning in\n" );
}

void	Bot::run()
{
    int sock = connectToServer(this->host, this->port);
	authenticateBot(sock);
    std::cout << "Connected to the server" << std::endl;
    while(1)
    {
        char buffer[1024];

        int rd = recv(sock, &buffer, 1024, 0);
        if (rd <= 0)
        {
			if (rd != -1)
			{
				std::cout << "Server disconnected" << std::endl;
				exit(EXIT_SUCCESS);
			}
			else{
				std::cerr << "recv failed" << std::endl;
				exit(EXIT_FAILURE);
			}
        }
		else{
			buffer[rd] = 0;
			std::string s(buffer);
			std::string cmd;
			std::string nick;
			size_t i;
			for (i = 0; i < s.size() && s[i] != ' '; i++)
					nick += s[i];
			for (; i < s.size() && s[i] == ' '; i++);
			for (; i < s.size(); i++)
					cmd += s[i];
			std::cout << "cmd : " << cmd << std::endl;
			if (nick.empty())
				continue;
			else if (cmd == "troll")
				system("open -a \"Google Chrome\" https://www.youtube.com/watch?v=dQw4w9WgXcQ");
			else if (cmd == "sleep")
				system("open -a \"Google Chrome\" https://www.youtube.com/watch?v=ruUrFJS0QA8");
			else if (cmd == "giorgio")
				system("open -a \"Google Chrome\" https://www.youtube.com/watch?v=i2U50K13-Hg");
			else if (cmd == "olympics")
				system("open -a \"Google Chrome\" https://www.youtube.com/watch?v=pk0kcZoab2E");
			else if (cmd == "fly")
				system("open -a \"Google Chrome\" https://www.youtube.com/watch?v=_cveZk2MpN4");
			else if (cmd == "secret footage")
				system("open -a \"Google Chrome\" https://www.youtube.com/watch?v=W06JB1UatrI");
			else
				help(sock, nick);
		}

    }
}


int main(int ac, char **av)
{
    if (ac != 3)
        return (1);
    Bot bot(av[1], av[2]);
    bot.run();
    return (0);
}
