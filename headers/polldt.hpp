#ifndef POLLDT_HPP
#define POLLDT_HPP

#include <vector>
#include <poll.h>

class   polldt
{
    private:
	std::vector<struct pollfd>	_poll;

    public:
		~polldt();
		polldt();

		void	addSock(int fdSock);
		void	removeSock(int fdSock);
		short	getEvent(int index);
		short	getRevent(int index);
		size_t	getSize();
		
		pollfd	*getPointer();
		int		getFd(int index);
};
#endif