#include "mainwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{

	QApplication a(argc, argv);	 // default object
	MainWindow w;		// Object for GUI and testing
	return a.exec();	// Infinite loop until stop
}
