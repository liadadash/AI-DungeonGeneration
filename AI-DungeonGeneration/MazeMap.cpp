#include "MazeMap.h"



MazeMap::MazeMap()
{
	numberOfRooms = 0;
}


MazeMap::~MazeMap()
{
}


int MazeMap::AddRoom()
{
	arr.push_back(RoomMap(numberOfRooms));
	return numberOfRooms++;
}

void MazeMap::AddNodeToRoom(int roomNumber, RoomMapNode node)
{
	arr.at(roomNumber).AddRoom(node);
}

RoomMap MazeMap::GetRoom(int index)
{
	return arr.at(index);
}
