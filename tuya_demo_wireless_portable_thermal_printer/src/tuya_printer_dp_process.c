/**
* @file tuya_printer_dp_process.c
* @author zzw
* @email: huanling.zhang@tuya.com
* @brief tuya_printer_dp_process module is used to handle cloud interaction commands
* @version 0.1
* @date 2021-08-26
*
* @copyright Copyright (c) tuya.inc 2021
*
*/

/*********************************************************************
********************************includes******************************
*********************************************************************/
#include "uni_log.h"
#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_wifi_defs.h"
/*********************************************************************
****************************private includes**************************
*********************************************************************/
#include "tuya_motor.h"
#include "tuya_bmp_decode.h"
#include "tuya_thermal_head.h"
#include "tuya_download_print.h"
#include "tuya_printer_dp_process.h"
/*********************************************************************
******************************macro define****************************
*********************************************************************/


/*********************************************************************
****************************typedef define****************************
*********************************************************************/


/*********************************************************************
****************************variable define***************************
*********************************************************************/

/*********************************************************************
****************************function define***************************
*********************************************************************/
/**
* @function:tuya_update_bat_val_dp
* @brief: battery value update
* @param[in]: none
* @return: none
*/
VOID tuya_update_bat_val_dp(UINT8_T battery_val, BOOL_T bat_alarm)
{
    OPERATE_RET op_ret = OPRT_OK;
    INT_T dp_cnt = 2, alarm = 0;  

    /* exit without connecting to the router */
    GW_WIFI_NW_STAT_E wifi_state = STAT_LOW_POWER;
    get_wf_gw_nw_status(&wifi_state);
    if (wifi_state <= STAT_AP_STA_DISC || wifi_state == STAT_STA_DISC) {
        return;
    }

    TY_OBJ_DP_S *dp_arr = (TY_OBJ_DP_S *)Malloc(dp_cnt*SIZEOF(TY_OBJ_DP_S));
    if (NULL == dp_arr) {
        PR_ERR("Bat_Val malloc failed");
        return;
    }

    memset(dp_arr, 0, dp_cnt*SIZEOF(TY_OBJ_DP_S));

    dp_arr[0].dpid = DPID_POWER_DISPLAY;
    dp_arr[0].type = PROP_ENUM;     
    dp_arr[0].time_stamp = 0;
    dp_arr[0].value.dp_enum = battery_val; 

	dp_arr[1].dpid = DPID_LOW_POWER; 
    dp_arr[1].type = PROP_BOOL;     
    dp_arr[1].time_stamp = 0;
    dp_arr[1].value.dp_bool = bat_alarm; 
    
    op_ret = dev_report_dp_json_async(NULL ,dp_arr, dp_cnt);

    Free(dp_arr);

    dp_arr = NULL;
    if (OPRT_OK != op_ret) {
        PR_ERR("Bat_Val_dev_report_dp_json_async relay_config data error,err_num",op_ret);
    }

    return;
}

