
/**
 * \file
 *         Program for compressing a signal using DCT and decompress the signal again.  
 * \author
 *         Malthe Tøttrup <201907882@post.au.dk>
 */

#include "contiki.h"

#include <stdio.h> /* For printf() */
#include <math.h>

#define pi 3.142857
#define L 8
#define data_size 512

//delta function 
float delta(int k){
    if(k == 0){
        return 1.0;
    }
    else{
        return 0.0;
    }
}

// Function to compress a signal using discrete cosine transform, and decompress it again.  
void dct(float x[data_size], int M)
{
    int rows = L;
    int cols = data_size/L;
    int clock_H = 0;
    int clock_wo_H = 0;

    float H[rows][rows];
    float y[rows][cols];

    clock_H = clock_time();

    //compute H
    for (int k=0; k < L; k++){
        for (int n=0; n < L; n++){
            H[k][n] = sqrt((2.0-delta(k))/(float)L) * cos(pi/(float)L*(n+0.5)*k); 
        }
    }

    clock_wo_H = clock_time();
    // ------------ compress --------------
    // "matrix" mult y = Hx
    // note that x is a vector, so we taking a block of x with length L at a time 
    // additionally we need to only select first M coesfficients
    for(int i=0; i<cols; i++)    
    {
        for(int j=0; j<rows; j++)    
        {   
            y[j][i]=0;
            if (j < L-M){
                for(int k=0; k<rows; k++)    
                {   
                    y[j][i] += H[j][k]*x[k+L*i];   
                }
            }
        } 
    }

    int clock_diff_H = (clock_time() - clock_H)*1000/CLOCK_SECOND;
    int clock_diff_wo_H = (clock_time() - clock_wo_H)*1000/CLOCK_SECOND;
    printf("With H = %d, W/O H = %d\n", clock_diff_H, clock_diff_wo_H);
    

    // -------- decompress --------------
    float y_flat[data_size];
    // flattening y for no reason
    for(int i = 0; i < cols; i++){
        for (int j = 0; j < rows; j++){
            y_flat[i*L+j] = y[j][i];
        }
    }

    printf("----------------------------------------------------------------------------\n");


    float x_new[rows][cols];

    for(int i=0; i<cols; i++)    
    {
        for(int j=0; j<rows; j++)    
        {   
            x_new[j][i]=0;
            for(int k=0; k<rows; k++)    
            {   
                x_new[j][i] += H[k][j]*y_flat[k+L*i];   
            }
            //printf("%0.4f\n", x_new[j][i]); //uncomment this to see output signal (native only)
        } 
    }
}

