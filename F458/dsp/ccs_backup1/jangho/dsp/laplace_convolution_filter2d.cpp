#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

void custom_filter2d(Mat in, Mat out, int depth, Mat kernel,
        Point anchor, int blas, int border_type)
{
    int i, j, k;
    int tmp = 0;
    Mat t = in.clone();

    for(i=0; i<5; i++)
    {
        out.at<float>(i, 0) =
            t.at<float>(i, 1) * kernel.at<float>(0, 0) +
            t.at<float>(i, 0) * kernel.at<float>(0, 1) +
            t.at<float>(i, 1) * kernel.at<float>(0, 2);
        for(j=1; j<4; j++)
        {
            out.at<float>(i, j) =
                t.at<float>(i, j-1)*kernel.at<float>(0,0)+
                t.at<float>(i, j)*kernel.at<float>(0,1)+
                t.at<float>(i, j+1)*kernel.at<float>(0,2);
        }
        out.at<float>(i, 4) =
            t.at<float>(i, 3) * kernel.at<float>(0, 0)+
            t.at<float>(i, 4) * kernel.at<float>(0, 1)+
            t.at<float>(i, 3) * kernel.at<float>(0, 2);
    }
}

int main(int argc, char ** argv)
{
    Mat m = Mat::ones(5, 5, CV_32F);
    Mat m2 = Mat::ones(5, 5, CV_32F);
    Mat m3 = Mat::ones(5, 5, CV_32F);

    Mat kernel = Mat(1, 3, CV_32F);
    Point anchor = Point(-1, -1); // 테두리를 잡을것인가 ? 말것인가 ?

    kernel.at<float>(0, 0) = -1;
    kernel.at<float>(0, 1) = 0;
    kernel.at<float>(0, 2) = 1;

    m.at<float>(0, 0) = 0;
    m.at<float>(2, 2) = 2;
    m.at<float>(4, 4) = 3;

    cout<<"m :" <<endl<<m<<endl;

    custom_filter2d(m, m2, -1, kernel, anchor, 0, BORDER_DEFAULT);

    cout<<"custom m : "<<endl<<m2<<endl;

    filter2D(m, m3, -1, kernel, anchor, 0, BORDER_DEFAULT);

    cout<<"non-custom m : "<<endl<<m3<<endl;

    return 0;
}
