

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QKeyEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>


QRgb bilinearInterpol(const QImage& img, float x, float y, float dx, float dy)
{
	float x1 = x - dx;
	float x2 = x + dx;
	float y1 = y - dy;
	float y2 = y + dy;

	QRgb q11 = (x1 > -1 && y1 > -1) ? img.pixel(x1, y1) : img.pixel(x, y);
	QRgb q21 = (x2 < img.width() && y1 > -1) ? img.pixel(x2, y1) : img.pixel(x, y);
	QRgb q12 = (x1 > -1 && y2 < img.height()) ? img.pixel(x1, y2) : img.pixel(x, y);
	QRgb q22 = (x2 < img.width() && y2 < img.height()) ? img.pixel(x2, y2) : img.pixel(x, y);

	float q11r = float(qRed(q11)) / 255.0f;
	float q21r = float(qRed(q21)) / 255.0f;
	float q12r = float(qRed(q12)) / 255.0f;
	float q22r = float(qRed(q22)) / 255.0f;
	float r1 = ((x2 - x) / (x2 - x1))*q11r + ((x - x1) / (x2 - x1))*q21r;
	float r2 = ((x2 - x) / (x2 - x1))*q12r + ((x - x1) / (x2 - x1))*q22r;
	float red = ((y2 - y) / (y2 - y1)) * r1 + ((y - y1) / (y2 - y1)) * r2;

	float q11g = float(qGreen(q11)) / 255.0f;
	float q21g = float(qGreen(q21)) / 255.0f;
	float q12g = float(qGreen(q12)) / 255.0f;
	float q22g = float(qGreen(q22)) / 255.0f;
	float g1 = ((x2 - x) / (x2 - x1))*q11g + ((x - x1) / (x2 - x1))*q21g;
	float g2 = ((x2 - x) / (x2 - x1))*q12g + ((x - x1) / (x2 - x1))*q22g;
	float green = ((y2 - y) / (y2 - y1)) * g1 + ((y - y1) / (y2 - y1)) * g2;

	float q11b = float(qBlue(q11)) / 255.0f;
	float q21b = float(qBlue(q21)) / 255.0f;
	float q12b = float(qBlue(q12)) / 255.0f;
	float q22b = float(qBlue(q22)) / 255.0f;
	float b1 = ((x2 - x) / (x2 - x1))*q11b + ((x - x1) / (x2 - x1))*q21b;
	float b2 = ((x2 - x) / (x2 - x1))*q12b + ((x - x1) / (x2 - x1))*q22b;
	float blue = ((y2 - y) / (y2 - y1)) * b1 + ((y - y1) / (y2 - y1)) * b2;


	int r = red * 255.f;
	int g = green * 255.f;
	int b = blue * 255.f;

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
		"<br>		a. With control key, you will draw lines.<br>" \
		"<br>		b. With shift key, you will draw points.<br>" \
		"<br>		c. After draw, you can switch between lines and points pressing keys 'L' or 'P'.<br>" \
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