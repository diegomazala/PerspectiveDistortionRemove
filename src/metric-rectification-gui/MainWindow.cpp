

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QKeyEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>


MainWindow::MainWindow(QWidget *parent) : 
			QMainWindow(parent),
			ui(new Ui::MainWindow),
			currentFileName(QString()),
			currentLine(0),
			rectific(5),
			inputImage(1,1,QImage::Format_RGB888),
			outputImage(1, 1, QImage::Format_RGB888)
{
	ui->setupUi(this);

	inputImage.fill(Qt::GlobalColor::black);
	outputImage.fill(Qt::GlobalColor::black);
}

MainWindow::~MainWindow()
{
	delete ui;
}

GLImageWidget* MainWindow::getGLWidget()
{
	return ui->glImageWidget;
}

void MainWindow::fileOpen()
{
	// look for shader dir 
	QDir dir;
	std::string open_dir("data");
	for (int i = 0; i < 5; ++i)
		if (!dir.exists(open_dir.c_str()))
			open_dir.insert(0, "../");

	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), open_dir.c_str(), tr("Images (*.png *.bmp *.jpg *.tif)"));

	if (!fileName.isEmpty())
	{
		currentFileName = fileName;
		// load image
		if (inputImage.load(fileName))
		{
			ui->glImageWidget->setImage(inputImage);
			ui->imageSizeXLineEdit->setText(QString::number(inputImage.width()));
			ui->imageSizeYLineEdit->setText(QString::number(inputImage.height()));
			ui->inputRadioButton->setChecked(true);
		}
	}
}


void MainWindow::fileSave()
{
	if (!currentFileName.isEmpty())
	{
		
	}
	else
	{
		fileSaveAs();
	}
}


void MainWindow::fileSaveAs()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Images (*.png *.bmp *.jpg *.tiff)"));
	if (!fileName.isEmpty())
	{
		currentFileName = fileName;
		fileSave();
	}
}


void MainWindow::aboutDialogShow()
{
	QString message
		("<p>" \
		"<p>");

	QMessageBox::about(this, tr("..."), message);

}


void MainWindow::onGLMouseMove(int x, int y)
{
	ui->mousePosXLineEdit->setText(QString::number(x));
	ui->mousePosYLineEdit->setText(QString::number(y));
}



void MainWindow::onCalculateButtonPress()
{

	rectific.solve();

	Eigen::Vector3d img(inputImage.width(), inputImage.height(), 1.0f);

	double xmin, xmax, ymin, ymax;
	rectific.computImageSize(0, 0, inputImage.width(), inputImage.height(), xmin, xmax, ymin, ymax);

	double aspect = (xmax - xmin) / (ymax - ymin);
	outputImage = QImage(inputImage.width(), inputImage.width() / aspect, inputImage.format());
	outputImage.fill(qRgb(0, 0, 0));


	double dx = (xmax - xmin) / double(outputImage.width());
	double dy = (ymax - ymin) / double(outputImage.height());

	for (int x = 0; x < outputImage.width(); ++x)
	{
		for (int y = 0; y < outputImage.height(); ++y)
		{
			Eigen::Vector3d px(x, y, 1);

			double tx = 0.0f;
			double ty = 0.0f;
			Eigen::Vector2d t = rectific.multiplyPointMatrix(xmin + x * dx, ymin + y * dy);

			if (t.x() > -1 && t.y() > -1
				&& t.x() < inputImage.width()
				&& t.y() < inputImage.height())
			{
				//QRgb rgb = bilinearInterpol(inputImage, t.x(), t.y(), dx / 2.0f, dy / 2.0f);
				//outputImage.setPixel(x, y, rgb);
				
				//outputImage.setPixel(x, outputImage.height() - 1 - y, inputImage.pixel(t.x(), inputImage.height() - 1 - t.y()));
				outputImage.setPixel(x, y, inputImage.pixel(t.x(), t.y()));
			}
		}
	}

	ui->outputRadioButton->setChecked(true);
	update();
}


void MainWindow::onInputImageToggled(bool toggled)
{
	ui->glImageWidget->setImage(inputImage);
	ui->glImageWidget->setLinesEnable(true);
	update();
}


void MainWindow::onOutputImageToggled(bool toggled)
{
	ui->glImageWidget->setImage(outputImage);
	ui->glImageWidget->setLinesEnable(false);
	update();
}


void MainWindow::onLine0RadioButtonToggled(bool toggle)
{
	if (toggle)
	{
		currentLine = 0;
		ui->glImageWidget->setCurrentLine(currentLine);
	}
}

void MainWindow::onLine1RadioButtonToggled(bool toggle)
{
	if (toggle)
	{
		currentLine = 2;
		ui->glImageWidget->setCurrentLine(currentLine);
	}
}

