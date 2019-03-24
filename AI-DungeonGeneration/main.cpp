#include "GLUT.h"
#include <math.h>
#include <time.h>
#include <vector>
#include <queue>
#include <algorithm>
#include "Point2D.h"
#include "Room.h"
#include "Node.h"
#include "CompareNodes.h"
#include "Parent.h"
#include "Store.h"
#include "MazeMap.h"
#include "Const.h"
#include "Player.h"

char mazeTxt[20] = "maze.txt";

using namespace std;

const int W = 1000; // window width
const int H = 600; // window height
const int NUM_ROOMS = 15;
const int MAX_BLOCKS_IN_ROOM = 3;

const int PLAYER1 = 6;
const int PLAYER2 = 7;

const int NUM_PLAYERS = 2;

const double SQSIZE = 2.0 / MSIZE;

int maze[MSIZE][MSIZE];

Room all_rooms[2 * NUM_ROOMS];
MazeMap *mazeMap;
Player *players[NUM_PLAYERS];

int playerTurn = 0;

int numberOfRoom = NUM_ROOMS;

Store health_stores[NUM_HEALTH_STORE];
Store munitions_stores[NUM_MUNITIONS_STORE];

// gray queue
vector <Point2D> gray;
vector <Point2D> black;
vector <Parent> parents;

priority_queue<Node, vector<Node>, CompareNodes> pq;

Point2D start, target;

void SetupMaze();
void loadMazeDataFromFile(FILE *file);
void SetPlayer();
void CreateGameMap();

bool play = false;

void init()
{
	int i, j;
	Point2D* pt;

	srand(time(0));

	// clean up the maze
	for (i = 0; i < MSIZE; i++)
		for (j = 0; j < MSIZE; j++)
			maze[i][j] = WALL;


	FILE *file = fopen(mazeTxt, "r");

	for (i = 0; i < NUM_ROOMS; i++)
		mazeMap->AddRoom();

	if (file != NULL) {
		loadMazeDataFromFile(file);
		fclose(file);
	}
	else {
		SetupMaze();
	}

	CreateGameMap();

	SetPlayer();
	glClearColor(0.7, 0.7, 0.7, 0);

	glOrtho(-1, 1, -1, 1, -1, 1);
}

void SetPlayer()
{
	int setInRoom[NUM_PLAYERS], w, h, x, y;
	boolean canSet;
	for (int i = 0; i < NUM_PLAYERS; i++)
	{
		setInRoom[i] = rand() % NUM_ROOMS;
		canSet = true;

		for (int j = 0; j < i; j++)
		{
			if (setInRoom[j] == setInRoom[i]) {
				canSet = false;
				break;
			}
		}
		if (canSet)
		{
			Room room = all_rooms[setInRoom[i]];
			h = room.GetHeight();
			w = room.GetWidth();
			x = (rand() % w) + room.GetCenter().GetX() - (w / 2);
			y = (rand() % h) + room.GetCenter().GetY() - (h / 2);
			if (maze[y][x] != SPACE)
			{
				canSet = false;
			}
		}
		if (canSet)
		{
			players[i] = new Player(Point2D(x, y), setInRoom[i], PLAYER1 + i);
		}
		else
		{
			i--;
		}
	}
	for (int i = 0; i < NUM_PLAYERS; i++)
	{
		players[i]->setEnemy(players[NUM_PLAYERS - 1 - i]);
	}
}

