
#include "PDR.h"
#include <iostream>


PDR::PDR(int point_count) :
	worldPoints(point_count, Eigen::Vector2f::Zero()),
	imagePoints(point_count, Eigen::Vector2f::Zero()),
	pointCount(point_count),
	A(point_count * 2, point_count * 2),
	x(point_count * 2),
	b(point_count * 2),
	h(point_count - 1, point_count - 1)
{
}


PDR::~PDR()
{
}


void PDR::setWorldPoint(int index, float x, float y)
{
	assert(index < pointCount && index > -1);
	worldPoints[index] = Eigen::Vector2f(x, y);
}


void PDR::setImagePoint(int index, float x, float y)
{
	assert(index < pointCount && index > -1);
	imagePoints[index] = Eigen::Vector2f(x, y);
}




void PDR::solve()
{
	for (int i = 0; i < pointCount; ++i)
	{
		b[i * 2 + 0] = imagePoints[i].x();
		b[i * 2 + 1] = imagePoints[i].y();
	}

	for (int p = 0; p < pointCount; ++p)
	{
		int i = p * 2 + 0;
		int j = 0;

		A(i, j++) = worldPoints[p].x();
		A(i, j++) = worldPoints[p].y();
		A(i, j++) = 1.0f;
		A(i, j++) = 0.0f;
		A(i, j++) = 0.0f;
		A(i, j++) = 0.0f;
		A(i, j++) = -worldPoints[p].x() * imagePoints[p].x();
		A(i, j++) = -worldPoints[p].y() * imagePoints[p].x();

		i = p * 2 + 1;
		j = 0;

		A(i, j++) = 0.0f;
		A(i, j++) = 0.0f;
		A(i, j++) = 0.0f;
		A(i, j++) = worldPoints[p].x();
		A(i, j++) = worldPoints[p].y();
		A(i, j++) = 1.0f;
		A(i, j++) = -worldPoints[p].x() * imagePoints[p].y();
		A(i, j++) = -worldPoints[p].y() * imagePoints[p].y();
	}
	
	std::cout << "Matrix A: \n" << A << std::endl << std::endl;

	x = A.colPivHouseholderQr().solve(b);
	std::cout << "b:" << std::endl;
	for (int i = 0; i < x.size(); ++i)
		std::cout << std::fixed << x[i] << '\t';
	float relative_error = (A*x - b).norm() / b.norm(); // norm() is L2 norm
	std::cout << "\n\nThe relative error is: " << relative_error << std::endl << std::endl;

	int rows = pointCount - 1;
	int cols = pointCount - 1;
	for (int v = 0; v < pointCount * 2; ++v)
	{
		int i = v / rows;
		int j = v % cols;

		h(i, j) = x[v];
	}
	h(rows - 1, cols - 1) = 1.0f;

	std::cout << "\n\nH:\n" << h << std::endl << std::endl;


}

void PDR::computePixel(float x, float y, float& rx, float& ry) const
{
	Eigen::Vector3f p(x, y, 1.0f);
	Eigen::Vector3f r = h.inverse() * p;
	rx = r.x() / r.z();
	ry = r.y() / r.z();
}

void PDR::computImageSize(int in_width, int in_height, int& out_width, int& out_height, int& shift_x, int& shift_y) const
{
	Eigen::Vector3f p[4] = { 
		{ 0, 0, 1.0f }, 
		{ float(in_width), 0, 1.0f }, 
		{ float(in_width), float(in_height), 1.0f }, 
		{ 0, float(in_height), 1.0f } };

	Eigen::Vector3f r[4];

	for (int i = 0; i < 4; ++i)
	{
		r[i] = h.inverse() * p[i];
		r[i] = Eigen::Vector3f(r[i].x() / r[i].z(), r[i].y() / r[i].z(), 1.0f);

		std::cout << std::fixed
				<< "p[" << i << "] : (" << p[i].x() << ", " << p[i].y() << ")  \t\t" 
				<< "r[" << i << "] : (" << r[i].x() << ", " << r[i].y() << ")" << std::endl;
	}

	float min_x = r[0].x();
	float max_x = r[0].x();

	float min_y = r[0].y();
	float max_y = r[0].y();

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


	std::cout << "\n\nmin-max : (" << min_x << "," << min_y << ") (" << max_x << ", " << max_y << ")" << std::endl;

	std::cout << "Image Size: " << max_x - min_x << ", " << max_y - min_y << std::endl;

	out_width = max_x - min_x;
	out_height = max_y - min_y;
}