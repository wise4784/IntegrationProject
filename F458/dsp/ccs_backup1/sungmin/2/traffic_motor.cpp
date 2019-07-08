#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "serial.h"


using namespace std;
using namespace cv;

int tindex;

extern char *dev0;

Mat roi[3];
int fd;

void chgindex(int signo){
    tindex++;
    if(tindex%3==0){
        tindex=0;
    }
}

void destroy(int signo){
    destroyWindow("img");
   // close_dev(fd);
    cout<<"sigINT"<<endl;
    exit(1);
}

int main(void)
{
    Mat img=imread("traffic.jpg");

    dev0="/dev/ttyUSB0";
    //fd=serial_config(dev0);
    fd=1;

    signal(SIGALRM,chgindex);
    signal(SIGINT,destroy);

    if(img.empty()){
        return -1;
    }

    roi[0]=img(Range(0,200),Range(0,300));
    roi[1]=img(Range(200,400),Range(301,600));
    roi[2]=img(Range(400,600),Range(601,900));

    imwrite("red.jpg",roi[0]);
    imwrite("yellow.jpg",roi[1]);
    imwrite("green.jpg",roi[2]);

    imshow("img",img);
    waitKey(1);


    for(;;){
        imshow("img",roi[tindex]);
        waitKey(1);
        printf("tindex=%d\n",tindex);

        switch(tindex){
        case 0:
            send_data(fd,"1",1,0);
            break;
        case 1:
            send_data(fd,"2",1,0);
            break;
        case 2:
            send_data(fd,"3",1,0);
            break;
        }
        printf("\n");

        alarm(2+rand()%4);
        pause();

/*        switch(tindex){
        case 0:
            imshow("red",roi[tindex]);
            waitKey(1);
            destroyWindow("red");
            cout<<"tindex:"<<tindex<<endl;
            break;
        case 1:
            imshow("yellow",roi[tindex]);
            waitKey(1);
            destroyWindow("yellow");
            cout<<"tindex:"<<tindex<<endl;
            break;
        case 2:
            imshow("green",roi[tindex]);
            waitKey(1);
            destroyWindow("green");
            cout<<"tindex:"<<tindex<<endl;
            break;
        }*/
    }
}
