// 21600057_Assignment4
#include "cv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

Mat histo(Mat temp);
Mat drawHistogram(Mat temp);
Mat putTextToH(Mat temp, string text[], int num);

int main()
{
	Mat moon, hist_moon, graph_moon, graph_hist_moon;
	moon = imread("moon.png", IMREAD_GRAYSCALE);
	
	hist_moon = histo(moon);

	imshow("h1", drawHistogram(moon));
	imshow("before", moon);
	
	imshow("h2", drawHistogram(hist_moon));
	imshow("after", hist_moon);

	waitKey(0);
	destroyAllWindows();
}

Mat histo(Mat temp)
{
	Mat result;
	equalizeHist(temp, result);

	return result;
}

Mat drawHistogram(Mat temp)
{
	Mat hist, histimage, Rhist, Rhistimage;
	int i, hist_w, hist_h, bin_w, histSize, RhistSize, Rbin_w;
	string text[8] = { "" }, space = " : ";

	float range[] = { 0, 256 };
	const float* histRange = { range };
	
	hist_w = 512;
	hist_h = 512;
	histSize = 16;
	bin_w = cvRound((double)hist_w / histSize);

	RhistSize = 8;
	Rbin_w = cvRound((double)hist_w / RhistSize);

	histimage = Mat(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));
	Rhistimage = Mat(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));

	//compute the histogram
	calcHist(&temp, 1, 0, Mat(), hist, 1, &histSize, &histRange);
	calcHist(&temp, 1, 0, Mat(), Rhist, 1, &RhistSize, &histRange);

	normalize(hist, hist, 0, histimage.rows, NORM_MINMAX, -1, Mat());

	for (i = 0; i<histSize; i++)
		rectangle(histimage, Point(bin_w * i, hist_h), Point(bin_w * i + hist_w / histSize, hist_h - cvRound(hist.at<float>(i))), Scalar(0, 0, 0), -1);

	//노말라이제이션 bin의 값을 토탈 픽셀수로 나눈다. total pixel # is 113,220
	//ex) 1st bin[0 - 3] : 16 / total n of pixcel  w * h = total n of pixcel
	for (i = 0; i < RhistSize; i++)
		text[i] = "bin " + to_string(i + 1) + space + to_string((float)Rhist.at<float>(i) / 113220);
		
	putTextToH(temp, text, RhistSize);
	
	return histimage;
}

Mat putTextToH(Mat temp, string text[], int num)
{
	for (int i = 0; i < num; i++)
		putText(temp, text[i], Point(10, 20+(20*i)), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1);

	return temp;
}