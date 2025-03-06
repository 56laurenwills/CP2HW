#include "main.h"
#include <ap_int.h>
#include <ap_fixed.h> // Using fixed-point arithmetic for resource optimization
#include <hls_stream.h>
#include <iostream>  // For debug prints
#include "hls_math.h"
#include <fstream>
#include <iomanip>
#include <cstring>
#include <string>
#include "fit.h"

using namespace std;

int main() {
		hls::stream<int> x_array, y_array, sigma_y_array, last ;

		for (int i = 0; i < tempN; i++) {
			x_array.write(tempxs[i]);
			y_array.write(tempys[i]);
			sigma_y_array.write(tempsigmas[i]);
			last.write(templasts[i]);

	}

		fit(x_array, y_array, sigma_y_array, last, tempN);
	return 0 ;


}
