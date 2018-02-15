#ifndef SystemManager_H_
# define SystemManager_H_

#include <vector>
#include <memory>
#include "System.h"
#include "Ressources.h"

struct ServerInGameInfo;
class PropertyComponent;

class SystemManager
{
	std::vector<std::shared_ptr<System>>	systems;
	std::vector<std::shared_ptr<Entity>>	tmpEntities;
	std::vector<ServerInGameInfo>	entPlayerInfo;
	ServerInGameInfo				entReadyButtonInfo;
	std::vector<std::shared_ptr<Entity>>			starfieldEntities;
	bool	isGameRunning;
	int		entityNb = 0;
	std::shared_ptr<Entity>			notificationEnt;
	std::string	playerNames[4] = { "","","","" };
public:
	SystemManager();
	~SystemManager();
	void	execLoop();
	void	addSystem(std::shared_ptr<System>);
	void	removeSystem(const std::string&);
	void	initBaseSystems();
	void	initStarfield();
	void	initBasePlayerEntInfo();
	std::vector<ServerInGameInfo> initBaseConnexionEntities();
	std::vector<ServerInGameInfo> initBaseConnectedEntities();
	std::vector<ServerInGameInfo> initBaseInRoomEntities();
	std::vector<ServerInGameInfo> initStarfieldEntityInfo();
	ServerInGameInfo	genInGameInfo(const std::string &name,
		const int &posX,
		const int& posY,
		const int &loopMode,
		const AnimationType &type);
	void	genLoadRessourcesInfo(const std::string &name,
		const std::string &fileName,
		const int &beginX,
		const int &beginY,
		const int &height,
		const int &width,
		const int &incX,
		const AnimationType &type,
		const int &nbSprite,
		const bool &scale);
	void	settingUpConnectedMenuEntity(std::shared_ptr<Entity> ent, std::shared_ptr<PropertyComponent>	propComp);
	void	settingUpConnexionMenuEntity(std::shared_ptr<Entity> ent, std::shared_ptr<PropertyComponent> propComp, const int &idx);
	void	clearEntities();
	void	initBaseRessources();
	void	actualiseEntity(ServerInGameInfo const &info);
	ServerInGameInfo	genRoomStruct(const ServerRoomsInfo &info, const int &roomNb);
	void	interpretNetworkInfo();
	void	menuInitialisation();
	void	actualiseRooms();
	void	actualiseInRoom();
	void	restoreBaseEnt();
	void	addToSystems(std::shared_ptr<Entity> ent);
	void	deleteUnusedEntities();
	void	initNotification();
};

#endif /*! SystemManager_H_*/