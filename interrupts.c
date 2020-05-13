/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#include <xc.h>         /* XC8 General Include File */


#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */
#include <pic16f628a.h>

#include "user.h"
#include "data_storage.h"

/* TODO Define system operating frequency */
#define _XTAL_FREQ 4000000

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
       data_write(FILLING_COUNTER,data_read(FILLING_COUNTER)+1);
       
       temp=PORTB; //read port before INTF reset 
       INTF=0; 
       
    } else if (RBIF){
        if (BUTTON_OF_POWER==PRESSED) {
            if(data_read(SYSTEM_STATE)==IS_ON){
                if (SENSOR_OF_WATER_LEVEL==IS_ON) {
                    // if POWER OFF was pressed when water inside - flushing cycle
                    data_write(FILLING_COUNTER,FILLING_TICKS_AMOUNT);
                    data_write(WORK_CYCLE_COUNTER,FLUSHING_TIME);
                    data_write(DRAINING_COUNTER,0);
                    data_write(LAUNDRY_STATE,FLUSHING);
                } else {
                    data_write(SYSTEM_STATE,IS_OFF);
                };
            
            } else {
                data_write(SYSTEM_STATE,IS_ON);
                if (SENSOR_OF_WATER_LEVEL==IS_ON) {
                    data_write(LAUNDRY_STATE,ERROR);
                } else {
                    data_write(LAUNDRY_STATE,ADD_DETERGENT);
                    if (eeprom_read(LAUNDRY_STATE)!=ADD_DETERGENT) {
                        eeprom_write(LAUNDRY_STATE,ADD_DETERGENT);
                    };
                };
            };
            data_write(DISHWASHER_PAUSED_FLAG, IS_OFF);
            
        } else if (BUTTON_OF_START_PAUSE==PRESSED) {
             if(data_read(SYSTEM_STATE)==IS_ON){
                if (data_read(LAUNDRY_STATE)==ERROR) {
                    // start washing cycle with defined timing
                    if (data_read(DRAINING_COUNTER)>=DRAINING_TIME
                        && SENSOR_OF_WATER_ON_FLOOR==IS_OFF) {
                        if (SENSOR_OF_WATER_LEVEL==IS_ON) {
                            data_write(DRAINING_COUNTER,0);
                        } else {
                            data_write(LAUNDRY_STATE,ADD_DETERGENT);
                        };
                    };
                } else if (data_read(LAUNDRY_STATE)==ADD_DETERGENT) {
                    // start washing cycle with defined timing
                    data_write(LAUNDRY_STATE,DETERGENT_WASHING);
                    
                } else if (data_read(LAUNDRY_STATE)==ADD_LEMON_ACID) {
                    data_write(LAUNDRY_STATE,ACID_WASHING);
                    
                } else if (data_read(LAUNDRY_STATE)==COMPLETED) {
                    data_write(LAUNDRY_STATE,ADD_DETERGENT);
                    
                } else { 
                    if (data_read(DISHWASHER_PAUSED_FLAG)==IS_ON){
                        data_write(DISHWASHER_PAUSED_FLAG, IS_OFF);
                        
                    } else {
                        data_write(DISHWASHER_PAUSED_FLAG, IS_ON);
                    };
                };
             };
        };
        
        if (SENSOR_OF_220V==IS_OFF) {
                
                save_data_to_eeprom();
                LED1 = LIGHT_ON;
                LED2 = LIGHT_ON;
                LED3 = LIGHT_ON;
                __delay_ms(100);
                LED1 = LIGHT_OFF;
                LED2 = LIGHT_OFF;
                LED3 = LIGHT_OFF;
                __delay_ms(100);
                LED1 = LIGHT_ON;
                LED2 = LIGHT_ON;
                LED3 = LIGHT_ON;
                __delay_ms(100);
                LED1 = LIGHT_OFF;
                LED2 = LIGHT_OFF;
                LED3 = LIGHT_OFF;
                __delay_ms(100);
                LED1 = LIGHT_ON;
                LED2 = LIGHT_ON;
                LED3 = LIGHT_ON;
                __delay_ms(100);
                //data_write(DISHWASHER_PAUSED_FLAG, IS_ON);    
        };
        
        
       temp=PORTB;  //read port before RBIF reset 
       RBIF=0;
    };
}
#endif