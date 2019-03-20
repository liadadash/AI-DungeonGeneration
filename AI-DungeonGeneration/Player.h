#pragma once
#include "Point2D.h"
#include "Const.h"
#include "Store.h"

enum PlayerState {
	FIGHT, RUN_AWAY, HEALTH, MUNITIONS
};

class Player
{
public:
	Player();
	~Player();
	Player(Point2D &pos, Point2D &target, int myColor);
	void Play();
private:
	PlayerState state;
	int room;
	Point2D position, target;
	int myColor, lastColor;
};

