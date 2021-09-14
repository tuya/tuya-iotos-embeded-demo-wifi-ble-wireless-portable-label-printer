/**
* @file tuya_printer_dp_process.h
* @author zzw
* @email: huanling.zhang@tuya.com
* @brief tuya_printer_dp_process module is used to handle cloud interaction commands
* @version 0.1
* @date 2021-08-26
*
* @copyright Copyright (c) tuya.inc 2021
*
*/

#ifndef __TUYA_PRINTER_DP_PROCESS_H__
#define __TUYA_PRINTER_DP_PROCESS_H__
/*********************************************************************
********************************includes******************************
*********************************************************************/
#include "tuya_cloud_types.h"
#include "tuya_cloud_com_defs.h"
/*********************************************************************
****************************private includes**************************
*********************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
******************************macro define****************************
*********************************************************************/
/* printer DPID */
#define DPID_PRINT_NUM      101    
#define DPID_CREATE_LINK1   102     
#define DPID_START_PRINT    103    
#define DPID_PRTINER_STATE  104    
#define DPID_OUT_OF_PAPER   105     
#define DPID_POWER_DISPLAY  106     
#define DPID_LOW_POWER      107     
#define DPID_NOW_PRINT_NUM  108     
#define DPID_PRINT_RESULT   109
#define DPID_CREATE_LINK2   110     
#define DPID_HASH_MAC   	111 
#define DPID_TOTAL_LEN	    112 

/*********************************************************************
****************************typedef define****************************
*********************************************************************/
typedef enum {
	_10p = 0,
	_20p,
	_30p,
	_40p,
	_50p,
	_60p,
	_70p,
	_80p,
	_90p,
	_100p,	
}TY_POWER_PCT_E;
/*********************************************************************
****************************variable define***************************
*********************************************************************/

/*********************************************************************
****************************function define***************************
*********************************************************************/
VOID_T tuya_update_printer_all_dp(VOID_T);
VOID_T tuya_printer_deal_dp_proc(IN CONST TY_OBJ_DP_S *root);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*__TUYA_PRINTER_DP_PROCESS_H__*/