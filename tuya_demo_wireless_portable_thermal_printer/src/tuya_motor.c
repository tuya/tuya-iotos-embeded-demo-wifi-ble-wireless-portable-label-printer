/**
* @file tuya_motor.c
* @author zzw
* @email: huanling.zhang@tuya.com
* @brief tuya_motor module is used to motor driver
* @version 0.1
* @date 2021-08-26
*
* @copyright Copyright (c) tuya.inc 2021
*
*/

/*********************************************************************
********************************includes******************************
*********************************************************************/
#include <stdbool.h>
/*********************************************************************
****************************private includes**************************
*********************************************************************/
#include "tuya_motor.h"
#include "tuya_thermal_head.h"
/*********************************************************************
******************************macro define****************************
*********************************************************************/
#define PHASE_NUM   4
/*********************************************************************
****************************typedef define****************************
*********************************************************************/

/*********************************************************************
****************************variable define***************************
*********************************************************************/
/* stepper motor phases */
UINT8_T motor_phases[][PHASE_NUM] = {
                                        {1, 0, 0, 0},   // step_1 ~ step_8
                                        {1, 0, 0, 1},
                                        {0, 0, 0, 1},
                                        {0, 1, 0, 1},
                                        {0, 1, 0, 0},
                                        {0, 1, 1, 0},
                                        {0, 0, 1, 0},
                                        {1, 0, 1, 0},
                                    };
INT_T ARRAY_SIZE = SIZEOF(motor_phases) / SIZEOF((motor_phases)[0]);
bool isIdle = TRUE;     // Idle state flag 

UINT8_T motor_pin[4] = {0};
/*********************************************************************
****************************function define***************************
*********************************************************************/
/**
 * @function: tuya_motor_pin_init
 * @brief: motor ports init
 * @param: none
 * @return: none
 * @others: none
 */
VOID tuya_motor_pin_init(IN CONST TY_GPIO_PORT_E port_1, IN CONST TY_GPIO_PORT_E port_2, \
                            IN CONST TY_GPIO_PORT_E port_3, IN CONST TY_GPIO_PORT_E port_4)
{
    motor_pin[0] = port_1;
    motor_pin[1] = port_2;
    motor_pin[2] = port_3;
    motor_pin[3] = port_4;

    /* stepper motor four phases ports init, output mode:low power */
    tuya_gpio_inout_set(motor_pin[0], FALSE);
    tuya_gpio_inout_set(motor_pin[1], FALSE);
    tuya_gpio_inout_set(motor_pin[2], FALSE);
    tuya_gpio_inout_set(motor_pin[3], FALSE);
    tuya_gpio_write(motor_pin[0], FALSE);
    tuya_gpio_write(motor_pin[1], FALSE);
    tuya_gpio_write(motor_pin[2], FALSE);
    tuya_gpio_write(motor_pin[3], FALSE);

    return;
}
/**
* @function:tuya_motor_set_motor_phases
* @brief: set motor phases
* @param[in]: phase[4]
* @return: none
*/
STATIC VOID tuya_motor_set_motor_phases(CONST UINT8_T phase[4])
{
    tuya_gpio_write(motor_pin[0], phase[0]);
    tuya_gpio_write(motor_pin[1], phase[1]);
    tuya_gpio_write(motor_pin[2], phase[2]);
    tuya_gpio_write(motor_pin[3], phase[3]);

    return;
}
/**
* @function:tuya_motor_idle_motor
* @brief: idel status no power consumption
* @param[in]: none
* @return: none
*/
VOID tuya_motor_idle_motor(VOID)
{
    if (isIdle) {
        return;
    }

    CONST UINT8_T idle_phase[] = {0, 0, 0, 0};
    tuya_motor_set_motor_phases(idle_phase);
    isIdle = TRUE;

    return;
}
/**
* @function:tuya_motor_set_motor_step
* @brief: set motor forward or backward
* @param[in]: direction: FORWARD->forward  BACKWARD->backward
* @return: none
*/
STATIC VOID tuya_motor_set_motor_step(INT8_T direction) 
{
    STATIC CHAR_T currStep = -1;   // Current number of steps
    isIdle = FALSE;

    CONST INT_T totalSteps = ARRAY_SIZE;
    currStep = (currStep + totalSteps + direction) % totalSteps;
    tuya_motor_set_motor_phases(motor_phases[currStep]);
}
/**
* @function:tuya_motor_feedPaper_line
* @brief: Feed paper for `count` lines
* @param[in]: count -> print line num
* @param[in]: direction: FORWARD->forward  BACKWARD->backward
* @param[in]: speed: Adjust the motor speed unit:ms
* @return: none
*/
VOID tuya_motor_feedPaper_line(IN UINT_T count, IN INT8_T direction, IN UINT8_T speed)
{
	INT_T i;

	if (0 == count) {
		return;
	}

	CONST UINT_T stepsPerLine = ARRAY_SIZE / 2;
	for (i = 0; i < (stepsPerLine * count); i++) {
		tuya_motor_set_motor_step(direction);
		tuya_hal_system_sleep(speed);	// delay 2ms unit:ms
	}

    return;
}