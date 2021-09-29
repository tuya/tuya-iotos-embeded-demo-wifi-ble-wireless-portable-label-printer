/**
* @file tuya_download_print.c
* @author zzw
* @email: huanling.zhang@tuya.com
* @brief tuya_download_print module is used to download image and print
* @version 0.1
* @date 2021-09-14
*
* @copyright Copyright (c) tuya.inc 2021
*
*/

/*********************************************************************
********************************includes******************************
*********************************************************************/
#include "uni_thread.h"
#include "tuya_hal_thread.h"
#include "tuya_iot_internal_api.h"
/*********************************************************************
****************************private includes**************************
*********************************************************************/
#include "tuya_motor.h"
#include "tuya_bmp_decode.h"
#include "tuya_download_print.h"
#include "tuya_printer_dp_process.h"
/*********************************************************************
******************************macro define****************************
*********************************************************************/
#define PULL_SIZE      1024    // Size of each image download 1K
/*********************************************************************
****************************typedef define****************************
*********************************************************************/
typedef struct{
    UINT_T  total_len;
	UINT8_T paper_num;	// print num
	UINT_T  print_count;	// printing num
    UCHAR_T first_url[255];
    UCHAR_T second_url[255];
    BYTE_T  file_hmac[32];
}TY_BMPINFO_T;
/*********************************************************************
****************************variable define***************************
*********************************************************************/
SEM_HANDLE pv_handle;
STATIC TY_BMPINFO_T bmp_info;
THREAD_HANDLE download_thrd;
/*********************************************************************
****************************function define***************************
*********************************************************************/
VOID tuya_down_print_file_thread(VOID *param);

