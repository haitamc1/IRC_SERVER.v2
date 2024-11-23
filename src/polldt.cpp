#include "../headers/polldt.hpp"

polldt  ::  ~polldt()	{}

polldt  ::  polldt(){}

void	tm()
{
	// struct pollfd *data = new struct pollfd[128];
}

void	polldt	::	addSock(int fdSock)
{
	struct pollfd	elm;

	std::memset(&elm, 0, sizeof(elm));
	elm.fd = fdSock;
	elm.events = POLLIN;
	_poll.push_back(elm);
}

void	polldt	::	removeSock(int fdSock)
{
	for (std::vector<struct pollfd>::iterator it = _poll.begin(); it != _poll.end(); it++)
	{
		if (it->fd == fdSock)
		{
			_poll.erase(it);
			break;
		}
	}
}

short	polldt	::	getEvent(int index)	{return (_poll[index].events);}

short	polldt	::	getRevent(int index)	{return (_poll[index].revents);}

pollfd	*polldt	::	getPointer()	{return _poll.data();}

int	polldt	::	getFd(int index)	{return _poll[index].fd;}

size_t	polldt	::	getSize()	{return _poll.size();}
