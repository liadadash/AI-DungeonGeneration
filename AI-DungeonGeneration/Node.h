#pragma once
#include "Point2D.h"
class Node
{
public:
	Node();
	~Node();
private:
	Point2D point,target;
	double g;
public:
	double GetF() const;
	double GetG() const;
	double GetH() const;
	Node(const Point2D& p, const Point2D& t, double g);
	Point2D GetPoint();
};

