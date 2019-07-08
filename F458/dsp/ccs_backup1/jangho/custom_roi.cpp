#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

Mat custom_roi(Mat img)
{
	Mat output;
	Mat mask = Mat::zeros(img.size(), img.type());
		
	Points pts[4] = {
		Point(460, 310),
		Point(480, 310),
		Point(800, 539),
		Point(120, 539),
	};
	
	fillConvexPoly(mask, pts, 4, Scalar(0, 255, 0));
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

	imwrite("Origin Image", img);
	imwrite("Custom ROI Image", croi);
	

	waitKey(0);

	destroyWindow("Origin Image");
	destroyWindow("Custom ROI Image");

	return 0;
}
