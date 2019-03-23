#pragma once
#include "Point2D.h"
#include "Const.h"
#include <vector>
#include "TargetNode.h"
#include "MazeMap.h"

class AStar
{
public:
	AStar();
	AStar(TargetNode &pos);
	~AStar();
	int run(TargetNode target);
private:
	void AStar::SetSolution(TargetNode target);
	int getDiraction(Point2D &p);
	TargetNode myPos,lastTarget;
	std::vector <Point2D> solution;
};

