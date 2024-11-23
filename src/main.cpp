#include "../headers/server.hpp"


bool	check_port(std::string str){
	return (str.find_first_not_of("0123456789") == std::string::npos);
}

int main(int ac, char *av[])
{
	if (ac != 3)
	{
		std :: cout << "Wrong Number of Arguments" << std :: endl; 
		return 0;
	}
	std::string port = std::string(av[1]);

	if (!check_port(port)){
		std::cerr<<"ERROR\n";
		return 1;
	}
	server	elm((std::string)av[2], port);
	elm.Connect();

	return 0;
}