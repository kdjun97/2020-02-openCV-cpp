// 21600057_Assignment3
#include "cv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

Mat LenaFilter(Mat temp);
Mat MoonFilter(Mat temp);
Mat SaltnpepperFilter(Mat temp);

int main()
{
	Mat lena, lena_filtered, moon, moon_filtered, saltnpepper, saltnpepper_filtered;
	lena = imread("lena.png", IMREAD_GRAYSCALE); // read gray scale
	imshow("lena", lena);

	lena_filtered = LenaFilter(lena);
	imshow("lena_filtered", lena_filtered);

	moon = imread("moon.jpg", IMREAD_GRAYSCALE);
	imshow("moon", moon);

	moon_filtered = MoonFilter(moon);
	imshow("moon_filtered", moon_filtered);

	saltnpepper = imread("saltnpepper.png", IMREAD_GRAYSCALE);
	imshow("saltnpepper", saltnpepper);

	saltnpepper_filtered = SaltnpepperFilter(saltnpepper);
	imshow("saltnpepper_filtered", saltnpepper_filtered);

	waitKey(0);
	destroyAllWindows();
}

Mat LenaFilter(Mat temp)
{
	Mat result = temp; // Shallow copy.

	Rect left_roi(0, 0, result.cols / 2, result.rows);
	Mat left = result(left_roi); // roi setting (left)

	blur(left, left, Size(7, 7));

	return result;
}

Mat MoonFilter(Mat temp)
{
	Mat temporary = temp.clone();
	Mat result = temp; // Shallow copy
	Mat lap;

	Rect right1_roi(temporary.cols / 2, 0, temporary.cols / 2, temporary.rows);
	Mat right1 = temporary(right1_roi); // roi settting (origianl right part)

	Rect right2_roi(result.cols / 2, 0, result.cols / 2, result.rows);
	Mat right2 = result(right2_roi); // roi settting

	Laplacian(right2, lap, CV_16S, 1);
	lap.convertTo(right2, CV_8U);

	right2 += right1;

	return result;
}

Mat SaltnpepperFilter(Mat temp)
{
	Mat result;

	medianBlur(temp, result, 9);

	return result;
}