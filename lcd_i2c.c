#include "i2c.h"
#include <xc.h>
#define _XTAL_FREQ 1000000
//#define CONTRAST 0x20  //for 3.3V
#define CONTRAST 0x08  //for 5V
const char lcd_DDRAM[4] = {0x00, 0x40, 0x14, 0x54};

void lcd_INST(char command){
    I2C_Master_Start();
    I2C_Master_Write(0x7C);     //SlaveAdress + Write
    I2C_Master_Write(0x00);     //Instruction
    I2C_Master_Write(command);  //data byte
    I2C_Master_Stop();
    __delay_us(30);
}

void lcd_DATA(char data){
    I2C_Master_Start();
    I2C_Master_Write(0x7C);     //SlaveAdress + Write
    I2C_Master_Write(0x40);     //Data
    I2C_Master_Write(data);     //data byte
    I2C_Master_Stop();
    __delay_us(30);
}

void lcd_Init(){
    __delay_ms(40);
    lcd_INST(0x38);      //Function Set
    lcd_INST(0x39);      //Function Set
    lcd_INST(0x14);      //Internal OSC Frequency
    lcd_INST(0x70 + (CONTRAST & 0x0f));        //Contrast Set
    lcd_INST(0x54 + ((CONTRAST & 0xf0)>>4));   //Power/ICON/Contrast control
    lcd_INST(0x6c);      //Follower control
    __delay_ms(200);
    lcd_INST(0x38);      //Function Set
    lcd_INST(0x0c);      //Display ON/OFF control
    lcd_INST(0x01);      //Clear Display
    __delay_ms(2);
}

void lcd_SetCursor(char row, char column){                      //行、列は0番目から
    const char lcd_DDRAM[4] = {0x00, 0x40, 0x14, 0x54};         //各行の先頭アドレス
    char adrs = lcd_DDRAM[row] + column;                        //指定位置のアドレス
    lcd_INST(0x80 + adrs);
}