#pragma config FEXTOSC = OFF    // FEXTOSC External Oscillator mode Selection bits (Oscillator not enabled)
#pragma config RSTOSC = HFINT1  // Power-up default value for COSC bits (HFINTOSC (1MHz))
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled; I/O or oscillator function on OSC2)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)
#pragma config MCLRE = ON       // Master Clear Enable bit (MCLR/VPP pin function is MCLR; Weak pull-up enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config WDTE = OFF       // Watchdog Timer Enable bits (WDT disabled; SWDTEN is ignored)
#pragma config LPBOREN = OFF    // Low-power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bits (Brown-out Reset enabled, SBOREN bit ignored)
#pragma config BORV = LOW       // Brown-out Reset Voltage selection bit (Brown-out voltage (Vbor) set to 2.45V)
#pragma config PPS1WAY = ON     // PPSLOCK bit One-Way Set Enable bit (The PPSLOCK bit can be cleared and set only once; PPS registers remain locked after one clear/set cycle)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will cause a Reset)
#pragma config DEBUG = OFF      // Debugger enable bit (Background debugger disabled)
#pragma config WRT = OFF        // User NVM self-write protection bits (Write protection off)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (High Voltage on MCLR/VPP must be used for programming.)
#pragma config CP = OFF         // User NVM Program Memory Code Protection bit (User NVM code protection disabled)
#pragma config CPD = OFF        // Data NVM Memory Code Protection bit (Data NVM code protection disabled)

#include <xc.h>
#include <stdio.h>
#include "i2c.h"
#include "lcd_i2c.h"

#define _XTAL_FREQ 1000000

#define A RC5
#define B RC4

volatile signed char rot = 0;
volatile char rotFlag = 0;

void Init(){
    TRISA  = 0b00001000;
    ANSELA = 0b00000000;
        
    TRISC  = 0b00110011;
    IOCCN  = 0b00010000;    //Negative (H->L)割り込み
    ANSELC = 0b00000000;
    
    IOCIE = 1;              //IOC割り込み許可
    
    PEIE = 1;
    GIE = 1;
}

void __interrupt isr(){
    GIE = 0;
    if(IOCCF4){         //RC4:B H->Lの変化
        IOCCF4 = 0;
        
        if(A == 1)      //RC5:A
            rot--;
        else
            rot++;
        
        rotFlag = 1;
    }
    GIE = 1;
}

void putch(char c){
    lcd_DATA(c);
}

void main(void) {
    Init();
    I2C_Master_Init(100000);
    lcd_Init();
    
    lcd_SetCursor(0,0);
    printf("Rotary Encoder");
    
    while(1){
        if(rotFlag){
            rotFlag = 0;
            
            lcd_SetCursor(1, 0);
            printf("%d  ",rot);
        }
    }
}
