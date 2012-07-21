// #include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include "CC1100LIB.C"
/** Constants */
#define F_CPU 1000000U

// Global Variables 
char cData = ' ';

/** This function needs to setup the variables used by the UART to enable the UART and tramsmit at 9600bps. This 
function should always return 0. Remember, by defualt the Wunderboard runs at 1mHz for its system clock.*/
unsigned char InitializeUART (void){

    /* Set baud rate */
    UBRR1H = 0;
    UBRR1L = 12;
    /* Set the U2X1 bit */
    UCSR1A = UCSR1A | 0b00000010 ;
    /* Enable transmitter */
    UCSR1B = UCSR1B | 0b00001000;
    /*Enable the Reciever*/
    UCSR1B = UCSR1B | 0b00010000;
    /* Set frame format: 8data, 1stop bit */
    UCSR1C = 0b00000110;
    return 0;
}

/** This function needs to write a single byte to the UART. It must check that the UART is ready for a new byte 
and return a 1 if the byte was not sent. 
@param [in] data This is the data byte to be sent.
@return The function returns a 1 or error and 0 on successful completion.*/

unsigned char SendByteUART (unsigned char data){
    if( !( UCSR1A & (1<<UDRE1)) ){
        return 1;
    }
    UDR1 = data;
    return 0;
}

/** This function needs to writes a string to the UART. It must check that the UART is ready for a new byte and 
return a 1 if the string was not sent. 
@param [in] data This is a pointer to the data to be sent.
@return The function returns a 1 or error and 0 on successful completion.*/

unsigned char SendStringUART (unsigned char *data){
    int i = 0;
    while(data[i] != '\0'){
        while(SendByteUART(data[i])){}
        _delay_ms(10);
        i++;
    }
    return 0;
}

/** This function needs to setup the variables used by TIMER0 Compare Match (CTC) mode with 
a base clock frequency of clk/1024. This function should return a 1 if it fails and a 0 if it 
does not. Remember, by default the Wunderboard runs at 1mHz for its system clock.
@return This function returns a 1 is unsuccessful, else return 0.*/
unsigned char initializeTIMER0(void)
{
    /* Set the CTC mode */
    TCCR0A = (1 << WGM01);
    
    /* Set the Clock Frequency */
    TCCR0B = ((1 << CS02) | (1 << CS00));
    
    /* Set initial count value */
    OCR0A = 0x00;
    
    return 0;
}

/** This function checks if TIMER0 has elapsed. 
@return This function should return a 1 if the timer has elapsed, else return 0*/
unsigned char checkTIMER0(void)
{
    if (TIFR0 & (1 << OCF0A))
    {
        TIFR0 |= (1 << OCF0A);
        return 1;
    }
    else
    {
        return 0;
    }
}

/** This function takes two values, clock and count. The value of count should be copied into OCR0A and the value of clock should be used to set CS02:0. The TCNT0 variable should also be reset to 0 so that the new timer rate starts from 0.  
@param [in] clock Insert Comment
@param [in] count Insert Comment
@return The function returns a 1 or error and 0 on successful completion.*/
unsigned char setTIMER0(unsigned char clock, unsigned char count)
{
    // Check for invalid clock select
    if (clock & ~0x07)
    {
        return 1;
    }
    
    OCR0A = count;
    TCCR0B &= ~(0x07);
    TCCR0B |= clock;
    TCNT0 = 0x00;
    
    return 0;
}

