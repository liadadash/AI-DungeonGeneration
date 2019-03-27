#include "Player.h"

extern int maze[MSIZE][MSIZE];
extern Store health_stores[NUM_HEALTH_STORE];
extern Store munitions_stores[NUM_MUNITIONS_STORE];
extern MazeMap* mazeMap;
extern bool play;
extern Room all_rooms[2 * NUM_ROOMS];
extern int numberOfRoom;

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
	myHealth = MAX_HEALTH;
	myMunitions = MAX_MUNITIONS;
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

bool Player::iAmInRoom()
{
	int myRoomNum = position->GetRoom();
	Point2D *myPos = position->GetPosition();
	Room myRoom = all_rooms[myRoomNum];
	if (myRoom.IsInRoom(myPos->GetX(), myPos->GetY()))
	{
		RoomMap *roomMap = mazeMap->GetRoom(myRoomNum);
		for (int i = 0; i < roomMap->GetNumOfConn(); i++)
		{
			if ((*myPos) == (*roomMap->GetConnectedRooms().at(i)->GetFromPoint()))
				return false;
		}
		return true;
	}
	return false;
}

bool Player::iCanFire()
{
	int myRoomNum = position->GetRoom();
	Point2D *myPos = position->GetPosition();
	int enemyRoomNum = myEnemy->position->GetRoom();
	Point2D *enemyPos = myEnemy->position->GetPosition();
	Room myRoom = all_rooms[myRoomNum];
	Room enemyRoom = all_rooms[enemyRoomNum];

	if (myRoom.IsInRoom(myPos->GetX(), myPos->GetY()) && enemyRoom.IsInRoom(enemyPos->GetX(), enemyPos->GetY()))
	{
		int size;
		if (myPos->GetX() == enemyPos->GetX())
		{
			size = abs(myPos->GetY() - enemyPos->GetY());
			for (int i = 1; i < size; i++)
			{
				if (myPos->GetY() > enemyPos->GetY())
				{
					if (maze[myPos->GetY() - i][myPos->GetX()] == BLOCK)
					{
						return false;
					}
				}
				else
				{
					if (maze[myPos->GetY() + i][myPos->GetX()] == BLOCK)
					{
						return false;
					}
				}

			}
		}
		else
		{
			if (myPos->GetY() == enemyPos->GetY())
			{
				size = abs(myPos->GetX() - enemyPos->GetX());
				for (int i = 1; i < size; i++)
				{
					if (myPos->GetX() > enemyPos->GetX())
					{
						if (maze[myPos->GetY()][myPos->GetX() - i] == BLOCK)
						{
							return false;
						}
					}
					else
					{
						if (maze[myPos->GetY()][myPos->GetX() + i] == BLOCK)
						{
							return false;
						}
					}

				}
			}
			else
			{
				bool done = false, doneX = false, doneY = false;
				int x = 0, y = 0, checkX, checkY;
				do
				{
					if (!doneX)
					{
						if (myPos->GetX() > enemyPos->GetX())
						{
							if (myPos->GetX() - x >= enemyPos->GetX())
								checkX = myPos->GetX() + x--;
							else
								doneX = true;
						}
						else
						{
							if (myPos->GetX() + x <= enemyPos->GetX())
								checkX = myPos->GetX() + x++;
							else
								doneX = true;
						}
					}

					if (!doneY)
					{
						if (myPos->GetY() > enemyPos->GetY())
						{
							if (myPos->GetY() - y >= enemyPos->GetY())
								checkY = myPos->GetY() + y--;
							else
								doneY = true;
						}
						else
						{
							if (myPos->GetY() + y <= enemyPos->GetY())
								checkY = myPos->GetY() + y++;
							else
								doneY = true;
						}
					}

					if (!doneX || !doneY)
					{
						if (maze[checkY][checkX] == BLOCK)
							return false;
					}
				} while (!done);
			}
		}
		return true;
	}
	return false;
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
	myMunitions--;
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
			if (EnemyDistance() < START_FIRE && iCanFire())
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
					if (EnemyDistance() < START_FIRE && iCanFire())
					{
						state = FIRE;
					}
					else
					{

						state = RUN_TO_ENEMY;

					}
				}
				else {
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
			if (myEnemy->myHealth < myHealth) {
				if (myMunitions > minMunitions)
				{
					if (EnemyDistance() <= enemyClose)
					{
						if (EnemyDistance() < START_FIRE && iCanFire())
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
						state = RUN_TO_ENEMY;
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
	case HEALTH_COLOR:
		myHealth = MAX_HEALTH;
		break;
	case MUNITIONS_COLOR:
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
	checkColor();
	maze[p.GetY()][p.GetX()] = myColor;
}

void Player::goToDir(int dir)
{
	int x = position->GetPosition()->GetX(), y = position->GetPosition()->GetY();
	checkColor();
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
	delete target;

	if (position->GetRoom() == enemyRoom)
	{
		target = new TargetNode(enemyRoom, new Point2D(*(myEnemy->position->GetPosition())));
		dir = astar->run(*(myEnemy->position->GetPosition()));
	}
	else
	{
		RoomMapNode* node = mazeMap->FindPath(position->GetRoom(), enemyRoom);
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
	delete target;

	if (position->GetRoom() == room)
	{
		target = new TargetNode(room, new Point2D(health_stores[healthStoreNum].GetLocation()));
		dir = astar->run(health_stores[healthStoreNum].GetLocation());
	}
	else
	{
		RoomMapNode* node = mazeMap->FindPath(position->GetRoom(), room);
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
	delete target;

	if (position->GetRoom() == room)
	{
		target = new TargetNode(room, new Point2D(munitions_stores[munitionsStoreNum].GetLocation()));
		dir = astar->run(munitions_stores[munitionsStoreNum].GetLocation());
	}
	else
	{
		RoomMapNode* node = mazeMap->FindPath(position->GetRoom(), room);
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


void Player::runToSafePlace()
{
	Point2D* myPos = position->GetPosition();
	Point2D* enemyPos = myEnemy->position->GetPosition();
	Room room = all_rooms[position->GetRoom()];
	if (iAmInRoom() && abs(myPos->GetX() - enemyPos->GetX()) > abs(myPos->GetY() - enemyPos->GetY()))
	{
		if (myPos->GetX() > enemyPos->GetX())
		{
			if (room.IsInRoom(myPos->GetX() + 1, myPos->GetY()))
			{
				goToDir(RIGHT);
				return;
			}
			else
			{
				if (myPos->GetY() > enemyPos->GetY())
				{
					if (room.IsInRoom(myPos->GetX(), myPos->GetY() + 1))
					{
						goToDir(UP);
						return;
					}
					if (room.IsInRoom(myPos->GetX(), myPos->GetY() - 1))
					{
						goToDir(DOWN);
						return;
					}
				}
				else
				{
					if (room.IsInRoom(myPos->GetX(), myPos->GetY() - 1))
					{
						goToDir(DOWN);
						return;
					}
					if (room.IsInRoom(myPos->GetX(), myPos->GetY() + 1))
					{
						goToDir(UP);
						return;
					}
				}
			}
		}
		else
		{
			if (room.IsInRoom(myPos->GetX() - 1, myPos->GetY()))
			{
				goToDir(LEFT);
				return;
			}
			else
			{
				if (myPos->GetY() > enemyPos->GetY())
				{
					if (room.IsInRoom(myPos->GetX(), myPos->GetY() + 1))
					{
						goToDir(UP);
						return;
					}
					if (room.IsInRoom(myPos->GetX(), myPos->GetY() - 1))
					{
						goToDir(DOWN);
						return;
					}
				}
				else
				{
					if (room.IsInRoom(myPos->GetX(), myPos->GetY() - 1))
					{
						goToDir(DOWN);
						return;
					}
					if (room.IsInRoom(myPos->GetX(), myPos->GetY() + 1))
					{
						goToDir(UP);
						return;
					}
				}
			}
		}
	}
	else
	{
		if (myPos->GetY() > enemyPos->GetY())
		{
			if (room.IsInRoom(myPos->GetX(), myPos->GetY() + 1))
			{
				goToDir(UP);
				return;
			}
			else
			{
				if (myPos->GetX() > enemyPos->GetX())
				{
					if (room.IsInRoom(myPos->GetX() + 1, myPos->GetY()))
					{
						goToDir(RIGHT);
						return;
					}
					if (room.IsInRoom(myPos->GetX() - 1, myPos->GetY()))
					{
						goToDir(LEFT);
						return;
					}
				}
				else
				{
					if (room.IsInRoom(myPos->GetX() - 1, myPos->GetY()))
					{
						goToDir(LEFT);
						return;
					}
					if (room.IsInRoom(myPos->GetX() + 1, myPos->GetY()))
					{
						goToDir(RIGHT);
						return;
					}
				}
			}
		}
		else
		{
			if (room.IsInRoom(myPos->GetX(), myPos->GetY() - 1))
			{
				goToDir(DOWN);
				return;
			}
			else
			{
				if (myPos->GetX() > enemyPos->GetX())
				{
					if (room.IsInRoom(myPos->GetX() + 1, myPos->GetY()))
					{
						goToDir(RIGHT);
						return;
					}
					if (room.IsInRoom(myPos->GetX() - 1, myPos->GetY()))
					{
						goToDir(LEFT);
						return;
					}
				}
				else
				{
					if (room.IsInRoom(myPos->GetX() - 1, myPos->GetY()))
					{
						goToDir(LEFT);
						return;
					}
					if (room.IsInRoom(myPos->GetX() + 1, myPos->GetY()))
					{
						goToDir(RIGHT);
						return;
					}
				}
			}
		}
	}

	delete target;
	int dir;
	if (position->GetRoom() < numberOfRoom - 1)
	{
		target = new TargetNode(position->GetRoom() + 1, new Point2D(all_rooms[position->GetRoom() + 1].GetCenter()));
		dir = astar->run(all_rooms[position->GetRoom() + 1].GetCenter());
	}
	else
	{
		target = new TargetNode(position->GetRoom() - 1, new Point2D(all_rooms[position->GetRoom() - 1].GetCenter()));
		dir = astar->run(all_rooms[position->GetRoom() - 1].GetCenter());
	}
}


void Player::Play()
{
	if (myHealth <= 0 || myEnemy->myHealth <= 0)
	{
		if (myHealth <= 0)
			maze[position->GetPosition()->GetY()][position->GetPosition()->GetX()] = SPACE;
		else
			maze[position->GetPosition()->GetY()][position->GetPosition()->GetX()] = myColor;
		play = false;
		return;
	}
	PlayerState lastState = state;
	PlayDecision();
	bool done = false;
	if (lastState == state && target != NULL &&  state != FIRE)
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
			runToSafePlace();
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