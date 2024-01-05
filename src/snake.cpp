#include "snake.h"
#include <cstring>
#include <pthread.h>
#include <unistd.h>

CSnake::CSnake(CRect r, char _c /*=' '*/) : CFramedWindow(r, _c)
{
	srand(time(0));
	reset();
	paint();
}

void CSnake::paint()
{
	paintLevel();
	paintWindow();
	update();
	if (gameOver) {
		paintSnake();
		paintGameOver();
	}
	else {
		if (help) {
			paintInfo();
		}
		else {
			paintApple();
			paintSnake();
			usleep(100000 - 1000 * level);
		}
	}
}

void CSnake::reset()
{
	level = 1;
	help = true;
	running = false;
	gameOver = false;
	direction = KEY_RIGHT;
	spawnApple();

	// snake
	snakeParts.clear();
	snakeParts.push_back(CPoint(3, 1)); // head
	snakeParts.push_back(CPoint(2, 1));
	snakeParts.push_back(CPoint(1, 1));
}

void CSnake::update()
{
	if (running) {
		snakeParts.insert(snakeParts.begin(), snakeParts[0]);
		if (direction == KEY_UP) {
			snakeParts[0] += CPoint(0, -1);
		}
		else if (direction == KEY_DOWN) {
			snakeParts[0] += CPoint(0, 1);
		}
		else if (direction == KEY_LEFT) {
			snakeParts[0] += CPoint(-1, 0);
		}
		else if (direction == KEY_RIGHT) {
			snakeParts[0] += CPoint(1, 0);
		}

		// over window
		if (snakeParts[0].x == -1) {
			snakeParts[0].x = geom.size.x - 3;
		}
		if (snakeParts[0].x == geom.size.x - 2) {
			snakeParts[0].x = 0;
		}
		if (snakeParts[0].y == -1) {
			snakeParts[0].y = geom.size.y - 4;
		}
		if (snakeParts[0].y == geom.size.y - 3) {
			snakeParts[0].y = 0;
		}
		if (isAppleEaten() || eaten) {
			level++;
			spawnApple();
		}
		else {
			snakeParts.pop_back();
		}
		checkCollision();
	}
}

bool CSnake::handleEvent(int key)
{
	if (key == 'p') {
		running ? running = false : running = true;
		help = false;
		return true;
	}
	if (key == 'h') {
		running = false;
		help ? help = false : help = true;
		return true;
	}
	if (key == 'r') {
		reset();
		help = false;
		running = true;
		return true;
	}
	if (key == '\t') {
		running = false;
	}

	if (running) {
		switch (key) {
		case KEY_UP:
			if (direction != KEY_DOWN)
				direction = KEY_UP;
			return true;
		case KEY_DOWN:
			if (direction != KEY_UP)
				direction = KEY_DOWN;
			return true;
		case KEY_RIGHT:
			if (direction != KEY_LEFT)
				direction = KEY_RIGHT;
			return true;
		case KEY_LEFT:
			if (direction != KEY_RIGHT)
				direction = KEY_LEFT;
			return true;
		};
	}
	return CFramedWindow::handleEvent(key);
}

void CSnake::paintSnake()
{
	size_t size = snakeParts.size();
	for (size_t i = 1; i < size; ++i) {
		int x = snakeParts[i].x;
		int y = snakeParts[i].y;
		gotoyx(geom.topleft.y + y + 2, geom.topleft.x + x + 1);
		printw("+");
	}
	int x = snakeParts[0].x;
	int y = snakeParts[0].y;
	gotoyx(geom.topleft.y + y + 2, geom.topleft.x + x + 1);
	printw("*");
}

void CSnake::paintLevel()
{
	gotoyx(geom.topleft.y, geom.topleft.x);
	printw("| LEVEL: %d |", level);
}

void CSnake::paintWindow()
{
	for (int i = geom.topleft.y + 1; i < geom.topleft.y + geom.size.y; i++) {
		gotoyx(i, geom.topleft.x);
		if ((i == geom.topleft.y + 1) ||
			(i == geom.topleft.y + geom.size.y - 1)) {
			printw("+");
			for (int j = 1; j < geom.size.x - 1; j++)
				printw("-");
			printw("+");
		}
		else {
			printw("|");
			for (int j = 1; j < geom.size.x - 1; j++)
				printw("%c", c);
			printw("|");
		}
	}
}

#define CONTENT_SIZE 5
void CSnake::paintInfo()
{
	char content[CONTENT_SIZE][50] = {
		"h - toggle help information", "p - toggle pause/play mode",
		"r - restart game", "arrows - move snake (in play mode) or",
		"         move window (in pause mode)"};

	for (int i = 0; i < CONTENT_SIZE; ++i) {
		gotoyx(geom.topleft.y + 5 + i, geom.topleft.x + 5);
		printw("%s", content[i]);
	}
}

void CSnake::paintGameOver()
{
	gotoyx(geom.topleft.y + 5, geom.topleft.x + 5);
	printw("GAME OVER");
}

void CSnake::paintApple()
{
	if (!eaten) {
		gotoyx(geom.topleft.y + apple.y, geom.topleft.x + apple.x);
		printw("O");
	}
}

bool CSnake::isAppleEaten()
{
	for (auto& p : snakeParts) {
		if (apple.x == p.x + 1 && apple.y == p.y + 2) {
			eaten = true;
			return true;
		}
	}
	return false;
}

void CSnake::checkCollision()
{
	CPoint head = snakeParts[0];
	for (size_t i = 1; i < snakeParts.size(); i++) {
		if (head.x == snakeParts[i].x && head.y == snakeParts[i].y) {
			gameOver = true;
			running = false;
			return;
		}
	}
}

void CSnake::spawnApple()
{
	bool freeSpace;
	do {
		freeSpace = true;
		apple.x = rand() % (geom.size.x - 2) + 1;
		apple.y = rand() % (geom.size.y - 3) + 2;

		for (CPoint& p : snakeParts) {
			if (apple.x == p.x + 1 && apple.y == p.y + 2) {
				freeSpace = false;
			}
		}
	} while (!freeSpace);
	eaten = false;
}