/**
* @function:tuya_down_print_creat_thread
* @brief: create thread and semaphore
* @param[in]: none
* @return: none
*/
VOID tuya_down_print_creat_thread(VOID) 
{
    OPERATE_RET op_ret = OPRT_OK;

    op_ret = tuya_hal_semaphore_create_init(&pv_handle, 0, 1);
    if (OPRT_OK != op_ret) {
        PR_ERR("creat_semaphore_failed!!!:%d", op_ret);
        return op_ret;
    }

    if (NULL == download_thrd) {
        tuya_hal_thread_create(&download_thrd, "dowmload_bmp", 1024*3, TRD_PRIO_2, tuya_down_print_file_thread, NULL);
    }

    return;
}
/**
* @function:tuya_down_print_set_num
* @brief: set number of copies
* @param[in]: num  print num
* @return: none
*/
VOID tuya_down_print_set_num(UINT8_T num)
{   
    bmp_info.paper_num = num;

    return;
}
/**
* @function: tuya_down_print_set_len
* @brief: set image total length
* @param[in]: total_len image total length
* @return: none
*/
VOID tuya_down_print_set_len(UINT_T total_len)
{   
    bmp_info.total_len = total_len;

    return;
}
/**
* @function:tuya_down_print_splice_url
* @brief: srtcat two url
* @param[in]: str_1 first_url
* @param[in]: str_2 second_url
* @return: none
*/
VOID tuya_down_print_splice_url(UCHAR_T *str_1, UCHAR_T *str_2)
{
    if (NULL == str_2 && str_1 != NULL) {
        memset (bmp_info.first_url, 0, 255);
        memcpy(bmp_info.first_url, str_1, strlen(str_1));
    } else if (NULL == str_1 && str_2 != NULL) {
        memset (bmp_info.second_url, 0, 255);
        memcpy(bmp_info.second_url, str_2, strlen(str_2));
    }

    return;
}
/**
* @function:tuya_down_print_start
* @brief:creat semaphore start print 
* @param[in]: none
* @return: none
*/
VOID tuya_down_print_start(VOID)
{
    tuya_hal_semaphore_post(pv_handle);

    return;
}
/**
* @function: iot_download_image_cb
* @brief: print the parsed bmp data
* @param[in]: priv_data -> NULL
* @param[in]: total_len -> size of bmp 
* @param[in]: offset -> offset of undownloaded data
* @param[in]: data -> downloaded data
* @param[in]: len -> length of downloaded data
* @param[out]: remain_len -> undownloaded data length
* @return: success -> OPRT_OK
*/
STATIC OPERATE_RET iot_download_image_cb(IN PVOID_T priv_data, IN CONST UINT_T total_len,IN CONST UINT_T offset,\
                                            IN CONST BYTE_T *data,IN CONST UINT_T len,OUT UINT_T *remain_len)
{
    STATIC BOOL_T is_head_parse = FALSE;
    OPERATE_RET op_ret = OPRT_OK;
    STATIC UINT_T data_offset = 0, bmp_size = 0;
    UINT_T print_len = 0, start_pos = 0;
    STATIC BMP_HANDLE bmp_handle = NULL;
    UCHAR_T *print_buf = NULL, *bmp_data = NULL;

    PR_NOTICE("total_len:%d offset:%d len:%d", total_len, offset, len);
    
    /* if view dowaload date open this macro */
    #if 0   
    INT_T i;
    for (i = 0; i < len; i++) {
        PR_DEBUG_RAW("%02x ", data[i]);
        if ( (i + 1) % 20 == 0) {
            PR_DEBUG_RAW("\r\n");
        }
    }
    #endif

    if (FALSE == is_head_parse) {   // start parse bmp header
        op_ret = tuya_bmp_parse_head(data, len, &data_offset, &bmp_size, &bmp_handle);
        if (op_ret != OPRT_OK) {
            PR_ERR("head parse fail %d", op_ret);
            *remain_len = len;
            return op_ret;
        }

        is_head_parse = TRUE;

        if (len <= data_offset) {
            PR_ERR("len <= data_offset");
            return OPRT_OK;
        }

        start_pos = data_offset;
        op_ret = tuya_bmp_parse_data(bmp_handle, &data[start_pos], len-start_pos, &print_buf, &print_len);
        if (op_ret != OPRT_OK) {
            PR_ERR("parse bmp data fail %d", op_ret);
            *remain_len = len - start_pos;
            return op_ret;
        }

        // start print
        tuya_tmlhead_start_print(0, 48, print_buf, print_len);
        tuya_bmp_out_buf_release(print_buf), print_buf = NULL;
        *remain_len = len - start_pos - print_len;

        return OPRT_OK;
    } else {
        if ((offset + len) < data_offset) {
            PR_ERR("(offset + len) < data_offset) ");
            return OPRT_OK;
        }

        start_pos = (offset > data_offset) ? 0 : (data_offset - offset);

        op_ret = tuya_bmp_parse_data(bmp_handle, &data[start_pos], len-start_pos, &print_buf, &print_len);
        if (op_ret != OPRT_OK) {
            PR_ERR("tuya_bmp_parse_data %d ", op_ret);
            *remain_len = len-start_pos;
            return OPRT_OK;
        }

        // start print
        tuya_tmlhead_start_print(0, 48, print_buf, print_len);
        tuya_bmp_out_buf_release(print_buf), print_buf = NULL;
        *remain_len = len - start_pos - print_len;

        tuya_update_print_num(bmp_info.print_count + 1);

        return OPRT_OK;
    }

    return OPRT_OK;    
}
/**
* @function: tuya_bmp_download_file_thread
* @brief: image print task
* @param[in]: none
* @return: none
*/
VOID tuya_down_print_file_thread(VOID *param)
{
    CHAR_T file_hmac[32];

    while (1) {
        CHAR_T *image_url = (CHAR_T *)malloc(512*sizeof(CHAR_T));

        tuya_hal_semaphore_wait(pv_handle);
        strcat(image_url, bmp_info.first_url);
        strcat(image_url, bmp_info.second_url);
        for (bmp_info.print_count = 0; bmp_info.print_count < bmp_info.paper_num; bmp_info.print_count++) {
            iot_httpc_download_file(image_url, PULL_SIZE, iot_download_image_cb, NULL, bmp_info.total_len, file_hmac); 
            tuya_motor_feedPaper_line(80, FORWARD, 2);   
        }

        tuya_motor_idle_motor();
        bmp_info.print_count = 1, bmp_info.paper_num = 1;
        tuya_update_print_finish_dp(bmp_info.print_count, bmp_info.paper_num);
        tuya_hal_system_sleep(5000);

        Free(image_url);
    
    }

    return;
}
