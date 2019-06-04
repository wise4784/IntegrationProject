#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


int main(void)
{
	Mat srclmg = imread("test.jpeg", IMREAD_GRAYSCALE);

	if(srclmg.empty())
		return -1;

	imwrite("this.jpg", srclmg);

	return 0;
}
