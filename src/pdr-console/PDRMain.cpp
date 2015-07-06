#include <QApplication>

#include <QImage>
#include "PDR.h"
#include <iostream>
#include <fstream>



template <typename T>
static T Lerp(T const& x, T const& x0, T const& x1, T const& y0, T const& y1)
{
	if ((x1 - x0) == 0)
		return (y0 + y1) / 2;
	return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
}


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


int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cerr << "Error: Missing parameters.\n"
			<< "Usage: <app.exe> <config_file>"
			<< std::endl;
		return EXIT_FAILURE;
	}

	std::cout.precision(2);		// output info

	PDR pdr(4);

	std::string inputFileName = "input.png";
	std::string outputFileName = "output.png";
	float scale = 1.0f;
	bool interpolate = true;

	std::ifstream config(argv[1]);
	if (config.is_open())
	{
		for (std::string line; std::getline(config, line);)
		{
			if (line.empty())
				continue;

			std::istringstream iss(line);
			std::vector<std::string> str_list{ 
						std::istream_iterator<std::string>{iss},
						std::istream_iterator<std::string>{} };


			if (str_list[0] == "input")
			{
				inputFileName = str_list[1];
			}
			else if (str_list[0] == "output")
			{
				outputFileName = str_list[1];
			}
			else if (str_list[0] == "interpolation")
			{
				interpolate = !(str_list[1] == "off");
			}
			else if (str_list[0] == "scale")
			{
				scale = std::stof(str_list[1]);
			}
			else if (str_list[0] == "image_points")
			{
				for (int i = 0; i < 4; ++i)
				{
					float x, y;
					config >> x >> y;
					pdr.setImagePoint(i, x, y);
				}
			}
			else if (str_list[0] == "world_points")
			{
				for (int i = 0; i < 4; ++i)
				{
					float x, y;
					config >> x >> y;
					pdr.setWorldPoint(i, x * scale, y * scale);
				}
			}

		}
		config.close();
	}
	
	pdr.solve();
	
	QImage input(inputFileName.c_str());
	
	float xmin = 0;
	float xmax = 0;
	float ymin = 0;
	float ymax = 0;

	//pdr.computImageSize(0, 0, input.width(), input.height(), xmin, xmax, ymin, ymax);
	pdr.computImageSize(xmin, xmax, ymin, ymax);

	float aspect = (xmax - xmin) / (ymax - ymin);
	QImage output(input.width(), input.width() / aspect, input.format());
	output.fill(qRgb(0, 0, 0));

	std::cout << "Output size: " << output.width() << ", " << output.height() << std::endl;

	float dx = (xmax - xmin) / float(output.width());
	float dy = (ymax - ymin) / float(output.height());

	std::cout << std::fixed << "dx, dy: " << dx << ", " << dy << std::endl;

	for (int x = 0; x < output.width(); ++x)
	{
		for (int y = 0; y < output.height(); ++y)
		{
			float tx = 0;
			float ty = 0;
			pdr.recoverPixel(xmin + x * dx, ymin + y * dx, tx, ty);

			if (tx > -1 && ty > -1
				&& tx < input.width()
				&& ty < input.height())
			{
				if (interpolate)
				{
					QRgb rgb = bilinearInterpol(input, tx, ty, dx, dy);
					output.setPixel(x, y, rgb);
				}
				else
				{
					output.setPixel(x, y, input.pixel(tx, ty));
				}
			}
		}
	}

	output.save(outputFileName.c_str());

	return EXIT_SUCCESS;
}
