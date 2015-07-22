#ifndef _METRIC_RETIFICATION_H_
#define _METRIC_RETIFICATION_H_

#include <vector>
#include <Eigen/Dense>

class MetricRectification
{

public:
	MetricRectification(int point_count);
	virtual ~MetricRectification();

	void setVertexLine(int line_index, int vertex_index, Eigen::Vector3f v);

	void solve();
	
	void computImageSize(float in_x, float in_y, float in_width, float in_height, float& min_x, float& max_x, float& min_y, float& max_y) const;
	Eigen::Vector2f multiplyPointMatrixInverse(float x, float y) const;
	Eigen::Vector2f multiplyPointMatrix(float x, float y) const;

	Eigen::MatrixXf H() const { return h; };

	static Eigen::Vector3f normalizeLine(Eigen::Vector3f p0, Eigen::Vector3f p1);

private:

	std::vector<Eigen::Vector3f> lineVertices;
	std::vector<Eigen::Vector3f> lines;
	const int linePairsCount;

	Eigen::MatrixXf h;
};


#endif // _METRIC_RETIFICATION_H_
