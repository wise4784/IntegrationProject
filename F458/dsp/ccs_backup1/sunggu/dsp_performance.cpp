//Currently we use only Cortex-A5
//So now we use C6678 DSP

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/ocl.hpp>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

double tdiff_calc(struct timespec &tp_start, struct timespec &tp_end)
{
	return (double)(tp_end.tv_nsec - tp_start.tv_nsec) * 0.000001 +
		(double)(tp_end.tv_sec - tp_start.tv_sec) * 1000.0;
}

using namespace cv;

int main(int argc, char **argv)
{
	struct timespec tp0, tp1, tp2, tp3;
	UMat	img, gray;
	imread(("sample.jpg"), 1).copyTo(img);

	clock_gettime(CLOCK_MONOTONIC, &tp0);
	cvtColor(img, gray, COLOR_BGR2GRAY);
	clock_gettime(CLOCK_MONOTONIC, &tp1);
	GaussianBlur(gray, gray, Size(5, 5), 1.25);
	clock_gettime(CLOCK_MONOTONIC, &tp2);
	Canny(gray, gray, 0, 30);
	clock_gettime(CLOCK_MONOTONIC, &tp3);

	printf("BGR2GRAY tdiff = %lf ms\n", tdiff_calc(tp0, tp1));
	printf("Gauss Blur tdiff = %lf ms\n", tdiff_calc(tp1, tp2));
	printf("Canny tdiff = %lf ms\n", tdiff_calc(tp2, tp3));
	
	imwrite("test.jpg", gray);
	
	imshow("test Image", gray);
	waitKey(0);
	destroyWindow("test Image");
	

	return 0;
}
