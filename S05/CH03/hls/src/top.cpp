/***************************************************************************/
#include "top.h"

void hls_sobel(AXI_STREAM& INPUT_STREAM, AXI_STREAM& OUTPUT_STREAM, int rows, int cols)
{
//Create AXI streaming interfaces for the core
#pragma HLS INTERFACE axis port=INPUT_STREAM
#pragma HLS INTERFACE axis port=OUTPUT_STREAM

#pragma HLS INTERFACE s_axilite port=rows
#pragma HLS INTERFACE s_axilite port=cols
#pragma HLS INTERFACE s_axilite port=return

#pragma HLS INTERFACE ap_stable port=rows
#pragma HLS INTERFACE ap_stable port=cols

    RGB_IMAGE img_0(rows, cols);
    RGB_IMAGE img_1(rows, cols);
    RGB_IMAGE img_2(rows, cols);
    RGB_IMAGE img_3(rows, cols);
    RGB_IMAGE img_4(rows, cols);
    RGB_IMAGE img_5(rows, cols);
    RGB_IMAGE img_6(rows, cols);
    RGB_IMAGE img_7(rows, cols);
    RGB_IMAGE img_8(rows, cols);
    RGB_IMAGE img_9(rows, cols);
    RGB_PIXEL pix(50, 50, 50);

#pragma HLS dataflow
    hls::AXIvideo2Mat(INPUT_STREAM, img_0);
    hls::Duplicate(img_0, img_1, img_2);

    hls::Sobel<0,1,3>(img_1, img_3);
    hls::Sobel<1,0,3>(img_2, img_4);

    hls::AddWeighted(img_3, 1, img_4, 1, 0, img_5);

    hls::SubS(img_5, pix, img_6);
    hls::Scale(img_6, img_7, 2, 0);
    hls::Erode(img_7, img_8);
    hls::Dilate(img_8, img_9);
    hls::Mat2AXIvideo(img_9, OUTPUT_STREAM);
}
