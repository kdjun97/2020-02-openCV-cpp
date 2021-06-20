// 21600057_Assignment9
#include "cv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat ori_finger, ori_adaptive_1, ori_adaptive; //src
	Mat finger_print, adaptive_1, adaptive; //dst

	ori_finger = imread("Finger.png", IMREAD_GRAYSCALE);
	ori_adaptive_1 = imread("adaptive_1.jpg", IMREAD_GRAYSCALE);
	ori_adaptive = imread("adaptive.png", IMREAD_GRAYSCALE);

//	imshow("ori_finger", ori_finger);
//	imshow("ori_adaptive_1", ori_adaptive_1);
//	imshow("ori_adaptive", ori_adaptive);
	
	threshold(ori_finger, finger_print, 180, 255, THRESH_BINARY);
	adaptiveThreshold(ori_adaptive_1, adaptive_1, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 85, 15);
	adaptiveThreshold(ori_adaptive, adaptive, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 10);

	imshow("finger_print", finger_print);
	imshow("adaptive_1", adaptive_1);
	imshow("adaptive", adaptive);

	waitKey(0);
	
	return 0;
}