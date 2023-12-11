#ifndef __WINSYS_H__
#define __WINSYS_H__

#include "cpoint.h"
#include "screen.h"
#include <ctype.h>
#include <list>
#include <string>
using namespace std;

class CView {
  protected:
    CRect geom;

  public:
    CView(CRect g) : geom(g){};
    virtual void paint() = 0;
    virtual bool handleEvent(int key) = 0;
    virtual void move(const CPoint& delta)
    {
        geom.topleft += delta;
    };
    virtual ~CView(){};
};

class CWindow : public CView {
  protected:
    char c;

  public:
    CWindow(CRect r, char _c = '*') : CView(r), c(_c){};
    void paint()
    {
        for (int i = geom.topleft.y; i < geom.topleft.y + geom.size.y; i++) {
            gotoyx(i, geom.topleft.x);
            for (int j = 0; j < geom.size.x; j++)
                printw("%c", c);
        };
    };
    bool handleEvent(int key)
    {
        switch (key) {
        case KEY_UP:
            move(CPoint(0, -1));
            return true;
        case KEY_DOWN:
            move(CPoint(0, 1));
            return true;
        case KEY_RIGHT:
            move(CPoint(1, 0));
            return true;
        case KEY_LEFT:
            move(CPoint(-1, 0));
            return true;
        };
        return false;
    };
};

class CFramedWindow : public CWindow {
  public:
    CFramedWindow(CRect r, char _c = '\'') : CWindow(r, _c){};
    void paint()
    {
        for (int i = geom.topleft.y; i < geom.topleft.y + geom.size.y; i++) {
            gotoyx(i, geom.topleft.x);
            if ((i == geom.topleft.y) ||
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
    };
};

class CInputLine : public CFramedWindow {
    string text;

  public:
    CInputLine(CRect r, char _c = ',') : CFramedWindow(r, _c){};
    void paint()
    {
        CFramedWindow::paint();
        gotoyx(geom.topleft.y + 1, geom.topleft.x + 1);

        for (unsigned j = 1, i = 0;
             (j + 1 < (unsigned)geom.size.x) && (i < text.length()); j++, i++)
            printw("%c", text[i]);
    };
    bool handleEvent(int c)
    {
        if (CFramedWindow::handleEvent(c))
            return true;
        if ((c == KEY_DC) || (c == KEY_BACKSPACE)) {
            if (text.length() > 0) {
                text.erase(text.length() - 1);
                return true;
            };
        }
        if ((c > 255) || (c < 0))
            return false;
        if (!isalnum(c) && (c != ' '))
            return false;
        text.push_back(c);
        return true;
    }
};

class CGroup : public CView {
    list<CView*> children;

  public:
    CGroup(CRect g) : CView(g){};
    void paint()
    {
        for (list<CView*>::iterator i = children.begin(); i != children.end();
             i++)
            (*i)->paint();
    };
    bool handleEvent(int key)
    {
        if (!children.empty() && children.back()->handleEvent(key))
            return true;
        if (key == '\t') {
            if (!children.empty()) {
                children.push_front(children.back());
                children.pop_back();
            };
            return true;
        }
        return false;
    };
    void insert(CView* v)
    {
        children.push_back(v);
    };
    ~CGroup()
    {
        for (list<CView*>::iterator i = children.begin(); i != children.end();
             i++)
            delete (*i);
    };
};

class CDesktop : public CGroup {
  public:
    CDesktop() : CGroup(CRect())
    {
        int y, x;
        init_screen();
        getscreensize(y, x);
        geom.size.x = x;
        geom.size.y = y;
    };
    ~CDesktop()
    {
        done_screen();
    };

    void paint()
    {
        for (int i = geom.topleft.y; i < geom.topleft.y + geom.size.y; i++) {
            gotoyx(i, geom.topleft.x);
            for (int j = 0; j < geom.size.x; j++)
                printw(".");
        };
        CGroup::paint();
    }

    int getEvent()
    {
        return ngetch();
    };

    void run()
    {
        int c;
        paint();
        refresh();
        while (1) {
            c = getEvent();
            if (c == 27)
                break;
            if (handleEvent(c)) {
                paint();
                refresh();
            };
        };
    };
};


#endif
