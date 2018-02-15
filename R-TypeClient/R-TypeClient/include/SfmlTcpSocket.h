#ifndef SfmlTcpSocket_H_
# define SfmlTcpSocket_H_

#include <string>
#include <vector>
#include <SFML\Network.hpp>
#include "ITCPSocket.h"
#include <thread>
#include "Protocol.h"

class SfmlTcpSocket : public ITCPSocket
{
	sf::TcpSocket	tcpSocket;
	sf::IpAddress	ip = "127.0.0.1";
	unsigned short	port = 4242;
public:
	SfmlTcpSocket() = default;
	const std::string		&getAddress() { return ip.toString(); }
	const int				&getPort() { return port; }
	virtual void	init();
	virtual bool	connect(const std::string &ip, const unsigned short &port);
	virtual void	disconnect();
	virtual bool	receive(char* str, const int &size);
	virtual bool	send(char* str, const int &size);
	virtual bool	isConnected();
	virtual void	setBlocking(const bool&b) { tcpSocket.setBlocking(b); }

};

#endif /*! AnimationSfmlTcpSocket_H_*/