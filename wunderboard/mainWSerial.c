#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <util/delay.h>
/** Constants */
#define F_CPU 1000000U
#define outp(a, b) b = a
int history[2]={0,0};

void initialize (void) {
	/** Port A is the switches and buttons. They should always be inputs. ( 0 = Input and 1 = Output )*/
	DDRA=0b00000000;

	/** Port B has the LED Array color control, SD card, and audio-out on it. Leave DDRB alone. ( 0 = Input and 1 = Output )*/
	DDRB=0b11000000;

	/** Port C is for the 'row' of the LED array. They should always be outputs. ( 0 = Input and 1 = Output )*/
	DDRC=0b11111111;

	/** Port D has the Serial on it. Leave DDRB alone. ( 0 = Input and 1 = Output )*/
	DDRD=0b00000000;

	/** Port E has the LED Array Column control out on it. Leave DDRE alone. ( 0 = Input and 1 = Output )*/
	DDRE=0b00000111;

	/** Port F has the accelerometer and audio-in on it. Leave DDRF alone. ( 0 = Input and 1 = Output )*/
	DDRF=0b00000000;
}

void clearArray(void)
{
	PORTB &= ~((1 << PB6) | (1 << PB7));	// Disable latches
	PORTC = 0x00;
	PORTB |= (1 << PB6) | (1 << PB7);		// Enable latches
	PORTB &= ~((1 << PB6) | (1 << PB7));	// Disable latches
}
int main() {
	initialize();
	clearArray();
	EIMSK|=(1<<INT0);
	EIMSK|=(1<<INT1);
	EIMSK|=(1<<PCINT4);
	
    MCUCR |= (1<<ISC10);// Rising-Edge Triggered INT0 
	MCUCR |= (1<<ISC10);
	
	PORTC=0b00001000;
	PORTB=0b10000000;
	
    sei(); // Enable Interrupts

    for(;;){
    // nothing happens.
    }
}

ISR(INT0_vect) {
    // this function is called when INT0 bit (PD0) is interrupted.
	if(history[0]==1&&history[1]==2){
		PORTC++;
	}else if(history[0]==2&&history[1]==1){
		PORTC--;
	}
	history[0]=history[1];
	history[1]=0;
}
ISR(INT1_vect) {
    // this function is called when INT1 bit (PD1) is interrupted.
	if(history[0]==2&&history[1]==0){
		PORTC++;
	}else if(history[0]==0&&history[1]==1){
		PORTC--;
	}
	history[0]=history[1];
	history[1]=1;
}
ISR(PCINT4_vect) {
    // this function is called when INT2 bit (PD2) is interrupted.
	if((PINB & 0b00010000) == 0b00010000){
		if(history[0]==0&&history[1]==1){
			PORTC++;
		}else if(history[0]==1&&history[1]==0){
			PORTC--;
		}
		history[0]=history[1];
		history[1]=1;
	}
}
