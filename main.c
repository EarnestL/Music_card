/*
 * Author - Earnest Lin
 * Description - Sound card with flashing LEDs
 * Microcontroller - PIC16F627A
 */

// CONFIG
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSC oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB4/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = OFF       // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>
#include <pic16f627a.h>
#include <math.h>

// BITS DEF
#define BUTTON1 PORTAbits.RA0
#define LED_flash PORTAbits.RA2
#define LED_enable PORTAbits.RA3
#define BUZZER PORTBbits.RB3

// NOTE FREQUENCIES
#define null 10
#define G4	392.00
#define G4s 415.30
#define A4 440.00
#define A4s 466.16
#define B4	493.88
#define C5	523.25
#define C5s 554.37
#define D5 587.33
#define D5s 622.25
#define E5	659.25
#define F5	698.46
#define F5s 739.99
#define G5	783.99
#define G5s	830.61
#define A5	880.00
#define A5s 932.33
#define B5 987.77
#define C6 1046.50
#define C6s 1108.73
#define D6 1174.66
#define D6s 1244.51
#define E6 1318.51
#define F6 1396.91
#define F6s 1479.98
#define G6 1567.98
#define G6s 1661.22
#define A6 1760
#define A6s 1864.66
#define B6 1975.53
#define C7 2093
#define C7s 2217.46


// DUTY CYCLE
#define Percent 50

// OSC CLOCK SPEED
#define _XTAL_FREQ 4000000

//FUNCTION DECLARATION
void init();
void Freq_alt (int);
void delay_ms(unsigned int);
void buz_switch(int);

//TUNES
//HAPPY BIRTHDAY
const int hbd_array[29][2] = {{C6,350},{null,1},{C6,200},{D6,500},{C6,500},{F6,500},{E6,750}
                         ,{C6,350},{null,1},{C6,200},{D6,500},{C6,500},{G6,500},{F6,750}
                         ,{C6,350},{null,1},{C6,200},{C7s,500},{A6,500},{F6,500},{E6,550},{D6,650}
                         ,{A6s,350},{null,1},{A6s,200},{A6,500},{F6,500},{G6,500},{F6,1000}};
//PERFECT NIGHT
/*const int pn_array[45][2] = {{F6,200},{null,1},{F6,300},{null,1},{F6,200},{null,1},{A6,200},{null,1},
                        {F6,200},{null,1},{F6,300},{null,1},{F6,200},{null,1},{C6,200},{null,1},{C6,300},{null,1},
                        {C6,200},{null,1},{C6,200},{null,1},{G6,150},{null,1},{E6,200},{null,1},{E6,200},{null,1},
                        {E6,200},{null,1},{C6,200},{null,1},{C6,200},{null,1},{C6,200},{null,1},{C6,200},{null,1},
                        {G6,200},{null,1},{F6,200},{null,1},{E6,200},{null,1},{F6,800}};
*/
int size = sizeof(hbd_array)/sizeof(hbd_array[0]);

// MAIN
void main() {
    
    init();
    // Main loop begins
    while(1){
        
        if(BUTTON1 == 0){
            buz_switch(1); //sound on
            LED_enable = 1; // enable LEDs
        
            for (int i = 0; i < size; i++){ // play song and flash LEDs for each duration of notes
                LED_flash = ~LED_flash;
                Freq_alt(hbd_array[i][0]);
                delay_ms(hbd_array[i][1]);
            }
        
            LED_enable = 0; // disable LEDs
            buz_switch(0); //sound off
        }
    }
    return;
};


// Function definition
void init(){
    // Configure the comparator
    // Comparator module is turned off and used as I/O pins
    CMCONbits.CM = 0b111; // Turn off the comparator
    CMCONbits.CIS = 0; // Set the input to C1Vin-
    CMCONbits.C1INV = 0; // Output polarity is non-inverted
    CMCONbits.C2INV = 0; // Output polarity is non-inverted

    // Set IO pins
    TRISA0 = 1;
    TRISA2 = 0;
    TRISA3 = 0;
    
    // PWM configuration and frequency initialization
    TRISB3 = 0; //enable output for PWM
    T2CON = 0b00000110;  //enable timer 2 + prescaler value 16
    Freq_alt(0);
};

// Function definition
void Freq_alt(int x){
    
    PR2 = (uint8_t)(((4000000)/(16*x))-1); //period of PWM is written to PR2
    
    uint8_t duty_cycle = (uint8_t)((Percent*PR2)/100); //calculate duty cycle
    
    CCPR1L = duty_cycle >> 2; //Duty cycle is written to CCPR1L (contains the high 8 bits)
};

// Function definition
// cannot use __delay_ms() directly as it only takes constant parameter
void delay_ms(unsigned int ms){
    
   while(ms > 0){
       __delay_ms(1);
      ms--;
    }
 };

void buz_switch(int x){
    if (x){
        CCP1CONbits.CCP1M = 0b1100; // enable PWM
    }
    else{
        CCP1CONbits.CCP1M = 0b0000; // disable PWM
    };
    
};


