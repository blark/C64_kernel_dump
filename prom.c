/*
 * C64 Kernel Rom Reader
 *
 */

#include <p18F4520.h>
#include <delays.h>
#include <usart.h>
#include <stdlib.h>
#include <stdio.h>

//most of these likely aren't needed but the code i ripped off the internet
//while debugging the serial connection had all of this, so meh why not.
#pragma config OSC = INTIO67    //internal oscillator
#pragma config FCMEN = OFF   //no fail safe clock monitor
#pragma config IESO = OFF   //oscillator switchover disabled
#pragma config PWRT = ON           //oscillator power up timer enabled (release version only)
#pragma config WDT = OFF   //watchdog timer disabled
#pragma config MCLRE = OFF    //MCLR pin 
#pragma config LPT1OSC = ON  //timer1 low power operation
#pragma config PBADEN = OFF   //portB 0to 4 digital - not analogue
#pragma config LVP = OFF     //low voltage programming disabled
#pragma config XINST = OFF         //do not allow PIC18 extended instructions
#pragma config  STVREN = ON         //stack overflow will cause reset
#pragma config CP0 = OFF      //code protection block 0
#pragma config CP1 = OFF
#pragma config CP2 = OFF
#pragma config CP3 = OFF
#pragma config CPB = OFF      //code protection boot block
#pragma config CPD = OFF      //code protection data EEPROM
#pragma config WRT0 = OFF      //write protection block 0
#pragma config WRT1 = OFF
#pragma config WRT2 = OFF
#pragma config WRT3 = OFF
#pragma config WRTB = OFF      //write protection boot block
#pragma config WRTC = OFF      //write protection configuration register
#pragma config WRTD = OFF      //write protection data EEPROM
#pragma config EBTR0 = OFF      //table read protection block 0
#pragma config EBTR1 = OFF
#pragma config EBTR2 = OFF
#pragma config EBTR3 = OFF
#pragma config EBTRB = OFF      //table read protection boot block 

void init() {
    //it's definitely important to set this...i found out after an hour of 
    //screwing around :)
    OSCCON = 0b01110010; //select 8 MHz clock (0x72)
    while(!OSCCONbits.IOFS); //wait for OSC to stabilize
    TRISA=0; 
    TRISB=0xFF; // port b is our 8 bit input port
    TRISC=0; 
    TRISD=0;
    TRISE=0; // CS is on E0 - hold it low
    LATA=0; // A8-A12
    LATC=0;
    LATD=0; // A0-A7 here
    LATE=0;
    ADCON1 = 0b00001111; //set all pins to digital mode
    /* Interrupt Setup */
    INTCON = 0x00; /* Clear Registers */
    PIR1 = 0x00;
    PIE1 = 0x00;
    TMR1L = 0x00;
    TMR1H = 0x00;
    T1CON = 0x00;
    RCON = 0x00;
    OpenUSART(USART_TX_INT_OFF &
            USART_RX_INT_OFF &
            USART_ASYNCH_MODE &
            USART_EIGHT_BIT &
            USART_CONT_RX &
            USART_BRGH_LOW, 12); // to get baud rate use Fosc/(baud rate*64)-1...8000000/(9600*64)-1
}

void main() {
    int i;
    int o;
    char buf1[35];
    char buf2[35];
    init();
    //putrsUSART( "\r\n\r\nCommodore 64 Kernel Rom Dump Test:\r\n\r\n" );
    Delay10KTCYx(15000);
    while (1)
    {
        // outer loop takes care of incrementing bits on LATA (a8-a12)
        // we can do this only because the lines are hooked up in order so
        // there is no need for binary math headaches. yes I'm lazy.
        for (o=0; o<32; o++){
            LATA=o;
            while(BusyUSART()); // waits for serial to finish
            // inner loop increments bits on a0-a7
            for (i=0; i<256; i++){
                LATD=i;
                Delay10KTCYx(10);
                sprintf(buf2, "\\x%02x", PORTB); //prints hex value formatted for python to serial
                putsUSART(buf2);
                while(BusyUSART());
            }
        }
        //putrsUSART("PROM DUMP COMPLETE.");
        while(1);
    }
}