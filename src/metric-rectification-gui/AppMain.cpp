#include <QApplication>
#include "MainWindow.h"
#include "GLLines.h"
#include <Eigen/Dense>
#include <iostream>

Eigen::Vector3f lineNormalized(Eigen::Vector3f p0, Eigen::Vector3f p1)
{
	Eigen::Vector3f l = p0.cross(p1);
	l.x() = l.x() / l.z();
	l.y() = l.y() / l.z();
	l.z() = 1.0f;
	//return l;
	return p0.cross(p1).normalized();
}

int main(int argc, char* argv[])
{
#if 0
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	

	GLLines* gllines = &w.getGLWidget()->glLines();


	gllines->setVertexLine(0, 0, QVector3D(52.3467f, 125.102f, 1.0f));
	gllines->setVertexLine(0, 1, QVector3D(340.253f, 130.147f, 1.0f));
	gllines->setVertexLine(1, 0, QVector3D(193.28f, 126.111f, 1.0f));
	gllines->setVertexLine(1, 1, QVector3D(225.493f, 360.173f, 1.0f));
	gllines->setVertexLine(2, 0, QVector3D(42.28f, 263.32f, 1.0f));
	gllines->setVertexLine(2, 1, QVector3D(296.967f, 397.502f, 1.0f));
	gllines->setVertexLine(3, 0, QVector3D(212.407f, 269.373f, 1.0f));
	gllines->setVertexLine(3, 1, QVector3D(34.2267f, 391.449f, 1.0f));
	gllines->setVertexLine(4, 0, QVector3D(294.953f, 318.809f, 1.0f));
	gllines->setVertexLine(4, 1, QVector3D(456.02f, 322.844f, 1.0f));
	gllines->setVertexLine(5, 0, QVector3D(492.26f, 208.84f, 1.0f));
	gllines->setVertexLine(5, 1, QVector3D(429.847f, 400.529f, 1.0f));
	gllines->setVertexLine(6, 0, QVector3D(299.987f, 31.2756f, 1.0f));
	gllines->setVertexLine(6, 1, QVector3D(555.68f, 273.409f, 1.0f));
	gllines->setVertexLine(7, 0, QVector3D(545.613f, 39.3467f, 1.0f));
	gllines->setVertexLine(7, 1, QVector3D(236.567f, 250.204f, 1.0f));
	gllines->setVertexLine(8, 0, QVector3D(95.6333f, 264.329f, 1.0f));
	gllines->setVertexLine(8, 1, QVector3D(501.32f, 273.409f, 1.0f));
	gllines->setVertexLine(9, 0, QVector3D(302.00f, 29.2578f, 1.0f));
	gllines->setVertexLine(9, 1, QVector3D(297.973f, 398.511f, 1.0f));
	gllines->computeCanonicalVertices(w.getGLWidget()->width(), w.getGLWidget()->height());
	gllines->onEnable(true);

	return a.exec();
#endif
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


	std::cout << vertices.size() << std::endl;

	std::vector<Eigen::Vector3f> lines;
	for (int i = 0; i < vertices.size() - 1; i += 2)
		lines.push_back(lineNormalized(vertices[i], vertices[i + 1]));
	
	for (int i = 0; i < lines.size(); ++i)
		std::cout << "l" << i << " : " << lines[i].x() << ", " << lines[i].y() << ", " << lines[i].z() << std::endl;

	std::cout << std::endl << std::endl;

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



	Eigen::MatrixXf A(6, 6);
	Eigen::Vector3f l, m;

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
	}
	A.row(5) << 0, 0, 0, 0, 0, 0;

	std::cout << A << std::endl << std::endl;

	Eigen::MatrixXf x = A.fullPivLu().kernel();
	std::cout << "x: ";
	for (int i = 0; i < x.size(); ++i)
		std::cout << std::fixed << x.data()[i] << '\t';
	//std::cout << std::endl << std::endl;
	return EXIT_SUCCESS;
}
