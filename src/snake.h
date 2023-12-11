#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "winsys.h"

class CSnake : public CFramedWindow {
  private:
	int level;
	bool running;
	
	void paintLevel();
	void paintWindow();
	void paintInfo();
	
  public:
	CSnake(CRect r, char _c = ' ');

	void paint();
	
	void reset();
	
};

#endif
