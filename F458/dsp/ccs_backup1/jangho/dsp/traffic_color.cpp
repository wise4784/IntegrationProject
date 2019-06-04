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
#include <time.h>

#include "serial.h"

using namespace std;
using namespace cv;

extern char *dev0;

jmp_buf env;
int fd;

int flag = 0;

Mat img;
Mat croi;
int idx;
char name[32] = "";
char *traffic[4] = {"red_traffic.jpg", "yellow_traffic.jpg", "green_traffic.jpg"};

Point pts[3][4] = {
	{
		Point(30, 30),
		Point(30,190),
		Point(190, 190),
		Point(190, 30)
	},
	{
		Point(30, 235),
		Point(30, 400),
		Point(190, 400),
		Point(190, 230)
	},
	{
		Point(30, 430),
		Point(30, 600),
		Point(200, 600),
		Point(200, 430)
	}
};

const Point *ppt1[1] = { pts[0] };
const Point *ppt2[1] = { pts[1] };
const Point *ppt3[1] = { pts[2] };

void call_exit(int signo)
{
	longjmp(env, 1);
}

void traffic_chg(int signo)
{
	idx = rand() % 3;
	strcpy(name, traffic[idx]);
	printf("idx = %d, name = %s\n", idx, name);

	img = cv::Mat::zeros(img.size(), img.type());
	img = imread(name, -1);

	flag = 1;
}

Mat custom_roi(Mat img, int idx)
{
	Mat output;
	Mat mask = Mat::zeros(img.size(), img.type());
	//Mat mask(img.size(), CV_8UC3);
	//Mat mask = Mat::zeros(img.size(), img.type());

#if 0
	Point pts[3][4];

	pts[0][0] = Point(328, 175);
	pts[0][1] = Point(384, 173);
	pts[0][2] = Point(385, 111);
	pts[0][3] = Point(330, 116);

	const Point *ppt[1] = { pts[0] };
#endif

	int npt[] = { 4 };

	switch(idx)
	{
		case 0:
			cv::fillPoly(mask, ppt1, npt, 1, cv::Scalar(255, 255, 255));
			break;
		case 1:
			cv::fillPoly(mask, ppt2, npt, 1, cv::Scalar(255, 255, 255));
			break;
		case 2:
			cv::fillPoly(mask, ppt3, npt, 1, cv::Scalar(255, 255, 255));
			break;
	}

	bitwise_and(img, mask, output);

	cout << output.type() << endl;

	return output;
}

#if 0
{
    Point(150, 54),
    Point(183, 53),
    Point(181, 82),
    Point(149, 81)
}
#endif

void chk_traffic_color(Mat croi, int idx)
{
    char buf[32] = "";
    printf("croi rows = %d, croi cols = %d\n", croi.rows, croi.cols);

	switch(idx)
	{
		case 0:
			printf("r = %d, g = %d, b = %d\n",
			       croi.at<Vec3b>(110, 110)[0],
			       croi.at<Vec3b>(110, 110)[1],
			       croi.at<Vec3b>(110, 110)[2]);

			sprintf(buf, "%d", 1);
			printf("buf = %s\n", buf);

			break;

		case 1:
			printf("r = %d, g = %d, b = %d\n",
				croi.at<Vec3b>(110, 310)[0],
				croi.at<Vec3b>(110, 310)[1],
				croi.at<Vec3b>(110, 310)[2]);

			sprintf(buf, "%d", 2);
			printf("buf = %s\n", buf);

			break;

		case 2:
			printf("r = %d, g = %d, b = %d\n",
				croi.at<Vec3b>(110, 510)[0],
				croi.at<Vec3b>(110, 510)[1],
				croi.at<Vec3b>(110, 510)[2]);

			sprintf(buf, "%d", 3);
			printf("buf = %s\n", buf);

			break;
	}
}

int main(int argc, char **argv)
{
	int nr, fd;
	int x, y, w, h;
	char buf[32] = "";
	char test_img[32] = "green_traffic.jpg";

	int ret;
	int wait_time;

	signal(SIGINT, call_exit);
	signal(SIGALRM, traffic_chg);

	srand(time(NULL));
	//fd = serial_config(dev0);
	fd = 1;

	printf("Automatic Traffic Light\n");
	img = imread(test_img, -1);
	//imshow("Green", img);
	//waitKey(0);
	//destroyWindow("Green");

	if(!(ret = setjmp(env)))
	{
		for(;;)
		{
			alarm(0);
			wait_time = rand() % 1 + 2;
			alarm(wait_time);

			while(!flag)
				;

			waitKey(wait_time * 1000);

			flag = 0;

			//cvtColor(img, img, COLOR_BGR2HSV);

			croi = custom_roi(img, idx);

			//send_data(fd, buf, 1, 0);
			//printf("\n");

			// TODO - Something wrong
			chk_traffic_color(croi, idx);

			imshow("Custom ROI Image", croi);

#if 0
			red.png -> 219, 17, 29
			yellow.jpg -> 251, 246, 84
			green.jpg -> 145, 145
#endif

			//memset(buf, 0x0, 32);
		}
	}

	//close_dev(fd);

	return 0;
}
