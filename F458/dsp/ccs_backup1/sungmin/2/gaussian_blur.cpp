#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    Mat img = imread("sample.jpg",-1);

    if(img.empty()){
        return -1;
    }

    Mat blur;
    GaussianBlur(img, blur, Size(11,11),0,0);

    imwrite("org_img.jpg",img);
    imwrite("blur.jpg",blur);

    imshow("Origin Image",img);
    imshow("Blur Image",blur);

    waitKey(0);

    destroyWindow("Origin Image");
    destroyWindow("Blur Image");

    return 0;
}
