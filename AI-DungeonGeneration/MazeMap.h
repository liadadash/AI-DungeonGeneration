#pragma once
#include <vector>
#include "RoomMap.h"
#include "Room.h"
#include "Const.h"

class MazeMap
{
public:
	MazeMap();
	~MazeMap();
	int AddRoom();
	void AddNodeToRoom(int roomNumber, RoomMapNode node);
	RoomMap GetRoom(int index);
	RoomMapNode FindPath(int fromRoom, int toRoom);
private:
	int numberOfRooms;
	std::vector<RoomMap> arr;
};

