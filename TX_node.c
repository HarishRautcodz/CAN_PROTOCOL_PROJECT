//TX_node.c
#include <lpc21xx.h>
#include "types.h"
#include "can.h"
int main(void)
{
        u8 i; struct CAN_Frame txFrame; i=sizeof(txFrame);
        txFrame.ID=2;   
		txFrame.vbf.RTR=0; //data frame   
        txFrame.Data1=0x87654321; 
		txFrame.Data2=0x38392635;
        Init_CAN1();
        while(1)
        {
                for(i=0;i<=8;i++)
                {
                        txFrame.vbf.DLC=i;
                        CAN1_Tx(txFrame);
                }
        }
}
