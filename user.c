/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <xc.h>         /* XC8 General Include File */
#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */
#include <pic16f628a.h>

#include "user.h"
#include "system.h"
#include "data_storage.h"
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
    
    
    if (SENSOR_OF_WATER_ON_FLOOR==IS_ON) {
        data_write(SYSTEM_STATE,IS_ON);
        data_write(LAUNDRY_STATE,ERROR);
    };
    
    
    // check previous state after restore AC power
    switch(data_read(LAUNDRY_STATE)) {
        case ADD_DETERGENT:
        case ADD_LEMON_ACID:
        case COMPLETED:
            if (SENSOR_OF_WATER_LEVEL==IS_ON) {
                data_write(SYSTEM_STATE,IS_ON);
                data_write(LAUNDRY_STATE,ERROR);
            };
            break;
    };
    
    
};

void dispatch_buttons_leds_sensors(void) {
    switch(data_read(LAUNDRY_STATE)) {
                
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
                    
                case ERROR:
                    
                    LED1 = !LED1;
                    LED2 = !LED2;
                    LED3 = !LED3;
                    LED_POWER = !LED_POWER;
                    LED_STOPPED = !LED_STOPPED;
                    break;
                    
                default:
                    LED1 = LIGHT_OFF;
                    LED2 = LIGHT_OFF;
                    LED3 = LIGHT_OFF;
                    LED_POWER = LIGHT_OFF;
                    LED_STOPPED = LIGHT_OFF;
                    data_write(LAUNDRY_STATE,ADD_DETERGENT);
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
    switch(data_read(LAUNDRY_STATE)){
        case DETERGENT_WASHING:
            dispatch_work_cycle(DETERGENT_WASHING_TIME,ADD_LEMON_ACID);
            break;
        case ACID_WASHING:
            dispatch_work_cycle(ACID_WASHING_TIME,FLUSHING);
            break;
        case FLUSHING:
            dispatch_work_cycle(FLUSHING_TIME,COMPLETED);
            break;
        case COMPLETED:
            STATE_OF_WASH_MOTOR = TURNED_OFF;
            STATE_OF_DRAIN_POMP = TURNED_OFF;
            STATE_OF_WATER_TAP  = TURNED_OFF;
            break;
        case ERROR:
            dispatch_error_cycle();
            break;
        default:
            STATE_OF_WASH_MOTOR = TURNED_OFF;
            STATE_OF_DRAIN_POMP = TURNED_OFF;
            STATE_OF_WATER_TAP  = TURNED_OFF;
            break;
    };
};

void dispatch_error_cycle(void){
            STATE_OF_WASH_MOTOR = TURNED_OFF;
            STATE_OF_WATER_TAP  = TURNED_OFF;
            
            if (SENSOR_OF_WATER_LEVEL==IS_ON
                && data_read(DRAINING_COUNTER)==DRAINING_TIME) {
                data_write(DRAINING_COUNTER,0);
            };
            if (data_read(DRAINING_COUNTER)<DRAINING_TIME) {
                // drain is pending
                STATE_OF_DRAIN_POMP = TURNED_ON;
                if (cycle_counter>=CYCLES_IN_ONE_MINUTES) {
                   data_write(DRAINING_COUNTER,data_read(DRAINING_COUNTER)+1);
                   cycle_counter=0;
                };
                cycle_counter++;
            } else {
                STATE_OF_DRAIN_POMP = TURNED_OFF;
            };
};


void dispatch_work_cycle(char work_cycle_time, char next_state){
    
    if ( data_read(FILLING_COUNTER)<FILLING_TICKS_AMOUNT) {
        if (data_read(FILLING_COUNTER)<2 
            &&  SENSOR_OF_WATER_LEVEL==IS_ON) {
                data_write(LAUNDRY_STATE,ERROR);
                return;
        };
        // filling is pending
        STATE_OF_WASH_MOTOR = TURNED_OFF;
        STATE_OF_DRAIN_POMP = TURNED_OFF;
        if (SENSOR_OF_WATER_LEVEL==IS_OFF) {
            STATE_OF_WATER_TAP  = TURNED_ON;
        } else {
            STATE_OF_WATER_TAP  = TURNED_OFF;
            data_write(FILLING_COUNTER,FILLING_TICKS_AMOUNT);            
        };
        
    } else if (data_read(FILLING_COUNTER)>(FILLING_TICKS_AMOUNT+2)) {
        // stop if WATER TAP IS CLOSED but water is loading
        data_write(LAUNDRY_STATE,ERROR);
        return;
        
    } else if (data_read(WORK_CYCLE_COUNTER)<work_cycle_time) {
        // washing is pending
        STATE_OF_DRAIN_POMP = TURNED_OFF;
        STATE_OF_WATER_TAP  = TURNED_OFF;
        if (SENSOR_OF_WATER_LEVEL==IS_OFF) {
            stop_all();
            reset_counters();
            data_write(LAUNDRY_STATE,ERROR);
        } else {
            STATE_OF_WASH_MOTOR = TURNED_ON;
            if (cycle_counter>=CYCLES_IN_ONE_MINUTES) {
                data_write(WORK_CYCLE_COUNTER,data_read(WORK_CYCLE_COUNTER)+1);
                cycle_counter=0;
            };
            cycle_counter++;
        }
        
    } else if (data_read(DRAINING_COUNTER)<DRAINING_TIME) {
        // drain is pending
        STATE_OF_WATER_TAP  = TURNED_OFF;
        STATE_OF_WASH_MOTOR = TURNED_OFF;
        STATE_OF_DRAIN_POMP = TURNED_ON;
        if (cycle_counter>=CYCLES_IN_ONE_MINUTES) {
            data_write(DRAINING_COUNTER,data_read(DRAINING_COUNTER)+1);
            cycle_counter=0;
        };
        cycle_counter++;
    } else {
        reset_counters();
        data_write(LAUNDRY_STATE,next_state);
    };
};


void reset_counters(void){
    cycle_counter = 0;
    data_write(FILLING_COUNTER,0);
    data_write(WORK_CYCLE_COUNTER,0);
    data_write(DRAINING_COUNTER,0);
}