#ifndef _METRIC_RETIFICATION_H_
#define _METRIC_RETIFICATION_H_

#include <vector>
#include <Eigen/Dense>

class MetricRectification
{

public:
	MetricRectification(int point_count);
	virtual ~MetricRectification();


	void setVertexLine(int line_index, int vertex_index, Eigen::Vector3d v);
	
	void solve();
	
	void computImageSize(double in_x, double in_y, double in_width, double in_height, double& min_x, double& max_x, double& min_y, double& max_y) const;
	Eigen::Vector2d multiplyPointMatrixInverse(double x, double y) const;
	Eigen::Vector2d multiplyPointMatrix(double x, double y) const;

	Eigen::MatrixXd H() const { return h; };

	static Eigen::Vector3d normalizeLine(Eigen::Vector3d p0, Eigen::Vector3d p1);

private:

	std::vector<Eigen::Vector3d> lineVertices;
	std::vector<Eigen::Vector3d> lines;
	const int linePairsCount;

	Eigen::MatrixXd h;

};


#endif // _METRIC_RETIFICATION_H_
