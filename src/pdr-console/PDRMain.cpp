#include <QApplication>

#include <QImage>
#include "PDR.h"
#include <iostream>




template <typename T>
static T Lerp(T const& x, T const& x0, T const& x1, T const& y0, T const& y1)
{
	if ((x1 - x0) == 0)
		return (y0 + y1) / 2;
	return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
}


int main(int argc, char* argv[])
{

	// Usage Example:
	// $ ./pdr-console.exe ../../data/brahma01.jpg ../../data/_output.jpg 1

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

	float s = atof(argv[3]);
	PDR pdr(4);

	pdr.setImagePoint(0, 324, 52);
	pdr.setImagePoint(1, 631, 113);
	pdr.setImagePoint(2, 633, 412);
	pdr.setImagePoint(3, 350, 560);

	pdr.setWorldPoint(0, 0.0f * s, 0.0f * s);
	pdr.setWorldPoint(1, 81.9f * s, 0.0f * s);
	pdr.setWorldPoint(2, 81.9f * s, 61.3f * s);
	pdr.setWorldPoint(3, 0.0f * s, 61.3f * s);
	
	pdr.solve();
	
	std::string inputFileName = "input.jpg";
	std::string outputFileName = "output.jpg";
	if (argc > 1)
		inputFileName = argv[1];
	if (argc > 2)
		outputFileName = argv[2];

	QImage input(inputFileName.c_str());
	
	float minX = 0;
	float maxX = 0;
	float minY = 0;
	float maxY = 0;

	pdr.computImageSize(input.width(), input.height(), minX, maxX, minY, maxY);

	//QImage output(maxX - minX, maxY - minY, input.format());
	QImage output(input.size(), input.format());
	output.fill(qRgb(0, 0, 0));

	std::cout << "Output size: " << output.width() << ", " << output.height() << std::endl;


#if 1
	float dx = output.width() / (maxX - minX);
	float dy = output.height() / (maxY - minY);

	for (int x = minX; x < maxX; ++x)
	{
		for (int y = minY; y < maxY; ++y)
		{
			float img_x = 0;
			float img_y = 0;
			pdr.recoverPixel(x, y, img_x, img_y);

			if (img_x > -1 && img_y > -1
				&& img_x < input.width() 
				&& img_y < input.height())
			{
				float tx = 0;
				float ty = 0;
				pdr.computePixel(img_x, img_y, tx, ty);

				output.setPixel((tx - minX) * dx, (ty - minY) * dy, input.pixel(img_x, img_y));
			}
		}
	}

#else

	float dx = output.width() / (maxX - minX);
	float dy = output.height() / (maxY - minY);

	for (int x = 0; x < input.width(); ++x)
	{
		for (int y = 0; y < input.height(); ++y)
		{
			float tx = 0;
			float ty = 0;
			pdr.computePixel(x, y, tx, ty);

			output.setPixel((tx - minX) * dx, (ty - minY) * dy, input.pixel(x, y));
		}
	}
#endif
	output.save(outputFileName.c_str());

	return EXIT_SUCCESS;
}
