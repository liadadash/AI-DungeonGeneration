#include "RoomMap.h"



RoomMap::RoomMap()
{
}


RoomMap::~RoomMap()
{
	for (int i = 0; i < numOfConnectedRoom; i++)
	{
		delete arr.at(0);
	}
}

RoomMap::RoomMap(int roomNumber)
{
	room = roomNumber;
	numOfConnectedRoom = 0;
}

void RoomMap::AddRoom(RoomMapNode *node)
{
	arr.push_back(node);
	numOfConnectedRoom++;
}

int RoomMap::GetNumOfConn()
{
	return numOfConnectedRoom;
}

std::vector<RoomMapNode*> RoomMap::GetConnectedRooms()
{
	return arr;
}