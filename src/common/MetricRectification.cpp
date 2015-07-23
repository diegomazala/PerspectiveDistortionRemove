
#include "MetricRectification.h"
#include <iostream>


MetricRectification::MetricRectification(int line_pairs_count) :
	lineVertices(line_pairs_count * 2 * 2, Eigen::Vector3d::Zero()),
	//lines(line_pairs_count * 2, std::make_pair(Eigen::Vector3d::Zero(), Eigen::Vector3d::Zero())),
	linePairsCount(line_pairs_count)
{
}


MetricRectification::~MetricRectification()
{
}


void MetricRectification::setVertexLine(int line_index, int vertex_index, Eigen::Vector3d v)
{
	assert(index < lineVertices.size() && index > -1);

	if (vertex_index == 0)
		lineVertices[line_index * 2 + 0] = v;	// line_begin
	else
		lineVertices[line_index * 2 + 1] = v;	// line_end
}


Eigen::Vector3d MetricRectification::normalizeLine(Eigen::Vector3d p0, Eigen::Vector3d p1)
{
	Eigen::Vector3d l = p0.cross(p1);
	l.x() = l.x() / l.z();
	l.y() = l.y() / l.z();
	l.z() = 1.0;
	//return l;
	return p0.cross(p1).normalized();
}



