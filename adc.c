//adc.c

// adc.c
#include <LPC21xx.h>
#include "types.h"
#include "adc.h"
#include "adc_defines.h"
#include "delay.h"

void Init_ADC(void)
{
    // Configure ADC: Power up, select default channel (CH0), and set clock divider
    ADCR = PDN_BIT | CLKDIV | CHANNEL_SEL;
}

f32 Read_ADC(u8 chNo)
{
    u16 adcVal = 0;
    f32 eAR = 0.0;

    // Select ADC channel
    ADCR &= ~(0xFF);         // Clear existing channel selection
    ADCR |= (1 << chNo);     // Select required ADC channel

    // Start conversion
    ADCR |= (1 << ADC_START_BIT);

    delay_us(3);  // Allow time for sampling

    // Wait until conversion is complete
    while (!(ADDR & (1UL << DONE_BIT)));


    // Stop conversion
    ADCR &= ~(7 << ADC_START_BIT);  // Clear start bits

    // Extract 10-bit ADC result (bits 15:6)
    adcVal = (ADDR >> 6) & 0x3FF;

    // Convert to voltage assuming 3.3V reference and 10-bit ADC
    eAR = ((adcVal * 3.3f) / 1023.0f);

    return eAR;
}

/*
#include <LPC21xx.h>

#include "types.h"
#include "defines.h"
#include "adc.h"
#include "adc_defines.h"
#include "delay.h"

void Init_ADC(void)
{
        ADCR=PDN_BIT|CLKDIV|CHANNEL_SEL;
}

f32 Read_ADC(u8 chNo)
{
        u16 adcVal=0;
        f32 eAR;
        WRITEBYTE(ADCR,0,chNo);
        SETBIT(ADCR,ADC_START_BIT);
        delay_us(3);
        while(!READBIT(ADDR,DONE_BIT));
        CLRBIT(ADCR,ADC_START_BIT);
        adcVal=(ADDR>>6)&0x3FF;
        eAR=((adcVal*3.3)/1023);
        return eAR;
}
*/
