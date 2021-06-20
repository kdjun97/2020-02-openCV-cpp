// 21600057_Project1
#include "cv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void Reset_for_first(Mat &a, Mat &b, Mat &c);
void Display_image(Mat &a, Mat &b, Mat &c);
void Negative_transformation(Mat &temp);
void Gamma_transformation(Mat &temp);
void Hist_equalization(Mat &temp);
void Color_slicing(Mat &temp);
void Color_conversion(Mat &temp);
void Averaging_filter(Mat &temp);
void White_balancing(Mat &temp);

int main(void)
{
	int key = 0;

	Mat lena, colorful, balancing;
	
	Reset_for_first(lena, colorful, balancing);
	Display_image(lena, colorful, balancing);

	while(1)
	{
		key = waitKey(0);

		switch (key)
		{
		case 114: // pressed key : 'r'
			Reset_for_first(lena, colorful, balancing);
			Display_image(lena, colorful, balancing);
			break;
		case 110: // pressed key : 'n'
			Negative_transformation(lena);
			Display_image(lena, colorful, balancing);
			break;
		case 103: // pressed key : 'g'
			Gamma_transformation(lena);
			Display_image(lena, colorful, balancing);
			break;
		case 104: // pressed key : 'h'
			Hist_equalization(lena);
			Display_image(lena, colorful, balancing);
			break;
		case 115: // pressed key : 's'
			Color_slicing(colorful);
			Display_image(lena, colorful, balancing);
			break;
		case 99: // pressed key : 'c'
			Color_conversion(colorful);
			Display_image(lena, colorful, balancing);
			break;
		case 97: // pressed key : 'a'
			Averaging_filter(balancing);
			Display_image(lena, colorful, balancing);
			break;
		case 119: // pressed key : 'w'
			White_balancing(balancing);
			Display_image(lena, colorful, balancing);
			break;
		default:
			break;
		}
	}

}

void Reset_for_first(Mat &a, Mat &b, Mat &c)
{
	a = imread("lena.png", IMREAD_COLOR);
	b = imread("colorful.jpg", IMREAD_COLOR);
	c = imread("balancing.jpg", IMREAD_COLOR);
}

void Display_image(Mat &a, Mat &b, Mat &c)
{
	imshow("lena", a);
	imshow("colorful", b);
	imshow("balancing", c);
}

void Negative_transformation(Mat &temp)
{
	Mat temporary, mat_split[3];
	cvtColor(temp, temporary, CV_BGR2HSV);
	split(temporary, mat_split);

	for (int i = 0; i < mat_split[2].rows; i++)
	{
		for (int j = 0; j < mat_split[2].cols; j++)
			mat_split[2].at<uchar>(i, j) = 255 - mat_split[2].at<uchar>(i, j);
	}
	merge(mat_split, 3, temporary);
	cvtColor(temporary, temporary, CV_HSV2BGR);
	temp = temporary;
}

void Gamma_transformation(Mat &temp)
{
	float gamma = 2.5; // the value is const

	Mat temporary, mat_split[3];
	cvtColor(temp, temporary, CV_BGR2HSV);

	split(temporary, mat_split);
	mat_split[2].convertTo(mat_split[2], CV_32F); // for float

	normalize(mat_split[2], mat_split[2], 0.0, 1.0, NORM_MINMAX); // pixel range 0~1.0
	for (int i = 0; i < mat_split[2].rows; i++)
	{
		for (int j = 0; j < mat_split[2].cols; j++)
			mat_split[2].at<float>(i, j) = (float)pow((float)mat_split[2].at<float>(i, j), gamma);
	}
	normalize(mat_split[2], mat_split[2], 0, 255, NORM_MINMAX); // pixel range 0~255
	mat_split[2].convertTo(mat_split[2], CV_8U); // conver to unsigned 8 bit

	merge(mat_split, 3, temporary);
	cvtColor(temporary, temporary, CV_HSV2BGR);
	temp = temporary;
}

void Hist_equalization(Mat &temp)
{
	Mat temporary, mat_split[3];
	cvtColor(temp, temporary, CV_BGR2HSV);
	split(temporary, mat_split);

	equalizeHist(mat_split[2], mat_split[2]);

	merge(mat_split, 3, temporary);
	cvtColor(temporary, temporary, CV_HSV2BGR);
	temp = temporary;
}

void Color_slicing(Mat &temp)
{
	Mat temporary, mat_split[3];
	cvtColor(temp, temporary, CV_BGR2HSV);
	split(temporary, mat_split);

	for (int i = 0; i < mat_split[0].rows; i++)
	{
		for (int j = 0; j < mat_split[0].cols; j++)
			if (!(mat_split[0].at<uchar>(i, j) > 9 && mat_split[0].at<uchar>(i, j) < 23))
				mat_split[1].at<uchar>(i,j) = 0;
	}
	merge(mat_split, 3, temporary);
	cvtColor(temporary, temporary, CV_HSV2BGR);
	temp = temporary;
}

void Color_conversion(Mat &temp)
{
	Mat temporary, mat_split[3];
	cvtColor(temp, temporary, CV_BGR2HSV);
	split(temporary, mat_split);

	for (int i = 0; i < mat_split[0].rows; i++)
	{
		for (int j = 0; j < mat_split[0].cols; j++)
		{
			mat_split[0].at<uchar>(i, j) += 50; // increase Hue value by 50
			
			if (mat_split[0].at<uchar>(i, j) > 129)
				mat_split[0].at<uchar>(i, j) -= 129;
		}
	}
	merge(mat_split, 3, temporary);
	cvtColor(temporary, temporary, CV_HSV2BGR);
	temp = temporary;
}

void Averaging_filter(Mat &temp)
{
	Rect roi(0, 0, temp.cols, temp.rows);
	blur(temp(roi), temp(roi), Size(9, 9));
}

void White_balancing(Mat &temp)
{
	Mat temporary = temp, mat_split[3];
	int row = temporary.rows, col = temporary.cols;
	split(temporary, mat_split);

	int b=0, g=0, r=0, ave_b, ave_g, ave_r;

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			b += mat_split[0].at<uchar>(i, j);
			g += mat_split[1].at<uchar>(i, j);
			r += mat_split[2].at<uchar>(i, j);
		}
	} // sum

	ave_b = b / (row * col);
	ave_g = g / (row * col);
	ave_r = r / (row * col); // these values assumption gray by 128

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (((128 * mat_split[0].at<uchar>(i, j)) / ave_b) >= 255)
				mat_split[0].at<uchar>(i, j) = 255; // white
			else
				mat_split[0].at<uchar>(i, j) = (128 * mat_split[0].at<uchar>(i, j)) / ave_b;

			if (((128 * mat_split[1].at<uchar>(i, j)) / ave_g) >= 255)
				mat_split[1].at<uchar>(i, j) = 255; // white
			else
				mat_split[1].at<uchar>(i, j) = (128 * mat_split[1].at<uchar>(i, j)) / ave_g;

			if (((128 * mat_split[2].at<uchar>(i, j)) / ave_r) >= 255)
				mat_split[2].at<uchar>(i, j) = 255; // white
			else
				mat_split[2].at<uchar>(i, j) = (128 * mat_split[2].at<uchar>(i, j)) / ave_r;
		}
	} // calculate

	merge(mat_split, 3, temporary);
	temp = temporary;
}