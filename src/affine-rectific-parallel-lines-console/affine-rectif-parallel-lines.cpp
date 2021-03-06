#include <QApplication>
#include <QImage>
#include <Eigen/Dense>
#include <iostream>
#include <fstream>
#include "Interpolation.h"
#include "Arpl.h"

void readConfigFile(const char* config_file, Arpl& arpl, std::string& input_file_name, std::string& output_file_name, bool& interpolate)
{
	std::ifstream config(config_file);
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
				input_file_name = str_list[1];
			}
			else if (str_list[0] == "output")
			{
				output_file_name = str_list[1];
			}
			else if (str_list[0] == "interpolation")
			{
				interpolate = !(str_list[1] == "off");
			}
			else if (str_list[0] == "lines_image_points")
			{
				for (int i = 0; i < 4; ++i)
				{
					float x0, y0, x1, y1;
					config >> x0 >> y0 >> x1 >> y1;
					arpl.setLinePoint(i, x0, y0, x1, y1);
				}
			}
		}
		config.close();
	}
}

// Example usage:  $./affine-rectific-parallel-lines-console.exe ../../data/config-affine-rectific-parallel-lines.txt
int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cerr << "Error: Missing parameters.\n"
			<< "Usage: <app.exe> <config_file>"
			<< std::endl;
		return EXIT_FAILURE;
	}

	Arpl arpl;
	std::string inputFileName = "input.png";
	std::string outputFileName = "output.png";
	bool interpolate = true;

	readConfigFile(argv[1], arpl, inputFileName, outputFileName, interpolate);
	
	arpl.computeHMatrix();

	QImage input(inputFileName.c_str());

	Eigen::Vector3f img(input.width(), input.height(), 1.0f);

	float xmin = 0;
	float xmax = 0;
	float ymin = 0;
	float ymax = 0;
	arpl.computImageSize(0, 0, input.width(), input.height(), xmin, xmax, ymin, ymax);

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
			Eigen::Vector3f px(x, y, 1);

			float tx = 0.0f;
			float ty = 0.0f;
			Eigen::Vector2f t = arpl.multiplyPointMatrixInverse(xmin + x * dx, ymin + y * dy);

			if (t.x() > -1 && t.y() > -1
				&& t.x() < input.width()
				&& t.y() < input.height())
			{
				if (interpolate)
				{
					QRgb rgb = bilinearInterpol(input, t.x(), t.y(), dx / 2.0, dy / 2.0);
					output.setPixel(x, y, rgb);
				}
				else
				{
					output.setPixel(x, y, input.pixel(t.x(), t.y()));
				}
			}
		}
	}


	output.save(outputFileName.c_str());

	return EXIT_SUCCESS;
}
