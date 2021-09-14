/**
* @file tuya_battery_monitor.c
* @author zzw
* @email: huanling.zhang@tuya.com
* @brief tuya_battery_monitor module is used to detect battery power
* @version 0.1
* @date 2021-08-26
*
* @copyright Copyright (c) tuya.inc 2021
*
*/

/*********************************************************************
********************************includes******************************
*********************************************************************/


/*********************************************************************
****************************private includes**************************
*********************************************************************/
#include "tuya_device.h"
#include "tuya_thermal_head.h"
#include "tuya_battery_monitor.h"
#include "tuya_printer_dp_process.h"
/*********************************************************************
******************************macro define****************************
*********************************************************************/
#define CKECK_TIME    3000   // 3s
/*********************************************************************
****************************typedef define****************************
*********************************************************************/


/*********************************************************************
****************************variable define***************************
*********************************************************************/
tuya_adc_t *temper_adc;

/*********************************************************************
****************************function define***************************
*********************************************************************/
/**
 * @function: tuya_batmon_ch443k_toggle
 * @brief: CH443K switch function switching
 * @param[in]: port->GPIO  
 * @param[in]: TRUE->high power FALSE->low power
 * @return: none
 * @others: none
 */
OPERATE_RET tuya_batmon_ch443k_toggle(TY_GPIO_PORT_E port, BOOL_T high)
{
	if (TRUE == high) {
		/*CH1 battery value detection*/
        tuya_gpio_write(port, TRUE);
        return 1;
	} else if (FALSE == high) {
        /*CH0 resistance temperature detection*/
        tuya_gpio_write(port, FALSE);
        return 0;
    }

    return OPRT_OK;
}
/**
 * @function: tuya_batmon_ch443k_pin_init
 * @brief: CH443K switch and adc init
 * @param[in]: adc_cfg   adc config
 * @param[in]: port_type  the port is used to ADC | IO | PWM | IIC
 * @param[in]: adc_port  the port is used to select  ADC0 | ADC1 | ADC2
 * @others: none
 */
VOID tuya_batmon_ch443k_pin_init(tuya_drv_type_t port_type, tuya_adc_port_t adc_port)
{   
    /* ch443k switch port init */
    tuya_gpio_inout_set(ADC_SELECT, FALSE);  

    /* adc init */
    temper_adc = (tuya_adc_t *)tuya_driver_find(port_type, adc_port);
    TUYA_ADC_CFG(temper_adc, adc_port, 0);
    tuya_adc_init(temper_adc);
    tuya_batmon_ch443k_toggle(ADC_SELECT, TRUE);

    return;
}
/**
* @function:tuya_batmon_BatVal_get
* @brief: get printer battery value
* @param[in]: vref -> adc reference voltage  unit:mv
* @param[in]: sample -> adc sampling accuracy 
* @param[in]: ratio -> partial voltage ratio of power supply circuit  
* @return: battery_val -> Battery voltage value
*/
STATIC UINT16_T tuya_batmon_batval_get(UINT16_T vref, UINT16_T sample, FLOAT_T ratio)
{
    UINT16_T Bat_val = 0;
    UINT16_T voltage_val = 0;
    UINT16_T adc_Bat = 0, i = 0, adc_Avg = 0;

    for (i = 0; i < 5; i++) {
        tuya_adc_convert(temper_adc, &adc_Bat, 1);
        adc_Avg += adc_Bat;
    }
    
    adc_Avg /= 5;
    voltage_val = (adc_Avg * vref) / sample + 75;   // ADC Sample 12bit Ref voltage 2.4V
    Bat_val = voltage_val * ratio;  // Voltage divider The actual battery voltage value is 4 times the voltage value taken by the adc  unit:mv
    PR_NOTICE("battery_val:%dmv", Bat_val);

    return Bat_val;
}
/**
* @function:tuya_batmon_batstatus
* @brief: Battery level display
* @param[in]: none
* @return: none
*/
VOID tuya_batmon_batstatus(VOID)
{
    UINT8_T vlotage_percent = 0;
    UINT16_T battery_val = 0;
    BOOL_T bat_alarm = 0;

    while (1) {
        battery_val = tuya_batmon_batval_get(2400, 4096, 4);

        if (battery_val <= precent_10) {
            bat_alarm = 1;
            tuya_batmon_ch443k_toggle(BAT_LED_PIN, FALSE);
            tuya_gpio_write(BAT_LED_PIN, FALSE);
        } else {
            bat_alarm = 0;
            tuya_batmon_ch443k_toggle(BAT_LED_PIN, TRUE);
            tuya_gpio_write(BAT_LED_PIN, TRUE);
        }

        if (battery_val >= precent_100) {
            vlotage_percent = _100p;
        } else if (battery_val < precent_100 && battery_val >= precent_90) {
            vlotage_percent = _90p;
        } else if (battery_val < precent_90 && battery_val >= precent_80) {
            vlotage_percent = _80p;
        } else if (battery_val < precent_80 && battery_val >= precent_70) {
            vlotage_percent = _70p;
        } else if (battery_val < precent_70 && battery_val >= precent_60) {
            vlotage_percent = _60p;
        } else if (battery_val < precent_60 && battery_val >= precent_50) {
            vlotage_percent = _50p;
        } else if (battery_val < precent_50 && battery_val >= precent_40) {
            vlotage_percent = _40p;
        } else if (battery_val < precent_40 && battery_val >= precent_30) {
            vlotage_percent = _30p;
        } else if (battery_val < precent_30 && battery_val >= precent_20) {
            vlotage_percent = _20p;
        } else {
            vlotage_percent = _10p;
        }
        
        /* Voltage detection frequency */
        tuya_hal_system_sleep(CKECK_TIME);
        tuya_update_bat_val_dp(vlotage_percent, bat_alarm);

    }
}
/**
 * @function: tuya_app_printer_bat_value_task
 * @brief: Battery power detection task
 * @param: none
 * @return:none
 * @others: none
 */
VOID tuya_batmon_batval_task(VOID)
{
    OPERATE_RET op_ret = OPRT_OK;

    op_ret = tuya_hal_thread_create(NULL, "get_bat_value", 512*4, TRD_PRIO_4, tuya_batmon_batstatus, NULL);   
    if (OPRT_OK != op_ret) {
        PR_ERR("creat BatVal_task failed!!!:%d", op_ret);
        return op_ret;
    }

}