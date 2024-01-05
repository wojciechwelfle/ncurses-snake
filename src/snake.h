#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "winsys.h"
#include <thread>
#include <unistd.h>
#include <vector>

class CSnake : public CFramedWindow {
  private:
	int level;
	int direction;
	bool running;
	bool help;
	bool gameOver;
	CPoint apple;
	bool eaten;
	vector<CPoint> snakeParts;

	void paintLevel();
	void paintWindow();
	void paintInfo();
	void paintSnake();
	void paintApple();
	void paintGameOver();
	void spawnApple();
	bool isAppleEaten();
	void checkCollision();

  public:
	CSnake(CRect r, char _c = ' ');
	void paint();
	void reset();
	bool handleEvent(int key);
	void update();
};

#endif
