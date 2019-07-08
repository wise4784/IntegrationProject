#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char ** argv)
{
	int x, y, w, h;
	int rgb_threshold[4] = {128, 128, 128};
	
	Mat img = imread("sample.jpg", -1);
	Mat filter_img;
	
	cvtColor(img, filter_img, COLOR_BGR2RGB);

	h = img.rows;
	w = img.cols;

	if(img.empty())
		return -1;
	
	cout << "img.depth = "<<img.depth() <<","
		<< "img.channels = "<<img.channels() << endl;

	namedWindow("Second Image", WINDOW_NORMAL);
	imshow("Second Image", filter_img);

	for(y=0; y<h; y++)
	{
		for(x=0; x<w; x++)
		{
			if(filter_img.at<Vec3b>(y, x)[0] <rgb_threshold[0])
				filter_img.at<Vec3b>(y, x)[0] = 0;
			if(filter_img.at<Vec3b>(y, x)[1] <rgb_threshold[1])
				filter_img.at<Vec3b>(y, x)[1] = 0;
			if(filter_img.at<Vec3b>(y, x)[2] <rgb_threshold[2])
				filter_img.at<Vec3b>(y, x)[2] = 0;
		}
	}
    namedWindow("Filter Image", WINDOW_NORMAL);
	imshow("Filter Image", filter_img);

	waitKey(0);

	destroyWindow("Sencond Image");
	destroyWindow("Filter Image");

	return 0;
}
