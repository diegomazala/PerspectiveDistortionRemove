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
	void onNewPoint(int, int, int, int);
	void onCalculateButtonPress();
	void onInputImageToggled(bool);
	void onOutputImageToggled(bool);

	void onLine0RadioButtonToggled(bool);
	void onLine1RadioButtonToggled(bool);
	void onLine2RadioButtonToggled(bool);
	void onLine3RadioButtonToggled(bool);

private:

	Ui::MainWindow *ui;
	QString currentFileName;
	int currentLine;


	QImage inputImage;
	QImage outputImage;
};

static QRgb bilinearInterpol(const QImage& img, float x, float y, float dx, float dy);

#endif // _MAIN_WINDOW_H_