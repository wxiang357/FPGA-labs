/***************************************************************************/
#include "top.h"
#include "opencv_top.h"

// I/O Image Settings
#define INPUT_IMAGE           "lena.jpg"
#define OUTPUT_IMAGE          "result_1080p.bmp"
#define OUTPUT_IMAGE_GOLDEN   "result_1080p_golden.bmp"

using namespace std;
using namespace cv;

//extern const unsigned char gImage_green[1048576];
//extern const unsigned char gImage_lena[1048576];


int main (int argc, char** argv)
{
	IplImage* src = cvLoadImage(INPUT_IMAGE);
    IplImage* dst = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    
    AXI_STREAM  src_axi, dst_axi;
    IplImage2AXIvideo(src, src_axi);
    hls_sobel(src_axi, dst_axi, src->height, src->width);
    AXIvideo2IplImage(dst_axi, dst);
    cvSaveImage(OUTPUT_IMAGE,dst);
    cvShowImage("hls_dst", dst);
    
    opencv_image_filter(src, dst);
    cvShowImage("cv_dst", dst);
    cvSaveImage(OUTPUT_IMAGE_GOLDEN,dst);
    cvShowImage("src", src);
    waitKey(0);

    cvReleaseImage(&src);
    cvReleaseImage(&dst);


	/*
	Mat fromC(512, 512, CV_8UC3);
	for (int i = 0; i < 512; i++) {
		for (int j = 0; j < 512; j++) {
			uchar r = gImage_green[(j + i*512) * 4 + 1];
			uchar g = gImage_green[(j + i*512) * 4 + 2];
			uchar b = gImage_green[(j + i*512) * 4 + 3];

			//fromC.at<Vec3b>(i, j)[0] = (r & 0x1f) / 0x1f * 0xff;
			//fromC.at<Vec3b>(i, j)[1] = (g & 0x1f) / 0x1f * 0xff;
			//fromC.at<Vec3b>(i, j)[2] = (b & 0x1f) / 0x1f * 0xff;

			fromC.at<Vec3b>(i, j)[0] = (r & 0xf0);
			fromC.at<Vec3b>(i, j)[1] = (g & 0xf0);
			fromC.at<Vec3b>(i, j)[2] = (b & 0xf0);
		}
	}


	//imshow("green", src);
	imshow("fromc", fromC);
	waitKey();
*/
	return 0;

}
