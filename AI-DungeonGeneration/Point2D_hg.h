#pragma once
#include "Point2D.h"
#include <math.h>

#define WEIGHT 3

class Point2D_hg
{
public:
	Point2D_hg();
	~Point2D_hg();
	Point2D_hg(Point2D &myPos, Point2D &targetPoint);
	Point2D_hg(Point2D_hg *parent, Point2D &myPos, Point2D &targetPoint);
	Point2D getPoint();
	bool operator==(const Point2D_hg& other);
	int getH();
	int getG();
	int getF();
	Point2D_hg* getParent();
private:
	Point2D point;
	Point2D_hg *parent;
	int h;
	int g;
	int f;
};

