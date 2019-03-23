#pragma once
#include "Point2D.h"
#include "Const.h"
#include "Store.h"
#include "MazeMap.h"
#include "TargetNode.h"


enum PlayerState {
	FIGHT, RUN_AWAY, HEALTH, MUNITIONS
};

class Player
{
public:
	Player();
	~Player();
	Player(Point2D &pos, int myRoom, Point2D &target, int targetRoom, int myColor);
	void Play();
private:
	PlayerState state;
	TargetNode position, target;
	int myColor, lastColor;
};

