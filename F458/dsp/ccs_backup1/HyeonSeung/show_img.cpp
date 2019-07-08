#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int main(int argc, char **argv)
{
	Mat img = imread(argv[1], -1);

	if(img.empty())
		return -1;

	namedWindow("First Image", cv::WINDOW_AUTOSIZE);
	imshow("First Image", img);

	waitKey(0);

	destroyWindow("First Image");

	return 0;
}

