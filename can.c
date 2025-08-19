//can.c
#include <LPC21xx.h>
#include "types.h"
#include "defines.h"
#include "delay.h"
#include "can_defines.h"
#include "adc_defines.h"
#include "can.h"

//#define FOSC 12000000
//#define CCLK (FOSC*5) 
//#define PCLK (CCLK/4)

void Init_CAN1(void)
{
        //cfg p0.25 as CAN1_RX pin(RD1)
        PINSEL1|=0x00040000;
        //Reset CAN1 controller
        SETBIT(C1MOD,RM_BIT);
        //all received messages are accepted
        CLRBIT(AFMR,AccOFF_BIT);
        SETBIT(AFMR,AccBP_BIT);
        //AFMR=((AFMR&~(3<<AccOFF_BIT))|(2<<AccOFF_BIT));
        //Set baud Rate for CAN
        C1BTR=BTR_LVAL;
        //Enable CAN1 controller
        CLRBIT(C1MOD,RM_BIT);
}


// Transmit CAN frame with a check for transmission failure
void CAN1_Tx(struct CAN_Frame txFrame)
{
        // Define a timeout counter (you can adjust the number of attempts)
        int timeout_counter = 10000;  // Set an arbitrary large number for timeout

        // Wait for the TX buffer to be empty, but with a timeout to prevent getting stuck
        while (READBIT(C1GSR, TBS1_BIT) == 0)
        {
                timeout_counter--;
                if (timeout_counter <= 0)
                {
                        // Timeout: Could not transmit frame, exit the function
                        return;
                }
        }

        // Configure Tx ID
        C1TID1 = txFrame.ID;

        // Configure RTR and DLC
        C1TFI1 = (txFrame.vbf.RTR << RTR_BIT) | (txFrame.vbf.DLC << DLC_BITS);

        // If it's not a Remote frame (RTR = 0), write data to TX buffer
        if (txFrame.vbf.RTR != 1)
        {
                // Write data bytes 1-4
                C1TDA1 = txFrame.Data1;
                // Write data bytes 5-8
                C1TDB1 = txFrame.Data2;
        }

        // Select Tx Buff 1 & Start transmission
        C1CMR |= 1 << STB1_BIT | 1 << TR_BIT;

        // Wait until transmission is complete
        timeout_counter = 10000;  // Reset timeout for completion check
        while (READBIT(C1GSR, TCS1_BIT) == 0)
        {
                timeout_counter--;
                if (timeout_counter <= 0)
                {
                        // Timeout: Transmission failed or taking too long, exit the function
                        return;
                }
        }
}

void CAN1_Rx(struct CAN_Frame *rxFrame)
{
        //wait for CAN frame recv status
        while(READBIT(C1GSR,RBS_BIT_READ)==0);
        //read 11-bit CANid of recvd frame.
        rxFrame->ID=C1RID;
        //& read & extract data/remote frame status
        rxFrame->vbf.RTR=(C1RFS>>RTR_BIT)&1;
        //& extract data length
        rxFrame->vbf.DLC=(C1RFS>>DLC_BITS)&0x0f;
        //check if recvd frame is data frame,
        if(rxFrame->vbf.RTR==0)
        {
                //extract data bytes 1-4
                rxFrame->Data1=C1RDA;
                //extract data bytes 5-8
                rxFrame->Data2=C1RDB;
        }
        // Release receive buffer command
        SETBIT(C1CMR,RRB_BIT);
}



/*
#include <LPC21xx.h>
#include "types.h"
#include "defines.h"
#include "delay.h"
#include "can_defines.h"
#include "adc_defines.h"
#include "can.h"
*/

/*
struct CAN_Frame
{       
        u32 ID;
        struct BitField
        {
                u32 RTR : 1;
                u32 DLC : 4;
        }BFV;   
        u32 Data1,Data2;
        //u8 Data[8];
};
*/

