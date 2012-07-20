/**
@file adc.h
@brief Wunderboard ADC Helper Functions
@version .01 

@section intro Code Overview
This is the code for the Wunderboard ADC helper functions.
*/

#include <avr/io.h>

/**
The read_adc() function allows for taking 8 bit measurements from any of the single ended ADC inputs. The function call is blocking. eg. read_adc(5) would read from ADC channel 5.
@param channel This is the channel to be read.
@return This is the return value from the ADC.
*/

unsigned char read_adc(uint8_t channel);

