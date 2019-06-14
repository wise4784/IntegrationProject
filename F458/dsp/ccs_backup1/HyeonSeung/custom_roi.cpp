#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

Mat custom_roi(Mat img)
{
	Mat output;
	Mat mask = Mat::zeros(img.size(), img.type());

	Point pts[6] = {
			Point(14,417), Point(14,363), Point(328,197), Point(353, 197), Point(628,327),Point(626,415)
		};
	fillConvexPoly(mask, pts, 6, Scalar(25, 45, 255));
	bitwise_and(img, mask, output);

	return output;
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

	namedWindow("Origin Image", cv::WINDOW_AUTOSIZE);
	imshow("Origin Image", img);
	imshow("Custom ROI Image", croi);

	waitKey(0);

	destroyWindow("Origin Image");
	destroyWindow("Custom ROI Image");

	return 0;
}