/*
typedef struct CAN_Frame
{       
        u32 ID;
        struct BitField
        {
                u32 RTR : 1;
                u32 DLC : 4;
        }BFV;   
        s32 Data1,Data2;
        //u8 Data[8];
}CANF;  
*/
/*
void Init_CAN1(void)
{
    //cfg p0.25 as CAN1_RX pin(RD1)
    PINSEL1|=0x00040000;
    //Reset CAN1 controller
    SETBIT(C1MOD,RM_BIT);
    //all received messages are accepted
         CLRBIT(AFMR,AccOFF_BIT);
    SETBIT(AFMR,AccBP_BIT);
          //AFMR=((AFMR&~(3<<AccOFF_BIT))|(2<<AccOFF_BIT));
    //Set baud Rate for CAN
    C1BTR=BTR_LVAL;
   //Enable CAN1 controller
    CLRBIT(C1MOD,RM_BIT);
}


// Transmit CAN frame with a check for transmission failure
void CAN1_Tx(CANF txFrame)
{
    // Define a timeout counter (you can adjust the number of attempts)
    int timeout_counter = 10000;  // Set an arbitrary large number for timeout

    // Wait for the TX buffer to be empty, but with a timeout to prevent getting stuck
    while (READBIT(C1GSR, TBS1_BIT_READ) == 0)
    {
        timeout_counter--;
        if (timeout_counter <= 0)
        {
            // Timeout: Could not transmit frame, exit the function
            return;
        }
    }

    // Configure Tx ID
    C1TID1 = txFrame.ID;

    // Configure RTR and DLC
    C1TFI1 = (txFrame.vbf.RTR << RTR_BIT) | (txFrame.vbf.DLC << DLC_BITS);

    // If it's not a Remote frame (RTR = 0), write data to TX buffer
    if (txFrame.vbf.RTR != 1)
    {
        // Write data bytes 1-4
        C1TDA1 = txFrame.Data1;
        // Write data bytes 5-8
        C1TDB1 = txFrame.Data2;
    }

    // Select Tx Buff 1 & Start transmission
    C1CMR |= 1 << STB1_BIT_SET | 1 << TR_BIT_SET;

    // Wait until transmission is complete
    timeout_counter = 10000;  // Reset timeout for completion check
    while (READBIT(C1GSR, TCS1_BIT_READ) == 0)
    {
        timeout_counter--;
        if (timeout_counter <= 0)
        {
            // Timeout: Transmission failed or taking too long, exit the function
            return;
        }
    }
}
*/
/*
void CAN1_Tx(CANF txFrame)
{               
   // Checking that the TX buffer is empty
   while(READBIT(C1GSR,TBS1_BIT)==0);
        //while(((C1GSR>>TBS1_BIT)&1)==0);
   // Cfg Tx ID
   C1TID1=txFrame.ID;
   // Cfg RTR & DLC     
   C1TFI1=(txFrame.BFV.RTR<<RTR_BIT)|
          (txFrame.BFV.DLC<<DLC_BITS);
    //Check whether D/R Frame to Transmit
   if(txFrame.BFV.RTR!=1)
   {    
   */  //if D Frame,wr to data transmit buffers
        //  C1TDA1= txFrame.Data1; /*bytes 1-4 */
//         C1TDB1= txFrame.Data2; /*bytes 5-8 */
/*   }
   //Select Tx Buff 1 & Start Xmission
   C1CMR|=1<<STB1_BIT|1<<TR_BIT;
   //wait until tx complete
   while(READBIT(C1GSR,TCS1_BIT)==0); 
}  
  */
/*
void CAN1_Rx(CANF *rxFrame)
{
  //wait for CAN frame recv status
  while(READBIT(C1GSR,RBS_BIT_READ)==0);
  //read 11-bit CANid of recvd frame.
  rxFrame->ID=C1RID;
  //& read & extract data/remote frame status
  rxFrame->vbf.RTR=(C1RFS>>RTR_BIT)&1;
  //& extract data length
  rxFrame->vbf.DLC=(C1RFS>>DLC_BITS)&0x0f;
  //check if recvd frame is data frame,
  if(rxFrame->vbf.RTR==0)
  {
   //extract data bytes 1-4
   rxFrame->Data1=C1RDA;
   //extract data bytes 5-8
   rxFrame->Data2=C1RDB;
  }
  // Release receive buffer command
  SETBIT(C1CMR,RRB_BIT_SET);
}
*/















/*
#include <lpc21xx.h>
#include "types.h"
#include"can.h"
#include"can_defines.h"

#define PCLK       60000000  //Hz  

 */
