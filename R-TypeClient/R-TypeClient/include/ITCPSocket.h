#ifndef ITCPSocket_H_
# define ITCPSocket_H_

class ITCPSocket
{
public:
	ITCPSocket() = default;
	virtual ~ITCPSocket() = default;
	virtual void	init() = 0;
	virtual bool	connect(const std::string &ip, const unsigned short &port) = 0;
	virtual void	disconnect() = 0;
	virtual bool	receive(char* str, const int &size) = 0;
	virtual bool	send(char* str, const int &size) = 0;
	virtual bool	isConnected() = 0;
	virtual void	setBlocking(const bool&) = 0;
};

#endif /*! ITCPSocket_H_*/