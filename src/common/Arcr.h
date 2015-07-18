#ifndef _ARCR_H_
#define _ARCR_H_

#include <vector>
#include <Eigen/Dense>

class Arcr
{

public:
	Arcr();
	virtual ~Arcr();

	void setLinePoint(int line_index, float x0, float y0, float x1, float y1, float x2, float y2);
	void setRatio(int line_index, float ratio);

	void computeHMatrix();

	Eigen::Vector2f multiplyPointMatrixInverse(float x, float y) const;
	
	void computImageSize(float in_x, float in_y, float in_width, float in_height, float& min_x, float& max_x, float& min_y, float& max_y) const;

	Eigen::MatrixXf H() const { return h; };

private:
	const int lineCount;
	std::vector<Eigen::Vector3f> imagePoints;
	std::vector<float> ratios;
	
	Eigen::MatrixXf A;
	Eigen::VectorXf x;
	Eigen::VectorXf b;
	Eigen::MatrixXf h;

};


#endif // _ARCR_H_
