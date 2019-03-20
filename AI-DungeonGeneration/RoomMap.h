#pragma once
#include <vector>
#include "RoomMapNode.h"

class RoomMap
{
public:
	RoomMap();
	~RoomMap();
	RoomMap(int roomNumber);
	void AddRoom(RoomMapNode node);
	std::vector<RoomMapNode> GetConnectedRooms();
	int GetNumOfConn();
private:
	int room;
	int numOfConnectedRoom;
	std::vector<RoomMapNode> arr;
};

