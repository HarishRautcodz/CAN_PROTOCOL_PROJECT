//can.h
//can.h

#ifndef __CAN_H__
#define __CAN_H__

#include "types.h"

// CAN Frame Structure

typedef struct CAN_Frame
{
        u32 ID;
        struct BitField
        {
                u8 RTR : 1;
                u8 DLC : 4;
        } vbf;
        u32 Data1,Data2;  // 8 bytes total
        //      u32 rx.data;
}CANF;

void Init_CAN1(void);
void CAN1_Tx(struct CAN_Frame txFrame);
void CAN1_Rx(struct CAN_Frame *rxFrame);
void config_eint(void);
void config_vic(void);

#endif

/*
#ifndef __CAN_H__
#define __CAN_H__
#include "types.h"

typedef struct CAN_Frame
{
        u32 ID;
        struct BitField
        {
                u8 RTR : 1;
                u8 DLC : 4;
        }vbf;
        u32 Data1,Data2;//8-bytes
}CANF;

void Init_CAN1(void);
void CAN1_Tx(CANF txFrame);
void CAN1_Rx(CANF *rxFrame);
void config_eint(void);
void config_vic(void);

#endif
*/

/*
void Init_CAN1(void);
void CAN1_Tx(struct CAN_Frame *);
void CAN1_Rx(struct CAN_Frame *);

void Init_CAN2(void);
void CAN2_Tx(struct CAN_Frame);
void CAN2_Rx(struct CAN_Frame *);

void Init_CAN3(void);
void CAN3_Tx(struct CAN_Frame);
void CAN3_Rx(struct CAN_Frame *);
#endif
 */
