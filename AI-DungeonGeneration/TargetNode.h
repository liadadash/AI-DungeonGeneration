#pragma once
#include "Point2D.h"
class TargetNode
{
public:
	TargetNode();
	TargetNode(int room, Point2D &pos);
	~TargetNode();
	int GetRoom();
	Point2D GetPosition();
	bool operator==(const TargetNode& other);
private:
	int room;
	Point2D pos;
};

