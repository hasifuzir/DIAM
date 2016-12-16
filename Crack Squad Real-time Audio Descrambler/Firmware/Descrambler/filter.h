/*
 * Real-time Audio Descrambler Scenario
 * Digital MSP432
 * Daniel Saul, 2015
 *
 * filter.h
 *
 */

#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

// 8kHz Bandstop Chebyshev Filter Coefficients
static float bandstop_g = 0.9873701369842;
static float bandstop_num[3] = {0.9873701369842, -1.058453023, 0.9873701369842};
static float bandstop_den[3] = {1, -1.058453022724, 0.9747402739684};
//static float bandstop_num[3] = {1, -1.0737724345776501, 1};
//static float bandstop_den[3] = {1, -0.98284543516314971, 0.83064009377319237};
//static float bandstop_num[3] = {1, -1.0719921365629379, 1};
//static float bandstop_den[3] = {1, -1.0457051642716415, 0.95095678150106822};
//float bandstop_buf[3];


// 5kHz Low-pass Chebyshev Filter Coefficients
static float lowpass_g = 0.8912509381337;

static float lowpass_a_g = 0.08716604622656;
static float lowpass_a_num[3] = {0.087166, 0.174332, 0.087166};
static float lowpass_a_den[3] = {1, -1.49955449681, 0.8482186817167};
//float lowpass_a_buf[3];

static float lowpass_b_g = 0.02362756463502;
static float lowpass_b_num[3] = {0.02362756463502, 0.04725512927, 0.02362756463502};
static float lowpass_b_den[3] = {1, -1.554785179597, 0.6492954381366};
//float lowpass_b_buf[3];


float bandstop(float input);
float lowpass(float input);
float filter(float x, float *w, float *b, float *a);
void update_buffer(float *w);

#endif

// /*
//  * Real-time Audio Descrambler Scenario
//  * Digital MSP432
//  * Daniel Saul, 2015
//  *
//  * filter.h
//  *
//  */

// #ifndef FILTER_H
// #define FILTER_H

// #include <stdint.h>
// #include <stdbool.h>
// #include <math.h>

// // 8kHz Bandstop Chebyshev Filter Coefficients
// static float bandstop_g = 0.9873701369842;
// static float bandstop_num[3] = {0.9873701369842, -1.058453023, 0.9873701369842};
// static float bandstop_den[3] = {1, -1.058453022724, 0.9747402739684};

// static float bandstop2_num[3] = {1, -1.4583978325075411, 1};
// static float bandstop2_den[3] = {1, -1.058453022724, 0.9287059849659447};
// //static float bandstop_num[3] = {1, -1.0737724345776501, 1};
// //static float bandstop_den[3] = {1, -0.98284543516314971, 0.83064009377319237};
// //static float bandstop_num[3] = {1, -1.0719921365629379, 1};
// //static float bandstop_den[3] = {1, -1.0457051642716415, 0.95095678150106822};
// //float bandstop_buf[3];


// // 5kHz Low-pass Chebyshev Filter Coefficients
// static float lowpass_g = 0.8912509381337;

// static float lowpass_a_g = 0.08716604622656;
// static float lowpass_a_num[3] = {0.087166, 0.174332, 0.087166};
// static float lowpass_a_den[3] = {1, -1.49955449681, 0.8482186817167};
// //float lowpass_a_buf[3];

// static float lowpass_b_g = 0.02362756463502;
// static float lowpass_b_num[3] = {0.02362756463502, 0.04725512927, 0.02362756463502};
// static float lowpass_b_den[3] = {1, -1.554785179597, 0.6492954381366};
// //float lowpass_b_buf[3];


// float bandstop(float input);
// float bandstop2(float input);
// float lowpass(float input);
// float filter(float x, float *w, float *b, float *a);
// void update_buffer(float *w);

// #endif