void AddNewNode(Node current, int direction)
{
	Node* tmp;
	Point2D* pt;
	vector<Point2D>::iterator gray_it;
	vector<Point2D>::iterator black_it;
	double space_weight = 0.1, wall_weight = 5, weight;
	int dx, dy;

	switch (direction)
	{
	case UP:
		dx = 0;
		dy = -1;
		break;
	case DOWN:
		dx = 0;
		dy = 1;
		break;
	case LEFT:
		dx = -1;
		dy = 0;
		break;
	case RIGHT:
		dx = 1;
		dy = 0;
		break;
	}// switch

	if (direction == UP && current.GetPoint().GetY() > 0 ||
		direction == DOWN && current.GetPoint().GetY() < MSIZE - 1 ||
		direction == LEFT && current.GetPoint().GetX() > 0 ||
		direction == RIGHT && current.GetPoint().GetX() < MSIZE - 1)
	{
		pt = new Point2D(current.GetPoint().GetX() + dx, current.GetPoint().GetY() + dy);
		gray_it = find(gray.begin(), gray.end(), *pt);
		black_it = find(black.begin(), black.end(), *pt);
		if (gray_it == gray.end() && black_it == black.end()) // this is a new point
		{
			// very important to tunnels
			if (maze[current.GetPoint().GetY() + dy][current.GetPoint().GetX() + dx] == WALL)
				weight = wall_weight;
			else weight = space_weight;
			// weight depends on previous weight and wheater we had to dig
			// to this point or not
			tmp = new Node(*pt, target, current.GetG() + weight);
			pq.emplace(*tmp); // insert first node to priority queue
			gray.push_back(*pt); // paint it gray
			// add Parent
			parents.push_back(Parent(tmp->GetPoint(), current.GetPoint(), true));
		}
	}
}

void RunAStar4Tunnels(int fromRoom, int toRoom)
{
	Node current;
	Node* tmp;
	Point2D* pt;
	vector<Point2D>::iterator gray_it;
	vector<Point2D>::iterator black_it;
	bool finished = false;
	double space_weight = 0.5, wall_weight = 0.5, weight;

	while (!pq.empty() && !finished)
	{
		current = pq.top();
		pq.pop(); // remove it from pq

		if (current.GetH() == 0) // the solution has been found
		{
			vector<Parent>::iterator itr;
			finished = true;
			// go back to start and change WALL to SPACE
			itr = find(parents.begin(), parents.end(),
				Parent(current.GetPoint(), current.GetPoint(), true));
			Point2D p;
			while (itr->HasParent())
			{
				Point2D tmp_prev = itr->GetPrev();
				Point2D tmp_cur = itr->GetCurrent();
				// set SPACE
				if (maze[tmp_cur.GetY()][tmp_cur.GetX()] == WALL)
					maze[tmp_cur.GetY()][tmp_cur.GetX()] = SPACE;
				itr = find(parents.begin(), parents.end(),
					Parent(tmp_prev, current.GetPoint(), true));
			}
		}
		else // check the neighbours
		{
			// remove current from gray 
			gray_it = find(gray.begin(), gray.end(), current.GetPoint());
			if (gray_it != gray.end())
				gray.erase(gray_it);
			// and paint it black
			black.push_back(current.GetPoint());
			// try to go UP
			AddNewNode(current, UP);
			// try to go DOWN
			AddNewNode(current, DOWN);
			// try to go LEFT
			AddNewNode(current, LEFT);
			// try to go RIGHT
			AddNewNode(current, RIGHT);
		}
	} // while
}

void DigTunnels()
{
	int i, j;
	bool found;
	for (i = 0; i < NUM_ROOMS; i++)
	{
		for (j = i + 1; j < NUM_ROOMS; j++)
		{
			start = all_rooms[i].GetCenter();
			target = all_rooms[j].GetCenter();

			printf("Start: %d      Target: %d\n", i, j);

			Node* tmp = new Node(start, target, 0);
			while (!pq.empty())
				pq.pop();

			pq.emplace(*tmp); // insert first node to priority queue
			gray.clear();
			gray.push_back(start); // paint it gray
			black.clear();
			parents.clear();
			parents.push_back(Parent(tmp->GetPoint(),
				tmp->GetPoint(), false));
			RunAStar4Tunnels(i, j);
			delete tmp;
		}
	}
}

