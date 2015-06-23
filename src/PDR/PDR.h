#ifndef _PDR_H_
#define _PDR_H_

#include <vector>
#include <Eigen/Dense>

class PDR
{

public:
	PDR(int point_count);
	virtual ~PDR();

	void setWorldPoint(int index, float x, float y);
	void setImagePoint(int index, float x, float y);

	void solve();
	void computePixel(float x, float y, float& rx, float& ry) const;
	void computImageSize(int in_width, int in_height, int& out_width, int& out_height, int& shift_x, int& shift_y) const;

	Eigen::MatrixXf hInverse() const { return h.inverse(); };

private:

	std::vector<Eigen::Vector2f> worldPoints;
	std::vector<Eigen::Vector2f> imagePoints;
	const int pointCount;

	Eigen::MatrixXf A;
	Eigen::VectorXf x;
	Eigen::VectorXf b;
	
	Eigen::MatrixXf h;
};


#endif // _PDR_H_
