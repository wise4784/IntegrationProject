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
		Point(131, 97),
		Point(0, 140),
		Point(0, 177),
		Point(284, 177),
		Point(284, 162),
		Point(161, 97)
	};
	fillConvexPoly(mask, pts, 6, Scalar(255, 255, 255));
	bitwise_and(img, mask, output);

	return output;
}

Mat edge_detect(Mat blur)
{
	Mat output;

	cvtColor(blur, output, COLOR_RGB2GRAY);
	threshold(output, output, 140, 255, THRESH_BINARY);

	Point anchor = Point(-1, -1);
	Mat kernel = Mat(1, 3, CV_32F);

	kernel.at<float>(0, 0) = -1;
	kernel.at<float>(0, 1) = 0;
	kernel.at<float>(0, 2) = 1;

	filter2D(output, output, -1, kernel, anchor, 0, BORDER_DEFAULT);

	return output;
}

int main(int argc, char ** argv)
{
	Mat img = imread("sample.jpg", -1);
	Mat blur;
	Mat edge;
	Mat croi;

	GaussianBlur(img, blur, Size(3, 3), 0, 0);

	edge = edge_detect(blur);
	croi = custom_roi(edge);	
	
	imwrite("org_img.jpg", img);
	imwrite("sobel_roi.jpg", croi);

	namedWindow("Origin Image", WINDOW_NORMAL);
	namedWindow("Filtered Image", WINDOW_NORMAL);

	imshow("Origin Image", img);
	imshow("Filtered Image", croi);

	waitKey(0);

	destroyWindow("Origin Image");
	destroyWindow("Filtered Image");

	return 0;
}
