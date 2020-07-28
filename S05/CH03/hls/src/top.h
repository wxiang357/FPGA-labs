/***************************************************************************
***************************************************************************/

#ifndef _TOP_H_
#define _TOP_H_

#include "hls_video.h"

// maximum image size
#define MAX_WIDTH  512
#define MAX_HEIGHT 512

// typedef video library core structures
typedef hls::stream<ap_axiu<32,1,1,1> >               AXI_STREAM;
typedef hls::Scalar<3, unsigned char>                 RGB_PIXEL;
typedef hls::Mat<MAX_HEIGHT, MAX_WIDTH, HLS_8UC3>     RGB_IMAGE;

// top level function for HW synthesis
void hls_sobel(AXI_STREAM& src_axi, AXI_STREAM& dst_axi, int rows, int cols);

#endif
