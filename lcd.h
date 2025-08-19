//lcd.h
//lcd.h

#include"types.h"
void writeLCD(u8 byte);
void cmdLCD(u8 cmd);
void charLCD(u8 asii);
void InitLCD(void);
void strLCD(s8*);
void U32LCD(u32);
void S32LCD(s32);
void F32LCD(f32,u8 udp);
void BuildCGRAM(u8*,u8);
//void BuildCGRAM(u8[][8],u8);
void print_num(signed char num);
void Display5BarFuel(u8 percentage);
//void D64LCD(d32);


/*
#include"types.h"
void writeLCD(u8 byte);
void cmdLCD(u8 cmd);
void charLCD(u8 ascii);
void InitLCD(void);
void strLCD(s8*s);
void U32LCD(u32);
void S32LCD(s32);
void F32LCD(f32 fNum,u8 nDp);
void lcd_print_num(signed char num);
void Display5BarFuel(u8 percentage);
//void D64LCD(d64);
//void Build(CGRAM(s8*,u8));
*/


