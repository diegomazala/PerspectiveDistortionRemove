
#include "Arpl.h"
#include <iostream>


Arpl::Arpl() :
	lineCount(4),
	imagePoints(lineCount * 2, Eigen::Vector3f::Zero()),
	h(3, 3)
{
}


Arpl::~Arpl()
{
}



void Arpl::setLinePoint(int line_index, float x0, float y0, float x1, float y1)
{
	assert(line_index < lineCount * 2 && line_index > -1);
	imagePoints[line_index * 2 + 0] = Eigen::Vector3f(x0, y0, 1.0f);
	imagePoints[line_index * 2 + 1] = Eigen::Vector3f(x1, y1, 1.0f);
}



void Arpl::computeHMatrix()
{
	Eigen::Vector3f l0 = imagePoints[0].cross(imagePoints[1]);
	Eigen::Vector3f l1 = imagePoints[2].cross(imagePoints[3]);
	Eigen::Vector3f l2 = imagePoints[0].cross(imagePoints[2]);
	Eigen::Vector3f l3 = imagePoints[1].cross(imagePoints[3]);

	Eigen::Vector3f x0 = l0.cross(l1);
	Eigen::Vector3f x1 = l2.cross(l3);

	Eigen::Vector3f l = x1.cross(x0);
	l.x() = l.x() / l.z();
	l.y() = l.y() / l.z();
	l.z() = 1.0f;

	h = Eigen::Matrix3f::Identity();
	h.row(2) = l;

	std::cout << std::fixed << "l0 : " << l0.x() << ", " << l0.y() << ", " << l0.z() << std::endl;
	std::cout << std::fixed << "l1 : " << l1.x() << ", " << l1.y() << ", " << l1.z() << std::endl;
	std::cout << std::fixed << "l2 : " << l2.x() << ", " << l2.y() << ", " << l2.z() << std::endl;
	std::cout << std::fixed << "l3 : " << l3.x() << ", " << l3.y() << ", " << l3.z() << std::endl;
	std::cout << std::fixed << "x0 : " << x0.x() << ", " << x0.y() << ", " << x0.z() << std::endl;
	std::cout << std::fixed << "x1 : " << x1.x() << ", " << x1.y() << ", " << x1.z() << std::endl;
	std::cout << std::fixed << "line : " << l.x() << ", " << l.y() << ", " << l.z() << std::endl << std::endl;
	std::cout << std::fixed << "Matrix H: : " << h << std::endl << std::endl;
}





Eigen::Vector2f Arpl::multiplyPointMatrixInverse(float x, float y) const
{
	Eigen::Vector3f p(x, y, 1.0f);
	Eigen::Vector3f r = h.inverse() * p;
	return Eigen::Vector2f(r.x() / r.z(), r.y() / r.z());
}


void Arpl::computImageSize(float in_x, float in_y, float in_width, float in_height, float& min_x, float& max_x, float& min_y, float& max_y) const
{
	Eigen::Vector3f p[4] = { 
		{ in_x, in_y, 1.0f }, 
		{ in_width, in_y, 1.0f }, 
		{ in_width, in_height, 1.0f }, 
		{ in_x, in_height, 1.0f } };


	Eigen::Vector3f r[4];

	for (int i = 0; i < 4; ++i)
	{
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


