// 21600057_Project2
#include "cv.hpp"
#include <iostream>
#define VIDEONAME "Road-1.mp4"

using namespace std;
using namespace cv;

void Window_show(String WName, int x, int y, Mat& temp);
void Canny_image(Mat &temp);
double Line_Filtering_Merging(Mat canny, double min_degree, double max_degree, Mat& dest, bool ck);
bool Lane_departure(Mat& dest, double x);
int putTextToM(Mat& temp, int x, int y, int count);

int main()
{
	Mat frame, canny_left, canny_right, gray_image;
	Mat Left_filtered, Right_filtered;
	Rect left_roi(200, 400, 400, 200), right_roi(600, 400, 400, 200);
	VideoCapture cap;
	double time_in_msec, fps, Left_x, Right_x;
	int count = 0; // this global variable is dpuplicate ck and display for lane departure

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

			Left_x = Line_Filtering_Merging(canny_left, 30, 60, frame, true);
			Right_x = Line_Filtering_Merging(canny_right, 120, 170, frame, false);

			if (Lane_departure(frame, Left_x)) // if true
				count = 100;

			count = putTextToM(frame, frame.cols / 2 - 100, 50, count);
			
			if (Lane_departure(frame, Right_x)) // if true
				count = 100;

			count = putTextToM(frame, frame.cols / 2 - 100, 50, count);

			//Window_show("Left canny", 200, 0, canny_left);
			//Window_show("Right canny", 600, 0, canny_right);
			//Window_show("Left_filtered", 200, 0, Left_filtered);
			//Window_show("Right_filtered", 600, 0, Right_filtered);

			imshow("Frame", frame);
			waitKey(33);
		}
	} while (time_in_msec <= 35000.0); // Display video for 0~20 seconds

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
double Line_Filtering_Merging(Mat canny, double min_degree, double max_degree, Mat& dest, bool ck)
{
	Mat result = dest;

	double x, m;
	float rho, theta, a, b, x0, y0, rsum = 0, tsum = 0, xadd, yadd;
	Point p1, p2;
	vector<Vec2f> lines;

	HoughLines(canny, lines, 1, CV_PI / 180, 60, 0, 0, min_degree * CV_PI / 180, max_degree * CV_PI / 180);
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

	// 두개의 점을 구한다. 그리고 그 점으로 직선의 방정식을 세운다.
	//cout << "첫좌표:" << x0 + xadd << "," << y0 + yadd << endl;
	//cout << "두번째 좌표:" << x0 + xadd + 300 * -b << "," << y0 + yadd + 300*a << endl;
	//circle(result, Point(x0+xadd,y0+yadd), 10, Scalar(122, 123, 47), 8, 8); // 첫 좌표
	//circle(result, Point(x0 + xadd + 300*-b, y0 + yadd + 300*a), 10, Scalar(181, 15, 236), 8, 8); // 두번째 좌표.

	// y = m(x-x1)+y1 , m = y2-y1/x2-x1
	m = (double)((y0 + yadd + 300 * a) - (y0+yadd)) / ((x0 + xadd + 300 * -b) - (x0 + xadd));
	x = (result.rows - 200 - (y0 + yadd) + m * (x0 + xadd)) / m;
	//cout << "기울기 =" << m << " x=" << x << endl;

	p1 = Point(cvRound(x0 + xadd + 2000 * (-b)), cvRound(y0 + yadd + 2000 * a));
	p2 = Point(cvRound(x0 + xadd - 2000 * (-b)), cvRound(y0 + yadd - 2000 * a));
	
	//line(result, p1, p2, Scalar(0, 0, 255), 3, 8);
	//line(result, Point((result.cols) / 2, result.rows), Point((result.cols) / 2, 0), Scalar(0, 255, 0), 3, 8); // green line
	//line(result, Point(0, result.rows-200), Point(result.cols, result.rows-200), Scalar(255, 0, 0), 3, 8); // blue line 

	return x;
}

bool Lane_departure(Mat& dest, double x)
{
	Mat result = dest;
	double center, y;
	y = result.rows - 200;
	center = result.cols / 2;

	//line(result, Point(((result.cols) / 2)-50, result.rows), Point(((result.cols) / 2)-50, 0), Scalar(255, 255, 0), 3, 8); // sky blue left line
	//line(result, Point(((result.cols) / 2)+50, result.rows), Point(((result.cols) / 2)+50, 0), Scalar(255, 255, 0), 3, 8); // sky blue right line

	//circle(result, Point(x, y), 10, Scalar(9, 221, 232), 4, 8); // 차량 앞쪽의 선이랑 detect 한 빨간선의 교점
	if (abs(center - x) < 60)
	{
		//cout << "Warning!: Lane departure" << endl;
		return true;
	}
	else // 거리 가까워지면 차선변경. 그 후 거리 적당히 멀어질때까지 디텍션하다가 false값
		return false;
}

int putTextToM(Mat& temp, int x, int y, int count)
{
	if (count == 0)
		return count;
	else
	{
		count--;
		putText(temp, "Warning!: Lane departure", Point(x, y), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2);
		return count;
	}
}