/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

#define LED1                        RB3     //output
#define LED2                        RB2     //output
#define LED3                        RB1     //output
#define LED_POWER                   RA3     //output
#define LED_STOPPED                 RA2     //output

#define STATE__OF_WASH_MOTOR        RA7      //output
#define STATE_OF_DRAIN_POMP         RA0      //output
#define STATE_OF_WATER_TAP          RA1      //output

#define SENSOR_OF_WATER_COUNTER     RB0      //input
#define SENSOR_OF_WATER_LEVEL       RB6      //input
#define SENSOR_OF_WATER_ON_FLOOR    RB5      //input

#define BUTTON_OF_START_PAUSE       RB4      //input
#define BUTTON_OF_POWER             RB7      //input

#define SELECTOR_1                  RA4      //input
#define SELECTOR_2                  RA6      //input
#define SELECTOR_3                  RA5      //input

// for LEDs
#define LIGHT_ON    1
#define LIGHT_OFF    0

// for sensors and buttons
#define IS_ON       0
#define IS_OFF      1



/* TODO Application specific user parameters used in user.c may go here */



/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */

void InitApp(void);         /* I/O and Peripheral Initialization */
