

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
	QString message("...");
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
		switch (vertex_index)
		{
		case 0:
			ui->line0x0SpinBox->setValue(x);
			ui->line0y0SpinBox->setValue(y);
			break;
		case 1:
			ui->line0x1SpinBox->setValue(x);
			ui->line0y1SpinBox->setValue(y);
			break;
		case 2:
			ui->line0x2SpinBox->setValue(x);
			ui->line0y2SpinBox->setValue(y);
			break;
		}
		break;
	case 1:
		switch (vertex_index)
		{
		case 0:
			ui->line1x0SpinBox->setValue(x);
			ui->line1y0SpinBox->setValue(y);
			break;
		case 1:
			ui->line1x1SpinBox->setValue(x);
			ui->line1y1SpinBox->setValue(y);
			break;
		case 2:
			ui->line1x2SpinBox->setValue(x);
			ui->line1y2SpinBox->setValue(y);
			break;
		}
		break;
	default:
		break;
	}
}


void MainWindow::onCalculateButtonPress()
{
	arcr.setLinePoint(0, 
		ui->line0x0SpinBox->value(), ui->line0y0SpinBox->value(),
		ui->line0x1SpinBox->value(), ui->line0y1SpinBox->value(), 
		ui->line0x2SpinBox->value(), ui->line0y2SpinBox->value());
	
	arcr.setLinePoint(1,
		ui->line1x0SpinBox->value(), ui->line1y0SpinBox->value(),
		ui->line1x1SpinBox->value(), ui->line1y1SpinBox->value(),
		ui->line1x2SpinBox->value(), ui->line1y2SpinBox->value());

	arcr.computeHMatrix();

	Eigen::Vector3f img(inputImage.width(), inputImage.height(), 1.0f);
	float xmin = 0;
	float xmax = 0;
	float ymin = 0;
	float ymax = 0;
	arcr.computImageSize(0, 0, inputImage.width(), inputImage.height(), xmin, xmax, ymin, ymax);

	float aspect = (xmax - xmin) / (ymax - ymin);
	outputImage = QImage(inputImage.width(), inputImage.width() / aspect, inputImage.format());
	outputImage.fill(qRgb(0, 0, 0));

	std::cout << "Output size: " << outputImage.width() << ", " << outputImage.height() << std::endl;

	float dx = (xmax - xmin) / float(outputImage.width());
	float dy = (ymax - ymin) / float(outputImage.height());

	std::cout << std::fixed << "dx, dy: " << dx << ", " << dy << std::endl;

	for (int x = 0; x < outputImage.width(); ++x)
	{
		for (int y = 0; y < outputImage.height(); ++y)
		{
			Eigen::Vector3f px(x, y, 1);

			float tx = 0.0f;
			float ty = 0.0f;
			Eigen::Vector2f t = arcr.multiplyPointMatrixInverse(xmin + x * dx, ymin + y * dy);

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

