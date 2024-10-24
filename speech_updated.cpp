// speech_updated.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <cmath>
#include <cstdio>

// Function to perform wave normalization
double wave_normalization(double *content, int size, int limit = 5000) {
    // Find the maximum value in the content array
    double max_val = fabs(content[0]);
    for (int i = 1; i < size; i++) {
        if (fabs(content[i]) > max_val) {
            max_val = fabs(content[i]);
        }
    }

    // Calculate the scale factor and normalize the content array
    double scale_factor = 1.0;
    if (max_val > limit) {
        scale_factor = limit / max_val;
         
    }

    return scale_factor;
}

const char* analyse_zcr(int total_zcr_first_70, int total_zcr_last_30) {
    

    if (total_zcr_last_30 > total_zcr_first_70) {
        return "YES";
    } else {
        return "NO";
    }
}

int _tmain(int argc, _TCHAR* argv[]) {
    FILE *fptr;
    fptr = fopen("yes_sample_3.txt", "r");  // Open file in read mode
    double content[30000];
    int i = 0;
    double dc;
    double dc_1;
    double sum = 0;
	double energy=0;
	int zcr=0; 


    if (fptr != NULL) {
        while (fscanf(fptr, "%lf", &content[i]) == 1 && i < 30000) {
            i++;
        }
        printf("Number of elements inserted = %d\n", i); 

        int batches = i / 320;
		int total_zcr_first_70 = 0;
        int total_zcr_last_30 = 0;

        // Calculate the starting batch index for the last 30%
        int last_30_percent_start = ceil(batches * 0.4);

        for (int batch = 0; batch < batches; batch++) {
            dc = 0;
            for (int p = batch * 320; p < (batch + 1) * 320; p++) {
                dc += content[p];
            }
            dc_1 = dc / 320;
            sum += dc_1;
        }
        sum = sum / batches;
        double normalization_factor = wave_normalization(content, i);
        printf("Normalization factor = %.6f\n", normalization_factor);
		//substraction and normalization of samples and energy and zcr calculation for each batch
		printf("DC Shift corrected from each sample:%f; ",sum);
		for (int batch = 0; batch < batches; batch++) {	
			energy=0;
			zcr=0;
            for (int p = batch * 320; p < (batch + 1) * 320; p++) {
                content[p]=(content[p]-sum)*normalization_factor;
				energy=energy+(content[p]*content[p]);
				if(p>batch*320 && content[p]*content[p-1]<=0){
				zcr++;
				}
            }
			double avg_energy=energy/320;
			printf("for batch %d: energy:%f zcr:%d\n",batch+1,avg_energy,zcr);
			
			 if (batch >= last_30_percent_start) {
                total_zcr_last_30 += zcr;
            
            } else {
                total_zcr_first_70 += zcr;
               
            }
        }

		const char* result = analyse_zcr(total_zcr_first_70, total_zcr_last_30);
        printf("Detected word is: %s\n", result);
        
    } else {
        printf("File Open Unsuccessful\n");
    }

    fclose(fptr);
    return 0;
}

