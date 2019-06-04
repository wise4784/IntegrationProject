#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;
using namespace cv;

int tindex;
Mat img[3];

void all_destroy(int singno){
    destroyWindow("traffic");
    cout<<endl<<"ctrl+c"<<endl;
    exit(-1);
}

void chgimg(int signo){
    cout<<tindex<<endl;
    imshow("traffic",img[tindex++]);
    waitKey(100);
    if(tindex>=3){
        tindex=0;
    }
}

Mat custom_roi(Mat input, int i){
    Mat res;
    Point p[4];
    Mat mask=Mat::zeros(input.size(),input.type());
    switch(i){
    case 0:
        p[0].x=85;p[0].y=69;p[1].x=85;p[1].y=190;p[2].x=220;p[2].y=190;p[3].x=220;p[3].y=69;
        break;
    case 1:
        p[0].x=85;p[0].y=69;p[1].x=85;p[1].y=190;p[2].x=220;p[2].y=190;p[3].x=220;p[3].y=69;
        break;
    case 2:
        p[0].x=85;p[0].y=69;p[1].x=85;p[1].y=190;p[2].x=220;p[2].y=190;p[3].x=220;p[3].y=69;
        break;
    }
    fillConvexPoly(mask,p,4,Scalar(255,255,255));

    bitwise_and(input,mask,res);

    return res;
}

int main(void)
{
    signal(SIGINT,all_destroy);
    signal(SIGALRM,chgimg);

    img[0]=imread("red.jpg");
    img[1]=imread("yellow.jpg");
    img[2]=imread("green.jpg");

    img[0]=custom_roi(img[0],0);

    namedWindow("traffic",CV_WINDOW_NORMAL);

    imshow("traffic",img[tindex++]);
      waitKey(100);
    for(;;){
        //alarm(1+rand()%5);
       // pause();

       // cout<<1234<<endl;
    }

    return 0;
}
