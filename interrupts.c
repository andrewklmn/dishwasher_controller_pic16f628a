/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#include <xc.h>         /* XC8 General Include File */


#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */
#include <pic16f628a.h>
#include "user.h"

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/

/* Baseline devices don't have interrupts. Note that some PIC16's 
 * are baseline devices.  Unfortunately the baseline detection macro is 
 * _PIC12 */
#ifndef _PIC12

void __interrupt () my_isr_routine (void) {
    /* This code stub shows general interrupt handling.  Note that these
    conditional statements are not handled within 3 seperate if blocks.
    Do not use a seperate if block for each interrupt flag to avoid run
    time errors. */

    if(INTF){
       
        // water count sensor pressed 
       eeprom_write(FILLING_COUNTER,eeprom_read(FILLING_COUNTER)+1);
       
       temp=PORTB; //read port before INTF reset 
       INTF=0; 
       
    } else if (RBIF){
        if (BUTTON_OF_POWER==PRESSED) {
            if(eeprom_read(SYSTEM_STATE)==IS_ON){
                eeprom_write(SYSTEM_STATE,IS_OFF);
            } else {
                eeprom_write(SYSTEM_STATE,IS_ON);
            };
            eeprom_write(LAUNDRY_STATE,ADD_DETERGENT);
            eeprom_write(DISHWASHER_PAUSED_FLAG, IS_OFF);
            
        } else if (BUTTON_OF_START_PAUSE==PRESSED) {
             if(eeprom_read(SYSTEM_STATE)==IS_ON){
                if (eeprom_read(LAUNDRY_STATE)==ERROR) {
                    // start washing cycle with defined timing
                    if (eeprom_read(DRAINING_COUNTER)>=DRAINING_TIME
                        && SENSOR_OF_WATER_ON_FLOOR==IS_OFF) {
                        eeprom_write(LAUNDRY_STATE,ADD_DETERGENT);
                    };
                } else if (eeprom_read(LAUNDRY_STATE)==ADD_DETERGENT) {
                    // start washing cycle with defined timing
                    eeprom_write(LAUNDRY_STATE,DETERGENT_WASHING);
                    
                } else if (eeprom_read(LAUNDRY_STATE)==ADD_LEMON_ACID) {
                    eeprom_write(LAUNDRY_STATE,ACID_WASHING);
                    
                } else if (eeprom_read(LAUNDRY_STATE)==COMPLETED) {
                    eeprom_write(LAUNDRY_STATE,ADD_DETERGENT);
                    
                } else { 
                    if (eeprom_read(DISHWASHER_PAUSED_FLAG)==IS_ON){
                        eeprom_write(DISHWASHER_PAUSED_FLAG, IS_OFF);
                        
                    } else {
                        eeprom_write(DISHWASHER_PAUSED_FLAG, IS_ON);
                    };
                };
             };
        };
        
       temp=PORTB;  //read port before RBIF reset 
       RBIF=0;
    };
}
#endif