void loadMazeDataToFile()
{
	FILE *file = fopen(mazeTxt, "w");
	for (int i = 0; i < MSIZE; i++)
	{
		Point2D p = all_rooms[i].GetCenter();
		fprintf(file, "%d %d %d %d\n", p.GetX(), p.GetY(), all_rooms[i].GetWidth(), all_rooms[i].GetHeight());
	}
	for (int i = 0; i < MSIZE; i++)
	{
		for (int j = 0; j < MSIZE; j++)
		{
			fprintf(file, "%d ", maze[i][j]);
		}
		fprintf(file, "\n");
	}
	for (int i = 0; i < NUM_HEALTH_STORE; i++)
	{
		fprintf(file, "%d %d %d \n", health_stores[i].GetRoomNumber(), health_stores[i].GetLocation().GetX(), health_stores[i].GetLocation().GetY());
	}
	for (int i = 0; i < NUM_MUNITIONS_STORE; i++)
	{
		fprintf(file, "%d %d %d\n", munitions_stores[i].GetRoomNumber(), munitions_stores[i].GetLocation().GetX(), munitions_stores[i].GetLocation().GetY());
	}
	fclose(file);
}

void loadMazeDataFromFile(FILE *file)
{
	int w, h, x, y;
	for (int i = 0; i < MSIZE; i++)
	{
		fscanf(file, "%d %d %d %d\n", &x, &y, &w, &h);
		all_rooms[i] = Room(Point2D(x, y), w, h);
	}
	for (int i = 0; i < MSIZE; i++)
	{
		for (int j = 0; j < MSIZE; j++)
		{
			fscanf(file, "%d ", &(maze[i][j]));
		}
		fscanf(file, "\n");
	}
	int inRoom;
	for (int i = 0; i < NUM_HEALTH_STORE; i++)
	{
		fscanf(file, "%d %d %d\n", &inRoom, &x, &y);
		health_stores[i] = Store(Point2D(x, y), inRoom, health);
	}
	for (int i = 0; i < NUM_MUNITIONS_STORE; i++)
	{
		fscanf(file, "%d %d %d\n", &inRoom, &x, &y);
		munitions_stores[i] = Store(Point2D(x, y), inRoom, munitions);
	}
}

void HealthStore()
{
	int setInRoom, h, w, storeX, storeY;
	boolean canSet;
	for (int i = 0; i < NUM_HEALTH_STORE; i++)
	{
		setInRoom = rand() % NUM_ROOMS;
		canSet = true;
		for (int j = 0; j < i; j++)
		{
			if (health_stores[j].GetRoomNumber() == setInRoom) {
				canSet = false;
				break;
			}
		}
		if (canSet)
		{
			Room room = all_rooms[setInRoom];
			h = room.GetHeight();
			w = room.GetWidth();
			storeX = (rand() % w) + room.GetCenter().GetX() - (w / 2);
			storeY = (rand() % h) + room.GetCenter().GetY() - (h / 2);
			if (maze[storeY][storeX] == BLOCK)
			{
				canSet = false;
			}
		}
		if (canSet)
		{
			health_stores[i] = *(new Store(Point2D(storeX, storeY), setInRoom, health));
			maze[storeY][storeX] = HEALTH_COLOR;
		}
		else
		{
			i--;
		}
	}
}

void MunitionsStores()
{
	int setInRoom, h, w, storeX, storeY;
	boolean canSet;
	for (int i = 0; i < NUM_MUNITIONS_STORE; i++)
	{
		setInRoom = rand() % NUM_ROOMS;
		canSet = true;
		for (int j = 0; j < i; j++)
		{
			if (munitions_stores[j].GetRoomNumber() == setInRoom) {
				canSet = false;
				break;
			}
		}
		if (canSet)
		{
			Room room = all_rooms[setInRoom];
			h = room.GetHeight();
			w = room.GetWidth();
			storeX = (rand() % w) + room.GetCenter().GetX() - (w / 2);
			storeY = (rand() % h) + room.GetCenter().GetY() - (h / 2);
			if (maze[storeY][storeX] == BLOCK)
			{
				canSet = false;
			}
		}
		if (canSet)
		{
			munitions_stores[i] = *(new Store(Point2D(storeX, storeY), setInRoom, munitions));
			maze[storeY][storeX] = MUNITIONS_COLOR;
		}
		else
		{
			i--;
		}
	}
}

