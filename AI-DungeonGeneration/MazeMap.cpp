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
		parentNode[i] = NULL;
	}
	for (int i = 0; i < arr.at(fromRoom)->GetNumOfConn(); i++)
	{
		parent[arr.at(fromRoom)->GetConnectedRooms().at(i)->GetToRoom()] = fromRoom;
		gray.push_back(arr.at(fromRoom)->GetConnectedRooms().at(i));
		//printf("%d -> %d \n", fromRoom, arr.at(fromRoom)->GetConnectedRooms().at(i)->GetToRoom());
	}

	do
	{
		roomMapNode = gray[0];
		gray.erase(gray.begin());
		if (roomMapNode->GetToRoom() == toRoom) {
			found = true;
			//printf("%d -> %d \n", roomMapNode->GetToRoom(), toRoom);
		}
		else {
			for (int i = 0; i < arr.at(roomMapNode->GetToRoom())->GetNumOfConn(); i++)
			{
				if (parent[arr.at(roomMapNode->GetToRoom())->GetConnectedRooms().at(i)->GetToRoom()] == -1)
				{
					parent[arr.at(roomMapNode->GetToRoom())->GetConnectedRooms().at(i)->GetToRoom()] = roomMapNode->GetToRoom();
					parentNode[arr.at(roomMapNode->GetToRoom())->GetConnectedRooms().at(i)->GetToRoom()]
						= new RoomMapNode(new Point2D(*(roomMapNode->GetFromPoint())), new Point2D(*(roomMapNode->GetToPoint()))
							, roomMapNode->GetFromRoom(), roomMapNode->GetToRoom(), roomMapNode->GetDirection());
					gray.push_back(arr.at(roomMapNode->GetToRoom())->GetConnectedRooms().at(i));
				}
				//printf("%d -> %d \n", roomMapNode->GetToRoom(), arr.at(roomMapNode->GetToRoom())->GetConnectedRooms().at(i)->GetToRoom());
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
			roomMapNode = new RoomMapNode(new Point2D(*(roomMapNode->GetFromPoint())), new Point2D(*(roomMapNode->GetToPoint()))
				, roomMapNode->GetFromRoom(), roomMapNode->GetToRoom(), roomMapNode->GetDirection());
		}
		else {
			roomMapNode = parentNode[roomMapNode->GetToRoom()];
		}
	} while (!found);

	for (int i = 0; i < numberOfRooms; i++)
	{
		RoomMapNode* r = parentNode[i];
		int r2 = parent[i];
		if (parent[i] != -1 && parent[i] != fromRoom)
			delete parentNode[i];
	}
	delete[]parentNode;
	delete[]parent;
	return roomMapNode;
}