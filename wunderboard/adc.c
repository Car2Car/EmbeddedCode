/**
@file adc.c
@brief Wunderboard ADC Helper Functions
@version .01 

@section intro Code Overview
This is the code for the Wunderboard ADC helper functions.
*/

#include "adc.h"

unsigned char read_adc(uint8_t channel){

	unsigned char test;

	ADMUX = 0x60 | channel; // Set the channel to the one we want
	ADCSRA = 0b11000110; // Start a new sample.
	while ((ADCSRA & 0b00010000) == 0 ); // Wait for a Valid Sample
	ADCSRA |= 0b00010000; // Tell ADC you have the sample you want.
	ADCSRA |= 0b01000000; // Start a new sample.
	while ((ADCSRA & 0b00010000) == 0 ); // Wait for a Valid Sample
	ADCSRA |= 0b00010000; // Tell ADC you have the sample you want.
	
	test = ADCH; 
	ADCSRA = 0x00; // Disable the ADC

	return (test);
}
