/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <xc.h>         /* XC8 General Include File */
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
    PORTB = 0b11110001;    
    nRBPU = 0;    
    
    /* Setup analog functionality and port direction */

    /* Initialize peripherals */
    LED1 = LIGHT_ON;
    LED2 = LIGHT_ON;
    LED3 = LIGHT_ON;
    LED_POWER = LIGHT_ON;
    LED_STOPPED = LIGHT_ON;
    __delay_ms(200);
    LED1 = LIGHT_OFF;
    LED2 = LIGHT_OFF;
    LED3 = LIGHT_OFF;
    LED_POWER = LIGHT_OFF;
    LED_STOPPED = LIGHT_OFF;
    
    
    
    /* Enable interrupts */
    temp=PORTB;
    INTCON = 0b10011000;
    INTEDG = 0; 
    
    //eeprom_write(0x00, 10);
    
}

void dispatch_buttons_leds_sensors(void) {
    switch(eeprom_read(LAUNDRY_STATE)) {
                
                case ADD_DETERGENT:
                    // ready to start washing - ADD DETERGENT and PRESS START
                    LED1 = !LED1;
                    LED2 = LIGHT_OFF;
                    LED3 = LIGHT_OFF;
                    LED_POWER = LIGHT_ON;
                    LED_STOPPED = LIGHT_OFF;
                    break;
                    
                case DETERGENT_WASHING:
                    // fat detergent action is pending
                    LED1 = LIGHT_ON;
                    LED2 = LIGHT_OFF;
                    LED3 = LIGHT_OFF;
                    LED_POWER = LIGHT_ON;
                    LED_STOPPED = LIGHT_OFF;
                    break;
                    
                case ADD_LEMON_ACID:
                    // PAUSE -for adding_lemon acid action 
                    LED1 = LIGHT_OFF;
                    LED2 = !LED2;
                    LED3 = LIGHT_OFF;
                    LED_POWER = LIGHT_ON;
                    LED_STOPPED = LIGHT_OFF;
                    break;
                    
                case ACID_WASHING:
                    // acid washing action is pending 
                    LED1 = LIGHT_OFF;
                    LED2 = LIGHT_ON;
                    LED3 = LIGHT_OFF;
                    LED_POWER = LIGHT_ON;
                    LED_STOPPED = LIGHT_OFF;
                    break;
                case FLUSHING:
                    // clear water action pending
                    LED1 = LIGHT_OFF;
                    LED2 = LIGHT_OFF;
                    LED3 = LIGHT_ON;
                    LED_POWER = LIGHT_ON;
                    LED_STOPPED = LIGHT_OFF;
                    break;
                    
                case COMPLETED:
                    // wash cycle is complete - OPEN THE DOOR FOR DRYING
                    LED1 = LIGHT_OFF;
                    LED2 = LIGHT_OFF;
                    LED3 = LIGHT_OFF;
                    LED_POWER = LIGHT_ON;
                    LED_STOPPED = !LED_STOPPED;
                    break; 
                    
                case PAUSED:
                    // wash cycle is complete - OPEN THE DOOR FOR DRYING
                    LED1 = LIGHT_OFF;
                    LED2 = LIGHT_OFF;
                    LED3 = LIGHT_OFF;
                    LED_POWER = !LED_POWER;
                    LED_STOPPED = LIGHT_OFF;
                    break;
                    
                default:
                    LED1 = LIGHT_OFF;
                    LED2 = LIGHT_OFF;
                    LED3 = LIGHT_OFF;
                    LED_POWER = LIGHT_OFF;
                    LED_STOPPED = LIGHT_OFF;
                    eeprom_write(LAUNDRY_STATE,ADD_DETERGENT);
            };  
}


void stop_all(void){
    
    LED1 = LIGHT_OFF;
    LED2 = LIGHT_OFF;
    LED3 = LIGHT_OFF;
    
    LED_POWER =   LIGHT_OFF;
    LED_STOPPED = LIGHT_OFF;
    
    STATE_OF_WASH_MOTOR = TURNED_OFF;
    STATE_OF_DRAIN_POMP = TURNED_OFF;
    STATE_OF_WATER_TAP  = TURNED_OFF;

};

void dispatch_tap_motor_drain(void){
    switch(eeprom_read(LAUNDRY_STATE)){
        case DETERGENT_WASHING:
            dispatch_work_cycle(DETERGENT_WASHING_TIME,ADD_LEMON_ACID);
            break;
        case ACID_WASHING:
            dispatch_work_cycle(ACID_WASHING_TIME,FLUSHING);
            break;
        case FLUSHING:
            dispatch_work_cycle(FLUSHING_TIME,COMPLETED);
            break;
        default:
            STATE_OF_WASH_MOTOR = TURNED_OFF;
            STATE_OF_DRAIN_POMP = TURNED_OFF;
            STATE_OF_WATER_TAP  = TURNED_OFF;
            break;
    };
};


void dispatch_work_cycle(char work_cycle_time, char next_state){
    if (eeprom_read(FILLING_COUNTER)<FILLING_TICKS_AMOUNT) {
        // filling is pending
        STATE_OF_WASH_MOTOR = TURNED_OFF;
        STATE_OF_DRAIN_POMP = TURNED_OFF;
        STATE_OF_WATER_TAP  = TURNED_ON;        
    } else if (eeprom_read(WORK_CYCLE_COUNTER)<work_cycle_time) {
        // washing is pending
        STATE_OF_DRAIN_POMP = TURNED_OFF;
        STATE_OF_WATER_TAP  = TURNED_OFF;
        STATE_OF_WASH_MOTOR = TURNED_ON;
        eeprom_write(WORK_CYCLE_COUNTER,eeprom_read(WORK_CYCLE_COUNTER)+1);
                
    } else if (eeprom_read(DRAINING_COUNTER)<DRAINING_TIME) {
        // drain is pending
        STATE_OF_WATER_TAP  = TURNED_OFF;
        STATE_OF_WASH_MOTOR = TURNED_OFF;
        STATE_OF_DRAIN_POMP = TURNED_ON;
        eeprom_write(DRAINING_COUNTER,eeprom_read(DRAINING_COUNTER)+1);
    } else {
        reset_counters();
        eeprom_write(LAUNDRY_STATE,next_state);
    };
};


void reset_counters(void){
    eeprom_write(FILLING_COUNTER,0);
    eeprom_write(WORK_CYCLE_COUNTER,0);
    eeprom_write(DRAINING_COUNTER,0);
}