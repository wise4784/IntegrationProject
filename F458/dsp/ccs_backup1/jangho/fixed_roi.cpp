#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	Mat img = imread("sample.jpg", -1);

	int h = img.rows;	
	int w = img.cols;

	printf("h = %d, w = %d\n", h, w);

	if(img.empty())
		return -1;

	printf("h - 200 = %d, w - 400 = %d, h - 150 = %d, w - 550 = %d\n",
		h - 310, w - 539, h - 120, w - 475);
    // printf("h - 200 = %d, w - 400 = %d, h - 150 = %d, w - 550 = %d\n",
    //     h - 310, w - 539, h - 120, w - 475);

	Mat roi = img(Range(200, 400), Range(200, 400));
	// Mat roi = img(Range(310, 539), Range(120, 475));

	imwrite("org_img.jpg", img);
	imwrite("roi.jpg", roi);

	imshow("Origin Image", img);
	imshow("ROI Image", roi);

	waitKey(0);

	destroyWindow("Origin Iamge");
	destroyWindow("ROI Iamge");
	
	return 0;
}
