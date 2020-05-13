
/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

#define LED1                        RB3     //output
#define LED2                        RB2     //output
#define LED3                        RB1     //output
#define LED_POWER                   RA3     //output
#define LED_STOPPED                 RA2     //output

#define STATE_OF_WASH_MOTOR         RA7      //output
#define STATE_OF_DRAIN_POMP         RA0      //output
#define STATE_OF_WATER_TAP          RA1      //output

#define SENSOR_OF_WATER_COUNTER     RB0      //input
#define SENSOR_OF_WATER_LEVEL       RB6      //input
#define SENSOR_OF_WATER_ON_FLOOR    RA6      //input
#define SENSOR_OF_220V              RB5      //input

#define BUTTON_OF_START_PAUSE       RB4      //input
#define BUTTON_OF_POWER             RB7      //input

#define SELECTOR_1                  RA4      //input
#define SELECTOR_2                  RA5      //input

// for LEDs
#define LIGHT_ON    1
#define LIGHT_OFF   0

// for sensors and flags
#define IS_ON       0
#define IS_OFF      1

// for motor, tap and drain
#define TURNED_ON   1
#define TURNED_OFF  0


// for buttons
#define PRESSED     0
#define UNPRESSED   1

// addresses for eeprom
#define SYSTEM_STATE            0x00
#define LAUNDRY_STATE           0x01
#define DISHWASHER_PAUSED_FLAG  0x02

#define WORK_CYCLE_COUNTER      0x03
#define DRAINING_COUNTER        0x04
#define FILLING_COUNTER         0x05

// working cycles time definition in MIN
#define DETERGENT_WASHING_TIME 3
#define ACID_WASHING_TIME      3
#define FLUSHING_TIME          3
#define DRAINING_TIME          3

#define FILLING_TICKS_AMOUNT   3

#define CYCLES_IN_ONE_MINUTES  2

__EEPROM_DATA(IS_OFF,0,IS_OFF,0x00,0x00,0x00,0x00,0x00);

/* TODO Application specific user parameters used in user.c may go here */

enum laundry_state {
    ADD_DETERGENT,
    DETERGENT_WASHING,
    ADD_LEMON_ACID,
    ACID_WASHING,
    FLUSHING,
    COMPLETED,
    PAUSED,
    ERROR
};

unsigned char cycle_counter = 0; /* one dispatch cycle = 0.5s*/
unsigned char temp = 0;

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */

void InitApp(void);         /* I/O and Peripheral Initialization */

void dispatch_buttons_leds_sensors(void);
void dispatch_tap_motor_drain(void);
void dispatch_work_cycle(char work_cycle_time, char next_state);
void dispatch_error_cycle(void);

void reset_counters(void);
void stop_all(void);
