#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace cv;
using namespace std;

int bgr[3] = {0};
int bgrcnt[3] = {0};
int r =0, g =0, b =0;

Mat custom_roi(Mat img)
{
    Mat Output;
    Mat mask = Mat::zeros(img.size(), img.type());

    Point pts[4] ={
        Point(270, 178),
        Point(419, 178),
        Point(419, 282),
        Point(270, 282)
        };

    fillConvexPoly(mask, pts, 4, Scalar(0,0,255));
    bitwise_and(img, mask, Output);

    return Output;
}
 // 오름차순으로 정렬 후, 마지막 값 반환
int findbig(int *bgr_v)
{
    int i, j, tmp =0;
    int n =3;
    for(j=0; j<n; j++){
        for(i = 0; i<n-1; i++){
            if(bgr_v[i] > bgr_v[i+1]){
                tmp = bgr_v[i];
                bgr_v[i] = bgr_v[i+1];
                bgr_v[i+1] = tmp;
            }
        }
    }

    return bgr_v[2];
}

int main(int argc, char **argv)
{
    int bre, i, h, w, ra;
    int x, y;
    char dc;
    Mat img;

    //test code
    img = imread("redlight.jpg", -1);

    h = img.rows;
    w = img.cols;
    printf("h = %d, w = %d\n", h, w);

    /*
    srand(time(NULL));
    ra = (rand()%3)+1;

    switch(ra){
    case 1: img = imread("redlight.jpg", -1); break;
    case 2: img = imread("yellowlight.jpg", -1); break;
    case 2: img = imread("greenlight.jpg", -1); break;
    default: printf("Error!"); break;
    }
    */

    if(img.empty())
        return -1;

    //색 판단
    Mat croi = custom_roi(img);
    h = croi.rows;
    w = croi.cols;
    printf("h = %d, w = %d\n", h, w);

    // BGR 012
    for(y = 180; y < 280; y++)
    {
        for(x = 272; x < 416; x++)
        {
           bgr[0] = croi.at<Vec3b>(y,x)[0];
           bgr[1] = croi.at<Vec3b>(y,x)[1];
           bgr[2] = croi.at<Vec3b>(y,x)[2];

           b = bgr[0];
           g = bgr[1];
           r = bgr[2];
           //정렬을 사용하여 더 간단하게 만들자
          //rgb 중에 큰값찾기
           //가장 큰값 +1
           bre = findbig(bgr);

           if(bre == b){
               ++bgrcnt[0];
           }else if(bre == g){
               ++bgrcnt[1];
           }else if(bre == r){
               ++bgrcnt[2];
           }

        }
    }

    printf("b = %d, g = %d, r = %d\n",
               bgrcnt[0], bgrcnt[1], bgrcnt[2]);

    imwrite("org_img2.jpg", img);
    imwrite("croi2.jpg", croi);

    imshow("Origin Image", img);
    imshow("Custom Roi Image",croi);

    waitKey(0);

    destroyWindow("Origin Image");
    destroyWindow("Custom Roi Image");

    return 0;
}
