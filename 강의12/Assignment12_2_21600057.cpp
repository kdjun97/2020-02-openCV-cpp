// 21600057_Assignment12-2
#include "cv.hpp"
#include <iostream>
#define VIDEONAME "background.mp4"

using namespace std;
using namespace cv;

void DrawObject(vector<Rect> boundRect, vector<vector<Point>> contours, Mat result, Mat original);
bool calculate(int x1, int y1, int x2, int y2);
int absoulte(int a);
void Detect_Object(Mat frame, Mat background);
void PutText_Rect_Num(Mat result, int count);
Mat MorphologyCal(Mat result);

int main()
{
	Mat frame, background;
	VideoCapture cap(VIDEONAME);

	cap >> frame;
	cvtColor(frame, background, CV_BGR2GRAY);

	while (1)
	{
		cap >> frame;

		if (frame.empty()) // empty == true , In other words, frame is empty
		{
			cout << "The End!" << endl;
			break;
		}

		else
			Detect_Object(frame, background);
	}
	waitKey(0);
	destroyAllWindows();

	return 0;
}


void DrawObject(vector<Rect> boundRect, vector<vector<Point>> contours, Mat result , Mat original)
{
	int count = 0;
	bool True_False;

	for (int i = 0; i < contours.size(); i++)
		boundRect[i] = boundingRect(Mat(contours[i]));

	//draw rectangles over 700 pixels
	for (int i = 0; i < contours.size(); i++)
	{
		True_False = calculate(boundRect[i].tl().x, boundRect[i].tl().y, boundRect[i].br().x, boundRect[i].br().y);
		if (True_False)
		{
			if (absoulte(boundRect[i].tl().y - boundRect[i].br().y) > 250) 
				count++;
			
			if (absoulte(boundRect[i].tl().x - boundRect[i].br().x) > 190)
				count++;
			
			//rectangle(result, boundRect[i].tl(), boundRect[i].br(), Scalar(255, 255, 255), 2, 8, 0);
			count++;
		}
	}
	PutText_Rect_Num(original, count);
	imshow("Number of people", original);
	waitKey(33);
}

bool calculate(int x1, int y1, int x2, int y2)
{
	int x, y;
	x = absoulte(x1 - x2);
	y = absoulte(y1 - y2);
	if ((x*y) >= 700)
		return true;
	else
		return false;
}

int absoulte(int a)
{
	if (a < 0)
		return a *= -1;
	else
		return a;
}

void Detect_Object(Mat frame, Mat background)
{
	Mat foregroundMask, foregroundImg, result, original;
	vector<vector<Point>> contours;
	vector<Vec4i>hierarchy;

	original = frame;

	cvtColor(frame, frame, CV_BGR2GRAY);
	absdiff(frame, background, foregroundMask); // |current_frame - Background| = foregroundMask
	threshold(foregroundMask, foregroundMask, 50, 255, CV_THRESH_BINARY);

	//	foregroundMask.copyTo(foregroundImg);
	//	frame.copyTo(foregroundImg, foregroundMask);

	//	result = foregroundImg.clone();
	result = foregroundMask.clone();

	//imshow("기존의 것", result);

	result = MorphologyCal(result);

	waitKey(10);


	findContours(result, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	//	cvtColor(result, result, COLOR_GRAY2BGR);

	vector<Rect> boundRect(contours.size());
	//DrawObject(boundRect, contours, frame);
	DrawObject(boundRect, contours, frame, original);

}

void PutText_Rect_Num(Mat result, int count)
{
	string text = "# People: " + to_string(count);
	putText(result, text, Point(10, 40), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(255, 255, 255), 2);
}

Mat MorphologyCal(Mat result)
{
	Mat temp = result;
	Mat element = getStructuringElement(MORPH_RECT, Size(8, 8));

	for (int i = 0; i < 4; i++)
		dilate(temp, temp, element);

	for (int i = 0; i < 4; i++)
		erode(temp, temp, element);

	morphologyEx(temp, temp, MORPH_CLOSE, element);

	return temp;
}