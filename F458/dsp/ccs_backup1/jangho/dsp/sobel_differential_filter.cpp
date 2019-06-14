#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

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

	GaussianBlur(img, blur, Size(3, 3), 0, 0);

	edge = edge_detect(blur);
	
	imwrite("org_img.jpg", img);
	imwrite("differential.jpg", edge);

	imshow("Origin Image", img);
	imshow("First Order Differential Image", edge);

	waitKey(0);

	destroyWindow("Origin Image");
	destroyWindow("First Order Differential Image");

	return 0;
}
