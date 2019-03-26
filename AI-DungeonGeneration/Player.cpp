#include "Player.h"

extern int maze[MSIZE][MSIZE];
extern Store health_stores[NUM_HEALTH_STORE];
extern Store munitions_stores[NUM_MUNITIONS_STORE];
extern MazeMap* mazeMap;
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
	delete position;
	delete target;
}


Player::Player(Point2D *pos, int myRoom, int myColor)
{
	this->position = new TargetNode(myRoom, pos);
	this->myColor = myColor;
	target = NULL;
	lastColor = SPACE;
	state = NONE;
	astar = new AStar(pos);
	maze[pos->GetY()][pos->GetX()] = myColor;
	myHealth = 20;
	myMunitions = 5;
	minHealth = (rand() % (TOP_HEALTH - BUTTOM_HEALTH)) + BUTTOM_HEALTH;
	minMunitions = (rand() % (TOP_MUNITIONS - BUTTOM_MUNITIONS)) + BUTTOM_MUNITIONS;
	minHealthFireToKill = (rand() % (MIN_TOP_HEALTH - MIN_BUTTOM_HEALTH)) + MIN_BUTTOM_HEALTH;
	enemyClose = (rand() % (TOP_ENEMY_CLOSE - BUTTOM_ENEMY_CLOSE)) + BUTTOM_ENEMY_CLOSE;
}


int Player::GetCloseHealth()
{
	int minH, best, temp;
	for (int i = 0; i < NUM_HEALTH_STORE; i++)
	{
		temp = abs(position->GetPosition()->GetX() - health_stores[i].GetLocation().GetX()) +
			abs(position->GetPosition()->GetY() - health_stores[i].GetLocation().GetY());// Manhattan Distance
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
		temp = abs(position->GetPosition()->GetX() - munitions_stores[i].GetLocation().GetX()) +
			abs(position->GetPosition()->GetY() - munitions_stores[i].GetLocation().GetY());// Manhattan Distance
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
	return myHealth;
}

int Player::getMunitions()
{
	return myMunitions;
}

PlayerState Player::getState()
{
	return state;
}

int Player::EnemyDistance()
{
	return abs(position->GetPosition()->GetX() - (myEnemy->position->GetPosition()->GetX())) +
		abs(position->GetPosition()->GetY() - (myEnemy->position->GetPosition()->GetY()));// Manhattan Distance;
}

void Player::boom()
{
	int count = 0;
	int x = position->GetPosition()->GetX(), y = position->GetPosition()->GetY();
	for (int i = 1; i < START_FIRE; i++)
	{
		for (int j = 0; j < i * 2; j++)
		{
			if (x - i + j >= 0 && x - i + j < MSIZE - 1 && y + i < MSIZE - 1)//TOP
			{
				if (maze[y + i][x - i + j] == SPACE)
					count++;
			}
			if (x + i < MSIZE - 1 && y + i - j>0 && y + i - j < MSIZE - 1)//RIGHT
			{
				if (maze[y + i - j][x + i] == SPACE)
					count++;
			}
			if (x + i - j >= 0 && x + i - j < MSIZE - 1 && y - i >= 0)//BOTTOM
			{
				if (maze[y - i][x + i - j] == SPACE)
					count++;
			}
			if (x - i >= 0 && y - i + j < MSIZE - 1 && y - i + j >= 0)//LEFT
			{
				if (maze[y - i][x + i - j] == SPACE)
					count++;
			}
		}
	}

	if (count >= good) {
		myHealth = myHealth - (rand() % MAX_GOOD);
	}
	else {
		if (count >= mid) {
			myHealth = myHealth - (rand() % MAX_MID);
		}
		else
		{
			if (count >= bad) {
				myHealth = myHealth - (rand() % MAX_BAD);
			}
			else {
				myHealth--;
			}
		}
	}
	if (myHealth < 0)
	{
		myHealth = 0;
		play = false;
	}
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
				if (myMunitions > minMunitions)
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
			else
			{
				state = HEALTH;
			}
		}
	}
}

