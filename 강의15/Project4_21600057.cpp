// 21600057_Project4
#include "cv.hpp"
#include <iostream>
#define CARDNAME "namecard.JPG"

using namespace std;
using namespace cv;

typedef struct
{
	Point2i first;
	Point2i second;
	double d; // 거리
	double m; // 기울기
	int size;
} Focus;

Mat preprocess1(Mat &img);
Focus* preprocess2(Mat &img);
double distance(int x1, int y1, int x2, int y2);
double square(int n);
int True_False(Focus a, Focus b);
int abs(int a, int b);
Focus* Merge(int count, Focus after[]);
Point2i* equation(Focus* p);
void set_out(Point2f p[]);
void set_in(Point2f p[], Point* point, int size);

int main()
{
	cout << "d";
	Mat ori_card, gray_image, result, pre1;
	Focus* pre2;

	ori_card = imread(CARDNAME, IMREAD_COLOR);

	cvtColor(ori_card, gray_image, COLOR_BGR2GRAY);

	resize(ori_card, ori_card, Size(500, 500));
	resize(gray_image, gray_image, Size(500, 500));
	imshow("ori_card", ori_card);

	pre1 = preprocess1(gray_image);
	pre2 = preprocess2(pre1);

	Point* point = equation(pre2);
	Point2f in[4], out[4];

	set_in(in, point, pre2[0].size);
	set_out(out);

	for (int i = 0; i < pre2[0].size; i++)
		circle(ori_card, point[i], 1, Scalar(0, 255, 0), 5);

	Mat homo_mat = getPerspectiveTransform(in, out);
	warpPerspective(ori_card, result, homo_mat, Size(500, 500));

	imshow("result", result);

	waitKey(0);

	free(pre2);
	free(point);

	return 0;
}

/*
전처리 1. blur로 잡티 없애주고 edge 더 정확히 검출하게 돠줌.
전처리 2. Canny로 edge detect + 팽창시켜줌
*/
Mat preprocess1(Mat &img)
{
	imshow("d", img);
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	GaussianBlur(img, img, Size(15, 15), 2.0, 2.0);
	Canny(img, img, 100, 200);
	dilate(img, img, element);

	return img;
}

/*
전처리 3. line을 그려주고, line길이로 line 수를 줄여줌.
전처리 4. 비슷한 위치에 있는 line 병합해줌.
*/
Focus* preprocess2(Mat &img)
{
	int count = 0;
	double dis;
	Point p1, p2;
	vector<Vec4i> lines;

	HoughLinesP(img, lines, 1, CV_PI / 180, 200);

	Focus after[100];

	for (int i = 0; i < lines.size(); i++)
	{
		Vec4i w = lines[i];

		// y = m(x-x1)+y1 , m = y2-y1/x2-x1
		double m = (double)((w[3] - w[1]) / (double)(w[2] - w[0]));
		dis = distance(w[0], w[1], w[2], w[3]);

		if (dis > 100) // 거리가 100 보다 큰 것만 취급.
		{
			after[count].first = Point2i(w[0], w[1]);
			after[count].second = Point2i(w[2], w[3]);
			after[count].m = m;
			after[count].d = dis;
			count++;
		}

	}

	// 여기서 기울기 비슷한 좌표 +-5 거리에 있는 직선은 하나로 취급함.
	Focus* result = Merge(count, after);

	return result;
}

double distance(int x1, int y1, int x2, int y2)
{
	return sqrt((square(x2 - x1)) + (square(y2 - y1)));
}

double square(int n)
{
	return n * n;
}

int True_False(Focus a, Focus b)
{
	int t_xy = 7; // threshold value
	double t_m = 0.07;
	int mark = 0;

	// case 1, 앞좌표가 같은경우
	if (abs(a.first.x, b.first.x) <= t_xy)
	{
		if (abs(a.first.y, b.first.y) <= t_xy)
		{
			// +- threshold의 위치에 a,b가 존재함. 기울기가 같은지만 보면됨.
			if (abs(a.m - b.m) <= t_m)
				mark = 1; // 모든 조건 맞으면 mark해줌.
		}
	}

	// case 2, 뒷좌표가 같은경우
	else if (abs(a.second.x, b.second.x) <= t_xy)
	{
		if (abs(a.second.y, b.second.y) <= t_xy)
		{
			// +- threshold의 위치에 a,b가 존재함. 기울기가 같은지만 보면됨.
			if (abs(a.m - b.m) <= t_m)
				mark = 1; // 모든 조건 맞으면 mark해줌.
		}
	}

	if (mark == 1)
		return mark;

	else
		return mark;
}

int abs(int a, int b)
{
	int result = a - b;
	if (result < 0)
		return result *= -1;
	else
		return result;
}

Focus* Merge(int count, Focus after[])
{
	int new_count = 0, ok = 1;
	Focus* result = (Focus*)malloc(sizeof(Focus) * count);

	for (int i = 0; i < count; i++)
	{
		ok = 1;
		for (int j = i + 1; j < count; j++)
		{
			if (i == j)
				continue;

			// true = 1, false = 0
			if (True_False(after[i], after[j]) == 1)
				ok = 0;
		}

		if (ok == 1)
		{
			result[new_count].first = Point2i(after[i].first.x, after[i].first.y);
			result[new_count].second = Point2i(after[i].second.x, after[i].second.y);
			result[new_count].m = after[i].m;
			result[new_count].d = after[i].d;
			new_count++;
		}
	}
	result[0].size = new_count;

	return result;
}

/*
y = m(x-x1)+y1 , m = y2-y1/x2-x1
y = mx - mx1 + y1
직선의 방정식 4개를 구하고 교점을 구함.
*/
Point2i* equation(Focus* p)
{
	int count = 0, x, y;
	Point2i* result = (Point2i*)malloc(sizeof(Point2i) * p->size);

	for (int i = 0; i < p->size; i += 2)
	{
		for (int j = 1; j < p->size; j += 2)
		{
			x = (double)((p[j].first.y - p[i].first.y) + (p[i].m*(double)p[i].first.x) - (p[j].m*(double)p[j].first.x)) / (p[i].m - p[j].m);
			y = (p[i].m*x) - (p[i].m * (double)p[i].first.x) + p[i].first.y;

			result[count++] = Point2i(x, y);
		}
	}

	return result;
}

void set_out(Point2f p[])
{
	p[3] = (Point2f(0, 0));
	p[2] = (Point2f(500, 0));
	p[0] = (Point2f(500, 500));
	p[1] = (Point2f(0, 500));
}

void set_in(Point2f p[], Point* point, int size)
{
	for (int i = 0; i < size; i++)
		p[i] = point[i];
}