#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
	Mat img = imread("sample.jpg", -1);
	
	int w = img.cols;
	int h = img.rows;

	printf("h = %d, w = %d\n", h, w);

	if(img.empty())
		return -1;

	printf("h - 310 = %d, w - 539 = %d, h - 120 = %d, w - 475 = %d\n",
		h - 310, w - 539, h- 120, w-475);
	// img(range y, range x);
	Mat roi = img(Range(310, 539), Range(0, 667));

	imshow("Origin Image", img);
	imshow("ROI Image", roi);

	waitKey(0);

	destroyWindow("Origin Image");
	destroyWindow("ROI Image");

	return 0;
}
