#pragma once
#include "Point2D.h"
#include "Const.h"
#include <vector>
#include "TargetNode.h"
#include "MazeMap.h"
#include "Point2D_hg.h"
#include "ComparePoints.h"
#include <queue>

class AStar
{
public:
	AStar();
	AStar(Point2D *pos);
	~AStar();
	int run(Point2D target);
private:
	void AStar::SetSolution(Point2D target);
	int getDiraction(Point2D &p);
	Point2D* myPos,lastTarget;
	std::vector <Point2D_hg> solution;
};

