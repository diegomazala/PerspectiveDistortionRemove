
#include "Arcr.h"
#include <iostream>


Arcr::Arcr() :
	lineCount(2),
	imagePoints(lineCount * 3, Eigen::Vector3f::Zero()),
	A(3, 3),
	x(3),
	b(3),
	h(3, 3)
{
	imagePoints.resize(lineCount * 3, Eigen::Vector3f::Zero());
}


Arcr::~Arcr()
{
}



void Arcr::setLinePoint(int line_index, float x0, float y0, float x1, float y1, float x2, float y2)
{
	assert(line_index < lineCount * 3 && line_index > -1);
	imagePoints[line_index * 3 + 0] = Eigen::Vector3f(x0, y0, 1.0f);
	imagePoints[line_index * 3 + 1] = Eigen::Vector3f(x1, y1, 1.0f);
	imagePoints[line_index * 3 + 2] = Eigen::Vector3f(x2, y2, 1.0f);
}



void Arcr::computeHMatrix()
{
	for (auto p : imagePoints)
		std::cout << std::fixed << p.x() << ", " << p.y() << ", " << p.z() << std::endl;

	Eigen::MatrixXf H2x2(2, 2);
	
	
	// point 1 at infinity 
	float ia = 0.0f;
	float ib = (imagePoints[1] - imagePoints[0]).norm();
	float ic = (imagePoints[2] - imagePoints[0]).norm();

	std::cout << "\na,b,c: " << ia << ", " << ib << ", " << ic << std::endl << std::endl;

	float ratio = 1.0f;
	float wa = 0.0f;
	float wb = 1.0f;
	float wc = 1.0f + ratio;

	
#if 1
	b[0] = ia;
	b[1] = ib;
	b[2] = ic;
#else
	b[0] = wa;
	b[1] = wb;
	b[2] = wc;
#endif

	std::cout << "b:" << std::endl;
	for (int i = 0; i < b.size(); ++i)
		std::cout << std::fixed << b[i] << '\t';
	std::cout << std::endl << std::endl;

#if 1
	A(0, 0) = wa;
	A(0, 1) = 1.0f;
	A(0, 2) = -wa * ia;

	A(1, 0) = wb;
	A(1, 1) = 1.0f;
	A(1, 2) = -wb * ib;

	A(2, 0) = wc;
	A(2, 1) = 1.0f;
	A(2, 2) = -wc * ic;
#else
	A(0, 0) = ia;
	A(0, 1) = 1.0f;
	A(0, 2) = -ia * wa;

	A(1, 0) = ib;
	A(1, 1) = 1.0f;
	A(1, 2) = -ib * wb;

	A(2, 0) = ic;
	A(2, 1) = 1.0f;
	A(2, 2) = -ic * wc;
#endif

	std::cout << "Matrix A: \n" << A << std::endl << std::endl;

	x = A.colPivHouseholderQr().solve(b);				// computing vector x
	
	std::cout << "x:" << std::endl;
	for (int i = 0; i < x.size(); ++i)
		std::cout << std::fixed << x[i] << '\t';

	float relative_error = (A*x - b).norm() / b.norm(); // norm() is L2 norm
	std::cout << "\n\nThe relative error is: " << relative_error << std::endl << std::endl;

	H2x2(0, 0) = x[0];
	H2x2(0, 1) = x[1];
	H2x2(1, 0) = x[2];
	H2x2(1, 1) = 1.0f;

	Eigen::Vector2f p1inf = H2x2 * Eigen::Vector2f(1.0f, 0.0f);

	std::cout << std::fixed << "p1inf : " << p1inf.x() << ", " << p1inf.y() << std::endl << std::endl;
	p1inf.x() = p1inf.x() / p1inf.y();
	p1inf.y() = 1.0f;
	std::cout << std::fixed << "p1inf : " << p1inf.x() << ", " << p1inf.y() << std::endl;

	Eigen::Vector3f v = (imagePoints[1] - imagePoints[0]) / (imagePoints[1] - imagePoints[0]).norm();
	Eigen::Vector3f x1inf = imagePoints[0] + p1inf[0] * v;
	std::cout << std::fixed << "x1inf : " << x1inf.x() << ", " << x1inf.y() << ", " << x1inf.z() << std::endl;


	// point 2 at infinity 
	ia = 0.0f;
	ib = (imagePoints[4] - imagePoints[3]).norm();
	ic = (imagePoints[5] - imagePoints[3]).norm();

	std::cout << "\na,b,c: " << ia << ", " << ib << ", " << ic << std::endl << std::endl;


#if 1
	b[0] = ia;
	b[1] = ib;
	b[2] = ic;
#else
	b[0] = wa;
	b[1] = wb;
	b[2] = wc;
#endif

	std::cout << "b:" << std::endl;
	for (int i = 0; i < b.size(); ++i)
		std::cout << std::fixed << b[i] << '\t';
	std::cout << std::endl << std::endl;

#if 1
	A(0, 0) = wa;
	A(0, 1) = 1.0f;
	A(0, 2) = -wa * ia;

	A(1, 0) = wb;
	A(1, 1) = 1.0f;
	A(1, 2) = -wb * ib;

	A(2, 0) = wc;
	A(2, 1) = 1.0f;
	A(2, 2) = -wc * ic;
#else
	A(0, 0) = ia;
	A(0, 1) = 1.0f;
	A(0, 2) = -ia * wa;

	A(1, 0) = ib;
	A(1, 1) = 1.0f;
	A(1, 2) = -ib * wb;

	A(2, 0) = ic;
	A(2, 1) = 1.0f;
	A(2, 2) = -ic * wc;
#endif


	std::cout << "Matrix A: \n" << A << std::endl << std::endl;

	x = A.colPivHouseholderQr().solve(b);				// computing vector x
	
	std::cout << "x:" << std::endl;
	for (int i = 0; i < x.size(); ++i)
		std::cout << std::fixed << x[i] << '\t';

	relative_error = (A*x - b).norm() / b.norm(); // norm() is L2 norm
	std::cout << "\n\nThe relative error is: " << relative_error << std::endl << std::endl;

	H2x2(0, 0) = x[0];
	H2x2(0, 1) = x[1];
	H2x2(1, 0) = x[2];
	H2x2(1, 1) = 1.0f;

	
	Eigen::Vector2f p2inf = H2x2 * Eigen::Vector2f(1.0f, 0.0f);
	std::cout << std::fixed << "p2inf : " << p2inf.x() << ", " << p2inf.y() << std::endl << std::endl;
	p2inf.x() = p2inf.x() / p2inf.y();
	p2inf.y() = 1.0f;
	std::cout << std::fixed << "p2inf : " << p2inf.x() << ", " << p2inf.y() << std::endl;

	v = (imagePoints[4] - imagePoints[3]) / (imagePoints[4] - imagePoints[3]).norm();
	Eigen::Vector3f x2inf = imagePoints[3] + p2inf[0] * v;
	std::cout << std::fixed << "x2inf : " << x2inf.x() << ", " << x2inf.y() << ", " << x2inf.z() << std::endl;

	Eigen::Vector3f l = x1inf.cross(x2inf);
	l.x() = l.x() / l.z();
	l.y() = l.y() / l.z();
	l.z() = 1.0f;

	h = Eigen::Matrix3f::Identity();
	h.row(2) = l;

	std::cout << std::fixed << "line : " << l.x() << ", " << l.y() << ", " << l.z() << std::endl << std::endl;
	std::cout << std::fixed << "Matrix H: \n" << h << std::endl << std::endl;
}





Eigen::Vector2f Arcr::multiplyPointMatrixInverse(float x, float y) const
{
	Eigen::Vector3f p(x, y, 1.0f);
	Eigen::Vector3f r = h.inverse() * p;
	return Eigen::Vector2f(r.x() / r.z(), r.y() / r.z());
}


void Arcr::computImageSize(float in_x, float in_y, float in_width, float in_height, float& min_x, float& max_x, float& min_y, float& max_y) const
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

		std::cout << std::fixed
				<< "p[" << i << "] : (" << p[i].x() << ", " << p[i].y() << ")   \t" 
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


