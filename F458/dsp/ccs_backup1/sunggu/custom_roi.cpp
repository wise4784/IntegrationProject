#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

Mat custom_roi(Mat img)
{
	Mat Output;
	Mat mask = Mat::zeros(img.size(), img.type());

	Point pts[4] ={
		Point(251, 194),
		Point(307, 194),
		Point(542, 323),
		Point(5, 323)
		};

	fillConvexPoly(mask, pts, 4, Scalar(255,255,0));
	bitwise_and(img, mask, Output);

	return Output;
}

int main(int argc, char **argv)
{
	Mat img = imread("sample.jpg", -1);

	int h = img.rows;
	int w = img.cols;

	printf("h = %d, w = %d\n", h, w);

	if(img.empty())
		return -1;

	Mat croi = custom_roi(img);

	imwrite("org_img.jpg", img);
	imwrite("croi.jpg", croi);

	namedWindow("Origin Image", cv::WINDOW_NORMAL);
	imshow("Origin Image", img);
	imshow("Custom Roi Image",croi);

	waitKey(0);
 
	destroyWindow("Origin Image");
	destroyWindow("Custom Roi Image");

	return 0;
}
