#include "RoomMapNode.h"



RoomMapNode::RoomMapNode()
{
}


RoomMapNode::~RoomMapNode()
{
}


RoomMapNode::RoomMapNode(Point2D fromRoomPoint, Point2D toRoomPoint, int fromRoom, int toRoom, int direction)
{
	this->fromRoomPoint = fromRoomPoint;
	this->toRoomPoint = toRoomPoint;
	this->fromRoom = fromRoom;
	this->toRoom = toRoom;
	this->direction = direction;
}

Point2D RoomMapNode::GetFromPoint()
{
	return fromRoomPoint;
}

Point2D RoomMapNode::GetToPoint()
{
	return toRoomPoint;
}

int RoomMapNode::GetToRoom()
{
	return toRoom;
}

int RoomMapNode::GetDirection()
{
	return direction;
}

int RoomMapNode::GetFromRoom()
{
	return fromRoom;
}


bool RoomMapNode::operator==(const RoomMapNode& other)
{
	return fromRoomPoint == other.fromRoomPoint && toRoomPoint == other.toRoomPoint &&fromRoom == other.fromRoom &&
		toRoom == other.toRoom &&direction == other.direction;
}