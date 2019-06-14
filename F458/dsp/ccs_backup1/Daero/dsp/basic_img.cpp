#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(void)
{
	Mat srcImg = imread("test.jpeg", IMREAD_GRAYSCALE);

	if(srcImg.empty())
		return -1;

	imwrite("this.jpg", srcImg);

	return 0;
}
