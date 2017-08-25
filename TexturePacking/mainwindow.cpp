#include "mainwindow.h"	// for all the necessary libraries
#include <time.h>		// for testing the running time
#include <algorithm>	// for the sort algorithm in C++ Standard

int rectNum;		// record the total number of rectangles that needs 
					// to be packed, only useful in FFDH algorithm
int fixedWidth;			// record the fixed width in the strip
int optimalHeight;		// record the optimal height of packing 
std::vector<rectangle> list;	// contains the result rectangles
std::vector<rectangle> original;	// contains the original rectangles
std::vector<level> levelList;		// contains the existing levels
									// only useful in FFDH algorithm
// following are containers that are only useful in SAS algorithm
std::vector<rectangle> narrow;	// contains narrow rectangles
std::vector<rectangle> wide;	// contains wide rectangles
bool done;				// record the executing status
int algoNum;			// record the number of the taken algorithm

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{	
	//following are QLabels
	LLabel1 = new QLabel("Fixed width:");
	LLabel2 = new QLabel("Optimal height:");
	RLabel1 = new QLabel("Actual height:");
	RLabel2 = new QLabel("Approximation ratio:");
	RLabel3 = new QLabel("Iteration times:");
	RLabel4 = new QLabel("Time consumed: (s) ");

	//following are QLineEdit
	LLine1 = new QLineEdit();
	LLine2 = new QLineEdit();
	RLine1 = new QLineEdit();
	RLine2 = new QLineEdit();
	RLine3 = new QLineEdit();
	RLine4 = new QLineEdit();

	//following are QPushButton
	LBtn1 = new QPushButton("Import a test case");
	LBtn2 = new QPushButton("FFDH");
	LBtn3 = new QPushButton("SAS");
	LBtn4 = new QPushButton("SAS_advanced");
	RBtn1 = new QPushButton("Time test");

	//following are initialization of QTableWidget
	QStringList header;
	header << "width" << "height";

	table = new QTableWidget();
	table->setRowCount(20);
	table->setColumnCount(2);
	table->setHorizontalHeaderLabels(header);
	
	//following are QLayouts
	hLLayout1 = new QHBoxLayout();
	hLLayout1->addStretch();
	hLLayout1->addWidget(LLabel1);
	hLLayout1->addWidget(LLine1);
	hLLayout1->addWidget(LLabel2);
	hLLayout1->addWidget(LLine2);
	hLLayout1->addStretch();

	hLLayout2 = new QHBoxLayout();
	hLLayout2->addWidget(LBtn1);
	hLLayout2->addWidget(LBtn2);

	hLLayout3 = new QHBoxLayout();
	hLLayout3->addWidget(LBtn3);
	hLLayout3->addWidget(LBtn4);

	vLLayout = new QVBoxLayout();
	vLLayout->addLayout(hLLayout1);
	vLLayout->addWidget(table);
	vLLayout->addLayout(hLLayout2);
	vLLayout->addLayout(hLLayout3);

	hRLayout1 = new QHBoxLayout();
	hRLayout1->addStretch();
	hRLayout1->addWidget(RLabel1);
	hRLayout1->addWidget(RLine1);
	hRLayout1->addWidget(RLabel2);
	hRLayout1->addWidget(RLine2);
	hRLayout1->addStretch();

	hRLayout2 = new QHBoxLayout();
	hRLayout2->addStretch();
	hRLayout2->addWidget(RLabel3);
	hRLayout2->addWidget(RLine3);
	hRLayout2->addWidget(RBtn1);
	hRLayout2->addWidget(RLabel4);
	hRLayout2->addWidget(RLine4);
	hRLayout2->addStretch();

	vRLayout = new QVBoxLayout();
	vRLayout->addLayout(hRLayout1);
	vRLayout->addWidget(&painter);
	vRLayout->addLayout(hRLayout2);

	hMLayout = new QHBoxLayout();
	hMLayout->addLayout(vLLayout);
	hMLayout->addLayout(vRLayout);
	hMLayout->setStretchFactor(vLLayout, 1);
	hMLayout->setStretchFactor(vRLayout, 4);

	// following are constructing of the connections between
	// slot functions and signals
	connect(LBtn1, SIGNAL(clicked()), this, SLOT(import()));
	connect(LBtn2, SIGNAL(clicked()), this, SLOT(a1()));
	connect(LBtn3, SIGNAL(clicked()), this, SLOT(a2()));
	connect(LBtn4, SIGNAL(clicked()), this, SLOT(a3()));
	connect(RBtn1, SIGNAL(clicked()), this, SLOT(test()));

	//following are the setting of the mainwindow
	window = new QWidget;
	window->setLayout(hMLayout);
	window->setWindowTitle("Texture Packing");
	window->setFixedSize(800, 600);
	window->show();
}

