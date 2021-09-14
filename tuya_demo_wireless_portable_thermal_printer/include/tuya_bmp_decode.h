/**
* @file tuya_bmp_decode.h
* @author zzw
* @email: huanling.zhang@tuya.com
* @brief tuya_bmp_decode module is used to bmp decode
* @version 0.1
* @date 2021-08-26
*
* @copyright Copyright (c) tuya.inc 2021
*
*/

#ifndef __TUYA_BMP_DECODE_H__
#define __TUYA_BMP_DECODE_H__
/*********************************************************************
********************************includes******************************
*********************************************************************/
#include "tuya_motor.h"
#include "uni_thread.h"
#include "tuya_hal_thread.h"
#include "tuya_cloud_types.h"
#include "tuya_hal_semaphore.h"
/*********************************************************************
****************************private includes**************************
*********************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
******************************macro define****************************
*********************************************************************/


/*********************************************************************
****************************typedef define****************************
*********************************************************************/
typedef enum {
    _1Color_bmp = 62,
    _16Color_bmp = 118,
    _256Color_bmp = 1078,
    _24Bit_bmp = 54
}TY_BMPCOLOR_E;
typedef VOID*  BMP_HANDLE;
typedef VOID(* PRINT_NUM_CALLBACK)(VOID);
/*********************************************************************
****************************variable define***************************
*********************************************************************/
SEM_HANDLE pv_handle;
/*********************************************************************
****************************function define***************************
*********************************************************************/
OPERATE_RET tuya_bmp_Conv8bit(BYTE_T *data, FLOAT_T bmp_size, UINT16_T bmp_width, UINT16_T bmp_height);
OPERATE_RET tuya_bmp_parse_head(IN UCHAR_T *head, IN UINT_T head_len, OUT UINT_T *data_offset,\
                                     OUT UINT_T *bmp_size, OUT BMP_HANDLE *handle);
OPERATE_RET tuya_bmp_handle_release(BMP_HANDLE handle);
OPERATE_RET tuya_bmp_parse_data(IN BMP_HANDLE handle, IN UCHAR_T *data, IN UINT_T datalen,\
                                     OUT UCHAR_T **out_buf, OUT INT_T *proc_offset);
OPERATE_RET tuya_bmp_out_buf_release(UCHAR_T *buf);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*__TUYA_BMP_DECODE_H__*/