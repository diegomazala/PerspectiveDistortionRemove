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

	pdr.computImageSize(input.width(), input.height(), xmin, xmax, ymin, ymax);

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
					QRgb rgb = bilinearInterpol(input, tx, ty, 0.5f, 0.5f);
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