void MainWindow::onLine2RadioButtonToggled(bool toggle)
{
	if (toggle)
	{
		currentLine = 4;
		ui->glImageWidget->setCurrentLine(currentLine);
	}
}

void MainWindow::onLine3RadioButtonToggled(bool toggle)
{
	if (toggle)
	{
		currentLine = 6;
		ui->glImageWidget->setCurrentLine(currentLine);
	}
}

void MainWindow::onLine4RadioButtonToggled(bool toggle)
{
	if (toggle)
	{
		currentLine = 8;
		ui->glImageWidget->setCurrentLine(currentLine);
	}
}


void MainWindow::onNewPoint(int line_index, int vertex_index, int x, int y)
{
	rectific.setVertexLine(line_index, vertex_index, Eigen::Vector3d(x, y, 1));

	if (line_index % 2)
		vertex_index = 2 + vertex_index;

	line_index = line_index / 2;

	switch (line_index)
	{
	case 0:
		switch (vertex_index)
		{
		case 0:
			ui->pairLine0x0SpinBox->setValue(x);
			ui->pairLine0y0SpinBox->setValue(y);
			break;
		case 1:
			ui->pairLine0x1SpinBox->setValue(x);
			ui->pairLine0y1SpinBox->setValue(y);
		case 2:
			ui->pairLine0x2SpinBox->setValue(x);
			ui->pairLine0y2SpinBox->setValue(y);
		case 3:
			ui->pairLine0x3SpinBox->setValue(x);
			ui->pairLine0y3SpinBox->setValue(y);
		default:
			break;
		}
		break;
	case 1:
		switch (vertex_index)
		{
		case 0:
			ui->pairLine1x0SpinBox->setValue(x);
			ui->pairLine1y0SpinBox->setValue(y);
			break;
		case 1:
			ui->pairLine1x1SpinBox->setValue(x);
			ui->pairLine1y1SpinBox->setValue(y);
		case 2:
			ui->pairLine1x2SpinBox->setValue(x);
			ui->pairLine1y2SpinBox->setValue(y);
		case 3:
			ui->pairLine1x3SpinBox->setValue(x);
			ui->pairLine1y3SpinBox->setValue(y);
		default:
			break;
		}
		break;
	case 2:
		switch (vertex_index)
		{
		case 0:
			ui->pairLine2x0SpinBox->setValue(x);
			ui->pairLine2y0SpinBox->setValue(y);
			break;
		case 1:
			ui->pairLine2x1SpinBox->setValue(x);
			ui->pairLine2y1SpinBox->setValue(y);
		case 2:
			ui->pairLine2x2SpinBox->setValue(x);
			ui->pairLine2y2SpinBox->setValue(y);
		case 3:
			ui->pairLine2x3SpinBox->setValue(x);
			ui->pairLine2y3SpinBox->setValue(y);
		default:
			break;
		}
		break;
	case 3:
		switch (vertex_index)
		{
		case 0:
			ui->pairLine3x0SpinBox->setValue(x);
			ui->pairLine3y0SpinBox->setValue(y);
			break;
		case 1:
			ui->pairLine3x1SpinBox->setValue(x);
			ui->pairLine3y1SpinBox->setValue(y);
		case 2:
			ui->pairLine3x2SpinBox->setValue(x);
			ui->pairLine3y2SpinBox->setValue(y);
		case 3:
			ui->pairLine3x3SpinBox->setValue(x);
			ui->pairLine3y3SpinBox->setValue(y);
		default:
			break;
		}
		break;

	case 4:
		switch (vertex_index)
		{
		case 0:
			ui->pairLine4x0SpinBox->setValue(x);
			ui->pairLine4y0SpinBox->setValue(y);
			break;
		case 1:
			ui->pairLine4x1SpinBox->setValue(x);
			ui->pairLine4y1SpinBox->setValue(y);
		case 2:
			ui->pairLine4x2SpinBox->setValue(x);
			ui->pairLine4y2SpinBox->setValue(y);
		case 3:
			ui->pairLine4x3SpinBox->setValue(x);
			ui->pairLine4y3SpinBox->setValue(y);
		default:
			break;
		}
		break;
	default:
		break;
	}

	if (line_index != currentLine)	// update ui toggle
	{
		switch (line_index)
		{
		case 0:
			ui->line0RadioButton->setChecked(true);
			break;
		case 1:
			ui->line1RadioButton->setChecked(true);
			break;
		case 2:
			ui->line2RadioButton->setChecked(true);
			break;
		case 3:
			ui->line3RadioButton->setChecked(true);
			break;
		default:
			break;
		}
	}
}
