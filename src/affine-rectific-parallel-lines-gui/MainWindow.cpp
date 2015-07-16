

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QKeyEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>
#include "Interpolation.h"



MainWindow::MainWindow(QWidget *parent) : 
			QMainWindow(parent),
			ui(new Ui::MainWindow),
			currentFileName(QString()),
			currentLine(0),
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


void MainWindow::onNewPoint(int line_index, int vertex_index, int x, int y)
{
	switch (line_index)
	{
	case 0:
		if (vertex_index == 0)
		{
			ui->line0x0SpinBox->setValue(x);
			ui->line0y0SpinBox->setValue(y);
		}
		else
		{
			ui->line0x1SpinBox->setValue(x);
			ui->line0y1SpinBox->setValue(y);
		}
		break;
	case 1:
		if (vertex_index == 0)
		{
			ui->line1x0SpinBox->setValue(x);
			ui->line1y0SpinBox->setValue(y);
		}
		else
		{
			ui->line1x1SpinBox->setValue(x);
			ui->line1y1SpinBox->setValue(y);
		}
		break;
	case 2:
		if (vertex_index == 0)
		{
			ui->line2x0SpinBox->setValue(x);
			ui->line2y0SpinBox->setValue(y);
		}
		else
		{
			ui->line2x1SpinBox->setValue(x);
			ui->line2y1SpinBox->setValue(y);
		}
		break;
	case 3:
		if (vertex_index == 0)
		{
			ui->line3x0SpinBox->setValue(x);
			ui->line3y0SpinBox->setValue(y);
		}
		else
		{
			ui->line3x1SpinBox->setValue(x);
			ui->line3y1SpinBox->setValue(y);
		}
		break;
	default:
		break;
	}
}


void MainWindow::onCalculateButtonPress()
{
	arpl.setLinePoint(0, 
		ui->line0x0SpinBox->value(), ui->line0y0SpinBox->value(), 
		ui->line0x1SpinBox->value(), ui->line0y1SpinBox->value());
	
	arpl.setLinePoint(1,
		ui->line1x0SpinBox->value(), ui->line1y0SpinBox->value(),
		ui->line1x1SpinBox->value(), ui->line1y1SpinBox->value());

	arpl.setLinePoint(2,
		ui->line2x0SpinBox->value(), ui->line2y0SpinBox->value(),
		ui->line2x1SpinBox->value(), ui->line2y1SpinBox->value());

	arpl.setLinePoint(3,
		ui->line3x0SpinBox->value(), ui->line3y0SpinBox->value(),
		ui->line3x1SpinBox->value(), ui->line3y1SpinBox->value());

	arpl.computeHMatrix();

	Eigen::Vector3f img(inputImage.width(), inputImage.height(), 1.0f);

	float xmin, xmax, ymin, ymax;
	arpl.computImageSize(0, 0, inputImage.width(), inputImage.height(), xmin, xmax, ymin, ymax);
	
	float aspect = (xmax - xmin) / (ymax - ymin);
	outputImage = QImage(inputImage.width(), inputImage.width() / aspect, inputImage.format());
	outputImage.fill(qRgb(0, 0, 0));

	float dx = (xmax - xmin) / float(outputImage.width());
	float dy = (ymax - ymin) / float(outputImage.height());

	for (int x = 0; x < outputImage.width(); ++x)
	{
		for (int y = 0; y < outputImage.height(); ++y)
		{
			Eigen::Vector3f px(x, y, 1);

			float tx = 0.0f;
			float ty = 0.0f;
			Eigen::Vector2f t = arpl.multiplyPointMatrixInverse(xmin + x * dx, ymin + y * dy);

			if (t.x() > -1 && t.y() > -1
				&& t.x() < inputImage.width()
				&& t.y() < inputImage.height())
			{
				QRgb rgb = bilinearInterpol(inputImage, t.x(), t.y(), dx, dy);
				outputImage.setPixel(x, y, rgb);
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
		currentLine = 1;
		ui->glImageWidget->setCurrentLine(currentLine);
	}
}

void MainWindow::onLine2RadioButtonToggled(bool toggle)
{
	if (toggle)
	{
		currentLine = 2;
		ui->glImageWidget->setCurrentLine(currentLine);
	}
}

void MainWindow::onLine3RadioButtonToggled(bool toggle)
{
	if (toggle)
	{
		currentLine = 3;
		ui->glImageWidget->setCurrentLine(currentLine);
	}
}

