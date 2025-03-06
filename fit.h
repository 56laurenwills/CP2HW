#ifndef FIT_H
#define FIT_H

#include <hls_stream.h>
extern "C" {
	void fit(hls::stream<int>& x_array, hls::stream<int>& y_array,  hls::stream<int>& sigma_y_array, hls::stream<int>& last, int N);
}

#endif
