//interrupts.h
//interrupt.h
void eint0_isr(void) __irq;
void eint1_isr(void) __irq;

void Enable_EINT0(void);
void Enable_EINT1(void);

void config_eint(void);
void config_vic(void);
/*
void EINT0_Handler(void) __irq;
void EINT1_Handler(void) __irq;

void config_vic(void);
*/

/*
void eint0_isr(void) __irq;

void Enable_EINT0(void);

void eint0_isr(void) __irq;
void eint1_isr(void) __irq;

void Enable_EINT0(void);
void Enable_EINT1(void);
*/

/*
void eint0_isr(void) __irq;
void eint1_isr(void) __irq;

void Enable_EINT0(void);
void Enable_EINT1(void);
*/
