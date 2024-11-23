#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

class	Bot
{
	private:
		std::string host;
		std::string	port;
	public:
		~Bot();
		Bot(std::string  host, std::string port);
		void	run();
		void	help(int fd, std::string nick);

};