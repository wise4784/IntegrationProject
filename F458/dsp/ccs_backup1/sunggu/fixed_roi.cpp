#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
	Mat img = imread("roi_test.jpg", -1);

	int h = img.rows;
	int w = img.cols;

	printf("h = %d, w = %d\n", h, w);

	if(img.empty())
		return -1;

	printf("h-0 = %d, w-200 = %d, h -100 = %d, w - 200 = %d\n",
		h-0, w -200, h-100, w-200);

	Mat roi = img(Range(0, 200), Range(100, 200));

	imshow("Origin Image", img);
	imshow("ROI Image", roi);

	waitKey(0);

	destroyWindow("Origin Image");
	destroyWindow("ROI Image");

	return 0;
}