/*CAN Controller 1 Initialization : (defined in can.c )*/
/*
void Init_CAN1(void)
{
        //cfg p0.25 pin as CAN1_RX pin(RD1),TD1 is exclusive    
        PINSEL1|=RD1_PIN; //using defines from can_defines.h   
        // #define RD1_PIN 0x00040000 ,    
        //as RD1/ (i.e CAN1_RX)/p0.25   
        //Reset CAN1 controller   
        C1MOD=1;
        //All received messages are accepted       
        AFMR=2;
        //Set baud Rate for CAN   
        C1BTR=BTR_LVAL; //using defines from can_defines.h   
        //Enable CAN1 controller   
        C1MOD=0;
}
void CAN1_Tx(struct CAN_Frame txFrame)
{
        //wait for CAN frame recv status        
        while((C1GSR&RBS_BIT_READ)==0);
        //read 11-bit CANid of recvd frame.         
        txFrame->ID=C1RID;
        // read & extract data/remote frame status        
        txFrame->vbf.RTR=(C1RFS>>30)&1;
        //read & extract data length        
        txFrame->vbf.DLC=(C1RFS>>16)&0x0f;
        //check if recvd frame is data frame,extract data bytes        
        if(txFrame->vbf.RTR==0)
        {
                //read 1-4 bytes from C1RDA        
                txFrame->Data1=C1RDA;
                //read 5-8 bytes from C1RDB        
                txFrame->Data2=C1RDB;
        }

        // Release receive buffer         

        C1CMR=RRB_BIT_SET;

}
*/
/*
void CAN1_Tx(struct CAN_Frame txFrame)
{
        // Checking that the TX buffer is empty in C1GSR     
        while((C1GSR&TBS1_BIT_READ)==0); //if status is 1 then empty    

        // place 11-bit tx id in C1T1D1     
        C1TID1=txFrame.ID;
        // place cfg whether data/remote frame & no of data bytes in message      
        C1TFI1=txFrame.vbf.RTR<<30|txFrame.vbf.DLC<<16;
        // For Data Frame place 1 to 8 bytes data into Data Tx Buffers      
        if(txFrame.vbf.RTR!=1)
        {
                //Place data bytes 1-4 in C1TDA1      

                C1TDA1= txFrame.Data;
                //Place data bytes 5-8 in C1TDB1     
                C1TDB1= txFrame.Data;
        }
        //Select Tx Buf1 & Start Xmission using       
        C1CMR=STB1_BIT_SET|TR_BIT_SET;
        //monitor tx status in C1GSR     
        while((C1GSR&TCS1_BIT_READ)==0);
}
*/

