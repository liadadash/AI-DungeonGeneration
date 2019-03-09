#pragma once
class Point2D
{
public:
	Point2D();
	~Point2D();
private:
	int x;
	int y;
public:
	Point2D(int x, int y);
	int GetX() const;
	int GetY() const;
	bool operator==(const Point2D& other);
};