// destructor,nothing special
MainWindow::~MainWindow() {

}

// for importing the test case
void MainWindow::import() {
	// getting file path from user by dialog box
	QString path = QFileDialog::getOpenFileName(this, "Import File",
		".", "Test case Files(*.txt *.in)");

	// if the path is not empty
	if (!path.isEmpty()) {
		QFile file(path);
		// if the path is invalid, return 
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QMessageBox::warning(this, tr("Read File"),
				tr("Cannot open file:\n%1").arg(path));
			return;
		}
		// else we obtain the textstream in that file
		QTextStream in(&file);
		QString rawString = in.readAll();
		// we remove all the whith space and '\n' in the text
		QStringList rawList = rawString.split(QRegExp("[ \n]"));

		QStringList::const_iterator Iter = rawList.constBegin();
		
		bool ok;
		QString tempStr;

		// following are setting for showing the information from
		// the text
		tempStr = *Iter;
		fixedWidth = tempStr.toInt(&ok, 10);	// for fixedWidth
		LLine1->setText(tempStr);
		++Iter;

		tempStr = *Iter;
		optimalHeight = tempStr.toInt(&ok, 10);	// for optimalHeight
		LLine2->setText(tempStr);
		++Iter;

		tempStr = *Iter;
		rectNum = tempStr.toInt(&ok, 10);		// for rectNum
		++Iter;
		
		// clearing all the previous information before showing new one
		original.clear();
		int i = 0;
		int tempW, tempH;
		// for the table widget
		// the table widget will show the widths and heights of each
		// rectangles
		while (i < rectNum) {
			// for showing width
			tempStr = *Iter;
			QTableWidgetItem *item1 = new QTableWidgetItem(tempStr);
			tempW = tempStr.toInt(&ok, 10);
			table->setItem(i, 0, item1);
			++Iter;
			// for showing height
			tempStr = *Iter;
			QTableWidgetItem *item2 = new QTableWidgetItem(tempStr);
			tempH = tempStr.toInt(&ok, 10);
			table->setItem(i, 1, item2);
			++Iter;
			// push the rectangle into the original rectangle list
			rectangle tempRec(tempW, tempH);
			original.push_back(tempRec);
			++i;
		}
		done = false;	// no algorithm has been executed
		file.close();	// close the file
	}
	// if no path is obtained
	else {
		QMessageBox::warning(this, tr("Path"),
			tr("You did not select any file."));
	}
}

// the interface for FFDH algorithm
void MainWindow::a1() {
	algoNum = 1;
	list.clear();				// clear the result container
	list.assign(original.begin(), original.end());
								// initial the original container
				// later on it will be changed into result container
	firstFit();				// call the kernel
	painter.update();		// paint the result
}

// the interface for SAS algorithm
void MainWindow::a2() {
	algoNum = 2;
	SAS();					// call the kernel directly
	painter.update();		// paint the result
}

