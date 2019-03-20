#pragma once
#include "Point2D.h"
class Room
{
public:
	Room();
	~Room();
private:
	Point2D center;
	int width, height;
public:
	Room(const Point2D& center_point, int w, int h);
	bool IsInRoom(int x, int y);
	Point2D GetCenter() const;
	int GetWidth();
	int GetHeight();
	bool IsOverlap(const Room& other);
};

