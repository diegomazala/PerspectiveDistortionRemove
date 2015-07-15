#ifndef _ARPL_H_
#define _ARPL_H_

#include <vector>
#include <Eigen/Dense>

class Arpl
{

public:
	Arpl();
	virtual ~Arpl();

	void setLinePoint(int line_index, float x0, float y0, float x1, float y1);

	void computeHMatrix();

	Eigen::Vector2f multiplyPointMatrixInverse(float x, float y) const;
	
	void computImageSize(float in_x, float in_y, float in_width, float in_height, float& min_x, float& max_x, float& min_y, float& max_y) const;

	Eigen::MatrixXf H() const { return h; };

private:
	const int lineCount;
	std::vector<Eigen::Vector3f> imagePoints;
	Eigen::MatrixXf h;
};


#endif // _ARPL_H_
