#include "Store.h"



Store::Store()
{
	inRoom = 0;
}


Store::~Store()
{
}

Store::Store(const Point2D& point, int room, SotreType storeType)
{
	inRoom = room;
	location = point;
	type = storeType;
}

int Store::GetRoomNumber()
{
	return inRoom;
}

Point2D Store::GetLocation()
{
	return location;
}

SotreType Store::GetType()
{
	return type;
}