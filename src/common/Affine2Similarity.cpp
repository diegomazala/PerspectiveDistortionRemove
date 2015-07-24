
#include "Affine2Similarity.h"
#include <iostream>


Affine2Similarity::Affine2Similarity() :
	lineCount(4),
	imagePoints(lineCount * 2, Eigen::Vector3f::Zero()),
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
		std::cout << std::fixed << p.transpose() << std::endl;

	std::cout << std::endl;


	Eigen::Vector3f l, m, r1, r2;

	l = imagePoints[0].cross(imagePoints[1]);
	m = imagePoints[2].cross(imagePoints[3]);
	r1 << l(0) * m(0), l(0) * m(1) + l(1) * m(0), l(1) * m(1);

	// making homogeneous coords
	l /= l.z();
	m /= m.z();

	std::cout << std::fixed << "l : " << l.transpose() << std::endl;
	std::cout << std::fixed << "m : " << m.transpose() << std::endl;
	std::cout << std::fixed << "r : " << r1.transpose() << std::endl << std::endl;

	l = imagePoints[4].cross(imagePoints[5]);
	m = imagePoints[6].cross(imagePoints[7]);
	r2 << l(0) * m(0), l(0) * m(1) + l(1) * m(0), l(1) * m(1);

	// making homogeneous coords
	l /= l.z();
	m /= m.z();

	std::cout << std::fixed << "l : " << l.transpose() << std::endl;
	std::cout << std::fixed << "m : " << m.transpose() << std::endl;
	std::cout << std::fixed << "r : " << r1.transpose() << std::endl << std::endl;

	Eigen::MatrixXf A(2, 3);
	A << r1.transpose(), r2.transpose();

	std::cout << std::fixed << "A: \n" << A << std::endl << std::endl;

	Eigen::JacobiSVD<Eigen::MatrixXf> SVD(A, Eigen::ComputeFullV);
	Eigen::VectorXf S = SVD.matrixV().col(SVD.matrixV().cols() - 1);
	S(2) = 1;

	std::cout << std::fixed << "S: \n" << S << std::endl << std::endl;


	Eigen::MatrixXf KKt(2, 2);
	KKt << S(0), S(1), S(1), S(2);

	std::cout << std::fixed << "KKt: \n" << KKt << std::endl << std::endl;

	//Eigen::LLT<Eigen::MatrixXf> lltOfA(kkt);
	//Eigen::MatrixXf K = lltOfA.matrixU();
	Eigen::MatrixXf K = Eigen::LDLT<Eigen::MatrixXf>(KKt).matrixU();

	std::cout << std::fixed << "K: \n" << K << std::endl << std::endl;

	h << K(0), K(1), 0, K(2), K(3), 0, 0, 0, 1;

	std::cout << std::fixed << "H: : " << h << std::endl << std::endl;
}





Eigen::Vector2f Affine2Similarity::multiplyPointMatrixInverse(float x, float y) const
{
	Eigen::Vector3f p(x, y, 1.0f);
	Eigen::Vector3f r = h * p;
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
		r[i] = h.inverse() * p[i];
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


