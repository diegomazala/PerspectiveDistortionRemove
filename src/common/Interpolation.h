#ifndef __INTERPOLATION_H__
#define __INTERPOLATION_H__

#include <QImage>



template <typename T>
static T Lerp(T const& x, T const& x0, T const& x1, T const& y0, T const& y1)
{
	if ((x1 - x0) == 0)
		return (y0 + y1) / 2;
	return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
}


static QRgb bilinearInterpol(const QImage& img, float x, float y, float dx, float dy)
{
	float x1 = x - dx;
	float x2 = x + dx;
	float y1 = y - dy;
	float y2 = y + dy;

	QRgb q11 = (x1 > -1 && y1 > -1) ? img.pixel(x1, y1) : img.pixel(x, y);
	QRgb q21 = (x2 < img.width() && y1 > -1) ? img.pixel(x2, y1) : img.pixel(x, y);
	QRgb q12 = (x1 > -1 && y2 < img.height()) ? img.pixel(x1, y2) : img.pixel(x, y);
	QRgb q22 = (x2 < img.width() && y2 < img.height()) ? img.pixel(x2, y2) : img.pixel(x, y);

	float q11r = float(qRed(q11)) / 255.0f;
	float q21r = float(qRed(q21)) / 255.0f;
	float q12r = float(qRed(q12)) / 255.0f;
	float q22r = float(qRed(q22)) / 255.0f;
	float r1 = ((x2 - x) / (x2 - x1))*q11r + ((x - x1) / (x2 - x1))*q21r;
	float r2 = ((x2 - x) / (x2 - x1))*q12r + ((x - x1) / (x2 - x1))*q22r;
	float red = ((y2 - y) / (y2 - y1)) * r1 + ((y - y1) / (y2 - y1)) * r2;

	float q11g = float(qGreen(q11)) / 255.0f;
	float q21g = float(qGreen(q21)) / 255.0f;
	float q12g = float(qGreen(q12)) / 255.0f;
	float q22g = float(qGreen(q22)) / 255.0f;
	float g1 = ((x2 - x) / (x2 - x1))*q11g + ((x - x1) / (x2 - x1))*q21g;
	float g2 = ((x2 - x) / (x2 - x1))*q12g + ((x - x1) / (x2 - x1))*q22g;
	float green = ((y2 - y) / (y2 - y1)) * g1 + ((y - y1) / (y2 - y1)) * g2;

	float q11b = float(qBlue(q11)) / 255.0f;
	float q21b = float(qBlue(q21)) / 255.0f;
	float q12b = float(qBlue(q12)) / 255.0f;
	float q22b = float(qBlue(q22)) / 255.0f;
	float b1 = ((x2 - x) / (x2 - x1))*q11b + ((x - x1) / (x2 - x1))*q21b;
	float b2 = ((x2 - x) / (x2 - x1))*q12b + ((x - x1) / (x2 - x1))*q22b;
	float blue = ((y2 - y) / (y2 - y1)) * b1 + ((y - y1) / (y2 - y1)) * b2;

	
	int r = red * 255.f;
	int g = green * 255.f;
	int b = blue * 255.f;

	return qRgb(r, g, b);
}

#endif // __INTERPOLATION_H__