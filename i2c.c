//i2c.c

#include <LPC21xx.h>
#include "types.h"
#include "delay.h"
#include "i2c.h"
//#include "i2c_defines.h"

#define CCLK      60000000  //Hz
#define PCLK      CCLK/4    //Hz
#define I2C_SPEED 100000    //Hz
#define LOADVAL   ((PCLK/I2C_SPEED)/2)

#define SCL_EN  0x00000010
#define SDA_EN  0x00000040

#define   AA_BIT 2
#define   SI_BIT 3
#define  STO_BIT 4
#define  STA_BIT 5
#define I2EN_BIT 6

void init_i2c(void)
{
        //Configure I/O pin for SCL & SDA functions using PINSEL0
        PINSEL0=SCL_EN|SDA_EN;
        //Configure Speed for I2C Serial Communication
        //Using I2CSCLL
        I2SCLL=LOADVAL;
        //& I2CSCLH
        I2SCLH=LOADVAL;
        //I2C Peripheral Enable for Communication
        I2CONSET=1<<I2EN_BIT;
}

void i2c_start(void)
{
        // start condition
        I2CONSET=1<<STA_BIT;
        //wait for start bit status
        while(((I2CONSET>>SI_BIT)&1)==0);
        // clear start condition
        I2CONCLR=1<<STA_BIT;
}

void i2c_restart(void)
{
        // start condition
        I2CONSET=1<<STA_BIT;
        //clr SI_BIT
        I2CONCLR=1<<SI_BIT;
        //wait for SI bit status
        while(((I2CONSET>>SI_BIT)&1)==0);
        // clear start condition
        I2CONCLR=1<<STA_BIT;
}

void i2c_write(u8 dat)
{
        //put data into I2DAT
        I2DAT=dat;
        //clr SI_BIT
        I2CONCLR = 1<<SI_BIT;
        //wait for SI bit status
        while(((I2CONSET>>SI_BIT)&1)==0);
}

void i2c_stop(void)
{
        // issue stop condition
        I2CONSET=1<<STO_BIT;
        // clr SI bit status    
        I2CONCLR = 1<<SI_BIT;
        //stop will cleared automatically
        //while(((I2CONSET>>STO_BIT)&1));
}

u8 i2c_nack(void)
{
        I2CONSET = 0x00; //Assert Not of Ack
        I2CONCLR = 1<<SI_BIT;
        while(((I2CONSET>>SI_BIT)&1)==0);
        return I2DAT;
}

u8 i2c_masterack(void)
{
        I2CONSET = 0x04; //Assert Ack
        I2CONCLR = 1<<SI_BIT;
        while(((I2CONSET>>SI_BIT)&1)==0);
        I2CONCLR = 0x04; //Clear Assert Ack
        return I2DAT;
}