void ANT_Init(void)
{
    //BYTE xdata regValues[] = {1,2,3}; 
    BYTE xdata regdata[21][2];
    // Register Address | Register Value
    regdata[0][0]=0x0002; regdata[0][1]=0x06;
    regdata[1][0]=0x0003; regdata[1][1]=0x47;
    regdata[2][0]=0x0008; regdata[2][1]=0x05;
    regdata[3][0]=0x000B; regdata[3][1]=0x06;
    regdata[4][0]=0x000D; regdata[4][1]=0x10;
    regdata[5][0]=0x000E; regdata[5][1]=0xB1;
    regdata[6][0]=0x000F; regdata[6][1]=0x3B;
    regdata[7][0]=0x0010; regdata[7][1]=0xF6;
    regdata[8][0]=0x0011; regdata[8][1]=0x83;
    regdata[9][0]=0x0012; regdata[9][1]=0x13;
    regdata[10][0]=0x0015; regdata[10][1]=0x15;
    regdata[11][0]=0x0018; regdata[11][1]=0x18;
    regdata[12][0]=0x0019; regdata[12][1]=0x16;
    regdata[13][0]=0x0020; regdata[13][1]=0xFB;
    regdata[14][0]=0x0023; regdata[14][1]=0xE9;
    regdata[15][0]=0x0024; regdata[15][1]=0x2A;
    regdata[16][0]=0x0025; regdata[16][1]=0x00;
    regdata[17][0]=0x0026; regdata[17][1]=0x1F;
    regdata[18][0]=0x002C; regdata[18][1]=0x81;
    regdata[19][0]=0x002D; regdata[19][1]=0x35;
    regdata[20][0]=0x002E; regdata[20][1]=0x09;
    
    char x = 0;
    
    for(x = 0; x < 21; x++)
    {
        halSpiWriteBurstReg(regdata[x][0], regdata[x][1], sizeof(regdata[x][1]));
    }
        
}

void SPI_MasterInit(void)
{
    /* Set MOSI and SCK output, all others input */
    DDRB = (1<<DDB2)|(1<<DDB1);
    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}
void SPI_MasterTransmit(char cData)
{
    /* Start transmission */
    SPDR = cData;
    /* Wait for transmission complete */
    while(!(SPSR & (1<<SPIF)))
    ;
}
char SPI_MasterRecieve(char cData)
{
    SPDR = cData;                    //Load byte to Data register
    while(!(SPSR & (1<<SPIF)));     // Wait for transmission complete 
    cData=SPDR;
    return cData;
}


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
    PORTB &= ~((1 << PB6) | (1 << PB7));    // Disable latches
    PORTC = 0x00;
    PORTB |= (1 << PB6) | (1 << PB7);       // Enable latches
    PORTB &= ~((1 << PB6) | (1 << PB7));    // Disable latches
}
int main() {
    char recChar;
    initialize();
    clearArray();
    initializeTIMER0();
    // Initialize file system, check for errors
    InitializeUART();
    
    SendStringUART("Booting Up\n");
   
    // Define STATE enumeration and declare a current state variable
    enum STATE
    {
        intialize,
        idle,
        sending
    };
    enum STATE current_state;
    
    // Clk/64 and CTC mode
    TCCR1A = 0b01000000;
    TCCR1B = 0b00001011;
    
    // Intial LED Config
    PORTC=0b11111111;//Initial LED value
    PORTB=0b10000000;//Color is green
    
    // Store recieved character
    //recChar = xxxx;
    current_state = intialize;
    char sendping = ' ';
    char rec = ' ';
    for(;;)
    {
        // Get new input
        //recChar = xxxxx;
        
        switch(current_state)
        {
            case intialize:
                SendStringUART("Intializing RF Antenna\n");
                SPI_MasterInit();
                current_state = idle;
                break;
            case idle:
                SendStringUART("Idling..\n");
                rec=SPI_MasterRecieve(0b);
                SendStringUART("Char recieved: \n");
                SendByteUART(rec);
                if(rec = 'a')
                {
                    current_state = sending;
                }
                sendping = UDR1;
                if(sendping = 'a')
                {
                    SPI_MasterTransmit('a');
                }
                
                break;
            case sending:
                SPI_MasterTransmit('b');
                current_state=idle;
                break;
            default:
                SendStringUART("Invalid state\n");
                break;
        } 
    }
}
/* Interrupts ----
isr(status pin)
{   
    

}


*/