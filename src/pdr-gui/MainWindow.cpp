

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QKeyEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>



static QRgb bilinearInterpol(const QImage& img, float x, float y, float dx, float dy)
{
	float x0 = x - dx;
	float x1 = x + dx;
	float y0 = y - dy;
	float y1 = y + dy;

	QRgb rgb_xlt = (x0 > -1 && y0 > -1) ? img.pixel(x0, y0) : img.pixel(x, y);
	QRgb rgb_xrt = (x1 < img.width() && y0 > -1) ? img.pixel(x1, y0) : img.pixel(x, y);

	QRgb rgb_xlb = (x0 > -1 && y1 < img.height()) ? img.pixel(x0, y1) : img.pixel(x, y);
	QRgb rgb_xrb = (x1 < img.width() && y1 < img.height()) ? img.pixel(x1, y1) : img.pixel(x, y);

	int r = (0.25f * qRed(rgb_xlt)) + (0.25f * qRed(rgb_xrt)) + (0.25f * qRed(rgb_xlb)) + (0.25f * qRed(rgb_xrb));
	int g = (0.25f * qGreen(rgb_xlt)) + (0.25f * qGreen(rgb_xrt)) + (0.25f * qGreen(rgb_xlb)) + (0.25f * qGreen(rgb_xrb));
	int b = (0.25f * qBlue(rgb_xlt)) + (0.25f * qBlue(rgb_xrt)) + (0.25f * qBlue(rgb_xlb)) + (0.25f * qBlue(rgb_xrb));

	return qRgb(r, g, b);
}

MainWindow::MainWindow(QWidget *parent) : 
			QMainWindow(parent),
			ui(new Ui::MainWindow),
			currentFileName(QString()),
			pdr(4),
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


void MainWindow::fileOpen()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "../../data", tr("Images (*.png *.bmp *.jpg *.tif)"));

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
		("<p>Perspective distortion remove algorithm using Qt and Opengl" \
		"<p><p>" \
		"<br>   Usage: <br>" \
		"<br>   1. Select 4 points on the image. It can be input on left panel or keeping control key pressed and left click over the points.<br>" \
		"<br>   2. Input the 4 world points on the left panel. It must have the same order that you used for input image points. <br>" \
		"<br>   3. Press calculate button <br>" \
		"<p><p><p>" \
		"<p>Developed by: Diego Mazala, June-2015" \
		"<p>");

	QMessageBox::about(this, tr("Perspective Distortion Remove"), message);

}


void MainWindow::onGLMouseMove(int x, int y)
{
	ui->mousePosXLineEdit->setText(QString::number(x));
	ui->mousePosYLineEdit->setText(QString::number(y));
}


void MainWindow::onNewPoint(int index, int x, int y)
{
	switch (index)
	{
	case 0:
		ui->imagePointX0SpinBox->setValue(x);
		ui->imagePointY0SpinBox->setValue(y);
		break;
	case 1:
		ui->imagePointX1SpinBox->setValue(x);
		ui->imagePointY1SpinBox->setValue(y);
		break;
	case 2:
		ui->imagePointX2SpinBox->setValue(x);
		ui->imagePointY2SpinBox->setValue(y);
		break;
	case 3:
		ui->imagePointX3SpinBox->setValue(x);
		ui->imagePointY3SpinBox->setValue(y);
		break;
	default:
		break;
	}
}


void MainWindow::onCalculateButtonPress()
{
	pdr.setImagePoint(0, ui->imagePointX0SpinBox->value(), ui->imagePointY0SpinBox->value());
	pdr.setImagePoint(1, ui->imagePointX1SpinBox->value(), ui->imagePointY1SpinBox->value());
	pdr.setImagePoint(2, ui->imagePointX2SpinBox->value(), ui->imagePointY2SpinBox->value());
	pdr.setImagePoint(3, ui->imagePointX3SpinBox->value(), ui->imagePointY3SpinBox->value());

	pdr.setWorldPoint(0, ui->worldPointX0SpinBox->value(), ui->worldPointY0SpinBox->value());
	pdr.setWorldPoint(1, ui->worldPointX1SpinBox->value(), ui->worldPointY1SpinBox->value());
	pdr.setWorldPoint(2, ui->worldPointX2SpinBox->value(), ui->worldPointY2SpinBox->value());
	pdr.setWorldPoint(3, ui->worldPointX3SpinBox->value(), ui->worldPointY3SpinBox->value());

	pdr.solve();

	float xmin = 0;
	float xmax = 0;
	float ymin = 0;
	float ymax = 0;

	//pdr.computImageSize(0, 0, inputImage.width(), inputImage.height(), xmin, xmax, ymin, ymax);
	pdr.computImageSize(xmin, xmax, ymin, ymax);

	float aspect = (xmax - xmin) / (ymax - ymin);
	outputImage = QImage(inputImage.width(), inputImage.width() / aspect, inputImage.format());
	outputImage.fill(qRgb(0, 0, 0));

	float dx = (xmax - xmin) / float(outputImage.width());
	float dy = (ymax - ymin) / float(outputImage.height());

	for (int x = 0; x < outputImage.width(); ++x)
	{
		for (int y = 0; y < outputImage.height(); ++y)
		{
			float tx = 0;
			float ty = 0;
			pdr.recoverPixel(xmin + x * dx, ymin + y * dx, tx, ty);

			if (tx > -1 && ty > -1
				&& tx < inputImage.width()
				&& ty < inputImage.height())
			{
				QRgb rgb = bilinearInterpol(inputImage, tx, ty, dx, dy);
				outputImage.setPixel(x, y, rgb);
				//outputImage.setPixel(x, y, input.pixel(tx, ty));	// no interpolation
			}
		}
	}

	ui->outputRadioButton->setChecked(true);
	update();
}


void MainWindow::onInputImageToggled(bool toggled)
{
	ui->glImageWidget->setImage(inputImage);
	ui->glImageWidget->setRectEnable(true);
	update();
}


void MainWindow::onOutputImageToggled(bool toggled)
{
	ui->glImageWidget->setImage(outputImage);
	ui->glImageWidget->setRectEnable(false);
	update();
}