// the interface for SAS_advanced algorithm
void MainWindow::a3() {
	algoNum = 3;			// distinguish which am I using,
							// SAS or SAS_advanced algorithm
	SAS();					// call the kernel directly
	painter.update();		// paint the result
}

// the kernel of the first fit algorithm
void MainWindow::firstFit() {
	// first sort the original rectangles in height-decreasing order
	sort(list.begin(), list.end(), greater_h);
	// and the first rectangle will be packed in the first level
	list[0].x = 0, list[0].y = 0;

	// of course the previous information of the levels 
	// should be cleared and updated
	levelList.clear();
	level tempLevel(fixedWidth - list[0].width, list[0].height, 0);
	levelList.push_back(tempLevel);

	// actualHeight maintains the achieving height 
	int actualHeight;
	actualHeight = list[0].height;	// now the height is the height
									// of the first rectangle

	int i = 1, j;
	// when there are still rectangles that have not been packed
	while (i != rectNum) {
		// scan each existing level
		for (j = 0; j < levelList.size(); j++)
			// if the current level is fit for the 
			// processing rectangle
			if (levelList[j].levelW >= list[i].width) {
				// we pack the rectangle
				list[i].x = fixedWidth - levelList[j].levelW;
				list[i].y = levelList[j].start;
				// also update the information of the level
				levelList[j].levelW -= list[i].width;
				break;
			}
		if (j == levelList.size()) {
			// if not any level is fit for the rectangle processing 
			// we create a new level to pack it
			level tempLevel(fixedWidth - list[i].width, list[i].height,
				levelList[j - 1].start + levelList[j - 1].levelH);
			levelList.push_back(tempLevel);
			list[i].x = 0, list[i].y = levelList[j].start;
			// also update the height that we have achieved
			actualHeight = list[i].y + list[i].height;
		}
		i++;	// for next rectangle to be processed
	}

	done = true;	// finish the algorithm and paint the 
					// result automatically
	QString tempStr;				// also show the actualHeight
	tempStr.setNum(actualHeight);
	RLine1->setText(tempStr);
	// also calculate the approximation ratio and show it 
	tempStr.setNum(double(actualHeight) / optimalHeight);
	RLine2->setText(tempStr);
}