void CreateBlocks()
{
	int bloacks, h, w, blockH, blockW, startX, startY;
	int left, right, top, bottom;

	for (int i = 0; i < NUM_ROOMS; i++)
	{
		bloacks = rand() % MAX_BLOCKS_IN_ROOM + 1;
		Room room = all_rooms[i];
		h = room.GetHeight();
		w = room.GetWidth();
		for (int j = 0; j < bloacks; j++)
		{
			blockW = (rand() % (w / 2)) + 1;
			blockH = (rand() % (h / 2)) + 1;
			startX = (rand() % blockW) + room.GetCenter().GetX() - (w / 2) + 1;
			startY = (rand() % blockH) + room.GetCenter().GetY() - (h / 2) + 1;

			for (int sH = 0; sH < blockH; sH++)
			{
				for (int sW = 0; sW < blockW; sW++)
				{
					maze[sH + startY][sW + startX] = BLOCK;
				}
			}
		}
	}
}

bool MoreThanOneConn(int x, int y)
{
	int count = 0;;
	if (x - 1 > 0 && maze[y][x - 1] == SPACE)
		count++;
	if (y - 1 > 0 && maze[y - 1][x] == SPACE)
		count++;
	if (x + 1 < MSIZE - 1 && maze[y][x + 1] == SPACE)
		count++;
	if (y + 1 < MSIZE - 1 && maze[y + 1][x] == SPACE)
		count++;
	if (count > 2)
		return true;
	return false;
}

int FindRoom(int *x, int *y, int direction)
{
	bool found = false;
	do {
		switch (direction)
		{
		case UP:
			*y = *y + 1;
			if ((*y) + 1 > MSIZE - 1 || maze[(*y) + 1][*x] != SPACE)
			{
				if ((*x) - 1 > 0 && maze[*y][(*x) - 1] == SPACE)
					direction = LEFT;
				else
					direction = RIGHT;
			}
			break;
		case DOWN:
			*y = *y - 1;
			if ((*y) - 1 < 0 || maze[(*y) - 1][*x] != SPACE)
			{
				if ((*x) - 1 > 0 && maze[*y][(*x) - 1] == SPACE)
					direction = LEFT;
				else
					direction = RIGHT;
			}
			break;
		case RIGHT:
			*x = *x + 1;
			if ((*x) + 1 > MSIZE - 1 || maze[*y][(*x) + 1] != SPACE)
			{
				if ((*y) - 1 > 0 && maze[(*y) - 1][*x] == SPACE)
					direction = DOWN;
				else
					direction = UP;
			}
			break;
		case LEFT:
			*x = *x - 1;
			if ((*x) - 1 < 0 || maze[*y][(*x) - 1] != SPACE)
			{
				if ((*y) - 1 > 0 && maze[(*y) - 1][*x] == SPACE)
					direction = DOWN;
				else
					direction = UP;
			}
			break;
		}
		found = MoreThanOneConn(*x, *y);
	} while (!found);
	int roomNum = -1;
	for (int i = 0; i < numberOfRoom; i++)
	{
		if (all_rooms[i].IsInRoom(*x, *y))
		{
			roomNum = i;
			break;
		}
	}
	return roomNum;
}

void AddRoom(int x, int y)
{
	all_rooms[numberOfRoom++] = Room(Point2D(x, y), 1, 1);
}

//void roomColor(int index)
//{
//	Room r = all_rooms[index];
//	for (int i = 0; i < r.GetHeight(); i++)
//	{
//		for (int j = 0; j < r.GetWidth(); j++)
//		{
//			maze[r.GetCenter().GetY() - r.GetHeight() / 2 + i][r.GetCenter().GetX() - r.GetWidth() / 2 + j] = PLAYER1;
//		}
//	}
//}
//
//void printMap()
//{
//	roomColor(0);
//	for (int i = 0; i < numberOfRoom; i++)
//	{
//		for (int j = 0; j < mazeMap.GetRoom(i).GetNumOfConn(); j++)
//		{
//			RoomMapNode r = mazeMap.GetRoom(i).GetConnectedRooms().at(j);
//			printf("%d -fromPoint: %d %d, toPoint: %d %d  ,direction: %d , toRoom: %d \n", i, r.GetFromPoint().GetX(), r.GetFromPoint().GetY(),
//				r.GetToPoint().GetX(), r.GetToPoint().GetY(), r.GetDirection(), r.GetToRoom());
//		}
//	}
//}

