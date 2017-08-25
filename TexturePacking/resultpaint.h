#pragma once
#include <QApplication>	// for QApplication
#include <QWidget>		// for QWidget
#include <QPainter>		// for QPainter
#include <rectangle.h>	// for rectangle structure

class resultPaint : public QWidget {
public:
	resultPaint(QWidget * parent = 0);	// constructor
	~resultPaint();						// destructor
private:
	void paintEvent(QPaintEvent *);		// default painting function
	QPainter *paint;					// paint object pointer
};

// following are some useful extern variables for painting rectangles
// all these variables will be defined in the mainwindow.cpp
extern int rectNum;
extern int fixedWidth;
extern int optimalHeight;
extern std::vector<rectangle> list;
extern std::vector<rectangle> original;
extern bool done;