// kernel of the SAS algorithm
void MainWindow::SAS() {
	// following are erasing the previous information
	// in the containers that are to be used
	narrow.clear();	
	wide.clear();
	list.clear();

	// partition the original rectangles into two sorts:
	// narrow and wide
	for each (rectangle rec in original){
		if (rec.height > rec.width)
			narrow.push_back(rec);
		else
			wide.push_back(rec);
	}

	// if we use the SAS algorithm,
	// we only order two lists of rectangles in one dimension
	if (algoNum == 2) {
		sort(wide.begin(), wide.end(), greater_w);
		sort(narrow.begin(), narrow.end(), greater_h);
	}
	// if we use the SAS_algorithm,
	// we will order two lists of retangles in two dimension
	else {
		sort(wide.begin(), wide.end(), greater_w_greater_h);
		sort(narrow.begin(), narrow.end(), greater_h_greater_w);
	}

	// following are some useful local variables when processing 
	int currentX1, currentY1;
	int flag;
	int actualHeight = 0;	// also it's the height we have achieved

	while ( narrow.size() || wide.size() ) {
		// pick up the tallest rectangle between the firt rectangle
		// in the narrow list and that in the wide lists

		// if both narrow and wide is not empty,
		if ( narrow.size() && wide.size() ) {
			if (narrow[0].height > wide[0].height) {
				// pack the narrow
				narrow[0].x = 0;
				narrow[0].y = actualHeight;
				currentX1 = narrow[0].width;
				currentY1 = actualHeight;
				actualHeight += narrow[0].height;
				list.push_back(narrow[0]);
				narrow.erase(narrow.begin());
				flag = 0;
			}
			else {
				// pack the wide
				wide[0].x = 0;
				wide[0].y = actualHeight;
				currentX1 = wide[0].width;
				currentY1 = actualHeight;
				actualHeight += wide[0].height;
				list.push_back(wide[0]);
				wide.erase(wide.begin());
				flag = 1;
			}
		}
		// if wide is empty, the tallest must come from narrow
		else if ( narrow.size() ) {
			// pack the narrow
			narrow[0].x = 0;
			narrow[0].y = actualHeight;	
			currentX1 = narrow[0].width;
			currentY1 = actualHeight;
			actualHeight += narrow[0].height;
			list.push_back(narrow[0]);
			narrow.erase(narrow.begin());
			flag = 0;
		}
		// if narrow is empty, the tallest must come from wide
		else {
			// pack the wide
			wide[0].x = 0;
			wide[0].y = actualHeight;
			currentX1 = wide[0].width;
			currentY1 = actualHeight;
			actualHeight += wide[0].height;
			list.push_back(wide[0]);
			wide.erase(wide.begin());
			flag = 1;
		}

		// if tallest rectangle is from the narrow list
		// we use packWide to fill the remaining space
		if (flag == 0)
			packWide(currentX1, currentY1, fixedWidth, actualHeight);

		// if tallest rectangle is from the wide list
		// we use packNarrow to fill the remaing space
		else {
			if(algoNum == 2)
				packNarrow(currentX1, currentY1, 
					fixedWidth, actualHeight);
			else {
				packNarrow2(currentX1, currentY1,
					fixedWidth, actualHeight);
			}
		}
			
	}

	// everything has been done, and now we are gonna to 
	// show our result, including the graphic result,
	// the height we achieved and the calculated approximation ratio
	done = true;
	QString tempStr;
	tempStr.setNum(actualHeight);
	RLine1->setText(tempStr);
	tempStr.setNum(double(actualHeight) / optimalHeight);
	RLine2->setText(tempStr);
}

// packWide subroutine
// it may call packNarrow again and again
// it works in attempt to fill the remaining space
// when the tallest rectangles in one level is from narrow
void MainWindow::packWide(int x1, int y1, int x2, int y2) {
	// if the basic condition is satisfied, i.e.
	// wide is not empty and the first rectangle in wide 
	// can be packed. Otherwise we can do nothing
	if (wide.size() && x2 - x1 >= wide[0].width) {
		for (int j = 0; j < wide.size(); j++) {
			// if rectangle fits wide-wise and height-wise
			if (y2 - y1 >= wide[j].height) {
				wide[j].x = x1;
				wide[j].y = y1;
				list.push_back(wide[j]);

				// if rectangles of unequal widths are stacked(SAS)
				// or if rectangle just packed has created a space
				// on the right of it (SAS_advaneced)
				// we use packNarrow to reduce the redundant space
				// further
				if (x2 - (x1 + wide[j].width) > 0)
					if(x2 != fixedWidth && algoNum == 2)
						packNarrow(x1 + wide[j].width, y1, x2, y2);
					else if(algoNum == 3){
						packNarrow2(x1 + wide[j].width, y1, x2, y2);
					}
				x2 = x1 + wide[j].width;
				y1 += wide[j].height;
				wide.erase(wide.begin() + j);
				j--;
			}
			// In SAS_advanced algorithm, if wide are running out of 
			// rectangles, we can pack the narrow rectangles in the 
			// remaining space
			else if (algoNum == 3) {
				packNarrow(x1, y1, x2, y2);
			}
		}
	}
	return;
}

