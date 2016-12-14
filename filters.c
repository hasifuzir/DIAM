/*
 filters.c
 */
 
 #include "filters.h"
 
 //Acts as v where array corresponds to {(n-2), (n-1), n}, n is 2
 float bandstop_buffer[3] = {0, 0, 0};
 
 
 //Bandstop Filter
 //Delay is updated, output is updated
 float bandstop(float input){
	 
	 update_delay(bandstop_buffer);
	 float output = filter(input, bandstop_buffer, bandstop_num, bandstop_den);
	 
	 return output;
	 
 }
 
 
 //Direct Form 2 realisation of a 2nd Order IIR Filter
 //Equation according to Direct Form 2 IIR Filter
 //a is poles, b is zeros
 float filter(float x, float *v, float *b, float *a){
	 
 v[2] = x - (a[1] * v[1]) - (a[2] * v[0]);
 float y = (b[0] * v[2]) + (b[1] * v[2]) + (b[2] * v[0]);
 
 return y;
	 
 }
 
 
 //Update, this moves v(n) to v(n-1) and v(n-1) to v(n-2)
 void update_delay(float *v){
	 
	 v[1] = v[2];
	 v[0] = v[1];
	 
 }