void Player::checkColor()
{
	switch (lastColor)
	{
	case HEALTH:
		myHealth = MAX_HEALTH;
		break;
	case MUNITIONS:
		myMunitions = MAX_MUNITIONS;
		break;
	default:
		break;
	}
}

void Player::runTo(Point2D p)
{
	position->GetPosition()->SetPos(p.GetX(), p.GetY());
	lastColor = maze[p.GetY()][p.GetX()];
	maze[p.GetY()][p.GetX()] = myColor;
	checkColor();
}

void Player::goToDir(int dir)
{
	int x = position->GetPosition()->GetX(), y = position->GetPosition()->GetY();
	maze[y][x] = lastColor;
	int newX = x, newY = y;
	switch (dir)
	{
	case UP:
		newY++;
		break;
	case DOWN:
		newY--;
		break;
	case LEFT:
		newX--;
		break;
	case RIGHT:
		newX++;
		break;
	default:
		break;
	}
	runTo(Point2D(newX, newY));
}

void Player::runToEnemy()
{
	int dir, enemyRoom = myEnemy->position->GetRoom();
	if (position->GetRoom() == enemyRoom)
	{
		dir = astar->run(*(myEnemy->position->GetPosition()));
	}
	else
	{
		RoomMapNode* node = mazeMap->FindPath(position->GetRoom(), enemyRoom);
		delete target;
		if (*(position->GetPosition()) == *(node->GetFromPoint())) {
			target = new TargetNode(node->GetToRoom(), new Point2D(*(node->GetToPoint())));
			position->SetRoom(node->GetToRoom());
		}
		else {
			target = new TargetNode(node->GetToRoom(), new Point2D(*(node->GetFromPoint())));
		}
		dir = astar->run(*(target->GetPosition()));
		delete node;
	}
	goToDir(dir);
}

void Player::runToHealth()
{
	int dir, healthStoreNum = GetCloseHealth();
	int room = health_stores[healthStoreNum].GetRoomNumber();
	if (position->GetRoom() == room)
	{
		dir = astar->run(health_stores[healthStoreNum].GetLocation());
	}
	else
	{
		RoomMapNode* node = mazeMap->FindPath(position->GetRoom(), room);
		delete target;
		if (*(position->GetPosition()) == *(node->GetFromPoint())) {
			target = new TargetNode(room, new Point2D(*(node->GetToPoint())));
			position->SetRoom(node->GetToRoom());
		}
		else {
			target = new TargetNode(room, new Point2D(*(node->GetFromPoint())));
		}
		dir = astar->run(*(target->GetPosition()));
	}
	goToDir(dir);
}


void Player::runToMunitions()
{
	int dir, munitionsStoreNum = GetCloseMunitions();
	int room = munitions_stores[munitionsStoreNum].GetRoomNumber();
	if (position->GetRoom() == room)
	{
		dir = astar->run(munitions_stores[munitionsStoreNum].GetLocation());
	}
	else
	{
		RoomMapNode* node = mazeMap->FindPath(position->GetRoom(), room);
		delete target;
		if (*(position->GetPosition()) == *(node->GetFromPoint())) {
			target = new TargetNode(room, new Point2D(*(node->GetToPoint())));
			position->SetRoom(node->GetToRoom());
		}
		else {
			target = new TargetNode(room, new Point2D(*(node->GetFromPoint())));
		}
		dir = astar->run(*(target->GetPosition()));
	}
	goToDir(dir);
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
	bool done = false;
	if (lastState == state)
	{
		int dir = astar->run(*(target->GetPosition()));
		if (dir != ERROR_DIR)
		{
			done = true;
			goToDir(dir);
		}
	}
	if (!done)
	{
		switch (state)
		{
		case NONE:
			break;
		case RUN_TO_ENEMY:
			runToEnemy();
			break;
		case FIRE:
			myEnemy->boom();
			break;
		case RUN_AWAY:
			break;
		case HEALTH:
			runToHealth();
			break;
		case MUNITIONS:
			runToMunitions();
			break;
		case DIED:
			myHealth = 0;
			break;
		default:
			break;
		}
	}
}