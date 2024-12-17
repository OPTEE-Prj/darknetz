#ifndef UTILS_H_TA
#define UTILS_H_TA
#include <stdio.h>
#include "darknet_TA.h"

float sum_array_TA(float *a, int n);
float rand_uniform_TA(float min, float max);
float rand_normal_TA(float mu, float sigma);
float mag_array_TA(float *a, int n);
int int_index_TA(int *a, int val, int n);

#endif
