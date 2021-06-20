// 21600057_Assignment7
#include "cv.hpp"
#include <iostream>
#define VIDEONAME "Road-1.mp4"

using namespace std;
using namespace cv;

void Window_show(String WName, int x, int y, Mat& temp);
void Canny_image(Mat &temp);
void Line_Filtering_Merging(Mat canny, double min_degree, double max_degree, Mat& dest, bool ck);

int main()
{
	Mat frame, canny_left, canny_right, gray_image;
	Mat Left_filtered, Right_filtered;
	Rect left_roi(200, 400, 400, 200), right_roi(600, 400, 400, 200);
	VideoCapture cap;
	double time_in_msec, fps;

	if (cap.open(VIDEONAME) == 0) // if file open error,
	{
		cout << "Cant find the file!" << endl;
		waitKey(0); // 0 means forever
	}

	fps = cap.get(CAP_PROP_FPS);

	do
	{
		cap >> frame;
		time_in_msec = cap.get(CAP_PROP_POS_MSEC);
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

			Line_Filtering_Merging(canny_left, 30, 60, frame, true);
			Line_Filtering_Merging(canny_right, 120, 150, frame, false);

			//Window_show("Left canny", 200, 0, canny_left);
			//Window_show("Right canny", 600, 0, canny_right);
			//Window_show("Left_filtered", 200, 0, Left_filtered);
			//Window_show("Right_filtered", 600, 0, Right_filtered);

			imshow("Frame", frame);
			waitKey(10);
		}
	} while (time_in_msec <= 20000.0); // Display video for 0~20 seconds

	waitKey(0);
	destroyAllWindows();
}

void Window_show(String WName, int x, int y, Mat& temp)
{
	namedWindow(WName);
	moveWindow(WName, x, y);
	imshow(WName, temp);
}

void Canny_image(Mat &temp)
{
	Mat result = temp;
	blur(result, result, Size(5, 5));
	Canny(result, result, 10, 60, 3);
}

// if ck = true, then left / false , then right
void Line_Filtering_Merging(Mat canny, double min_degree, double max_degree, Mat& dest, bool ck)
{
	Mat result = dest;

	float rho, theta, a, b, x0, y0, rsum = 0, tsum = 0, xadd, yadd;
	Point p1, p2;
	vector<Vec2f> lines;

	HoughLines(canny, lines, 1, CV_PI / 180, 150, 0, 0, min_degree * CV_PI / 180, max_degree * CV_PI / 180);
	for (int i = 0; i < lines.size(); i++)
	{
		rsum += lines[i][0];
		tsum += lines[i][1];
	}

	rho = rsum / lines.size();
	theta = tsum / lines.size();

	a = cos(theta);
	b = sin(theta);
	x0 = a * rho;
	y0 = b * rho;

	if (ck == true)
	{
		xadd = 200;
		yadd = 400;
	}
	else
	{
		xadd = 600;
		yadd = 400;
	}

	p1 = Point(cvRound(x0 + xadd + 2000 * (-b)), cvRound(y0 + yadd + 2000 * a));
	p2 = Point(cvRound(x0 + xadd - 2000 * (-b)), cvRound(y0 + yadd - 2000 * a));

	line(result, p1, p2, Scalar(0, 0, 255), 3, 8);
}