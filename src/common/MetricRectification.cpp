
#include "MetricRectification.h"
#include <iostream>


MetricRectification::MetricRectification(int line_pairs_count) :
	lineVertices(line_pairs_count * 2 * 2, Eigen::Vector3f::Zero()),
	//lines(line_pairs_count * 2, std::make_pair(Eigen::Vector3f::Zero(), Eigen::Vector3f::Zero())),
	linePairsCount(line_pairs_count)
{
}


MetricRectification::~MetricRectification()
{
}

void MetricRectification::setVertexLine(int line_index, int vertex_index, Eigen::Vector3f v)
{
	assert(index < lineVertices.size() && index > -1);

	if (vertex_index == 0)
		lineVertices[line_index * 2 + 0] = v;	// line_begin
	else
		lineVertices[line_index * 2 + 1] = v;	// line_end
}


Eigen::Vector3f MetricRectification::normalizeLine(Eigen::Vector3f p0, Eigen::Vector3f p1)
{
	Eigen::Vector3f l = p0.cross(p1);
	l.x() = l.x() / l.z();
	l.y() = l.y() / l.z();
	l.z() = 1.0f;
	//return l;
	return p0.cross(p1).normalized();
}



void MetricRectification::solve()
{
	std::vector<Eigen::Vector3f> vertices;
	vertices.push_back(Eigen::Vector3f(52.3467f, 125.102f, 1.0f));
	vertices.push_back(Eigen::Vector3f(340.253f, 130.147f, 1.0f));
	vertices.push_back(Eigen::Vector3f(193.28f, 126.111f, 1.0f));
	vertices.push_back(Eigen::Vector3f(225.493f, 360.173f, 1.0f));
	vertices.push_back(Eigen::Vector3f(42.28f, 263.32f, 1.0f));
	vertices.push_back(Eigen::Vector3f(296.967f, 397.502f, 1.0f));
	vertices.push_back(Eigen::Vector3f(212.407f, 269.373f, 1.0f));
	vertices.push_back(Eigen::Vector3f(34.2267f, 391.449f, 1.0f));
	vertices.push_back(Eigen::Vector3f(294.953f, 318.809f, 1.0f));
	vertices.push_back(Eigen::Vector3f(456.02f, 322.844f, 1.0f));
	vertices.push_back(Eigen::Vector3f(492.26f, 208.84f, 1.0f));
	vertices.push_back(Eigen::Vector3f(429.847f, 400.529f, 1.0f));
	vertices.push_back(Eigen::Vector3f(299.987f, 31.2756f, 1.0f));
	vertices.push_back(Eigen::Vector3f(555.68f, 273.409f, 1.0f));
	vertices.push_back(Eigen::Vector3f(545.613f, 39.3467f, 1.0f));
	vertices.push_back(Eigen::Vector3f(236.567f, 250.204f, 1.0f));
	vertices.push_back(Eigen::Vector3f(95.6333f, 264.329f, 1.0f));
	vertices.push_back(Eigen::Vector3f(501.32f, 273.409f, 1.0f));
	vertices.push_back(Eigen::Vector3f(302.00f, 29.2578f, 1.0f));
	vertices.push_back(Eigen::Vector3f(297.973f, 398.511f, 1.0f));

	lineVertices = vertices;

	for (auto pl : lineVertices)
		std::cout << pl.x() << ", " << pl.y() << ", " << pl.z() << std::endl;

	//lines.clear();
	//for (int i = 0; i < lineVertices.size() - 1; i += 2)
	//	lines.push_back(normalizeLine(lineVertices[i], lineVertices[i + 1]));
	//lines.resize(10);
	//lines[0] = Eigen::Vector3f(-0.000141084, 0.00805224, -0.999968);
	//lines[1] = Eigen::Vector3f(-0.00568419, 0.000782299, 0.999984);
	//lines[2] = Eigen::Vector3f(-0.00218568, 0.00414856, -0.999989);
	//lines[3] = Eigen::Vector3f(-0.0016513, -0.00241022, 0.999996);
	//lines[4] = Eigen::Vector3f(-8.04546e-05, 0.00321109, -0.999995);
	//lines[5] = Eigen::Vector3f(-0.00178489, -0.000581155, 0.999998);
	//lines[6] = Eigen::Vector3f(-0.00374583, 0.0039556, 0.999985);
	//lines[7] = Eigen::Vector3f(-0.00165759, -0.00242947, 0.999996);
	//lines[8] = Eigen::Vector3f(-8.53647e-05, 0.00381402, -0.999993);
	//lines[9] = Eigen::Vector3f(-0.00330775, -3.60706e-05, 0.999995);


	std::vector<Eigen::Vector3f> lines;
	for (int i = 0; i < vertices.size() - 1; i += 2)
		lines.push_back(normalizeLine(lineVertices[i], lineVertices[i + 1]));

	for (int i = 0; i < lines.size(); ++i)
		std::cout << "l" << i << " : " << lines[i].x() << ", " << lines[i].y() << ", " << lines[i].z() << std::endl;
	

	Eigen::MatrixXf A(5, 5);
	Eigen::Vector3f l, m;
	Eigen::VectorXf b(5);

	for (int i = 0; i < 5; ++i)
	{
		l = lines[i * 2 + 0];
		m = lines[i * 2 + 1];

		A(i, 0) = l[0] * m[0];
		A(i, 1) = (l[0] * m[1] + l[1] * m[0]) / 2.0f;
		A(i, 2) = l[1] * m[1];
		A(i, 3) = (l[0] * m[2] + l[2] * m[0]) / 2.0f;
		A(i, 4) = (l[1] * m[2] + l[2] * m[1]) / 2.0f;
		A(i, 5) = l[2] * m[2];

		b[i] = -l[2] * m[2];
	}

	std::cout << "A: \n" << A << std::endl << std::endl;
	std::cout << "b: \n" << b << std::endl << std::endl;

	Eigen::MatrixXf x = A.colPivHouseholderQr().solve(b);
	std::cout << std::fixed << "x: \n" << x << std::endl << std::endl;


	Eigen::MatrixXf conic(3, 3);
	conic(0, 0) = x(0);
	conic(0, 1) = x(1) / 2.0f;
	conic(0, 2) = x(3) / 2.0f;
	conic(1, 0) = x(1) / 2.0f;
	conic(1, 1) = x(2);
	conic(1, 2) = x(4) / 2.0f;
	conic(2, 0) = x(3) / 2.0f;
	conic(2, 1) = x(4) / 2.0f;
	conic(2, 2) = 1.0f;
	std::cout << "Conic : " << std::endl << conic << std::endl << std::endl;

	Eigen::JacobiSVD<Eigen::MatrixXf> svd(conic, Eigen::ComputeFullU);
	h = svd.matrixU();

	std::cout << "H : " << std::endl
		<< h << std::endl << std::endl
		<< "Singular values: \n" << svd.singularValues()
		<< std::endl << std::endl;
}


void MetricRectification::computImageSize(float in_x, float in_y, float in_width, float in_height, float& min_x, float& max_x, float& min_y, float& max_y) const
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



Eigen::Vector2f MetricRectification::multiplyPointMatrixInverse(float x, float y) const
{
	Eigen::Vector3f p(x, y, 1.0f);
	Eigen::Vector3f r = h.inverse() * p;
	return Eigen::Vector2f(r.x() / r.z(), r.y() / r.z());
}

Eigen::Vector2f MetricRectification::multiplyPointMatrix(float x, float y) const
{
	Eigen::Vector3f p(x, y, 1.0f);
	Eigen::Vector3f r = h * p;
	return Eigen::Vector2f(r.x() / r.z(), r.y() / r.z());
}