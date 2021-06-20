// 21600057_Assignment12-1
#include "cv.hpp"
#include <iostream>

#define VIDEONAME_A "Faces.mp4"
#define VIDEONAME_B "background.mp4"

using namespace std;
using namespace cv;

void FaceDectect(Mat frame, char key);
void Draw_Rect_To_Face(vector<Rect> faces,Mat frame, char key);
int abs_minus_cal(int a, int b);

CascadeClassifier face_classifier;

int main()
{
	Mat frame, gray_frame;
	VideoCapture cap(VIDEONAME_A);
	
	if (!face_classifier.load("C:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml"))
		printf("Error!\n");

	int key = 0;

	while (1)
	{
		cap >> frame;

		if (frame.empty()) // empty == true , In other words, frame is empty
		{
			cout << "The End!" << endl;
			break;
		}

		else
		{
			key = waitKey(33);
			switch (key)
			{
			case 110: // pressed key : 'n'
				FaceDectect(frame,'n');
				break;
			case 102: // pressed key : 'f'
				FaceDectect(frame, 'f');
				break;
			case 109: // pressed key : 'm'
				FaceDectect(frame, 'm');
				break;
			default:
				break;
			}

			resize(frame, frame, Size(400, 300), INTER_LINEAR);
			imshow("Faces", frame);
		}
		
	}
	waitKey(0);
	destroyAllWindows();

	return 0;
}

void FaceDectect(Mat frame, char key)
{
	Mat gray_frame;
	vector <Rect> faces;

	cvtColor(frame, gray_frame, COLOR_BGR2GRAY);

	face_classifier.detectMultiScale(gray_frame, faces, 1.1, 3, 0, Size(10, 10));
	Draw_Rect_To_Face(faces, frame,  key);
}

void Draw_Rect_To_Face(vector<Rect> faces, Mat frame, char key)
{
	int temp;
	for (int i = 0; i < faces.size(); i++)
	{
		Point lb(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
		Point tr(faces[i].x, faces[i].y);

		temp = abs_minus_cal(lb.x, tr.x) * abs_minus_cal(lb.y, tr.y);

		switch (key)
		{
		case 'n':
			if (temp >= 4500 && temp < 7500)
				rectangle(frame, lb, tr, Scalar(0, 255, 0), 3, 4, 0);				
			break;
		case 'm':
			if (temp >= 2100 && temp < 4000)
				rectangle(frame, lb, tr, Scalar(0, 255, 0), 3, 4, 0);
			break;
		case 'f':
			if (temp < 2100)
				rectangle(frame, lb, tr, Scalar(0, 255, 0), 3, 4, 0);
			break;
		default:
			break;
		}
	}

}

int abs_minus_cal(int a, int b)
{
	int result = a - b;
	if (result < 0)
		return result * (-1);
	else
		return result;
}