// packNarrow subroutine, only for SAS algorithm
// it doesn't work in the perfect way
void MainWindow::packNarrow(int x1, int y1, int x2, int y2) {
	// when the narrow is not running out of rectangle
	if (narrow.size()) {
		int baseWidth;
		int currentX1 = x1;	// the coordinate of the starting point
		int currentY1 = y1;	// that we may pack a rectangle in
		// while the rectangle can fit wide-wise
		while (narrow.size() && x2 - currentX1 >= narrow[0].width) {
			baseWidth = narrow[0].width;
			//while the rectangle can fit height-wise
			while (narrow.size() && y2 - currentY1 >= narrow[0].height
				&& narrow[0].width <= baseWidth) {
				narrow[0].x = currentX1;
				narrow[0].y = currentY1;
				list.push_back(narrow[0]);
				// turn to next vertical space
				currentY1 += narrow[0].height;
				narrow.erase(narrow.begin());
			}
			// turn to next horizontal space
			currentX1 += baseWidth;
			currentY1 = y1;
		}
	}
	return;
}

// packNarrow2 subroutine, it may call itself recursively
// it will pack narrow rectangles as many as possible
// in attempt to fill the whole space
void MainWindow::packNarrow2(int x1, int y1, int x2, int y2) {
	if (narrow.size()) {
		// pack first narrow rectangle that 
		// fits height-wise and width-wise
		int i;
		for (i = 0; i < narrow.size(); i++) {
			if (x2 - x1 >= narrow[i].width 
				&& y2 - y1 >= narrow[i].height)
				break;
		}
		// if running out of narrow, then return
		if(i == narrow.size())
			return;
		narrow[i].x = x1;
		narrow[i].y = y1;
		list.push_back(narrow[i]);
		int deltax = narrow[i].width;
		int deltay = narrow[i].height;
		narrow.erase(narrow.begin() + i);
		// call itself to fill the remaining space on the
		// top of the packed rectangle and on the right of 
		// the packed rectangle
		packNarrow2(x1, y1 + deltay, x1 + deltax, y2);
		packNarrow2(x1 + deltax, y1, x2, y2);
	}
	// if running out of narrow rectangle,
	// we can fill the remaining space with wide rectangles
	// in SAS_advanced algorithm
	else if (algoNum == 3) {
		packWide2(x1, y1, x2, y2);
	}
}

// packWide2 subroutine only for SAS_advanced algorithm
// 
void MainWindow::packWide2(int x1, int y1, int x2, int y2) {
	if (wide.size() && x2 - x1 >= wide.back().width) {
		int j;
		// choose the first rectangle from wide
		// that can fit both wide-wise and height-wise
		for (j = 0; j < wide.size(); j++)
			if (x2 - x1 >= wide[j].width)
				break;
		if (j == wide.size())
			return;

		int start;
		for (start = j; start < wide.size(); start++) {
			if (y2 - y1 >= wide[start].height)
				break;
		}
		if (start == wide.size())
			return;

		j = start;
		int x3 = wide[j].width + x1;
		int y3 = y1;
		// while existing rectangles that can fit height-wise
		while (j != wide.size() && y2 - y1 >= wide[j].height) {
			wide[j].x = x1;
			wide[j].y = y1;
			y1 += wide[j].height;
			list.push_back(wide[j]);
			wide.erase(wide.begin() + j);
		}
		// recursively call packWide2 for fill the 
		// remaining space that are on the right-hand side
		packWide2(x3, y3, x2, y2);
	}
	return;
}

// testing interface
void MainWindow::test() {
	// firstly we obtain the iteration time from the user
	bool ok;
	QString tempStr = RLine3->text();
	int iteration = tempStr.toInt(&ok, 10);
	clock_t begin, end;
	double duration;

	// then we begin to calculate the time
	// each iteration will call the specific algorithm completely
	begin = clock();
	for (int m = 0; m < iteration; m++) {
		list.clear();
		list.assign(original.begin(), original.end());
		if (algoNum == 1)
			firstFit();
		else
			SAS();
	}
	end = clock();

	// Finally we will show the testing result
	duration = (double) (end - begin) / CLOCKS_PER_SEC;
	tempStr.setNum(duration);
	RLine4->setText(tempStr);
}
