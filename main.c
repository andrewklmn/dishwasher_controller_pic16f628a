/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include <pic16f628a.h>

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */
#include "data_storage.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/* i.e. uint8_t <variable_name>; */

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
int main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize I/O and Peripherals for application */
    InitApp();
    
    read_data_from_eeprom();
    
    while(1)
    {
        if (SENSOR_OF_WATER_ON_FLOOR==IS_ON) {
            //water on the floor detected
            data_write(SYSTEM_STATE,IS_ON);
            data_write(DISHWASHER_PAUSED_FLAG,IS_OFF);
            
            STATE_OF_WASH_MOTOR = TURNED_OFF;
            STATE_OF_WATER_TAP  = TURNED_OFF;
            
            reset_counters();
            data_write(LAUNDRY_STATE,ERROR);
            
            dispatch_buttons_leds_sensors();
            dispatch_tap_motor_drain();
        
        } else if (data_read(SYSTEM_STATE)==IS_ON) {
        
            if (data_read(DISHWASHER_PAUSED_FLAG)==IS_OFF) {
                dispatch_buttons_leds_sensors();
                dispatch_tap_motor_drain();
            } else {
                
                LED1 = LIGHT_OFF;
                LED2 = LIGHT_OFF;
                LED3 = LIGHT_OFF;
    
                LED_POWER = !LED_POWER;
                LED_STOPPED = LIGHT_OFF;
    
                STATE_OF_WASH_MOTOR = TURNED_OFF;
                STATE_OF_DRAIN_POMP = TURNED_OFF;
                STATE_OF_WATER_TAP  = TURNED_OFF;

            };
        } else {
            stop_all();
            data_write(LAUNDRY_STATE,ADD_DETERGENT);
        };
        
        __delay_ms(500);
    }
    return 0;
}

