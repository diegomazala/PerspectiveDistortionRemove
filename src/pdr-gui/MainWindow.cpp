

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QKeyEvent>
#include <QFileDialog>
#include <iostream>


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

	//ui->glImageWidget->setImage(QImage("../../../data/brahma01.jpg"));
}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::fileOpen()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "../../../data", tr("Images (*.png *.bmp *.jpg *.tif)"));

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

	float minX = 0;
	float maxX = 0;
	float minY = 0;
	float maxY = 0;

	pdr.computImageSize(inputImage.width(), inputImage.height(), minX, maxX, minY, maxY);

	float aspect = (maxX - minX) / (maxY - minY);
	outputImage = QImage(inputImage.width(), inputImage.width() / aspect, inputImage.format());
	outputImage.fill(qRgb(0, 0, 0));

	float dx = (maxX - minX) / outputImage.width();

	for (int x = 0; x < outputImage.width(); ++x)
	{
		for (int y = 0; y < outputImage.height(); ++y)
		{
			float tx = 0;
			float ty = 0;
			pdr.recoverPixel(minX + x * dx, minY + y * dx, tx, ty);

			if (tx > -1 && ty > -1
				&& tx < inputImage.width()
				&& ty < inputImage.height())
			{
				outputImage.setPixel(x, y, inputImage.pixel(tx, ty));
			}
		}
	}

	ui->outputRadioButton->setChecked(true);
	update();
}


void MainWindow::onInputImageToggled(bool toggled)
{
	ui->glImageWidget->setImage(inputImage);
	update();
}


void MainWindow::onOutputImageToggled(bool toggled)
{
	ui->glImageWidget->setImage(outputImage);
	update();
}