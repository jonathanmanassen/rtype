#ifndef Network_H_
# define Network_H_

#include <string>
#include <vector>
#include <SFML\Network.hpp>
#include <thread>
#include "Protocol.h"
#include "SfmlTcpSocket.h"
#include "SfmlUdpSocket.h"

class Network
{
	sf::TcpSocket	tcpSocket;
	sf::UdpSocket	udpSocket;
	bool			isUnreadedTcpMessage = false;
	bool			isUnreadedUdpMessage = false;
	std::thread	    tcpThread;
	std::thread		udpThread;
	std::thread		udpSendThread;
	sf::IpAddress	ip;
	std::string		address = "127.0.0.1";
	int				port = 4242;
	unsigned short	udpPort;
	int				id;
	std::vector<ServerRoomsInfo>	roomsInfo;
	ServerInRoomInfo	inRoomInfo;
	std::unordered_map<int,ServerInGameInfo>	inGameInfo;
	bool			isClearingInGameInfo = false;
	std::vector<ServerLoadRessourcesInfo>	ressourcesInfo;
	static Network *network;
	Network();
	bool	isWaiting = false;
	std::string	lastMessageType;
	std::string lastAction;
	std::list<ControllerAction>	messagesInQueue;
public:
	static Network *Network::get();
	void	sendUdpMessages();
	void	askToClearInGameMap() { isClearingInGameInfo = true; }
	bool	initTcpNetwork(std::string const &ip, int const &port);
	bool	initUdpNetwork();
	void	tcpDisconnnect();
	void	sendControllerMessage(const bool&, const bool&, const bool&, const bool&, bool const &isShooting);
	void	receiveUdpMessage();
	const std::string		&getAddress() { return address; }
	const int				&getPort() { return port; }
	const std::string		&getLastMessageType() { return lastMessageType; }
	const std::string		&getLastAction() { return lastAction; }
	void	receiveTcpMessage();
	const bool	&getIsUndredeTcpMessage() { return isUnreadedTcpMessage; }
	const bool	&getIsUndredeUdpMessage() { return isUnreadedUdpMessage; }
	void	setTcpMessageReaded() { isUnreadedTcpMessage = false; }
	void	setUdpMessageReaded() { isUnreadedUdpMessage = false; }
	const std::vector<ServerRoomsInfo>	&getRoomsInfo() { return roomsInfo; }
	const ServerInRoomInfo	&getInRoomInfo() { return inRoomInfo; }
	const std::unordered_map<int,ServerInGameInfo>	&getInGameInfo() { return inGameInfo; }
	const std::vector<ServerLoadRessourcesInfo>	&getRessourcesInfo() { return ressourcesInfo; }
	bool	hasInGameInfo(const int &id);
	void	startWait() { isWaiting = true; }
	void	disconnectTcpFromServer();
	void	disconnectUdpFromServer();
	void	sendTcpMessage(const ConnectionAction &act, std::string const &name, const int &roomNb, const std::string &level);
	void	clearInGameInfo() { inGameInfo.clear(); isClearingInGameInfo = false; }
};

#endif /*! AnimationNetwork_H_*/