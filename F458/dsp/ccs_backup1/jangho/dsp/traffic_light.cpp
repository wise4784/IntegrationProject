#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

#include <sys/types.h>
#include <sys/poll.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>
#include <fcntl.h>
#include <stdio.h>

#include "serial.h"

using namespace cv;
using namespace std;

struct sigaction sa;

extern char *dev0;
int fd;

Mat img;
Mat rimg;
Mat yimg;
Mat gimg;

float rsum[3], ysum[3], gsum[3], sum[3];

int select_img(void)
{
	int n;

	srand(time(NULL));

	n = rand() % 3 + 1;

	return n;
}

void img_init(void)
{
        int rw, rh, yw, yh, gw, gh;
        int x, y;

        Point pts1[4] = {
                         Point(193, 220),
                         Point(193, 251),
                         Point(228, 251),
                         Point(228, 220)
        };

        Point pts2[4] = {
                         Point(118, 170),
                         Point(118, 192),
                         Point(136, 192),
                         Point(136, 170)
        };

        Point pts3[4] = {
                         Point(159, 190),
                         Point(159, 248),
                         Point(214, 248),
                         Point(214, 190)
        };

        rimg = imread("red.jpg", -1);
        yimg = imread("yellow.jpg", -1);
        gimg = imread("green.jpg", -1);

        Mat rmask = Mat::zeros(rimg.size(), rimg.type());
        Mat ymask = Mat::zeros(yimg.size(), yimg.type());
        Mat gmask = Mat::zeros(gimg.size(), gimg.type());

        fillConvexPoly(rmask, pts1, 4, Scalar(255, 255, 255));
        fillConvexPoly(ymask, pts2, 4, Scalar(255, 255, 255));
        fillConvexPoly(gmask, pts3, 4, Scalar(255, 255, 255));

        bitwise_and(rimg, rmask, rmask);
        bitwise_and(yimg, ymask, ymask);
        bitwise_and(gimg, gmask, gmask);

        Mat rarea = rimg(Range(220+(251-220)/5, 251-(251-220)/5), Range(193+(228-193)/5, 228-(228-193)/5));
        Mat yarea = yimg(Range(170+(192-170)/5, 192-(192-170)/5), Range(118+(136-118)/5, 136-(136-118)/5));
        Mat garea = gimg(Range(190+(248-190)/5, 248-(248-190)/5), Range(159+(214-159)/5, 214-(214-159)/5));

        GaussianBlur(rarea, rarea, Size(5, 5), 0, 0);
        GaussianBlur(yarea, yarea, Size(5, 5), 0, 0);
        GaussianBlur(garea, garea, Size(5, 5), 0, 0);

        rw = rarea.cols;rh = rarea.rows;
        yw = yarea.cols;yh = yarea.rows;
        gw = garea.cols;gh = garea.rows;

        for(y=0; y<rh; y++)
        {
            for(x=0; x<rw; x++)
            {
                rsum[0] += rarea.at<Vec3b>(y, x)[0];
                rsum[1] += rarea.at<Vec3b>(y, x)[1];
                rsum[2] += rarea.at<Vec3b>(y, x)[2];
            }
        }

        for(y=0; y<yh; y++)
        {
            for(x=0; x<yw; x++)
            {
                ysum[0] += yarea.at<Vec3b>(y, x)[0];
                ysum[1] += yarea.at<Vec3b>(y, x)[1];
                ysum[2] += yarea.at<Vec3b>(y, x)[2];
            }
        }

        for(y=0; y<gh; y++)
        {
            for(x=0; x<gw; x++)
            {
                gsum[0] += garea.at<Vec3b>(y, x)[0];
                gsum[1] += garea.at<Vec3b>(y, x)[1];
                gsum[2] += garea.at<Vec3b>(y, x)[2];
            }
        }
        rsum[0] /= (rh*rw);
        rsum[1] /= (rh*rw);
        rsum[2] /= (rh*rw);

        ysum[0] /= (yh*yw);
        ysum[1] /= (yh*yw);
        ysum[2] /= (yh*yw);

        gsum[0] /= (gh*gw);
        gsum[1] /= (gh*gw);
        gsum[2] /= (gh*gw);

}

void select_pic(void)
{
    int n;
    n = rand() % 3 + 1;

    switch(n)
    {
        case 1:
            sum[0] = rsum[0];
            sum[1] = rsum[1];
            sum[2] = rsum[2];
            break;
        case 2:
            sum[0] = ysum[0];
            sum[1] = ysum[1];
            sum[2] = ysum[2];
            break;
        case 3:
            sum[0] = gsum[0];
            sum[1] = gsum[1];
            sum[2] = gsum[2];
            break;
        default:
            break;
    }
}

void my_sig(int signo)
{

}

int main(int argc, char ** argv)
{
    srand(time(NULL));

    int n;

    img_init();

    printf("rb = %f, rg = %f, rr = %f\n", rsum[0], rsum[1], rsum[2]);
    printf("yb = %f, yg = %f, yr = %f\n", ysum[0], ysum[1], ysum[2]);
    printf("gb = %f, gg = %f, gr = %f\n", gsum[0], gsum[1], gsum[2]);

    select_pic();

    printf("b = %f, g = %f, r = %f\n", sum[0], sum[1], sum[2]);



    if(sum[1] > 128 && sum[2] > 128)
    {
        namedWindow("ymask", WINDOW_AUTOSIZE);
        imshow("ymask", ymask);
    }
    else if(sum[2] > 128)
    {
        namedWindow("rmask", WINDOW_AUTOSIZE);
        imshow("rmask", rmask);
    }
    else if(sum[1] > 128)
    {
        namedWindow("gmask", WINDOW_AUTOSIZE);
        imshow("gmask", gmask);
    }

    imwrite("rarea.jpg", rarea);
    imwrite("yarea.jpg", yarea);
    imwrite("garea.jpg", garea);

	waitKey(0);

	destroyWindow("rmask");
	destroyWindow("ymask");
	destroyWindow("gmask");
/*
	sa.sa_handler = my_sig;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGALRM, &sa, NULL);

    signal(SIGINT, call_exit);

	//signal(SIGALRM, my_sig);
	alarm(1);

    if(!(ret = setjmp(env)))
    {
*/
        for(;;)
        {
  //          pause();
  //          alarm(1);
  //          destroyWindow("select image");

        }
 //   }
	//close_dev(fd);

	return 0;
}
