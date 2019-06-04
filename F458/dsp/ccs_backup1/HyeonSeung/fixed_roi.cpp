#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
	Mat img = imread("sample2.jpg", -1);

	int h = img.rows;
	int w = img.cols;

	printf("h = %d, w = %d\n", h, w);

	if(img.empty())
		return -1;

	printf("h - 303 = %d, w - 465 = %d, h - 606 = %d, w - 931 = %d\n", h - 303, w - 465, h - 606, w -931);

	Mat roi = img(Range(303, 465), Range(606, 931));

	imshow("Origin Image", img);
	imshow("ROI Image", roi);
	
	waitKey(0);

	destroyWindow("Origin Image");
	destroyWindow("ROI Image");

	return 0;
}

