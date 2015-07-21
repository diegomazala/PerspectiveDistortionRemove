#ifndef _AFFINE_2_SIMILARITY_H_
#define _AFFINE_2_SIMILARITY_H_

#include <vector>
#include <Eigen/Dense>

class Affine2Similarity
{

public:
	Affine2Similarity();
	virtual ~Affine2Similarity();

	void setLinePoint(int line_index, float x0, float y0, float x1, float y1);

	void computeHMatrix();

	Eigen::Vector2f multiplyPointMatrixInverse(float x, float y) const;
	
	void computImageSize(float in_x, float in_y, float in_width, float in_height, float& min_x, float& max_x, float& min_y, float& max_y) const;

	Eigen::MatrixXf H() const { return h; };

private:
	const int lineCount;
	std::vector<Eigen::Vector3f> imagePoints;
	
	Eigen::MatrixXf A;
	Eigen::VectorXf x;
	Eigen::VectorXf b;
	Eigen::MatrixXf h;
};


#endif // _AFFINE_2_SIMILARITY_H_
