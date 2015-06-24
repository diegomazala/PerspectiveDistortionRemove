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
	//void computImageSize(float in_width, float in_height, float& out_width, float& out_height, float& shift_x, float& shift_y) const;
	void computImageSize(float in_width, float in_height, float& min_x, float& max_x, float& min_y, float& max_y) const;

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
