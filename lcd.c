//lcd.c
//lcd.c
#include <LPC21xx.h>
#include "types.h"
#include "delay.h"
#include "lcd.h"
#include "lcd_defines.h"

extern u8 cgramLUT[][8];//={0X00,0x04,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0x00};

void writeLCD(u8 byte)
{
        IOCLR0=1<<LCD_RW;

        IOPIN0=((IOPIN0&~(0XFF<<LCD_DATA))|
                        (byte<<LCD_DATA));

        IOSET0=1<<LCD_EN;

        IOCLR0=1<<LCD_EN;

        delay_ms(2);
}

void cmdLCD(u8 cmd)
{
        IOCLR0=1<<LCD_RS;
        writeLCD(cmd);
}

void charLCD(u8 ascii)
{
        IOSET0=1<<LCD_RS;

        writeLCD(ascii);
}

void InitLCD(void)
{
        IODIR0|=((0XFF<<LCD_DATA)|(1<<LCD_RS)|
					(1<<LCD_RW)|(1<<LCD_EN));

        delay_ms(15);
        cmdLCD(0X30);
        cmdLCD(0X30);
        cmdLCD(0X30);

        cmdLCD(MODE_8BIT_2LINE);
        cmdLCD(DSP_ON_CUR_OFF);//ON//BLK
        cmdLCD(CLEAR_LCD);
        cmdLCD(SHIFT_CUR_RIGHT);
}

void strLCD(s8*s)
{
        while(*s)
                charLCD(*s++);
}

void U32LCD(u32 n)
{
        s32 i=0;
        u8 digit[10];
        if(n==0)
        {
                strLCD(" ");
        }
        else
        {
                while(n>0)
                {
                        digit[i]=(n%10)+48;
                        n=n/10;
                        i++;
                }
                for(--i;i>=0;i--)
                {
                        charLCD(digit[i]);
                }
        }
}

void S32LCD(s32 n)
{
        if(n<0)
        {
                charLCD('-');
                n=-n;
        }
        U32LCD(n);
}

void F32LCD(f32 fNum,u8 nDp)
{
        s32 i;
        u32 n;
        if(fNum<0)
        {
                charLCD(' ');
                fNum=-fNum;
        }
        n=fNum;
        U32LCD(n);
        charLCD('.');
        for(i=0;i<nDp;i++)
        {
                fNum=(fNum-n)*10;
                n=fNum;
                charLCD(n+48);
        }
}


void print_num(signed char num)
{
        char buf[5];
        int i = 0;

        if (num < 0) {
                charLCD('-');
                num = -num;
        }

        if (num == 0)
        {
                charLCD('0');
                return;
        }

        while (num > 0)
        {
                buf[i++] = (num % 10) + '0';
                num /= 10;
        }
        while (--i >= 0)
        {
                charLCD(buf[i]);
        }
}

//void D64LCD(d64)

/*
void BuildCGRAM(u8 *p,u8 nBytes)
{
        u8 i;
        cmdLCD(0x40);
        for(i=0;i<nBytes;i++)
        {
                charLCD(p[i]);
        }
      //  cmdLCD(0x80);
}
*/
void lcd_print_num(signed char num)
{
        char buf[100]; // Enough for "-128" + null terminator
        int i = 0;

        // Handle negative numbers
        if (num < 0) {
                charLCD('-');
                num = -num;
        }

        // Convert number to ASCII manually
        if (num == 0) {
                charLCD('0');
                return;
        }

        while (num > 0) {
                buf[i++] = (num % 10) + '0';
                num /= 10;
        }

        // Print the digits in reverse
        while (--i >= 0) {
                charLCD(buf[i]);
        }
}


/*
#include<LPC21xx.h>
#include"types.h"
#include"delay.h"
#include"lcd.h"
#include"lcd_defines.h"

void writeLCD(u8 byte)
{
        IOCLR0=1<<LCD_RW;

        IOPIN0=((IOPIN0&~(0XFF<<LCD_DATA))|
                                (byte<<LCD_DATA));

        IOSET0=1<<LCD_EN;

		delay_us(1);

        IOCLR0=1<<LCD_EN;

        delay_ms(2);
}

void cmdLCD(u8 cmd)
{
  IOCLR0=1<<LCD_RS;
  writeLCD(cmd);
}

void charLCD(u8 ascii)
{
   IOSET0=1<<LCD_RS;

   writeLCD(ascii);
}

void InitLCD(void)
{
   IODIR0|=((0XFF<<LCD_DATA)|(1<<LCD_RS)|
                                (1<<LCD_RW)|(1<<LCD_EN));

        delay_ms(15);
        cmdLCD(0X30);
		//delay_ms(5);
        cmdLCD(0X30);
		//delay_us(100);
        cmdLCD(0X30);
        cmdLCD(MODE_8BIT_2LINE);
        cmdLCD(DSP_ON_CUR_OFF);//ON//BLK
        cmdLCD(CLEAR_LCD);
        cmdLCD(SHIFT_CUR_RIGHT);
}

void strLCD(s8*s)
{
          while(*s)
                charLCD(*s++);
}

void U32LCD(u32 n)
{
         s32 i=0;
         u8 digit[10];
         if(n==0)
         {
                charLCD('0');
         }
         else
         {
                while(n>0)
                {
                        digit[i]=(n%10)+48;
                        n=n/10;
                        i++;
                }
                for(--i;i>=0;i--)
                {
                        charLCD(digit[i]);
                }
         }
}

void S32LCD(s32 n)
{
        if(n<0)
        {
                charLCD('-');
                n=-n;
        }
        U32LCD(n);
}

void F32LCD(f32 fNum,u8 nDp)
{
        s32 i;
        u32 n;
        if(fNum<0)
        {
                charLCD('-');
                fNum=-fNum;
        }
        n=fNum;
        U32LCD(n);
        charLCD('.');
        for(i=0;i<nDp;i++)
        {
                fNum=(fNum-n)*10;
                n=fNum;
                charLCD(n+48);
        }
}

void lcd_print_num(signed char num) 
{
    char buf[100]; // Enough for "-128" + null terminator
    int i = 0;

    // Handle negative numbers
    if (num < 0) {
        charLCD('-');
        num = -num;
    }

    // Convert number to ASCII manually
    if (num == 0) {
        charLCD('0');
        return;
    }

    while (num > 0) {
        buf[i++] = (num % 10) + '0';
        num /= 10;
    }

    // Print the digits in reverse
    while (--i >= 0) {
        charLCD(buf[i]);
    }
}

void Display5BarFuel(u8 percentage) 
{
        u8 level = (percentage + 9) / 20; // Round to nearest 20% increment (0-5)
        if (level > 5) level = 5;

    // Display on line 4
    //setcursor(4, 0);
        cmdLCD(0xD4);
    strLCD("FUEL: ");

    // Display the appropriate bar character
    charLCD(2 + level); // Uses characters 2-7 (empty to full)

    // Display percentage text
    strLCD(" ");
    //u32LCD(percentage);
    //charLCD('%');
    //strLCD("    "); // Clear any remaining characters
}
*/

/*
main()
{
	InitLCD();

	while(1)
	{
	    cmdLCD(CLEAR_LCD);
	    cmdLCD(GOTO_LINE1_POS0);
		strLCD("lokesh");
		delay_ms(100);
	}
}
*/
