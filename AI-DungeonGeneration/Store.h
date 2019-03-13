#pragma once
#include "Point2D.h"
enum SotreType
{
	health, munitions
};
class Store
{
public:
	Store();
	~Store();
	Store(const Point2D& point, int room, SotreType storeType);
	int GetRoomNumber();
	Point2D GetLocation();
	SotreType GetType();
private:
	int inRoom;
	Point2D location;
	SotreType type;
};