/*
void CAN1_Rx(struct CAN_Frame *rxFrame)
{
        //wait for CAN frame recv status        
        while((C1GSR&RBS_BIT_READ)==0);
        //read 11-bit CANid of recvd frame.         
        rxFrame->ID=C1RID;
        // read & extract data/remote frame status        
        rxFrame->vbf.RTR=(C1RFS>>30)&1;
        //read & extract data length        
        rxFrame->vbf.DLC=(C1RFS>>16)&0x0f;
        //check if recvd frame is data frame,extract data bytes        
        if(rxFrame->vbf.RTR==0)
        {
                //read 1-4 bytes from C1RDA        
                rxFrame->Data1=C1RDA;
                //read 5-8 bytes from C1RDB        
                rxFrame->Data2=C1RDB;
        }

        // Release receive buffer         

        C1CMR=RRB_BIT_SET;

}
*/
/*
//can2.c
void Init_CAN2(void)
{
        //cfg p0.25 pin as CAN1_RX pin(RD1),TD1 is exclusive    
        PINSEL1|=RD1_PIN; //using defines from can_defines.h   
        // #define RD1_PIN 0x00040000 ,    
        //as RD1/ (i.e CAN1_RX)/p0.25   
        //Reset CAN1 controller   
        C1MOD=1;
        //All received messages are accepted       
        AFMR=2;
        //Set baud Rate for CAN   
        C1BTR=BTR_LVAL; //using defines from can_defines.h   
        //Enable CAN1 controller   
        C1MOD=0;
}
void CAN2_Tx(struct CAN_Frame txFrame)
{
        // Checking that the TX buffer is empty in C1GSR     
        while((C1GSR&TBS1_BIT_READ)==0); //if status is 1 then empty    

        // place 11-bit tx id in C1T1D1     
        C1TID1=txFrame.ID;
        // place cfg whether data/remote frame & no of data bytes in message      
        C1TFI1=txFrame.vbf.RTR<<30|txFrame.vbf.DLC<<16;
        // For Data Frame place 1 to 8 bytes data into Data Tx Buffers      
        if(txFrame.vbf.RTR!=1)
        {
                //Place data bytes 1-4 in C1TDA1      

                C1TDA1= txFrame.Data;
                //Place data bytes 5-8 in C1TDB1     
                C1TDB1= txFrame.Data;
        }
        //Select Tx Buf1 & Start Xmission using       
        C1CMR=STB1_BIT_SET|TR_BIT_SET;
        //monitor tx status in C1GSR     
        while((C1GSR&TCS1_BIT_READ)==0);
}
void CAN2_Rx(struct CAN_Frame *rxFrame)
{
        //wait for CAN frame recv status        
        while((C1GSR&RBS_BIT_READ)==0);
        //read 11-bit CANid of recvd frame.         
        rxFrame->ID=C1RID;
        // read & extract data/remote frame status        
        rxFrame->vbf.RTR=(C1RFS>>30)&1;
        //read & extract data length        
        rxFrame->vbf.DLC=(C1RFS>>16)&0x0f;
        //check if recvd frame is data frame,extract data bytes        
        if(rxFrame->vbf.RTR==0)
        {
                //read 1-4 bytes from C1RDA        
                rxFrame->Data=C1RDA;
                //read 5-8 bytes from C1RDB        
                rxFrame->Data=C1RDB;
        }

        // Release receive buffer         

        C1CMR=RRB_BIT_SET;

}

//can3.c
void Init_CAN3(void)
{
        //cfg p0.25 pin as CAN1_RX pin(RD1),TD1 is exclusive    
        PINSEL1|=RD1_PIN; //using defines from can_defines.h   
        // #define RD1_PIN 0x00040000 ,    
        //as RD1/ (i.e CAN1_RX)/p0.25   
        //Reset CAN1 controller   
        C1MOD=1;
        //All received messages are accepted       
        AFMR=2;
        //Set baud Rate for CAN   
        C1BTR=BTR_LVAL; //using defines from can_defines.h   
        //Enable CAN1 controller   
        C1MOD=0;
}
void CAN3_Tx(struct CAN_Frame txFrame)
{
        // Checking that the TX buffer is empty in C1GSR     
        while((C1GSR&TBS1_BIT_READ)==0); //if status is 1 then empty    

        // place 11-bit tx id in C1T1D1     
        C1TID1=txFrame.ID;
        // place cfg whether data/remote frame & no of data bytes in message      
        C1TFI1=txFrame.vbf.RTR<<30|txFrame.vbf.DLC<<16;
        // For Data Frame place 1 to 8 bytes data into Data Tx Buffers      
        if(txFrame.vbf.RTR!=1)
        {
                //Place data bytes 1-4 in C1TDA1      

                C1TDA1= txFrame.Data;
                //Place data bytes 5-8 in C1TDB1     
                C1TDB1= txFrame.Data;
        }
        //Select Tx Buf1 & Start Xmission using       
        C1CMR=STB1_BIT_SET|TR_BIT_SET;
        //monitor tx status in C1GSR     
        while((C1GSR&TCS1_BIT_READ)==0);
}
void CAN3_Rx(struct CAN_Frame *rxFrame)
{
        //wait for CAN frame recv status        
        while((C1GSR&RBS_BIT_READ)==0);
        //read 11-bit CANid of recvd frame.         
        rxFrame->ID=C1RID;
        // read & extract data/remote frame status        
        rxFrame->vbf.RTR=(C1RFS>>30)&1;
        //read & extract data length        
        rxFrame->vbf.DLC=(C1RFS>>16)&0x0f;
        //check if recvd frame is data frame,extract data bytes        
        if(rxFrame->vbf.RTR==0)
        {
                //read 1-4 bytes from C1RDA        
                rxFrame->Data=C1RDA;
                //read 5-8 bytes from C1RDB        
                rxFrame->Data=C1RDB;
        }

        // Release receive buffer         

        C1CMR=RRB_BIT_SET;

}
*/
