#pragma once
#include "Point2D.h"
#include "Const.h"
#include "Store.h"
#include "MazeMap.h"
#include "TargetNode.h"
#include "AStar.h"

const int START_FIRE = 4;
const int MAX_HEALTH = 100;
const int MAX_MUNITIONS = 25;

const int good = 35;
const int mid = 20;
const int bad = 15;

const int MAX_GOOD = 20;
const int MAX_MID = 10;
const int MAX_BAD = 6;


enum PlayerState {
	NONE, RUN_TO_ENEMY, FIRE, RUN_AWAY, HEALTH, MUNITIONS, DIED
};

class Player
{
public:
	Player();
	~Player();
	Player(Point2D *pos, int myRoom, int playerColor);
	void Play();
	void setEnemy(Player *enemy);
	int getHealth();
	int getMunitions();
	PlayerState getState();
private:
	void runToHealth();
	void runToMunitions();
	void runTo(Point2D p);
	void checkColor();
	void goToDir(int dir);
	void boom();
	void runToEnemy();
	int EnemyDistance();
	int GetCloseHealth();
	int GetCloseMunitions();
	void PlayDecision();
	PlayerState state;
	AStar *astar;
	Player *myEnemy;
	TargetNode *position, *target;
	int myColor, lastColor;
	int myHealth, myMunitions;
	int minHealth, minMunitions,minHealthFireToKill,enemyClose;
};

