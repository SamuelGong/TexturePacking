#pragma once

// following are the Qt libraries
#include <QtWidgets/QMainWindow>	// for QWidget
#include <QLabel>					// for QLabel
#include <QLineEdit>				// for QLineEdit
#include <QPushButton>				// for QPushButton
#include <QHBoxLayout>				// for QHBoxLayout
#include <QVBoxLayout>				// for QVBoxLayout
#include <QString>					// for QString
#include <QStringList>				// for QStringList
#include <QTableWidget>				// for QTableWidget
#include <QFileDialog>				// for QFileDialog
#include <QMessageBox>				// for QMessageBox
#include <QTextStream>				// for QTextStream
#include <QRegExp>					// for QRegExp
#include <QDebug>					// for QDebug
// following are my header files
#include <resultpaint.h>	// for painting the result
#include <rectangle.h>		// for rectangle structure
#include <level.h>			// for level structure
#define testDef 1			// for testing

class MainWindow : public QMainWindow
{
	Q_OBJECT		// a kind of define

public:
	MainWindow(QWidget *parent = Q_NULLPTR);		// constructor
	~MainWindow();									// destructor
private:
	// following are the widgets that will be shown in the mainwinow
	QWidget *window;
	QHBoxLayout *hMLayout, *hLLayout1, *hRLayout1,
		*hLLayout2, *hRLayout2, *hLLayout3;
	QVBoxLayout *vLLayout, *vRLayout;

	QTableWidget *table;
	resultPaint painter;

	QLabel *LLabel1, *LLabel2, *RLabel1, *RLabel2, *RLabel3, *RLabel4;
	QLineEdit *LLine1, *LLine2, *RLine1, *RLine2, *RLine3, *RLine4;
	QPushButton *LBtn1, *LBtn2, *LBtn3, *LBtn4, *RBtn1;
	// following are the slot functions:
private slots:
	void import();	// for importing test case
	void a1();		// for executing FFDH algorithm
	void a2();		// for executing SAS algorithm
	void a3();		// for executing SAS_advanced algorithm
	void firstFit();	// kernel of First Fit algorithm
	void SAS();			// kernel of SAS algorithm
	void test();		// for testing the running time 
	// following are three subroutines for SAS/SAS_advanced algorithm
	void packWide(int x1, int y1, int x2, int y2);
	void packWide2(int x1, int y1, int x2, int y2);
	void packNarrow(int x1, int y1, int x2, int y2);
	void packNarrow2(int x1, int y1, int x2, int y2);
};