/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{
    
    PROCESS_BEGIN();
    int M = 4;
    float x[data_size] = {
        0.3500, 0.3500, 0.3500, 0.3500, 0.3500, 0.3500, 0.3500, 0.3500, 0.3500, 0.0200, -0.2100, -0.3300,-0.3700 ,-0.3350   ,-0.2850   ,-0.2300   ,-0.2150  ,-0.2000   ,-0.1900   ,-0.2000  ,-0.2000  ,-0.2100   ,-0.2300   ,-0.2500   ,-0.2550  ,-0.2450   ,-0.2400   ,-0.2150   ,-0.2350   ,-0.2300   ,-0.2300   ,-0.2050   ,-0.1800  ,-0.1650   ,-0.1950   ,-0.2100   ,-0.2150   ,-0.1950   ,-0.1700  ,-0.1550   ,-0.1750   ,-0.1950   ,-0.2100   ,-0.2000   ,-0.2000   ,-0.1950   ,-0.1850  ,-0.1850   ,-0.1800   ,-0.1750   ,-0.1750  ,-0.1650   ,-0.1400   ,-0.1200   ,-0.1150   ,-0.1300  ,-0.1550   ,-0.1900   ,-0.1900   ,-0.1850   ,-0.1800   ,-0.1600, -0.1400   ,-0.1300   ,-0.1100   ,-0.1200  ,-0.1300   ,-0.1300   ,-0.1250  ,-0.0950   ,-0.0950   ,-0.0900   ,-0.0800   ,-0.0750   ,-0.0700   ,-0.0750   ,-0.0650   ,-0.0550   ,-0.0600   ,-0.0550   ,-0.0400   ,-0.0300   ,-0.0200   ,-0.0150   ,-0.0150   ,-0.0100   ,0.0150   ,0.0350, 0.0250   ,0.0150    ,0.0150    ,0.0250    ,0.0250    ,0.0400    ,0.0400    ,0.0300    ,0.0150    ,0.0200   ,0.0350    ,0.0450   ,0.0500    ,0.0250    ,0.0200    ,0.0200    ,0.0100    ,0.0100         ,0   ,-0.0050   ,-0.0250   ,-0.0200   ,-0.0300  ,-0.0300   ,-0.0350   ,-0.0450  ,-0.0550   ,-0.0600   ,-0.0650   ,-0.0750   ,-0.0800  ,-0.0750   ,-0.0750   ,-0.0750   ,-0.0700   ,-0.0750  ,-0.0800   ,-0.0900   ,-0.1000   ,-0.0950   ,-0.0950   ,-0.0850   ,-0.0950   ,-0.1000   ,-0.1000   ,-0.1150   ,-0.1150   ,-0.1300   ,-0.1400  ,-0.1500   ,-0.1600   ,-0.1450   ,-0.1550   ,-0.1600   ,-0.1700   ,-0.1850   ,-0.1750  ,-0.1700  ,-0.1650   ,-0.1700   ,-0.1700   ,-0.1650   ,-0.1800   ,-0.1800   ,-0.2000   ,-0.2000   ,-0.2150   ,-0.2050   ,-0.2100   ,-0.2050   ,-0.2100  ,-0.2200   ,-0.2250   ,-0.2250   ,-0.2400   ,-0.2250   ,-0.2100   ,-0.2050   ,-0.1950   ,-0.1900   ,-0.2000   ,-0.1900  ,-0.1900   ,-0.1850   ,-0.1700   ,-0.1700  ,-0.1750   ,-0.1650   ,-0.1650   ,-0.1850   ,-0.1850   ,-0.2050  ,-0.2050   ,-0.2100   ,-0.1950   ,-0.1950   ,-0.1900   ,-0.1950  ,-0.1950  ,-0.1950   ,-0.1850   ,-0.1750   ,-0.1750   ,-0.2000  ,-0.2250   ,-0.2500   ,-0.2400   ,-0.2400   ,-0.2400  ,-0.2500   ,-0.2600   ,-0.2700   ,-0.2800   ,-0.3050   ,-0.3350   ,-0.3500   ,-0.3400   ,-0.2850, -0.2050  ,-0.1350   ,-0.0950   ,-0.0450   , 0.0100   , 0.0750   ,0.1400    ,0.1750    ,0.1350    ,0.0450  ,-0.1550   ,-0.4250   ,-0.7100   ,-0.9150   ,-1.0900   ,-1.2550  ,-1.3950   ,-1.4650   ,-1.5050   ,-1.4900   ,-1.4500   ,-1.3900  ,-1.3400   ,-1.2700   ,-1.1550   ,-1.0250   ,-0.9500  ,-0.9300   ,-0.9300   ,-0.9100   ,-0.8850   ,-0.8650   ,-0.8400   ,-0.8000   ,-0.7550   ,-0.7150   ,-0.6600   ,-0.6300   ,-0.5800  ,-0.5400   ,-0.5100   ,-0.4650  ,-0.4350   ,-0.3950   ,-0.3650   ,-0.3150   ,-0.2650   ,-0.2000   ,-0.1550   ,-0.0900   ,-0.0400   ,-0.0150   ,-0.0100  , 0.0050    ,0.0300    ,0.0550    ,0.0800    ,0.0850    ,0.0900   ,0.1100   ,0.1100    ,0.1300    ,0.1500    ,0.1550    ,0.1500    ,0.1800   ,0.1850   ,0.2050    ,0.2200    ,0.2350    ,0.2350   ,0.2550    ,0.2700    ,0.2900    ,0.2900    ,0.3000    ,0.3000    ,0.3050    ,0.3250    ,0.3150    ,0.3100   ,0.3150    ,0.3150    ,0.3450    ,0.3600    ,0.3700    ,0.3750    ,0.3750    ,0.3650    ,0.3750    ,0.3850    ,0.4100    ,0.4050    ,0.4000   ,0.4000 ,0.4050    ,0.4000    ,0.4100    ,0.3950    ,0.4000    ,0.3850    ,0.3900    ,0.3950  ,0.3850    ,0.3800   ,0.3550    ,0.3550    ,0.3550    ,0.3500    ,0.3250    ,0.3000    ,0.2800    ,0.2500    ,0.2500    ,0.2500    ,0.2500    ,0.2350    ,0.2100    ,0.1750    ,0.1650    ,0.1600    ,0.1450    ,0.1400   ,0.1450    ,0.1300    ,0.1000    ,0.0800    ,0.0400    ,0.0150   ,-0.0100   ,-0.0250  ,-0.0250   ,-0.0300   ,-0.0400   ,-0.0450   ,-0.0700   ,-0.1050  ,-0.0800   ,-0.0750   ,-0.0850   ,-0.0900   ,-0.1100   ,-0.1300   ,-0.1200   ,-0.1150   ,-0.1250   ,-0.1500   ,-0.1700   ,-0.1600   ,-0.1600   ,-0.1500   ,-0.1550   ,-0.1600   ,-0.1700   ,-0.1850   ,-0.1800   ,-0.1800   ,-0.1900   ,-0.1800  ,-0.1700   ,-0.1600   ,-0.1400   ,-0.1400   ,-0.1450   ,-0.1700  ,-0.1800   ,-0.1750   ,-0.1600   ,-0.1550   ,-0.1550   ,-0.1650   ,-0.1500   ,-0.1550  ,-0.1500   ,-0.1450   ,-0.1450   ,-0.1650   ,-0.1950   ,-0.2050   ,-0.1900   ,-0.1800  ,-0.1650   ,-0.1700   ,-0.1600   ,-0.1700   ,-0.1650   ,-0.1700  ,-0.1700   ,-0.1700   ,-0.1700   ,-0.1850  ,-0.1900   ,-0.1800   ,-0.1800   ,-0.1800   ,-0.1750   ,-0.2000   ,-0.2100   ,-0.2000   ,-0.2000   ,-0.2100   ,-0.2150  ,-0.2100   ,-0.2000   ,-0.1750   ,-0.1600   ,-0.1450   ,-0.1450   ,-0.1350   ,-0.1400   ,-0.1350   ,-0.1300   ,-0.1200   ,-0.1150   ,-0.1100   ,-0.1050   ,-0.1000   ,-0.0950   ,-0.1100   ,-0.1400   ,-0.1750   ,-0.1800   ,-0.1800   ,-0.1800   ,-0.1650   ,-0.1750   ,-0.1750   ,-0.1750   ,-0.1650   ,-0.1650   ,-0.1650   ,-0.1750   ,-0.1650   ,-0.1600   ,-0.1550   ,-0.1500   ,-0.1550   ,-0.1600   ,-0.1600   ,-0.1550   ,-0.1450   ,-0.1500   ,-0.1500   ,-0.1850   ,-0.2000   ,-0.2000   ,-0.1950   ,-0.1900   ,-0.1900   ,-0.1950   ,-0.2000   ,-0.1950   ,-0.1850   ,-0.1750   ,-0.1750   ,-0.1550   ,-0.1700   ,-0.1700   ,-0.1650   ,-0.1500   ,-0.1600   ,-0.1650   ,-0.1750   ,-0.1700   ,-0.1700   ,-0.1850   ,-0.2150   ,-0.2250   ,-0.1900   ,-0.1250   ,-0.0400   ,0.0500    ,0.1250    ,0.2150    ,0.3200    ,0.4150    ,0.5000    ,0.6050    ,0.7250    ,0.8450    ,0.9400    ,1.0600    ,1.1100    ,1.1000    ,1.0050    ,0.8600    ,0.6150    ,0.2400   ,-0.1100   ,-0.3350   ,-0.4600  ,-0.5150   ,-0.5400   ,-0.5100   ,-0.4400,-0.3850,-0.3600   ,-0.3500   ,-0.3350,-0.3200,-0.2950,-0.3000,-0.3000,-0.3250,-0.3350,-0.3350
    };
    
    dct(x, M);

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
