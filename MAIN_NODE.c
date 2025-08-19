//MAIN_NODE.c

#include<LPC21xx.h>
#include "delay.h"
#include "lcd_defines.h"
#include "lcd.h"
#include "adc.h"
#include "adc_defines.h"
#include "types.h"
#include "interrupt.h"
#include "can.h"
#include "can_defines.h"
#include "i2c.h"
#include "i2c_defines.h"
#include "i2c_eeprom.h"
#include "defines.h"

extern u8 cgramLUT[][8] = {
	{0x07, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // 0% (empty)
	{0x07, 0x1F, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1F},  // 0% (empty)
	{0x07, 0x1F, 0x11, 0x11, 0x11, 0x11, 0x1F, 0x1F},  // 20%
	{0x07, 0x1F, 0x11, 0x11, 0x11, 0x1F, 0x1F, 0x1F},  // 40%
	{0x07, 0x1F, 0x11, 0x11, 0x1F, 0x1F, 0x1F, 0x1F},  // 60%
	{0x07, 0x1F, 0x11, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F},  // 80%
	{0x07, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F}   // 100%
};

extern u8 left_blink, right_blink;
u32 fuel_per; 
s8  level; 

signed char x, y, z;

#define MMA7660_ADDR 0x4C
#define rLED (1<<16) // P1.16
#define gLED (1<<17) // P1.17

// Convert raw 6-bit unsigned to signed value
char to_signed_6bit(u8 val) 
{
	if (val > 31)
		return val - 64;
	return val;
}

u32 flag1=0, flag2=0;
CANF rxF, txF;

s32 val=0;
s32 per=0;

int rc_cnd(void)
{
	return (READBIT(C1GSR,RBS_BIT_READ))?1:0;
}

main()
{

	InitLCD();
	Init_CAN1();
	init_i2c();

	Enable_EINT0();
	Enable_EINT1();

	BuildCGRAM(&cgramLUT[0][0], 56); // 6 patterns × 8 bytes
					 //		delay_ms(100);

	rxF.Data1=0;

	IODIR1 = rLED | gLED;

	// Set MMA7660 to Active Mode
	i2c_device_write(MMA7660_ADDR, 0x07, 0x01);

	while(1)	
	{

		x = to_signed_6bit(i2c_device_read(MMA7660_ADDR, 0x00));
		y = to_signed_6bit(i2c_device_read(MMA7660_ADDR, 0x01));
		z = to_signed_6bit(i2c_device_read(MMA7660_ADDR, 0x02));

		if (x > 20 || y > 20 || z > 20)
		{
			IOSET1 = gLED;   // Bright if positive tilt
			IOCLR1 = rLED;
		} 						
		else if (x < -20 || y < -20 || z < -20)
		{
			IOSET1 = rLED;   // Red if negative tilt
			IOCLR1 = gLED;
		}
		else if((x<-8)&&(x<8) || (y<-8)&&(y<8) || (z<-17)&&(z<17) )
		{
			cmdLCD(GOTO_LINE3_POS0+10);
			strLCD("ALERT");
		}
		else 
		{ 
			IOCLR1 = gLED | rLED; // No strong tilt
			cmdLCD(GOTO_LINE3_POS0+10);
			strLCD("     ");
		}

		cmdLCD(GOTO_LINE1_POS0);
		strLCD("X = ");
		print_num(x);
		delay_ms(240);

		cmdLCD(GOTO_LINE2_POS0);
		strLCD("Y = ");
		print_num(y);
		delay_ms(240);

		cmdLCD(GOTO_LINE3_POS0);
		strLCD("Z = ");
		print_num(z);
		delay_ms(240);

		if(flag1==1)
		{
			flag1=0;
		}
		if(flag2==1)
		{
			flag2=0;
		}

		if(rc_cnd()) 
		{
			CAN1_Rx(&rxF);
			if(rxF.ID == 1)
			{
				cmdLCD(GOTO_LINE4_POS0);         
				strLCD("FUEL: ");                
				cmdLCD(GOTO_LINE4_POS0 + 6); 
				U32LCD(rxF.Data1);
				per=rxF.Data1;
				strLCD("  ");   

				cmdLCD(GOTO_LINE4_POS0 + 9);    
				charLCD('%');                    
				cmdLCD(GOTO_LINE4_POS0 + 10);    
				strLCD("      ");                   

				level = (fuel_per + 9) / 20; //(0-5)
				if (level > 5) 
					level = 5;

				val=2+level;

				charLCD(val);

				strLCD("  ");

				if(per<=0)
				{
					per=0;
					charLCD(0); // empty
					cmdLCD(GOTO_LINE4_POS0+11);
					strLCD("Empty  ");
				}
				if(per>0 && per<=19)
				{
					charLCD(1); // empty
					cmdLCD(GOTO_LINE4_POS0+11);
					strLCD("Low    ");
				}
				else if(per>=20 && per<=39)
				{
					charLCD(2); // 20%
					cmdLCD(GOTO_LINE4_POS0+11);
					strLCD("MedLow ");
				}
				else if(per>=40 && per<=59)
				{
					charLCD(3); // 40%
					cmdLCD(GOTO_LINE4_POS0+11);
					strLCD("Medium ");
				}
				else if(per>=60 && per<=79)
				{
					charLCD(4); // 60%
					cmdLCD(GOTO_LINE4_POS0+11);
					strLCD("High   ");
				}
				else if(per>=80 && per<=99)
				{
					charLCD(5); // 80%
					cmdLCD(GOTO_LINE4_POS0+11);
					strLCD("Full   ");
				}
				else if(per>=100)
				{
					per=100;
					charLCD(6); // 100%
					cmdLCD(GOTO_LINE4_POS0+11);
					strLCD("Fullll ");
				}
				else 
				{			
					cmdLCD(GOTO_LINE4_POS0);
					strLCD("FUEL:");
					cmdLCD(GOTO_LINE4_POS0+15);
					strLCD("ND  ");
				}
			}
			//		delay_ms(100);
		}
		else 
		{			
			cmdLCD(GOTO_LINE4_POS0);
			strLCD("FUEL:");
			cmdLCD(GOTO_LINE4_POS0+15);
			strLCD("ND  ");
		}
	}
}


void BuildCGRAM(u8 *p,u8 nBytes)
{
	u32 i;
	cmdLCD(GOTO_CGRAM_START);

	for(i=0;i<nBytes;i++)
	{
		charLCD(p[i]);
	}
	//delay_ms(1000);
}


/*
   extern u8 cgramLUT[][8] =
   {
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Cap only (almost empty)
   {0x07, 0x1F, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1F},  // 1/5 full
   {0x07, 0x1F, 0x11, 0x11, 0x11, 0x11, 0x1F, 0x1F},  // 2/5 full
   {0x07, 0x1F, 0x11, 0x11, 0x11, 0x1F, 0x1F, 0x1F},  // 3/5 full
   {0x07, 0x1F, 0x11, 0x11, 0x1F, 0x1F, 0x1F, 0x1F},  // 4/5 full
   {0x07, 0x1F, 0x11, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F}  // Fully filled
   };
   */

//BuildCGRAM(&cgramLUT[0][0], 48); // 6 * 8 = 48 bytes

/*
   if (level <= 20)
   {
   cmdLCD(GOTO_LINE4_POS0+8);
   U32LCD(srxF.Data1);
   cmdLCD(GOTO_LINE4_POS0+15);
   strLCD("Low");
   }
   else if (level <= 40)
   {
   cmdLCD(GOTO_LINE4_POS0+8);
   U32LCD(rxF.Data1);
   cmdLCD(GOTO_LINE4_POS0+15);
   strLCD("MedLow");
   }
   else if (level <= 60)
   {
   cmdLCD(GOTO_LINE4_POS0+8);
   U32LCD(rxF.Data1);
   cmdLCD(GOTO_LINE4_POS0+15);
   strLCD("Medium");
   }
   else if (level <= 80)
   {
   cmdLCD(GOTO_LINE4_POS0+8);
   U32LCD(rxF.Data1);
   cmdLCD(GOTO_LINE4_POS0+15);
   strLCD("High");
   }
   else if (level <= 100) 
   {	
   cmdLCD(GOTO_LINE4_POS0+8);
   U32LCD(rxF.Data1);
   cmdLCD(GOTO_LINE4_POS0+15);
   strLCD("Full");
   }
   else 
   {
   cmdLCD(GOTO_LINE4_POS0+15);
   strLCD("   ND         ");
   }
   delay_ms(500);
   }			 */
// }
//	 delay_ms(500);                                                           

