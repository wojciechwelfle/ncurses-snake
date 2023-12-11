#include "snake.h"
#include <cstring>

CSnake::CSnake(CRect r, char _c /*=' '*/) : CFramedWindow(r, _c)
{
	this->reset();
}

void CSnake::paint()
{
	paintLevel();
	paintWindow();
	paintInfo();
}

void CSnake::reset()
{
	level = 1;
	running = false;
}

void CSnake::paintLevel()
{
	char buffer[18] = {0};
	sprintf(buffer, "| LEVEL: %d |", level);
	gotoyx(geom.topleft.y, geom.topleft.x);
	for (size_t i = 0; i < strlen(buffer); ++i) {
		printw("%c", buffer[i]);
	}
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
	char content[CONTENT_SIZE][50] = {"h - toggle help information",
						 "p - toggle pause/play mode", "r - restart game",
						 "arrows - move snake (in play mode) or",
						 "         move window (in pause mode)"};

	for (int i = 0; i < CONTENT_SIZE; ++i) {
		gotoyx(geom.topleft.y + 5 + i, geom.topleft.x + 5);
		for(size_t j = 0; j < strlen(content[i]); ++j) {
			printw("%c", content[i][j]);
		}
	}
}
