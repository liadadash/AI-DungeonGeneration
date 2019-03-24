#include "MazeMap.h"



MazeMap::MazeMap()
{
	numberOfRooms = 0;
}


MazeMap::~MazeMap()
{
	for (int i = 0; i < numberOfRooms; i++)
	{
		delete arr.at(0);
	}
}


int MazeMap::AddRoom()
{
	arr.push_back(new RoomMap(numberOfRooms));
	return numberOfRooms++;
}

void MazeMap::AddNodeToRoom(int roomNumber, RoomMapNode* node)
{
	arr.at(roomNumber)->AddRoom(node);
}

RoomMap* MazeMap::GetRoom(int index)
{
	return arr.at(index);
}


RoomMapNode* MazeMap::FindPath(int fromRoom, int toRoom)
{
	bool found = false;
	std::vector <RoomMapNode*> gray;
	RoomMapNode* roomMapNode;
	int* parent = new int[numberOfRooms];
	RoomMapNode** parentNode = new RoomMapNode*[numberOfRooms];

	for (int i = 0; i < numberOfRooms; i++)
	{
		parent[i] = -1;
	}
	for (int i = 0; i < arr.at(fromRoom)->GetNumOfConn(); i++)
	{
		parent[arr.at(fromRoom)->GetConnectedRooms().at(i)->GetToRoom()] = fromRoom;
		parentNode[arr.at(fromRoom)->GetConnectedRooms().at(i)->GetToRoom()] = NULL;
		gray.push_back(arr.at(fromRoom)->GetConnectedRooms().at(i));
	}

	do
	{
		roomMapNode = gray[0];
		gray.erase(gray.begin());
		if (roomMapNode->GetToRoom() == toRoom) {
			found = true;
		}
		else {
			for (int i = 0; i < arr.at(roomMapNode->GetToRoom())->GetNumOfConn(); i++)
			{
				if (parent[arr.at(roomMapNode->GetToRoom())->GetConnectedRooms().at(i)->GetToRoom()] == -1)
				{
					parent[arr.at(roomMapNode->GetToRoom())->GetConnectedRooms().at(i)->GetToRoom()] = roomMapNode->GetToRoom();
					parentNode[arr.at(roomMapNode->GetToRoom())->GetConnectedRooms().at(i)->GetToRoom()] = new RoomMapNode(*roomMapNode);
					gray.push_back(arr.at(roomMapNode->GetToRoom())->GetConnectedRooms().at(i));
				}
			}
		}
	} while (!gray.empty() && !found);

	found = false;
	int room;
	do
	{
		room = parent[roomMapNode->GetToRoom()];
		if (parent[roomMapNode->GetToRoom()] == fromRoom) {
			found = true;
		}
		else {
			roomMapNode = parentNode[roomMapNode->GetToRoom()];
		}
	} while (!found);

	for (int i = 0; i < numberOfRooms; i++)
	{
		if (parent[i] != -1)
			delete parentNode[i];
	}
	delete[]parentNode;
	delete[]parent;
	return roomMapNode;
}
