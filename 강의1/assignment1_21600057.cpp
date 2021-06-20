// 21600057_Assignment1
#include "cv.hpp"
#include <iostream>
#define VIDEONAME "background.mp4"

using namespace std;
using namespace cv;

int main()
{
	Mat frame;
	VideoCapture cap;
	double time_in_msec, fps;
	int total_frames, current_frame = 0;

	if (cap.open(VIDEONAME) == 0) // if file open error,
	{
		cout << "Cant find the file!" << endl;
		waitKey(0); // 0 means forever
	}

	fps = cap.get(CAP_PROP_FPS);

	do
	{
		cap >> frame;
		total_frames = (int)cap.get(CAP_PROP_FRAME_COUNT);
		time_in_msec = cap.get(CAP_PROP_POS_MSEC);

		if (frame.empty()) // empty == true , In other words, frame is empty
		{
			cout << "The End!" << endl;
			break;
		}

		else
		{
			imshow("video", frame);
			cout << "frames: " << current_frame++ << " / " << total_frames << endl;
			waitKey(1000/fps);
		}
	} while (time_in_msec < 3000.0); // Display video for 3 seconds

	waitKey(0);
	destroyAllWindows();
}