void CreateGameMap()
{
	int x, y, roomNum, outX, outY, direction;
	Room room;
	for (int i = 0; i < numberOfRoom; i++)
	{
		room = all_rooms[i];
		x = room.GetCenter().GetX() - room.GetWidth() / 2;
		y = room.GetCenter().GetY() + room.GetHeight() / 2;
		for (int j = 0; j < room.GetWidth(); j++)//UP
		{
			if (y + 1 < MSIZE - 1 && maze[y + 1][x + j] == SPACE)
			{
				outX = x + j;
				outY = y;
				direction = UP;
				roomNum = FindRoom(&outX, &outY, direction);
				if (roomNum == -1)
				{
					AddRoom(outX, outY);
					roomNum = mazeMap->AddRoom();
				}
				mazeMap->AddNodeToRoom(i, new RoomMapNode(new Point2D(x + j, y), new Point2D(outX, outY), i, roomNum, UP));
			}
		}
		x = room.GetCenter().GetX() - room.GetWidth() / 2;
		y = room.GetCenter().GetY() - room.GetHeight() / 2;
		for (int j = 0; j < room.GetWidth(); j++)//DOWN
		{
			if (y - 1 > 0 && maze[y - 1][x + j] == SPACE)
			{
				outX = x + j;
				outY = y;
				direction = DOWN;
				roomNum = FindRoom(&outX, &outY, direction);
				if (roomNum == -1)
				{
					AddRoom(outX, outY);
					roomNum = mazeMap->AddRoom();
				}
				mazeMap->AddNodeToRoom(i,new RoomMapNode(new Point2D(x + j, y),new Point2D(outX, outY), i, roomNum, DOWN));
			}
		}
		x = room.GetCenter().GetX() - room.GetWidth() / 2;
		y = room.GetCenter().GetY() - room.GetHeight() / 2;
		for (int j = 0; j < room.GetHeight(); j++)//LEFT
		{
			if (x - 1 > 0 && maze[y + j][x - 1] == SPACE)
			{
				outX = x;
				outY = y + j;
				direction = LEFT;
				roomNum = FindRoom(&outX, &outY, direction);
				if (roomNum == -1)
				{
					AddRoom(outX, outY);
					roomNum = mazeMap->AddRoom();
				}
				mazeMap->AddNodeToRoom(i, new RoomMapNode(new Point2D(x, y + j), new Point2D(outX, outY), i, roomNum, LEFT));
			}
		}
		x = room.GetCenter().GetX() + room.GetWidth() / 2;
		y = room.GetCenter().GetY() - room.GetHeight() / 2;
		for (int j = 0; j < room.GetHeight(); j++)//RIGHT
		{
			if (x + 1 < MSIZE - 1 && maze[y + j][x + 1] == SPACE)
			{
				outX = x;
				outY = y + j;
				direction = RIGHT;
				roomNum = FindRoom(&outX, &outY, direction);
				if (roomNum == -1)
				{
					AddRoom(outX, outY);
					roomNum = mazeMap->AddRoom();
				}
				mazeMap->AddNodeToRoom(i, new RoomMapNode(new Point2D(x, y + j), new Point2D(outX, outY), i, roomNum, RIGHT));
			}
		}
	}
}

