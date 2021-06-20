// 21600057_Project3
#include "cv.hpp"
#include <iostream>

#define BACKGROUND "background.jpg"
#define SIZE_X 200
#define SIZE_Y 200

using namespace std;
using namespace cv;

CascadeClassifier face_classifier;
Rect FaceDetect(Mat frame);
void tracking(Mat frame, Rect roi, Mat background);

int main()
{
	Mat result, image, background;

	background = imread(BACKGROUND, 1);
	result = background.clone();
	resize(result, result, Size(SIZE_X, SIZE_Y));

	VideoCapture cap(0);

	if (!face_classifier.load("C:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml"))
		printf("Error!\n");

	while (1)
	{
		cap.read(image);
		if (!cap.isOpened())
		{
			cout << "error!" << endl;
			exit(-1);
		}

		else
		{
			resize(image, image, Size(SIZE_X, SIZE_Y));
			Rect roi = FaceDetect(image);
			
			if (!((roi.width == 0) && (roi.height == 0)))
				tracking(image, roi, background);
			else
				imshow("Project3", result);

			waitKey(10);
		}
	}
	waitKey(0);
	destroyAllWindows();

	return 0;
}

Rect FaceDetect(Mat frame)
{
	Mat gray_frame;
	vector<Rect> faces;

	cvtColor(frame, gray_frame, COLOR_BGR2GRAY);

	face_classifier.detectMultiScale(gray_frame, faces, 1.1, 3, 0, Size(10, 10));
	if (faces.size() == 0)
		faces.push_back(Rect(Point(0, 0), Point(0, 0)));
	
	return faces[0];
}

void tracking(Mat frame, Rect roi, Mat background)
{
	Mat hsv, m_backproj, m_model3d;
	float hrange[] = { 0, 180 };
	float srange[] = { 0, 255 };
	float vrange[] = { 0, 255 };
	const float* range[] = { hrange, srange, vrange };
	int chanels[] = { 0,1,2 };
	int hist_size[] = { 16,16,16 };
	Rect m_rc;

	cvtColor(frame, hsv, COLOR_BGR2HSV);

	Mat mask = Mat::zeros(roi.height, roi.width, CV_8U);
	ellipse(mask, Point(roi.width / 2, roi.height / 2), Size(roi.width / 2, roi.height / 2), 0, 0, 360, 255, CV_FILLED);
	Mat region_of_image(hsv, roi);
	calcHist(&region_of_image, 1, chanels, mask, m_model3d, 3, hist_size, range);
	m_rc = roi;

	//histogram backprojection
	calcBackProject(&hsv, 1, chanels, m_model3d, m_backproj, range);
	//tracking
	CamShift(m_backproj, m_rc, cvTermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 20, 1));
	//rectangle(frame, m_rc, Scalar(0, 0, 255), 3);

	Mat image, result, bgModel, fgModel;
	image = frame;

	grabCut(image, result, roi, bgModel, fgModel, 10, GC_INIT_WITH_RECT);
	compare(result, GC_PR_FGD, result, CMP_EQ);
	
	resize(background, background, Size(SIZE_X, SIZE_Y));

	image.copyTo(background, result);
	imshow("Project3", background);
}