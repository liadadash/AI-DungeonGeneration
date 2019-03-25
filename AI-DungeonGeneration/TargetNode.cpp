#include "TargetNode.h"



TargetNode::TargetNode()
{
	this->room = -1;
}

TargetNode::TargetNode(int room, Point2D* pos)
{
	this->room = room;
	this->pos = pos;
}


TargetNode::~TargetNode()
{
	delete pos;
}

int TargetNode::GetRoom()
{
	return room;
}

Point2D* TargetNode::GetPosition()
{
	return pos;
}

bool TargetNode::operator==(const TargetNode *other)
{
	return room == other->room && pos == other->pos;
}

void TargetNode::SetRoom(int room)
{
	this->room = room;
}
