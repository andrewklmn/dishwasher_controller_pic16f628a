/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */
#include <pic16f628a.h>

#include "user.h"
#include "system.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

void InitApp(void)
{
    /* Disable interrupts */
    INTCON = 0b00000000;
    
    /* TODO Initialize User Ports/Peripherals/Project here */
    CMCON = 0b00000111; //Disable PORTA Comparators  
    
    /* TODO Initialize User Ports/Peripherals/Project here */
    TRISA = 0b01110000;
    PORTA = 0b00000000;

    TRISB = 0b11110001;
    PORTB = 0b00000000;    
    nRBPU = 0;    
    
    /* Setup analog functionality and port direction */

    /* Initialize peripherals */
    LED1 = LIGHT_ON;
    __delay_ms(200);
    LED1 = LIGHT_OFF;
    LED2 = LIGHT_ON;
    __delay_ms(200);
    LED2 = LIGHT_OFF;
    LED3 = LIGHT_ON;
    __delay_ms(200);
    LED3 = LIGHT_OFF;
    LED_POWER = LIGHT_ON;
    __delay_ms(200);
    LED_POWER = LIGHT_OFF;
    LED_STOPPED = LIGHT_ON;
    __delay_ms(200);
    LED_STOPPED = LIGHT_OFF;    
    LED_POWER = LIGHT_ON;
    
    /* Enable interrupts */
}

