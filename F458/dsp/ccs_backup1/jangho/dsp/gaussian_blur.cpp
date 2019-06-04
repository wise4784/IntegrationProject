#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int main(int argc, char ** argv)
{
	Mat img = imread("noise2.jpg", -1);
	
	if(img.empty())
	        return -1;

	Mat blur;
	GaussianBlur(img, blur, Size(3,3), 0, 0);

	imwrite("org_img.jpg", img);
	imwrite("blur.jpg", blur);

	imshow("Origin Image", img);
	imshow("Blur Image", blur);

	waitKey(0);

	destroyWindow("Origin Image");
	destroyWindow("Blur Image");

	return 0;
}
