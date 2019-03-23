#include "AStar.h"

extern MazeMap mazeMap;

AStar::AStar()
{

}


AStar::~AStar()
{
}


AStar::AStar(TargetNode &pos)
{
	myPos = pos;
}

int AStar::getDiraction(Point2D &p)
{
	if (myPos.GetPosition().GetX() != p.GetX())
	{
		if (myPos.GetPosition().GetX() > p.GetX())
			return DOWN;
		else
			return UP;
	}
	else
	{
		if (myPos.GetPosition().GetY() > p.GetY())
			return LEFT;
		else
			return RIGHT;
	}
}


void AStar::SetSolution(TargetNode target)
{

}


int AStar::run(TargetNode target)
{
	if (myPos.GetRoom() == target.GetRoom())
	{
		if (lastTarget == target)
		{

		}
		else
		{

		}
	}
	else {
		if (lastTarget.GetRoom() == target.GetRoom())
		{

		}
		else
		{

		}
	}

	return 1;
	//if (lastTarget == target) {
	//	if (!solution.empty()) {
	//		int dir = getDiraction(solution.back());
	//		solution.pop_back();
	//		return dir;
	//	}
	//	else {
	//		//find the path to the next room
	//		//SetSolution(target);
	//		mazeMa
	//		int dir = getDiraction(solution.back());
	//		solution.pop_back();
	//		return dir;
	//	}
	//}
	//else {
	//	if (myPos.GetRoom() != lastTarget.GetRoom() && lastTarget.GetRoom() == target.GetRoom() && !solution.empty()) {
	//		int dir = getDiraction(solution.back());
	//		solution.pop_back();
	//		return dir;
	//	}
	//	else {
	//		if (myPos.GetRoom() == target.GetRoom()) {

	//		}
	//		else {

	//		}
	//	}
	//}




	//if (!solution.empty())
	//{
	//	if (lastTarget == target) {
	//		int dir = getDiraction(solution.back());
	//		solution.pop_back();
	//		return dir;
	//	}
	//	else {
	//		lastTarget = target;
	//		if (std::find(solution.begin(), solution.end(), target) != solution.end())
	//		{
	//			solution.erase(std::find(solution.begin(), solution.end(), target), solution.end());
	//			int dir = getDiraction(solution.back());
	//			solution.pop_back();
	//			return dir;
	//		}
	//		else
	//		{
	//			solution.clear();
	//		}
	//	}
	//}
	//
	////New run

	//return 1;
}