/**
* @function:tuya_update_paper_alarm
* @brief: out of paper alarm dp update
* @param[in]: alarm 0 have paper
                    1 out of paper 
* @return: none
*/
VOID_T tuya_update_paper_alarm(BOOL_T alarm)
{
    OPERATE_RET op_ret = OPRT_OK;

    /* exit without connecting to the router */
    GW_WIFI_NW_STAT_E wifi_state = STAT_LOW_POWER;
    get_wf_gw_nw_status(&wifi_state);
    if (wifi_state <= STAT_AP_STA_DISC || wifi_state == STAT_STA_DISC) {
        return;
    }

    TY_OBJ_DP_S *dp_arr = (TY_OBJ_DP_S *)Malloc(SIZEOF(TY_OBJ_DP_S));
    if (NULL == dp_arr) {
        PR_ERR("malloc paper_alarm failed");
        return;
    }

    memset(dp_arr, 0, SIZEOF(TY_OBJ_DP_S));

    dp_arr[0].dpid = DPID_OUT_OF_PAPER; 
    dp_arr[0].type = PROP_BOOL;     
    dp_arr[0].time_stamp = 0;
    dp_arr[0].value.dp_bool = alarm; 

    op_ret = dev_report_dp_json_async(NULL ,dp_arr, 1);

    Free(dp_arr);

    dp_arr = NULL;
    if (OPRT_OK != op_ret) {
        PR_ERR("paper_alarm_dev_report_dp_json_async relay_config data error,err_num",op_ret);
    }

    return;

}
/**
* @function:tuya_update_print_num
* @brief: number of copies being printed
* @param[in]: print_num
* @return: none
*/
VOID_T tuya_update_print_num(UINT8_T print_num)
{
    OPERATE_RET op_ret = OPRT_OK;

    GW_WIFI_NW_STAT_E wifi_state = STAT_LOW_POWER;
    get_wf_gw_nw_status(&wifi_state);
    if (wifi_state <= STAT_AP_STA_DISC || wifi_state == STAT_STA_DISC) {
        return;
    }

    TY_OBJ_DP_S *dp_arr = (TY_OBJ_DP_S *)Malloc(SIZEOF(TY_OBJ_DP_S));
    if (NULL == dp_arr) {
        PR_ERR("malloc print_num failed");
        return;
    }

    memset(dp_arr, 0, SIZEOF(TY_OBJ_DP_S));

    dp_arr[0].dpid = DPID_NOW_PRINT_NUM; 
    dp_arr[0].type = PROP_VALUE;     
    dp_arr[0].time_stamp = 0;
    dp_arr[0].value.dp_value = print_num; 

    op_ret = dev_report_dp_json_async(NULL ,dp_arr, 1);

    Free(dp_arr);

    dp_arr = NULL;
    if (OPRT_OK != op_ret) {
        PR_ERR("print_num_dev_report_dp_json_async relay_config data error,err_num",op_ret);
    }

    return;
}
/**
* @function:tuya_update_after_printing_dp
* @brief: printed and finished need to be update
* @param[in]:  print_num set 1
               paper_num set 1
* @return: none
*/
VOID_T tuya_update_print_finish_dp(UINT8_T print_num, UINT8_T paper_num)
{
    INT_T dp_cnt = 2;   

    GW_WIFI_NW_STAT_E wifi_state = STAT_LOW_POWER;
    get_wf_gw_nw_status(&wifi_state);
    if (wifi_state <= STAT_AP_STA_DISC || wifi_state == STAT_STA_DISC) {
        return;
    }

    TY_OBJ_DP_S *dp_arr = (TY_OBJ_DP_S *)Malloc(dp_cnt*SIZEOF(TY_OBJ_DP_S));
    if (NULL == dp_arr) {
        PR_ERR("malloc printed_dp failed");
        return;
    }
    memset(dp_arr, 0, dp_cnt*SIZEOF(TY_OBJ_DP_S));

    dp_arr[0].dpid = DPID_PRINT_NUM; 
    dp_arr[0].type = PROP_VALUE;     
    dp_arr[0].time_stamp = 0;
    dp_arr[0].value.dp_value = print_num; 

    dp_arr[1].dpid = DPID_NOW_PRINT_NUM; 
    dp_arr[1].type = PROP_VALUE;     
    dp_arr[1].time_stamp = 0;
    dp_arr[1].value.dp_value = paper_num; 

    dev_report_dp_json_async(NULL ,dp_arr, dp_cnt);

    Free(dp_arr);
    dp_arr = NULL;

    return;
}
/**
* @function: tuya_printer_deal_dp_proc
* @brief: DP send down processing
* @param[in]: root -> DP dpid、type、value、 time_stamp
* @return: none
*/
VOID_T tuya_printer_deal_dp_proc(IN CONST TY_OBJ_DP_S *root)
{
    UCHAR_T dpid;
    STATIC UINT8_T num = 0, total_len = 0;

    dpid = root->dpid;
    PR_NOTICE("dpid:%d",dpid);
    switch (dpid) {
        case DPID_PRINT_NUM:
            tuya_down_print_set_num(root->value.dp_value);
        break;
        case DPID_CREATE_LINK1:
            tuya_down_print_splice_url(root->value.dp_str, NULL);
        break;
        case DPID_CREATE_LINK2:
            tuya_down_print_splice_url(NULL, root->value.dp_str);
        break;
        case DPID_HASH_MAC:

        break;
        case DPID_TOTAL_LEN:
            tuya_down_print_set_len(root->value.dp_value);
        break;
        case DPID_START_PRINT:
            tuya_down_print_start();
        break;
        default :
        break;
    }

    return;
}






