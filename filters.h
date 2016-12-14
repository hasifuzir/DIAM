/*
 filters.h
 */
 
 #ifndef FILTER_H
 #define FILTER_H
 
 #include <stdint.h>
 #include <stdbool.h>
 #include <math.h>
 
 // 8kHz Bandstop (Chebyshev I) Filter
 
static float bandstop_gain = 0.91532004688659618;
static float bandstop_num[3] = {1, -1.0737724345776507, 1};
static float bandstop_den[3] = {1, -0.98284543516314971,  0.83064009377319237};

float bandstop(float input);
float filter(float x, float *v, float *b, float *a);
void update_delay(float *v);
 
 #endif
