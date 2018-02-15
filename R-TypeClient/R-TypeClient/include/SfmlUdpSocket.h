#ifndef SfmlUdpSocket_H_
# define SfmlUdpSocket_H_

#include <string>
#include <vector>
#include <SFML\Network.hpp>
#include "IUDPSocket.h"
#include <thread>
#include "Protocol.h"

class SfmlUdpSocket : public IUDPSocket
{
	sf::UdpSocket	udpSocket;
	unsigned short	port = 4242;
public:
	SfmlUdpSocket() = default;
	const int				&getPort() { return port; }
	virtual void	init();
	virtual bool	bind(const unsigned short &port);
	virtual void	unbind();
	virtual bool	receive(char* str, const size_t &size, unsigned short port);
	virtual bool	send(char* str, const int &size, const std::string &ip, const unsigned short &port);
	virtual void	setBlocking(const bool&b) { udpSocket.setBlocking(b); }

};

#endif /*! AnimationSfmlUdpSocket_H_*/