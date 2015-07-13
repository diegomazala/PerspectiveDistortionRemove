#include <QApplication>
#include <QImage>
#include <Eigen/Dense>
#include <iostream>
#include <fstream>
#include <interpolation.h>


void computImageSize(Eigen::Matrix3f h, float in_x, float in_y, float in_width, float in_height, float& min_x, float& max_x, float& min_y, float& max_y)
{
	Eigen::Vector3f p[4] = {
		{ in_x, in_y, 1.0f },
		{ in_width, in_y, 1.0f },
		{ in_width, in_height, 1.0f },
		{ in_x, in_height, 1.0f } };


	Eigen::Vector3f r[4];

	for (int i = 0; i < 4; ++i)
	{
		//r[i] = h.inverse() * p[i];
		r[i] = h * p[i];
		r[i] = Eigen::Vector3f(r[i].x() / r[i].z(), r[i].y() / r[i].z(), 1.0f);

		std::cout
			<< "p[" << i << "] : (" << p[i].x() << ", " << p[i].y() << ")  \t\t"
			<< "r[" << i << "] : (" << r[i].x() << ", " << r[i].y() << ")" << std::endl;
	}

	min_x = r[0].x();
	max_x = r[0].x();

	min_y = r[0].y();
	max_y = r[0].y();

	for (int i = 1; i < 4; ++i)
	{
		if (r[i].x() < min_x)
			min_x = r[i].x();

		if (r[i].y() < min_y)
			min_y = r[i].y();

		if (r[i].x() > max_x)
			max_x = r[i].x();

		if (r[i].y() > max_y)
			max_y = r[i].y();
	}


	std::cout << "\n\nMinMax : (" << min_x << "," << min_y << ") (" << max_x << ", " << max_y << ")" << std::endl;
	std::cout << "Image Size: " << max_x - min_x << ", " << max_y - min_y << std::endl;
}


void multiplyPixelMatrix(float x, float y, Eigen::Matrix3f h, float& rx, float& ry)
{
	Eigen::Vector3f p(x, y, 1.0f);
	Eigen::Vector3f r = h * p;
	rx = r.x() / r.z();
	ry = r.y() / r.z();
}

int main(int argc, char* argv[])
{
	Eigen::Vector3f p0(200, 278, 1);
	Eigen::Vector3f p1(127, 215, 1);
	Eigen::Vector3f p2(270, 212, 1);
	Eigen::Vector3f p3(199, 163, 1);

	Eigen::Vector3f l0 = p1 - p0;
	Eigen::Vector3f l1 = p3 - p2;
	l0.z() = 1.0f;
	l1.z() = 1.0f;

	std::cout << "l0 : " << l0.x() << ", " << l0.y() << ", " << l0.z() << std::endl;
	std::cout << "l1 : " << l1.x() << ", " << l1.y() << ", " << l1.z() << std::endl;

	Eigen::Vector3f l2 = p2 - p0;
	Eigen::Vector3f l3 = p1 - p3;
	l2.z() = 1.0f;
	l3.z() = 1.0f;

	std::cout << "l2 : " << l2.x() << ", " << l2.y() << ", " << l2.z() << std::endl;
	std::cout << "l3 : " << l3.x() << ", " << l3.y() << ", " << l3.z() << std::endl;

	Eigen::Vector3f x0 = l0.cross(l1);
	Eigen::Vector3f x1 = l2.cross(l3);

	std::cout << "x0 : " << x0.x() << ", " << x0.y() << ", " << x0.z() << std::endl;
	std::cout << "x1 : " << x1.x() << ", " << x1.y() << ", " << x1.z() << std::endl;

	Eigen::Vector3f l = x1.cross(x0);

	std::cout.precision(5);		// output info
	std::cout << std::fixed << "line : " << l.x() << ", " << l.y() << ", " << l.z() << std::endl << std::endl;
	l.normalize();
	//l.x() = l.x() / l.z();
	//l.y() = l.y() / l.z();
	//l.z() = 1.0f;
	std::cout << std::fixed << "line : " << l.x() << ", " << l.y() << ", " << l.z() << std::endl << std::endl;

	Eigen::Matrix3f H = Eigen::Matrix3f::Identity();
	H.row(2) = l;

	std::cout << std::fixed << "Matrix H: : " << H << std::endl << std::endl;
		
	
	//QImage input(inputFileName.c_str());
	QImage input("G:/Projects/PerspectiveDistortionRemove/data/floor.jpg");
	
	Eigen::Vector3f img(input.width(), input.height(), 1.0f);

	float xmin = 0;
	float xmax = 0;
	float ymin = 0;
	float ymax = 0;

	computImageSize(H, 0, 0, input.width(), input.height(), xmin, xmax, ymin, ymax);
	

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
			multiplyPixelMatrix(xmin + x * dx, ymin + y * dy, H, tx, ty);

			if (tx > -1 && ty > -1
				&& tx < input.width()
				&& ty < input.height())
			{
				//if (interpolate)
				//{
				//	QRgb rgb = bilinearInterpol(input, tx, ty, dx, dy);
				//	output.setPixel(x, y, rgb);
				//}
				//else
				{
					output.setPixel(x, y, input.pixel(tx, ty));
				}
			}
		}
	}


	//output.save(outputFileName.c_str());
	output.save("G:/Projects/PerspectiveDistortionRemove/bin/Debug/_affine.png");

	return EXIT_SUCCESS;
}
