// 21600057_Assignment2
#include "cv.hpp"
#include <iostream>
#include <cmath> // for pow 

using namespace std;
using namespace cv;

Mat NegativeTrans(Mat temp);
Mat LogTrans(Mat temp);
Mat GammaTrans(Mat temp);

int main()
{
	Mat img;
	Mat negative_img, log_img, gamma_img;

	img = imread("lena.png", IMREAD_GRAYSCALE); // read gray scale
	imshow("input image", img);

	negative_img = NegativeTrans(img);
	log_img = LogTrans(img);
	gamma_img = GammaTrans(img);

	imshow("Negative transformation", negative_img);
	imshow("Log transformation", log_img);
	imshow("Gamma transformation", gamma_img);

	waitKey(0);
	destroyAllWindows();
}

Mat NegativeTrans(Mat temp)
{
	Mat result = temp.clone();
	for (int i = 0; i < temp.rows; i++)
	{
		for (int j = 0; j < temp.cols; j++)
			result.at<uchar>(i, j) = 255 - temp.at<uchar>(i, j);
	}
	return result;
}

Mat LogTrans(Mat temp)
{
	Mat result, temporary;
	float scale = 1.5; // we can use any const value

	temp.convertTo(temporary, CV_32F); // for float
	temp.convertTo(result, CV_32F);

	for (int i = 0; i < temp.rows; i++)
	{
		for (int j = 0; j < temp.cols; j++)
			result.at<float>(i, j) = log(1 + temporary.at<float>(i, j)); // s = clog(1+r)
	}
	normalize(result, result, 0, 255, NORM_MINMAX); // pixel range 0~255
	convertScaleAbs(result, result, scale); // all index * scale and convert type U8

	return result;
}

Mat GammaTrans(Mat temp)
{
	Mat result, temporary;
	float gamma = 0.5; // the value is const

	temp.convertTo(temporary, CV_32F); // for float
	temp.convertTo(result, CV_32F);

	normalize(temporary, temporary, 0.0, 1.0, NORM_MINMAX); // pixel range 0~1.0

	for (int i = 0; i < temp.rows; i++)
	{
		for (int j = 0; j < temp.cols; j++)
			result.at<float>(i, j) = (float)pow((float)temporary.at<float>(i, j), gamma);
	}

	normalize(result, result, 0, 255, NORM_MINMAX); // pixel range 0~255
	result.convertTo(result, CV_8U); // conver to unsigned 8 bit
	return result;
}