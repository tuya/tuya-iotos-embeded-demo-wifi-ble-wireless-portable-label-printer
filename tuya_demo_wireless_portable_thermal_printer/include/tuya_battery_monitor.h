/**
* @file tuya_battery_monitor.h
* @author zzw
* @email: huanling.zhang@tuya.com
* @brief tuya_battery_monitor module is used to detect battery power
* @version 0.1
* @date 2021-08-26
*
* @copyright Copyright (c) tuya.inc 2021
*
*/

#ifndef __TUYA_BATTERY_MONITOR_H__
#define __TUYA_BATTERY_MONITOR_H__
/*********************************************************************
********************************includes******************************
*********************************************************************/
#include "uni_log.h"
#include "tuya_adc.h"
#include "tuya_gpio.h"
#include "tuya_cloud_types.h"
/*********************************************************************
****************************private includes**************************
*********************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
******************************macro define****************************
*********************************************************************/
#define ADC_SELECT  TY_GPIOA_22

/*********************************************************************
****************************typedef define****************************
*********************************************************************/
typedef enum {
    precent_10 = 7360,
    precent_20 = 7480,
    precent_30 = 7540,
    precent_40 = 7580,
    precent_50 = 7640,
    precent_60 = 7740,
    precent_70 = 7840,
    precent_80 = 7960,
    precent_90 = 8120,
    precent_100 = 8400

}TY_BAT_VALUE_E;

/*********************************************************************
****************************variable define***************************
*********************************************************************/

/*********************************************************************
****************************function define***************************
*********************************************************************/
VOID tuya_batmon_ch443k_pin_init(tuya_drv_type_t port_type, tuya_adc_port_t adc_port);
OPERATE_RET tuya_batmon_ch443k_toggle(TY_GPIO_PORT_E port, BOOL_T high);
VOID tuya_batmon_batval_task(VOID);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*__TUYA_BATTERY_MONITOR_H__*/








