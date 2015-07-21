
#include "Affine2Similarity.h"
#include <iostream>


Affine2Similarity::Affine2Similarity() :
	lineCount(4),
	imagePoints(lineCount * 2, Eigen::Vector3f::Zero()),
	A(2, 3),
	x(3),
	b(3),
	h(3, 3)
{
}


Affine2Similarity::~Affine2Similarity()
{
}



void Affine2Similarity::setLinePoint(int line_index, float x0, float y0, float x1, float y1)
{
	assert(line_index < lineCount * 2 && line_index > -1);
	imagePoints[line_index * 2 + 0] = Eigen::Vector3f(x0, y0, 1.0f);
	imagePoints[line_index * 2 + 1] = Eigen::Vector3f(x1, y1, 1.0f);
}



void Affine2Similarity::computeHMatrix()
{
	for (auto p : imagePoints)
		std::cout << std::fixed << p.x() << ", " << p.y() << ", " << p.z() << std::endl;


	Eigen::Vector3f l1 = imagePoints[0].cross(imagePoints[1]);
	Eigen::Vector3f m1 = imagePoints[2].cross(imagePoints[3]);
	Eigen::Vector3f l2 = imagePoints[4].cross(imagePoints[5]);
	Eigen::Vector3f m2 = imagePoints[6].cross(imagePoints[7]);

	l1.x() = l1.x() / l1.z();
	l1.y() = l1.y() / l1.z();
	l1.z() = 1.0f;

	m1.x() = m1.x() / m1.z();
	m1.y() = m1.y() / m1.z();
	m1.z() = 1.0f;

	l2.x() = l2.x() / l2.z();
	l2.y() = l2.y() / l2.z();
	l2.z() = 1.0f;

	m2.x() = m2.x() / m2.z();
	m2.y() = m2.y() / m2.z();
	m2.z() = 1.0f;


	b[0] = 0;
	b[1] = 0;
	b[2] = 0;

	A(0, 0) = l1.x() * m1.x();
	A(0, 1) = l1.x() * m1.y() + l1.y() * m1.x();
	A(0, 2) = l1.y() * m1.y();

	A(1, 0) = l2.x() * m2.x();
	A(1, 1) = l2.x() * m2.y() + l2.y() * m2.x();
	A(1, 2) = l2.y() * m2.y();

	x = A.colPivHouseholderQr().solve(b);				// computing vector x


	std::cout << std::fixed << "b = [0, 0, 0]" << std::endl;
	std::cout << std::fixed << "Matrix A: \n" << A << std::endl << std::endl;
	std::cout << "x:" << std::endl;
	for (int i = 0; i < x.size(); ++i)
		std::cout << std::fixed << x[i] << '\t';

	float relative_error = (A*x - b).norm() / b.norm(); // norm() is L2 norm
	std::cout << "\n\nThe relative error is: " << relative_error << std::endl << std::endl;

	std::cout << std::fixed << "l1 : " << l1.x() << ", " << l1.y() << ", " << l1.z() << std::endl;
	std::cout << std::fixed << "m1 : " << m1.x() << ", " << m1.y() << ", " << m1.z() << std::endl;
	std::cout << std::fixed << "l2 : " << l2.x() << ", " << l2.y() << ", " << l2.z() << std::endl;
	std::cout << std::fixed << "m2 : " << m2.x() << ", " << m2.y() << ", " << m2.z() << std::endl;
	std::cout << std::fixed << "Matrix H: : " << h << std::endl << std::endl;
}





Eigen::Vector2f Affine2Similarity::multiplyPointMatrixInverse(float x, float y) const
{
	Eigen::Vector3f p(x, y, 1.0f);
	Eigen::Vector3f r = h.inverse() * p;
	return Eigen::Vector2f(r.x() / r.z(), r.y() / r.z());
}


void Affine2Similarity::computImageSize(float in_x, float in_y, float in_width, float in_height, float& min_x, float& max_x, float& min_y, float& max_y) const
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


