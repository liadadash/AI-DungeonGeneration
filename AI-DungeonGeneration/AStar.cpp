#include "AStar.h"

extern int maze[MSIZE][MSIZE];

AStar::AStar()
{

}


AStar::~AStar()
{
}


AStar::AStar(Point2D *pos)
{
	myPos = pos;
	lastTarget = Point2D(-1, -1);
}

int AStar::getDiraction(Point2D &p)
{
	if (myPos->GetX() != p.GetX())
	{
		if (myPos->GetX() > p.GetX())
			return LEFT;
		else
			return RIGHT;
	}
	else
	{
		if (myPos->GetY() > p.GetY())
			return DOWN;
		else
			return UP;
	}
}


void AStar::SetSolution(Point2D target)
{
	Point2D_hg bestPoint;
	std::priority_queue <Point2D_hg, std::vector <Point2D_hg>, ComparePoints> pq;
	std::vector <Point2D_hg> black;
	std::vector <Point2D_hg> gray;
	bestPoint = Point2D_hg(*myPos, target);
	pq.emplace(bestPoint);
	gray.push_back(bestPoint);
	Point2D_hg *bestPointAsParent, neighborPos_hg;
	Point2D bestPointPos, neighborPos;
	std::vector<Point2D_hg>::iterator black_iterator;
	std::vector<Point2D_hg>::iterator gray_iterator;
	do {
		bestPoint = pq.top();
		pq.pop();
		gray_iterator = find(gray.begin(), gray.end(), bestPoint);
		gray.erase(gray_iterator);
		black.push_back(bestPoint);
		bestPointAsParent = new Point2D_hg(bestPoint);
		bestPointPos = bestPointAsParent->getPoint();
		if (bestPointPos == target) {
			while (bestPointAsParent->getParent() != NULL)
			{
				solution.push_back(*bestPointAsParent);
				bestPointAsParent = bestPointAsParent->getParent();
			}
			break;
		}

		neighborPos = Point2D(bestPointPos.GetX() + 1, bestPointPos.GetY());
		if (maze[neighborPos.GetY()][neighborPos.GetX()] != WALL && maze[neighborPos.GetY()][neighborPos.GetX()] != BLOCK) {
			neighborPos_hg = Point2D_hg(bestPointAsParent, neighborPos, target);
			black_iterator = find(black.begin(), black.end(), neighborPos_hg);
			gray_iterator = find(gray.begin(), gray.end(), neighborPos_hg);
			if (black_iterator == black.end() && gray_iterator == gray.end())
			{
				pq.emplace(neighborPos_hg);
				gray.push_back(neighborPos_hg);
			}
		}

		neighborPos = Point2D(bestPointPos.GetX() - 1, bestPointPos.GetY());
		if (maze[neighborPos.GetY()][neighborPos.GetX()] != WALL && maze[neighborPos.GetY()][neighborPos.GetX()] != BLOCK) {
			neighborPos_hg = Point2D_hg(bestPointAsParent, neighborPos, target);
			black_iterator = find(black.begin(), black.end(), neighborPos_hg);
			gray_iterator = find(gray.begin(), gray.end(), neighborPos_hg);
			if (black_iterator == black.end() && gray_iterator == gray.end())
			{
				pq.emplace(neighborPos_hg);
				gray.push_back(neighborPos_hg);
			}
		}

		neighborPos = Point2D(bestPointPos.GetX(), bestPointPos.GetY() + 1);
		if (maze[neighborPos.GetY()][neighborPos.GetX()] != WALL && maze[neighborPos.GetY()][neighborPos.GetX()] != BLOCK) {
			neighborPos_hg = Point2D_hg(bestPointAsParent, neighborPos, target);
			black_iterator = find(black.begin(), black.end(), neighborPos_hg);
			gray_iterator = find(gray.begin(), gray.end(), neighborPos_hg);
			if (black_iterator == black.end() && gray_iterator == gray.end())
			{
				pq.emplace(neighborPos_hg);
				gray.push_back(neighborPos_hg);
			}
		}

		neighborPos = Point2D(bestPointPos.GetX(), bestPointPos.GetY() - 1);
		if (maze[neighborPos.GetY()][neighborPos.GetX()] != WALL && maze[neighborPos.GetY()][neighborPos.GetX()] != BLOCK) {
			neighborPos_hg = Point2D_hg(bestPointAsParent, neighborPos, target);
			black_iterator = find(black.begin(), black.end(), neighborPos_hg);
			gray_iterator = find(gray.begin(), gray.end(), neighborPos_hg);
			if (black_iterator == black.end() && gray_iterator == gray.end())
			{
				pq.emplace(neighborPos_hg);
				gray.push_back(neighborPos_hg);
			}
		}
	} while (true);
}


int AStar::run(Point2D target)
{
	if (!(lastTarget == target && !solution.empty()))
	{
		lastTarget = Point2D(target);
		solution.clear();
		SetSolution(lastTarget);
	}
	if (!solution.empty())
	{
		Point2D_hg best = solution.back();
		solution.pop_back();
		return getDiraction(best.getPoint());;
	}
	return ERROR_DIR;
}