void MetricRectification::solve()
{
	std::vector<Eigen::Vector3d> vertices;
	vertices.push_back(Eigen::Vector3d(52.3467, 125.102, 1.0));
	vertices.push_back(Eigen::Vector3d(340.253, 130.147, 1.0));
	vertices.push_back(Eigen::Vector3d(193.28, 126.111, 1.0));
	vertices.push_back(Eigen::Vector3d(225.493, 360.173, 1.0));

	vertices.push_back(Eigen::Vector3d(42.28, 263.32, 1.0));
	vertices.push_back(Eigen::Vector3d(296.967, 397.502, 1.0));
	vertices.push_back(Eigen::Vector3d(212.407, 269.373, 1.0));
	vertices.push_back(Eigen::Vector3d(34.2267, 391.449, 1.0));
	
	vertices.push_back(Eigen::Vector3d(294.953, 318.809, 1.0));
	vertices.push_back(Eigen::Vector3d(456.02, 322.844, 1.0));
	vertices.push_back(Eigen::Vector3d(492.26, 208.84, 1.0));
	vertices.push_back(Eigen::Vector3d(429.847, 400.529, 1.0));

	vertices.push_back(Eigen::Vector3d(299.987, 31.2756, 1.0));
	vertices.push_back(Eigen::Vector3d(555.68, 273.409, 1.0));
	vertices.push_back(Eigen::Vector3d(545.613, 39.3467, 1.0));
	vertices.push_back(Eigen::Vector3d(236.567, 250.204, 1.0));
	
	vertices.push_back(Eigen::Vector3d(95.6333, 264.329, 1.0));
	vertices.push_back(Eigen::Vector3d(501.32, 273.409, 1.0));
	vertices.push_back(Eigen::Vector3d(302.00, 29.2578, 1.0));
	vertices.push_back(Eigen::Vector3d(297.973, 398.511, 1.0));

	lineVertices = vertices;

	for (auto pl : lineVertices)
		std::cout << pl.x() << ", " << pl.y() << ", " << pl.z() << std::endl;

	std::vector<Eigen::Vector3d> lines;
	for (int i = 0; i < lineVertices.size() - 1; i += 2)
		lines.push_back(normalizeLine(lineVertices[i], lineVertices[i + 1]));

	for (int i = 0; i < lines.size(); ++i)
		std::cout << "l" << i << " : " << lines[i].x() << ", " << lines[i].y() << ", " << lines[i].z() << std::endl;

	std::cout << std::endl << std::endl;

	//lines[0] = Eigen::Vector3d(-0.000141084, 0.00805224, -0.999968);
	//lines[1] = Eigen::Vector3d(-0.00568419, 0.000782299, 0.999984);
	//lines[2] = Eigen::Vector3d(-0.00218568, 0.00414856, -0.999989);
	//lines[3] = Eigen::Vector3d(-0.0016513, -0.00241022, 0.999996);
	//lines[4] = Eigen::Vector3d(-8.04546e-05, 0.00321109, -0.999995);
	//lines[5] = Eigen::Vector3d(-0.00178489, -0.000581155, 0.999998);
	//lines[6] = Eigen::Vector3d(-0.00374583, 0.0039556, 0.999985);
	//lines[7] = Eigen::Vector3d(-0.00165759, -0.00242947, 0.999996);
	//lines[8] = Eigen::Vector3d(-8.53647e-05, 0.00381402, -0.999993);
	//lines[9] = Eigen::Vector3d(-0.00330775, -3.60706e-05, 0.999995);


	Eigen::MatrixXd A(5, 5);
	Eigen::Vector3d l, m;
	Eigen::VectorXd b(5);

	for (int i = 0; i < 5; ++i)
	{
		l = lines[i * 2 + 0];
		m = lines[i * 2 + 1];

		A(i, 0) = l[0] * m[0];
		A(i, 1) = (l[0] * m[1] + l[1] * m[0]) / 2.0;
		A(i, 2) = l[1] * m[1];
		A(i, 3) = (l[0] * m[2] + l[2] * m[0]) / 2.0;
		A(i, 4) = (l[1] * m[2] + l[2] * m[1]) / 2.0;

		b[i] = -l[2] * m[2];
	}

	Eigen::MatrixXd x = A.colPivHouseholderQr().solve(b);

	std::cout << "A: \n" << A << std::endl << std::endl;
	std::cout << "b: \n" << b << std::endl << std::endl;
	std::cout << "x: \n" << x << std::endl << std::endl;


	Eigen::MatrixXd C(3, 3);
	C(0, 0) = x(0);
	C(0, 1) = x(1) / 2.0;
	C(0, 2) = x(3) / 2.0;
	C(1, 0) = x(1) / 2.0;
	C(1, 1) = x(2);
	C(1, 2) = x(4) / 2.0;
	C(2, 0) = x(3) / 2.0;
	C(2, 1) = x(4) / 2.0;
	C(2, 2) = 1.0;

	std::cout << "C : " << std::endl << C << std::endl << std::endl;

	Eigen::MatrixXd KKt(2, 2);
	KKt(0, 0) = C(0, 0);
	KKt(0, 1) = C(1, 0);
	KKt(1, 0) = C(0, 1);
	KKt(1, 1) = C(1, 1);

	std::cout << "KKt : " << std::endl << KKt << std::endl << std::endl;

	Eigen::MatrixXd vKKt(1, 2);
	vKKt(0, 0) = C(2, 0);
	vKKt(0, 1) = C(2, 1);

	std::cout << "vKKT : " << std::endl << vKKt << std::endl << std::endl;

	Eigen::MatrixXd V(1, 2);
	V = vKKt * KKt.inverse();

	std::cout << "V : " << std::endl << V << std::endl << std::endl;

	Eigen::LLT<Eigen::MatrixXd> llt(KKt);
	Eigen::MatrixXd K = llt.matrixU();
	Eigen::MatrixXd M(3, 3);
	M(0, 0) = K(0, 0);
	M(0, 1) = K(0, 1);
	M(0, 2) = 0;
	M(1, 0) = K(1, 0);
	M(1, 1) = K(1, 1);
	M(1, 2) = 0;
	M(2, 0) = V(0);
	M(2, 1) = V(1);
	M(2, 2) = 1;

	//M << K(0,0), K(0,1),0, K(1,0), K(1,1),0, V(0), V(1), 1;

	std::cout << "K : " << std::endl << K << std::endl << std::endl;

	h = M.inverse();
}


void MetricRectification::computImageSize(double in_x, double in_y, double in_width, double in_height, double& min_x, double& max_x, double& min_y, double& max_y) const
{
	Eigen::Vector3d p[4] = { 
		{ in_x, in_y, 1.0f }, 
		{ in_width, in_y, 1.0f }, 
		{ in_width, in_height, 1.0f }, 
		{ in_x, in_height, 1.0f } };


	Eigen::Vector3d r[4];

	for (int i = 0; i < 4; ++i)
	{
		r[i] = h.inverse() * p[i];
		r[i] = Eigen::Vector3d(r[i].x() / r[i].z(), r[i].y() / r[i].z(), 1.0f);

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



Eigen::Vector2d MetricRectification::multiplyPointMatrixInverse(double x, double y) const
{
	Eigen::Vector3d p(x, y, 1.0f);
	Eigen::Vector3d r = h.inverse() * p;
	return Eigen::Vector2d(r.x() / r.z(), r.y() / r.z());
}

Eigen::Vector2d MetricRectification::multiplyPointMatrix(double x, double y) const
{
	Eigen::Vector3d p(x, y, 1.0f);
	Eigen::Vector3d r = h * p;
	return Eigen::Vector2d(r.x() / r.z(), r.y() / r.z());
}
