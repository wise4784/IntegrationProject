#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    Mat img = imread("mysample.jpg", -1);

    if(img.empty())
        return -1;
#if 1
    Mat myclear;
    Point anchor2 = Point(-1,-1);
    Mat kernel2 = Mat(3, 3, CV_32F, Scalar(0));

    kernel2.at<float>(1,1) = 5;
    kernel2.at<float>(0,1) = -1;
    kernel2.at<float>(2,1) = -1;
    kernel2.at<float>(1,0) = -1;
    kernel2.at<float>(1,2) = -1;

    filter2D(img, myclear, -1, kernel2, anchor2, 0, BORDER_DEFAULT);
#endif
    Mat blur;
    GaussianBlur(myclear, blur, Size(9,9),0,0);
//    GaussianBlur(img, blur, Size(9,9),0,0);

#if 1
    Mat gray;
    cvtColor(blur, gray, COLOR_BGR2GRAY);
#endif

#if 1
    Mat thresholder;
    threshold(gray, thresholder, 120, 255, THRESH_BINARY);
#endif

#if 0
    Mat cani;
//    Canny(gray, cani, 80, 160, 3);
    Canny(blur, cani, 80, 160, 3);
#endif


#if 1
    Mat f2d;
    Point anchor = Point(-1,-1);
    Mat kernel = Mat(1, 3, CV_32F);

    kernel.at<float>(0,0) = -1;
    kernel.at<float>(0,1) = 0;
    kernel.at<float>(0,2) = 1;

    filter2D(thresholder, f2d, -1, kernel, anchor, 0, BORDER_DEFAULT);
#endif

#if 1
    Mat bi_and;
    Mat mask = Mat::zeros(f2d.size(), f2d.type());
//    Mat mask = Mat::zeros(cani.size(), cani.type());
    Point pts[4] ={
                   Point(0, 190),
                   Point(533, 190),
                   Point(533, 300),
                   Point(0, 300),
    };

    fillConvexPoly(mask, pts, 4, Scalar(255, 0, 0));
    bitwise_and(f2d, mask, bi_and);
//    bitwise_and(cani, mask, bi_and);

    vector<Vec4i> lines;
    HoughLinesP(bi_and, lines, 1, CV_PI/180, 20, 20, 30);


    for( int i=0; i<lines.size(); i++ )
     {
         Vec4i L = lines[i];
         line(img, Point(L[0],L[1]), Point(L[2],L[3]),
              Scalar(0,0,255), 2, LINE_AA );
     }
#endif


    imwrite("myline_plot.jpg", img);
    imwrite("mytbi_and4.jpg", bi_and);

    imshow("myline_plot", img);
    imshow("my_bi_and", bi_and);

    waitKey(0);

    destroyWindow("myline_plot");
    destroyWindow("my_bi_and");

    return 0;
}
