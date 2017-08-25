#include <resultpaint.h>

// constructor, set the default geometry size 
resultPaint::resultPaint(QWidget * parent) : QWidget(parent){
	setGeometry(0, 0, 50, 50);
}

// destructor and nothing special
resultPaint::~resultPaint() {

}

// default painting subroutine
// it can be called automatically when construct the widget
// Also it can be called after executing each algorithm
// to illustrate the result
void resultPaint::paintEvent(QPaintEvent *) {
	paint = new QPainter;
	// if not any test case has been loaded yet, do nothing
	if (original.size() == 0)
		return;
	// if just load a test case but no execution, we 
	// paint the optimal solution
	else if (done == false) {
		paint->begin(this);
		paint->setBrush(QBrush(Qt::lightGray, Qt::SolidPattern));
		paint->drawRect(250 - fixedWidth / 2, 20, fixedWidth, 500);
		paint->setBrush(QBrush(Qt::darkGray, Qt::SolidPattern));
		paint->drawRect(250 - fixedWidth / 2, 20, fixedWidth, optimalHeight);
		paint->end();
	}
	// if an algorithm has been executed and the result has been 
	// written in the list container , we read it and illustrate 
	// the result
	else {
		paint->begin(this);
		paint->setBrush(QBrush(Qt::lightGray, Qt::SolidPattern));
		paint->drawRect(250 - fixedWidth / 2, 20, fixedWidth, 500);
		
		paint->setBrush(QBrush(Qt::darkGray, Qt::SolidPattern));
		for (int i = 0; i < rectNum; i++) {
			paint->drawRect(250 - fixedWidth / 2 + list[i].x
				, 20 + list[i].y , list[i].width, list[i].height);
		}
		paint->end();
	}
}