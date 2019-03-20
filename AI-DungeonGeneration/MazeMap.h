#pragma once
#include <vector>
#include "RoomMap.h"
#include "Room.h"

class MazeMap
{
public:
	MazeMap();
	~MazeMap();
	int AddRoom();
	void AddNodeToRoom(int roomNumber, RoomMapNode node);
	RoomMap GetRoom(int index);
private:
	int numberOfRooms;
	std::vector<RoomMap> arr;
};