void SetupMaze()
{
	int i, j, counter;
	int left, right, top, bottom;
	bool isValidRoom;
	Room* pr = NULL;
	int w, h;
	for (counter = 0; counter < NUM_ROOMS; counter++)
	{
		// create room
		do
		{
			free(pr);
			w = 5 + rand() % 15;
			h = 5 + rand() % 25;
			if (w % 2 == 0)
				w++;
			if (h % 2 == 0)
				h++;
			pr = new Room(Point2D(rand() % MSIZE,
				rand() % MSIZE), w, h);
			top = pr->GetCenter().GetY() - pr->GetHeight() / 2;
			if (top < 0)
			{
				isValidRoom = false;
				continue;
			}
			bottom = pr->GetCenter().GetY() + pr->GetHeight() / 2;
			if (bottom >= MSIZE)
			{
				isValidRoom = false;
				continue;
			}
			left = pr->GetCenter().GetX() - pr->GetWidth() / 2;
			if (left < 0)
			{
				isValidRoom = false;
				continue;
			}
			right = pr->GetCenter().GetX() + pr->GetWidth() / 2;
			if (right >= MSIZE)
			{
				isValidRoom = false;
				continue;
			}

			isValidRoom = true;
			for (i = 0; i < counter && isValidRoom; i++)
				if (all_rooms[i].IsOverlap(*pr))
					isValidRoom = false;

		} while (!isValidRoom);

		all_rooms[counter] = *pr;
		for (i = top; i <= bottom; i++)
			for (j = left; j <= right; j++)
				maze[i][j] = SPACE;

	}
	DigTunnels();
	CreateBlocks();
	HealthStore();
	MunitionsStores();
	loadMazeDataToFile();
}

void DrawMaze()
{
	int i, j;

	for (i = 0; i < MSIZE; i++)
		for (j = 0; j < MSIZE; j++)
		{
			switch (maze[i][j])
			{
			case WALL:
				glColor3d(0.4, 0, 0); // dark red;
				break;
			case SPACE:
				glColor3d(1, 1, 1); // white;
				break;
			case HEALTH_COLOR:
				glColor3d(0, 0.9, 0); // green;
				break;
			case MUNITIONS_COLOR:
				glColor3d(1, .8, 0); //ORANGE;
				break;
			case BLOCK:
				glColor3d(0, 0, 0); // BLACK;
				break;
			case PLAYER1:
				glColor3d(0, 0, 1); // BLUE;
				break;
			case PLAYER2:
				glColor3d(1, 0, 0); // RED;
				break;

			}
			// draw square
			glBegin(GL_POLYGON);
			glVertex2d(j*SQSIZE - 1 - SQSIZE / 2, i*SQSIZE - 1 + SQSIZE / 2);
			glVertex2d(j*SQSIZE - 1 + SQSIZE / 2, i*SQSIZE - 1 + SQSIZE / 2);
			glVertex2d(j*SQSIZE - 1 + SQSIZE / 2, i*SQSIZE - 1 - SQSIZE / 2);
			glVertex2d(j*SQSIZE - 1 - SQSIZE / 2, i*SQSIZE - 1 - SQSIZE / 2);
			glEnd();
		}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	DrawMaze();

	glutSwapBuffers();// show what was drawn in "frame buffer"
}

void playTurn()
{
	players[(playerTurn++) % NUM_PLAYERS]->Play();
	if (playerTurn%NUM_PLAYERS == 0) {
		for (int i = 0; i < NUM_PLAYERS; i++)
		{
			printf("\t%d\t", i);
		}
		for (int i = 0; i < NUM_PLAYERS; i++)
		{
			printf("\t%d\t", players[i]->getHealth());
		}
		for (int i = 0; i < NUM_PLAYERS; i++)
		{
			printf("\t%d\t", players[i]->getMunitions());
		}
		for (int i = 0; i < NUM_PLAYERS; i++)
		{
			printf("\t%d\t", players[i]->getState());
		}
	}
}

void idle()
{
	if (play)
		playTurn();
	glutPostRedisplay();// calls indirectly to display
}

void Menu(int choice)
{
	switch (choice)
	{
	case 1:
		play = true;
		break;
	}
}

#include <cstdlib>

void exiting() {
	for (int i = 0; i < NUM_PLAYERS; i++)
	{
		delete players[i];
	}
	delete mazeMap;
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Dungeon Generation - AI");
	mazeMap = new MazeMap();
	glutDisplayFunc(display); // refresh function
	glutIdleFunc(idle); // idle: when nothing happens
	init();

	glutCreateMenu(Menu);
	glutAddMenuEntry("Play", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
	atexit(exiting);
}