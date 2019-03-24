#include "Player.h"

extern int maze[MSIZE][MSIZE];
extern Store health_stores[NUM_HEALTH_STORE];
extern Store munitions_stores[NUM_MUNITIONS_STORE];
extern MazeMap mazeMap;
extern bool play;

#define TOP_HEALTH 60
#define BUTTOM_HEALTH 30
#define TOP_MUNITIONS 10
#define BUTTOM_MUNITIONS 5

#define MIN_TOP_HEALTH 20
#define MIN_BUTTOM_HEALTH 5

#define TOP_ENEMY_CLOSE 40
#define BUTTOM_ENEMY_CLOSE 10

Player::Player()
{
}


Player::~Player()
{
	delete astar;
}


Player::Player(Point2D &pos, int myRoom, int myColor)
{
	this->position = TargetNode(myRoom, pos);
	this->myColor = myColor;
	lastColor = SPACE;
	state = NONE;
	astar = new AStar(pos);
	maze[pos.GetY()][pos.GetX()] = myColor;
	myHealth = MAX_HEALTH;
	myMunitions = MAX_MUNITIONS;
	minHealth = (rand() % (TOP_HEALTH - BUTTOM_HEALTH)) + BUTTOM_HEALTH;
	minMunitions = (rand() % (TOP_MUNITIONS - BUTTOM_MUNITIONS)) + BUTTOM_MUNITIONS;
	minHealthFireToKill = (rand() % (MIN_TOP_HEALTH - MIN_BUTTOM_HEALTH)) + MIN_BUTTOM_HEALTH;
	enemyClose= (rand() % (TOP_ENEMY_CLOSE - BUTTOM_ENEMY_CLOSE)) + BUTTOM_ENEMY_CLOSE;
}


int Player::GetCloseHealth()
{
	int minH, best, temp;
	for (int i = 0; i < NUM_HEALTH_STORE; i++)
	{
		temp = abs(position.GetPosition().GetX() - health_stores[i].GetLocation().GetX()) +
			abs(position.GetPosition().GetY() - health_stores[i].GetLocation().GetY());// Manhattan Distance
		if (i == 0 || best > temp)
		{
			best = temp;
			minH = i;
		}
	}
	return minH;
}


void Player::setEnemy(Player *enemy)
{
	this->myEnemy = enemy;
}

int Player::GetCloseMunitions()
{
	int minH, best, temp;
	for (int i = 0; i < NUM_MUNITIONS_STORE; i++)
	{
		temp = abs(position.GetPosition().GetX() - munitions_stores[i].GetLocation().GetX()) +
			abs(position.GetPosition().GetY() - munitions_stores[i].GetLocation().GetY());// Manhattan Distance
		if (i == 0 || best > temp)
		{
			best = temp;
			minH = i;
		}
	}
	return minH;
}

int Player::getHealth()
{
	return health;
}

int Player::getMunitions()
{
	return munitions;
}

PlayerState Player::getState()
{
	return state;
}

int Player::EnemyDistance()
{
	return abs(position.GetPosition().GetX() - (myEnemy->position.GetPosition().GetX())) +
		abs(position.GetPosition().GetY() - (myEnemy->position.GetPosition().GetY()));// Manhattan Distance;
}



void Player::PlayDecision()
{
	if (myHealth > minHealth) {
		if (myMunitions > minMunitions)
		{
			if (EnemyDistance() < START_FIRE)
			{
				state = FIRE;
			}
			else
			{
				state = RUN_TO_ENEMY;
			}
		}
		else
		{
			state = MUNITIONS;
		}
	}
	else {
		if (myMunitions > minMunitions)
		{
			if (EnemyDistance() <= enemyClose)
			{
				if (myEnemy->myHealth <= minHealthFireToKill)
				{
					if (EnemyDistance() < START_FIRE)
					{
						state = FIRE;
					}
					else
					{
						state = RUN_TO_ENEMY;
					}
				}
				else {
					state = RUN_AWAY;
				}
			}
			else
			{
				state = HEALTH;
			}
		}
		else
		{
			if (myEnemy->myHealth < myHealth) {
				if (myMunitions>minMunitions)
				{
					if (EnemyDistance() <= enemyClose)
					{
						if (EnemyDistance() < START_FIRE)
						{
							state = FIRE;
						}
						else
						{
							state = RUN_TO_ENEMY;
						}
					}
					else
					{
						state = HEALTH;
					}
				}
				else
				{
					if (EnemyDistance() <= enemyClose)
					{
						state = RUN_AWAY;
					}
					else
					{
						state = HEALTH;
					}
				}
			}
		}
	}
}


void Player::Play()
{
	if (myHealth <= 0)
	{
		play = false;
		return;
	}
	PlayerState lastState = state;
	PlayDecision();
	if (lastState == state)
	{

	}
	else {

	}
}