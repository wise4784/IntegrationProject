#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(void)
{
    Mat frame;
    VideoCapture inputVideo(1);

    if(!inputVideo.isOpened()){
        cout<<"Can't open Video!"<<endl;
        return 0;
    }

    Size size=Size((int)inputVideo.get(CAP_PROP_FRAME_WIDTH),(int)inputVideo.get(CAP_PROP_FRAME_HEIGHT));

    cout<<"Size="<<endl;

    int fourcc=VideoWriter::fourcc('x','v','i','d');

    double fps=30;
    bool isColor=true;
    VideoWriter outputVideo("output.avi",fourcc,fps,size,isColor);

    if(!outputVideo.isOpened()){
        cout<<"Can't open Video!"<<endl;
        return -1;
    }

    if(fourcc!=-1){
        imshow("frame",NULL);
        waitKey(100);
    }

    int delay=1000/fps;

    for(;;)
    {
        inputVideo>>frame;

        if(frame.empty()){
            break;
        }
        outputVideo<<frame;
        imshow("frame",frame);

        int ckey=waitKey(delay);
        if(ckey==27)
            break;
    }

    return 0;

}
