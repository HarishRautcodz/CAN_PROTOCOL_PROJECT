//INDICATOR_NODE.c

#include <LPC21xx.h>
#include "can.h"
#include "delay.h"
#include "defines.h"

#define RBS_BIT 0
#define LED_AL  0  // P0.0 to P0.7

CANF rxF;

int main() {
	volatile s32 i;
	u8 mode = 0; // 0: OFF, 1: LEFT, 2: RIGHT

	Init_CAN1();

	IODIR0 |= 0xFF << LED_AL;
	IOSET0 = 0xFF << LED_AL; // All LEDs OFF (active-low)

	while (1) 
	{
		// --- Check for new CAN message ---
		if (READBIT(C1GSR, RBS_BIT)) {
			CAN1_Rx(&rxF);
			if (rxF.ID == 2) {
				if (rxF.Data1 == 'L') {
					mode = (mode != 1) ? 1 : 0; // Toggle LEFT
				} else if (rxF.Data1 == 'R') {
					mode = (mode != 2) ? 2 : 0; // Toggle RIGHT
				}
			}
		}

		// --- LEFT pattern ---
		if (mode == 1) 
		{
			for (i = 0; i < 8 && mode == 1; i++)
			{
				IOCLR0 = 1 << (LED_AL + i); // ON
				delay_ms(100);
				//IOSET0 = 1 << (LED_AL + i); // OFF
				if(i==7)
				{
					IOSET0 = 0xFF << LED_AL;
				}
				// Check during blinking for new message
				if (READBIT(C1GSR, RBS_BIT))
				{
					CAN1_Rx(&rxF); // Always fetch new data

					if (rxF.ID == 2)
					{
						if (rxF.Data1 == 'L') 
						{
							mode = (mode != 1) ? 1 : 0; // Toggle LEFT
						} 
						else if (rxF.Data1 == 'R') 
						{
							mode = (mode != 2) ? 2 : 0; // Toggle RIGHT
						}
						break;                  
					}
				}
			}
		}

		// --- RIGHT pattern ---
		else if (mode == 2) 
		{
			for (i = 7; i >= 0 && mode == 2; i--) 
			{
				IOCLR0 = 1 << (LED_AL + i); // ON
				delay_ms(100);
				//IOSET0 = 1 << (LED_AL + i); // OFF
				if(i==0)
				{
					IOSET0 = 0xFF << LED_AL;
				}
				if (READBIT(C1GSR, RBS_BIT))
				{
					CAN1_Rx(&rxF);


					if (rxF.ID == 2)
					{
						if (rxF.Data1 == 'L')
						{
							mode = (mode != 1) ? 1 : 0; // Toggle LEFT
						} 
						else if (rxF.Data1 == 'R') 
						{
							mode = (mode != 2) ? 2 : 0; // Toggle RIGHT
						}

						break;
					}
				}
			}
		}

		// --- OFF mode ---
		else {
			IOSET0 = 0xFF << LED_AL;
		}
	}
}
