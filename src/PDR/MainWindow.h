#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_


#include <QMainWindow>
#include "PDR.h"

class AboutDialog;

namespace Ui 
{
	class MainWindow;
}


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void fileOpen();
	void fileSave();
	void fileSaveAs();
	void aboutDialogShow();
	void onGLMouseMove(int, int);
	void onNewPoint(int,int, int);
	void onCalculateButtonPress();
	void onInputImageToggled(bool);
	void onOutputImageToggled(bool);

private:

	Ui::MainWindow *ui;
	QString currentFileName;

	PDR pdr;
	QImage inputImage;
	QImage outputImage;
};

#endif // _MAIN_WINDOW_H_