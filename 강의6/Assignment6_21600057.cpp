// 21600057_Assignment6
#include "cv.hpp"
#include <iostream>
#define VIDEONAME "Road-1.mp4"

using namespace std;
using namespace cv;

void Left_show(Mat &temp);
void Right_show(Mat &temp);
void Canny_image(Mat &temp);

int main()
{
	Mat frame, canny_left, canny_right, gray_image;
	Rect left_roi(200, 400, 400, 200), right_roi(600, 400, 400, 200);
	VideoCapture cap;

	if (cap.open(VIDEONAME) == 0) // if file open error,
	{
		cout << "Cant find the file!" << endl;
		waitKey(0); // 0 means forever
	}

	while(1)
	{
		cap >> frame;

		if (frame.empty())
		{
			cout << "The End!" << endl;
			break;
		}

		else
		{
			cvtColor(frame, gray_image, CV_BGR2GRAY);

			canny_left = gray_image(left_roi);
			canny_right = gray_image(right_roi);

			Canny_image(canny_left);
			Canny_image(canny_right);

			Left_show(canny_left);
			Right_show(canny_right);

			waitKey(10);
		}
	}
	waitKey(0);
	destroyAllWindows();
}

void Left_show(Mat &temp)
{
	namedWindow("Left canny");
	moveWindow("Left canny", 200, 0);
	imshow("Left canny", temp);
}

void Right_show(Mat &temp)
{
	namedWindow("Right canny");
	moveWindow("Right canny", 600, 0);
	imshow("Right canny", temp);
}

void Canny_image(Mat &temp)
{
	Mat result = temp;
	blur(result, result, Size(5, 5));
	Canny(result, result, 10, 60, 3);
}