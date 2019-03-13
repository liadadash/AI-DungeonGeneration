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

char mazeTxt[20] = "maze.txt";

using namespace std;

const int W = 600; // window width
const int H = 600; // window height
const int NUM_ROOMS = 10;
const int NUM_HEALTH_STORE = 2;
const int NUM_MUNITIONS = 2;
const int MAX_BLOCKS_IN_ROOM = 3;

const int SPACE = 1;
const int WALL = 2;
const int HEALTH = 3;
const int MUNITIONS = 4;
const int BLOCK = 5;

const int TARGET = 5;
const int GRAY = 6;

const int UP = 1;
const int DOWN = 2;
const int LEFT = 3;
const int RIGHT = 4;

const int MSIZE = 100;
const double SQSIZE = 2.0 / MSIZE;

int maze[MSIZE][MSIZE];

Room all_rooms[NUM_ROOMS];

Store health_stores[NUM_HEALTH_STORE];
Store munitions_stores[NUM_MUNITIONS];

// gray queue
vector <Point2D> gray;
vector <Point2D> black;
vector <Parent> parents;

priority_queue<Node, vector<Node>, CompareNodes> pq;

Point2D start, target;

void SetupMaze();
void loadMazeDataFromFile(FILE *file);

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

	if (file != NULL) {
		loadMazeDataFromFile(file);
		fclose(file);
	}
	else {
		SetupMaze();
	}

	glClearColor(0.7, 0.7, 0.7, 0);

	glOrtho(-1, 1, -1, 1, -1, 1);
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

void RunAStar4Tunnels()
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

	for (i = 0; i < NUM_ROOMS; i++)
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
			RunAStar4Tunnels();
			delete tmp;
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
}

void loadMazeDataFromFile(FILE *file)
{
	int w, h, x, y;
	for (int i = 0; i < MSIZE; i++)
	{
		fscanf(file, "%d %d %d %d\n", &x, &y, &w, &h);
		all_rooms[i] = *(new Room(Point2D(x, y), w, h));
	}
	for (int i = 0; i < MSIZE; i++)
	{
		for (int j = 0; j < MSIZE; j++)
		{
			fscanf(file, "%d ", &(maze[i][j]));
		}
		fscanf(file, "\n");
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
			maze[storeY][storeX] = HEALTH;
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
	for (int i = 0; i < NUM_MUNITIONS; i++)
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
			munitions_stores[i] = *(new Store(Point2D(storeX, storeY), setInRoom, health));
			maze[storeY][storeX] = MUNITIONS;
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

			if (startY < 0) startY = 1;
			if (startY >= MSIZE) startY = MSIZE - 2;
			if (startX < 0) startX = 1;
			if (startX >= MSIZE) startX = MSIZE - 2;

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

void SetupMaze()
{
	int i, j, counter;
	int left, right, top, bottom;
	bool isValidRoom;
	Room* pr = NULL;

	for (counter = 0; counter < NUM_ROOMS; counter++)
	{
		// create room
		do
		{
			free(pr);
			pr = new Room(Point2D(rand() % MSIZE,
				rand() % MSIZE), 5 + rand() % 15, 5 + rand() % 25);
			top = pr->GetCenter().GetY() - pr->GetHeight() / 2;
			if (top < 0) top = 0;
			bottom = pr->GetCenter().GetY() + pr->GetHeight() / 2;
			if (bottom >= MSIZE) bottom = MSIZE - 1;
			left = pr->GetCenter().GetX() - pr->GetWidth() / 2;
			if (left < 0) left = 0;
			right = pr->GetCenter().GetX() + pr->GetWidth() / 2;
			if (right >= MSIZE) right = MSIZE - 1;

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
			case HEALTH:
				glColor3d(0, 0.9, 0); // green;
				break;
			case MUNITIONS:
				glColor3d(0, 0, 1); // blue;
				break;
			case BLOCK:
				glColor3d(0, 0, 0); // BLACK;
				break;
			case GRAY:
				glColor3d(1, .8, 0); // ORANGE;
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

void idle()
{
	glutPostRedisplay();// calls indirectly to display
}

void Menu(int choice)
{

}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Dungeon Generation - AI");

	glutDisplayFunc(display); // refresh function
	glutIdleFunc(idle); // idle: when nothing happens
	init();

	glutCreateMenu(Menu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	glutMainLoop();
}