#pragma once
#include "Point2D.h"

class RoomMapNode
{
public:
	RoomMapNode();
	~RoomMapNode();
	RoomMapNode(Point2D fromRoomPoint, Point2D toRoomPoint, int toRoom, int direction);
	Point2D GetFromPoint();
	Point2D GetToPoint();
	int GetToRoom();
	int GetDirection();
private:
	Point2D fromRoomPoint, toRoomPoint;
	int toRoom;
	int direction;
};

