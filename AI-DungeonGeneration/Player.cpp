#include "Player.h"

extern int maze[MSIZE][MSIZE];
extern Store health_stores[NUM_HEALTH_STORE];
extern Store munitions_stores[NUM_MUNITIONS_STORE];
extern MazeMap mazeMap;

Player::Player()
{
}


Player::~Player()
{
}


Player::Player(Point2D &pos, int myRoom, Point2D &target, int targetRoom, int myColor)
{
	this->position = TargetNode(myRoom, pos);;
	this->target = TargetNode(targetRoom, target);
	this->myColor = myColor;
	lastColor = SPACE;
	state = FIGHT;
}


void Player::Play()
{

}