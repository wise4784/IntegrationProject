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

using namespace std;
using namespace cv;

extern char *dev0;
jmp_buf env;
int fd;

void call_exit(int signo)
{
	longjmp(env, 1);
}

int main(int argc, char **argv)
{
	int nr, fd;
	int x, y, w, h;
	char buf[32] = "";

	int ret;

	signal(SIGINT, call_exit);

	//fd = serial_config(dev0);
	fd = 1;

	printf("1: Red, 2: Yellow, 3: Green Light\n");

	if(!(ret = setjmp(env)))
	{
		for(;;)
		{
			Mat img;
			Mat filter_img;
			nr = read(0, buf, sizeof(buf));

			if(!(strncmp(buf, "1", 1)))
			{
				img = imread("red.png", -1);
				cvtColor(img, img, COLOR_BGR2RGB);
			}
			else if(!(strncmp(buf, "2", 1)))
			{
				img = imread("yellow.jpg", -1);
				cvtColor(img, img, COLOR_BGR2RGB);
			}
			else if(!(strncmp(buf, "3", 1)))
			{
				img = imread("green.jpg", -1);
				cvtColor(img, img, COLOR_BGR2RGB);
			}
			else
			{
				printf("Wrong Number\n");
				continue;
			}

			printf("I read %s", buf);

			printf("r = %d, g = %d, b = %d\n",
				img.at<Vec3b>(200, 200)[0],
				img.at<Vec3b>(200, 200)[1],
				img.at<Vec3b>(200, 200)[2]);

			send_data(fd, buf, 1, 0);
			printf("\n");

#if 0
			red.png -> 219, 17, 29
			yellow.jpg -> 251, 246, 84
			green.jpg -> 145, 145
#endif

			memset(buf, 0x0, 32);
		}
	}

	//close_dev(fd);

#if 0
	Mat filter_img(img);

	for(y = 0; y < h; y++)
	{
		for(x = 0; x < w; x++)
		{
			// red
			if(img.at<Vec3b>(y, x)[0] < rgb_threshold[0])
				filter_img.at<Vec3b>(y, x)[0] = 0;
			// green
			if(img.at<Vec3b>(y, x)[1] < rgb_threshold[1])
				filter_img.at<Vec3b>(y, x)[1] = 0;
			// red + green
			if(img.at<Vec3b>(y, x)[2] < rgb_threshold[2])
				filter_img.at<Vec3b>(y, x)[2] = 0;
		}
	}

	imwrite("filtered.jpg", filter_img);
	imshow("Filter Image", filter_img);

	waitKey(0);

	destroyWindow("Second Image");
	destroyWindow("Filter Image");
#endif

	return 0;
}
