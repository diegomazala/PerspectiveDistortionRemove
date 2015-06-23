#include <QApplication>
#include "MainWindow.h"

#include <QImage>
#include "PDR.h"

int main(int argc, char* argv[])
{
	//QApplication a(argc, argv);
	//MainWindow w;
	//w.show();
	//return a.exec();

	// image size = 800 x 600
	
	// image points
	// 324, 52
	// 631, 113
	// 633, 412
	// 350, 560

	// world points
	// 0, 0
	// 81.9, 0
	// 81.9, 61.3
	// 0, 61.3

	float s = atoi(argv[3]);
	float shift_x = atoi(argv[4]);
	float shift_y = atoi(argv[5]);
	PDR pdr(4);

	pdr.setWorldPoint(0, 324, 52);
	pdr.setWorldPoint(1, 631, 113);
	pdr.setWorldPoint(2, 633, 412);
	pdr.setWorldPoint(3, 350, 560);

	pdr.setImagePoint(0, 0.0f + shift_x, 0.0f + shift_y);
	pdr.setImagePoint(1, 81.9f + shift_x, 0.0f + shift_y);
	pdr.setImagePoint(2, 81.9f + shift_x, 61.3f + shift_y);
	pdr.setImagePoint(3, 0.0f + shift_x, 61.3f + shift_y);

	//pdr.setImagePoint(0, 0.0f * s, 0.0f * s);
	//pdr.setImagePoint(1, 81.9f * s, 0.0f * s);
	//pdr.setImagePoint(2, 81.9f * s, 61.3f * s);
	//pdr.setImagePoint(3, 0.0f * s, 61.3f * s);


	//pdr.setImagePoint(0, 324, 52);
	//pdr.setImagePoint(1, 631, 113);
	//pdr.setImagePoint(2, 633, 412);
	//pdr.setImagePoint(3, 350, 560);

	//pdr.setWorldPoint(0, 0.0f, 0.0f);
	//pdr.setWorldPoint(1, 81.9f, 0.0f);
	//pdr.setWorldPoint(2, 81.9f, 61.3f);
	//pdr.setWorldPoint(3, 0.0f, 61.3f);

	//pdr.setWorldPoint(3, 0.0f, 0.0f);
	//pdr.setWorldPoint(2, 81.9f, 0.0f);
	//pdr.setWorldPoint(1, 81.9f, 61.3f);
	//pdr.setWorldPoint(0, 0.0f, 61.3f);

	pdr.solve();

	

	std::string inputFileName = "input.jpg";
	std::string outputFileName = "output.jpg";
	if (argc > 1)
		inputFileName = argv[1];
	if (argc > 2)
		outputFileName = argv[2];

	QImage input(inputFileName.c_str());
	QImage output(1200, 900, input.format());
	output.fill(qRgb(0, 0, 0));
	
	int newWidth = 0;
	int newHeight = 0;
	int shiftX = 0;
	int shiftY = 0;
	pdr.computImageSize(input.width(), input.height(), newWidth, newHeight, shiftX, shiftY);


	for (int x = 0; x < output.width(); ++x)
	{
		for (int y = 0; y < output.height(); ++y)
		{
			int nx = x; // +shift_x;
			int ny = y; // +shift_y;

			float rx = 0;
			float ry = 0;
			pdr.computePixel(nx, ny, rx, ry);

			if (rx > 0
				&& ry > 0
				&& rx < input.width()
				&& ry < input.height())
			{

				if (nx < output.width() && ny < output.height())
					output.setPixel(nx, ny, input.pixel(rx, ry));
			}
		}
	}

	output.save(outputFileName.c_str());
	return EXIT_SUCCESS;
}
