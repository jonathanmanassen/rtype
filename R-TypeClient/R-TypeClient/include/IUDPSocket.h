#ifndef IUDPSocket_H_
# define IUDPSocket_H_

class IUDPSocket
{
public:
	IUDPSocket() = default;
	virtual ~IUDPSocket() = default;
	virtual void	init() = 0;
	virtual bool	bind(const unsigned short &port) = 0;
	virtual void	unbind() = 0;
	virtual bool	receive(char* str, const size_t &size, unsigned short port) = 0;
	virtual bool	send(char* str, const int &size, const std::string &ip, const unsigned short &port) = 0;
	virtual void	setBlocking(const bool&) = 0;
};

#endif /*!IUDPSocket*/