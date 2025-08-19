//RX_node.c
#include <LPC21xx.h>
#include"can.h"
main()
{
        struct CAN_Frame rxFrame;
        Init_CAN1();
        while(1)
        {
                CAN1_Rx(&rxFrame);
        }

}
//MID=rxFrame.ID
//
  
