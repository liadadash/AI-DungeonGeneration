#include "Player.h"

extern int maze[MSIZE][MSIZE];
extern Store health_stores[NUM_HEALTH_STORE];
extern Store munitions_stores[NUM_MUNITIONS_STORE];

Player::Player()
{
}


Player::~Player()
{
}


Player::Player(Point2D &pos, Point2D &target,int myColor)
{
	position = pos;
	this->target = target;
	this->myColor = myColor;
	lastColor = SPACE;
	state = FIGHT;
}


void Player::Play()
{

}