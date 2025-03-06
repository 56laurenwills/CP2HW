#include "fit.h"
#include <stdio.h>
#include "ap_int.h"
#include "ap_fixed.h"
#include <iostream>
#include <bitset>
// I heavily relied on Hayden's examples for the bit assigning stuff
// I could not find an value larger than ~1200, so 12 bits should suffice
// The largest sum values was ~28,000 , so 16 bit signed should work
// As far as I could tell- all sigmas and x's are positive
// Some y's were negative

// Also the t_i value is making this SOOOOOOO difficult, because now I need to keep track of indices >:(




extern "C"{
	void fit(hls::stream<int>& x_array, hls::stream<int>& y_array,  hls::stream<int>& sigma_y_array, hls::stream<int>& last, int N){
#define MAX_INPUTS = 1326
		ap_uint<24> num_inputs = 0;
		ap_uint<24> index = 0;
		ap_fixed<24,16> sum_x = 0;
		ap_fixed<24,16> sum_y = 0;
		ap_fixed<24,16> sum = 0;
		ap_fixed<24,16> sum_t = 0;
		ap_fixed<24,16> sum_ty = 0;
		//ap_fixed<24,16> sigma_indexed[MAX_INPUTS];
		//ap_fixed<24,16> x_indexed[MAX_INPUTS];
		//ap_fixed<24,16> t_indexed[MAX_INPUTS];
		//I HAVE NO IDEA WHAT THE ERROR IS HERE AND I AM LOSTING MY MIND


		for (int i = 0; i < N; i++){
			ap_fixed<24,16> xs = x_array.read();
			ap_fixed<24,16> ys = y_array.read();
			ap_fixed<24,16> y_sigmas = sigma_y_array.read();
			ap_fixed<24,16> lasts = last.read();

			//x_indexed[num_inputs] = xs
			//sigma_indexed[num_inputs] = y_sigmas

			ap_fixed<24,16> y_sigmas_2 = (y_sigmas) * (y_sigmas);
			ap_fixed<24,16> inv_y_sigmas_2 = ap_fixed<24,16>(1.0) / y_sigmas_2;




			if (lasts == 1){
				for (int j = 0; j < num_inputs; j++){
					sum_x+= xs * inv_y_sigmas_2;
					sum_y+= ys * inv_y_sigmas_2;
					sum += ap_fixed<24,16>(1.0) / (inv_y_sigmas_2);
					//ap_fixed<24,16> xj = x_indexed[j];
					//ap_fixed<24,16> sigmaj = sigma_indexed[j];
					//ap_fixed<24,16> tj = t_indexed[j];
					ap_fixed<24,16> tj = ap_fixed<24,16>(1.0/y_sigmas)*(xs-sum_x/sum);
					sum_t += tj * tj;
					sum_ty += tj * ys * ap_fixed<24,16>(1.0/y_sigmas);
				}
				//do all of the calculations
				ap_fixed<24,16> b = (ap_fixed<24,16>(1.0)/ sum_t)*sum_ty;
				ap_fixed<24,16> sigma_b_2 = ap_fixed<24,16>(1.0)/ sum_t;
				ap_fixed<24,16> a = (sum_y - sum_x*b) / sum;
				ap_fixed<24,16> sigma_a_2 = ap_fixed<24,16>(1.0)/sum * (ap_fixed<24,16>(1.0) + ((sum_x*sum_x)/(sum * sum_t)));

				//for (int k = 0; k < num_inputs; k++){
					//ap_fixed<24,16> chi_sum = (y_indexed[k] - a - b* x_indexed[k]) / sigma_indexed[k];
					// ap_fixed<24,16> chi_sum_2 = chi_sum * chi_sum

				//}
				//ap_fixed<24,16> chi_2 = ap_fixed<24,16>(1.0) / (num_inputs -2) * chi_sum_2
				std::cout << a  << "+/-" << sigma_a_2;
				std::cout << b  << "+/-" << sigma_b_2;
				//std::cout << chi_2  << chi_2;


				//I could do not get the indexing to work and I have no idea why and I have spent so many hours trying
				// to figure it out and I am giving up. None of my numbers are correct or make any sense for that reason.
				// I am not even going to try to synthesize. I am giving up.


 		}
		}



	}
}
