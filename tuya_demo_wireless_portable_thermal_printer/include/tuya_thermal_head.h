/**
* @file tuya_thermal_head.h
* @author zzw
* @email: huanling.zhang@tuya.com
* @brief tuya_thermal_head module is used to printer driver
* @version 0.1
* @date 2021-08-27
*
* @copyright Copyright (c) tuya.inc 2021
*
*/

#ifndef __TUYA_THERMAL_HEAD_H__
#define __TUYA_THERMAL_HEAD_H__
/*********************************************************************
********************************includes******************************
*********************************************************************/
#include "tuya_gpio.h"
#include "uni_thread.h"
#include "tuya_hal_thread.h"
#include "tuya_hal_system.h"
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
#define PRINT_SCLK  TY_GPIOA_14
#define PRINT_MOSI  TY_GPIOA_16

#define NO_PAPER      1
#define PAPER_ALARM   1
#define PAPER_NORMAL  0

/*********************************************************************
****************************typedef define****************************
*********************************************************************/
typedef enum {
    print_success = 101,
    out_of_paper = 102

}TY_PRINT_STATUS_E;

/*********************************************************************
****************************variable define***************************
*********************************************************************/


/*********************************************************************
****************************function define***************************
*********************************************************************/
VOID tuya_tmlhead_init(IN CONST TY_GPIO_PORT_E port_1, IN CONST TY_GPIO_PORT_E port_2, \
							IN CONST TY_GPIO_PORT_E port_3, IN CONST TY_GPIO_PORT_E port_4);
OPERATE_RET tuya_tmlhead_start_print(IN UINT8_T start_bytes, IN UINT8_T line_bytes, IN UINT8_T *data,\
                                IN UINT_T data_len);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*__TUYA_THERMAL_HEAD_H__*/