#pragma once
#include "Point2D.h"

class RoomMapNode
{
public:
	RoomMapNode();
	~RoomMapNode();
	RoomMapNode(Point2D *fromRoomPoint, Point2D *toRoomPoint,int fromRoom, int toRoom, int direction);
	Point2D *GetFromPoint();
	Point2D *GetToPoint();
	int GetToRoom();
	int GetFromRoom();
	int GetDirection();
	bool operator==(const RoomMapNode& other);
private:
	Point2D *fromRoomPoint, *toRoomPoint;
	int fromRoom,toRoom;
	int direction;
};

