#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

int main(void)
{
	Mat img=imread("sample.jpg",-1);
	
	int h=img.rows;
	int w=img.cols;

	printf("h=%d, w=%d\n",h,w);

	if(img.empty()){
		return -1;
	}

	printf("h-310=%d, w-539=%d, h-120=%d, w-475=%d\n",h-310,w-539,h-120,w-475);

                                                                               
        Mat roi=img(Range(0,141),Range(0,250));//  (h(min, max), w(min, max))                                
        imwrite("org_img.jpg",img);                                                
        imwrite("roi.jpg",roi);  


	imshow("Origin Image",img);
	imshow("ROI image",roi);

	waitKey(0);
	
	destroyWindow("Origin Image");
	destroyWindow("ROI Image");

	return 0;
}
