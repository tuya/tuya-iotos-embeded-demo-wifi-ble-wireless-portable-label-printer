/**
* @file tuya_device.h
* @author zzw
* @email: huanling.zhang@tuya.com
* @brief tuya_device module is used to application file entry
* @version 0.1
* @date 2021-08-26
*
* @copyright Copyright (c) tuya.inc 2021
*
*/

#ifndef __TUYA_DEVICE_H__
#define __TUYA_DEVICE_H__
/*********************************************************************
********************************includes******************************
*********************************************************************/
#include "tuya_error_code.h"

/*********************************************************************
****************************private includes**************************
*********************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _TUYA_DEVICE_GLOBAL
    #define _TUYA_DEVICE_EXT 
#else
    #define _TUYA_DEVICE_EXT extern
#endif /* _TUYA_DEVICE_GLOBAL */ 

/*********************************************************************
******************************macro define****************************
*********************************************************************/
// device information define
#define DEV_SW_VERSION USER_SW_VER
#define PRODECT_ID "h9twaqx7vl3vpo5p"

/* wifi config */
#define WIFI_WORK_MODE_SEL          GWCM_OLD_PROD   // wifi mode select
#define WIFI_CONNECT_OVERTIME_S     180             // wifi network timeout，unit:s

#define WIFI_KEY_PIN                TY_GPIOA_10 // key port 
#define WIFI_KEY_TIMER_MS           100         // scan time
#define WIFI_KEY_SEQ_PRESS_MS       400         // SEQ time
#define WIFI_KEY_LOW_LEVEL_ENABLE   TRUE        // TRUE：key down low，FALSE：key down high
#define CONN_WF_TIME   3000         // long press connect wifi time

#define WIFI_LED_PIN                TY_GPIOA_17 // LED(yellow) port   low power 
#define WIFI_LED_LOW_LEVEL_ENABLE   FALSE       // TRUE：Low level light  FALSE：High level light
#define WIFI_LED_FAST_FLASH_MS      300         // quick flash time 300ms 
#define WIFI_LED_LOW_FLASH_MS       500         // slow flash time 500ms

/* according to hardware design change TY_GPIOA_ */
#define PH1     TY_GPIOA_26     // MOTOR0_AOUT1  A
#define PH2     TY_GPIOA_6      // MOTOR0_AOUT2 /A
#define PH3     TY_GPIOA_7      // MOTOR0_BOUT1  B
#define PH4     TY_GPIOA_8      // MOTOR0_BOUT2 /B

/* according to hardware design change TY_GPIOA_ */
#define PRINT_POWER     TY_GPIOA_9
#define PRINT_DST	    TY_GPIOA_20
#define PRINT_LAT	    TY_GPIOA_24
#define BAT_LED_PIN	    TY_GPIOA_28
#define PAPER_SENSOR    TY_GPIOA_15
/*********************************************************************
****************************typedef define****************************
*********************************************************************/


/*********************************************************************
****************************variable define***************************
*********************************************************************/


/*********************************************************************
****************************function define***************************
*********************************************************************/
_TUYA_DEVICE_EXT \
OPERATE_RET device_init(VOID_T);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*__TUYA_DEVICE_H__*/