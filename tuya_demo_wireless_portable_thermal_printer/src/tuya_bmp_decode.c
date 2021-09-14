/**
* @file tuya_bmp_decode.c
* @author zzw
* @email: huanling.zhang@tuya.com
* @brief tuya_bmp_decode module is used to bmp decode
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
#include "uf_file.h"
#include "tuya_cloud_types.h"
/*********************************************************************
****************************private includes**************************
*********************************************************************/
#include "tuya_bmp_decode.h"
/*********************************************************************
******************************macro define****************************
*********************************************************************/
#define PULL_SIZE      1024    // Size of each image download 1K
/*********************************************************************
****************************typedef define****************************
*********************************************************************/
typedef struct {
    //image info
    UINT_T   data_offset;
    UINT_T   bmp_size;
    UINT_T   bmp_width;
    UINT_T   bmp_height;
    UINT_T   bitcount;

}TY_BMP_INFOR_T;
/*********************************************************************
****************************variable define***************************
*********************************************************************/
UINT8_T print_num = 0;
/*********************************************************************
****************************function define***************************
*********************************************************************/
/**
* @function: tuya_tmlhead_bit_reverse
* @brief: Reverses one byte of data by bit eg: 0110 1100 -> 0011 0110
* @param[in]: bmp_data  
* @return: num -> the data of reverse
*/
STATIC UCHAR_T tuya_bmp_bit_reverse(UCHAR_T bmp_data)
{
    UCHAR_T num = 0, i;

    for (i = 0; i < 8; i++) {
        num |= (bmp_data & 1) << (7 - i);
        bmp_data >>= 1;
    }

    return num;
}
/**
* @function:tuya_bmp_parse_head
* @brief: parse bmp file header
* @param[in]: head -> bmp file data
* @param[in]: head_len -> data length
* @param[out]: data_offset -> the offset of bmp header
* @param[out]: bmp_size -> bmp sizes
* @param[out]: handle -> file processing handle
* @return: success -> OPRT_OK  fail -> else
*/
OPERATE_RET tuya_bmp_parse_head(IN UCHAR_T *head, IN UINT_T head_len, OUT UINT_T *data_offset, OUT UINT_T *bmp_size, OUT BMP_HANDLE *handle)
{
    TY_BMP_INFOR_T *bmp_infor = NULL;

    if (NULL == head || NULL == data_offset || NULL ==  bmp_size || NULL == handle) {
        return OPRT_INVALID_PARM;
    }

    if (head_len < 46) {     // The first 46 bytes include bmp basic information
        return OPRT_INVALID_PARM;
    }

    if (head[0] != 'B' || head[1] != 'M') {
        PR_ERR("this is not a bmp image");
        return OPRT_INVALID_PARM;
    }

    bmp_infor = (TY_BMP_INFOR_T *)Malloc(sizeof(TY_BMP_INFOR_T));
    if (NULL == bmp_infor) {
        return OPRT_MALLOC_FAILED;
    }
    memset(bmp_infor, 0x00, sizeof(TY_BMP_INFOR_T));

    /* bmp size info  */
    bmp_infor->bmp_size = (head[5]<<24 | head[4]<<16 | head[3]<<8 | head[2]); 
    /* bmp bitcount info */
    bmp_infor->data_offset  = head[13]<<24 | head[12]<<16 | head[11]<<8 | head[10]; 
    /* bmp width info */
    bmp_infor->bmp_width = head[21]<<24 | head[20]<<16 | head[19]<<8 | head[18];		
    /* bmp height info */
	bmp_infor->bmp_height = head[25]<<24 | head[24]<<16 | head[23]<<8 | head[22];
    bmp_infor->bitcount = bmp_infor->data_offset;	
	PR_INFO("bmp_size:%dbytes data_offset:%d  width:%dpx height:%dpx", bmp_infor->bmp_size, bmp_infor->data_offset, bmp_infor->bmp_width, bmp_infor->bmp_height);

    *data_offset = bmp_infor->data_offset;
    *bmp_size = bmp_infor->bmp_size;
    *handle = (BMP_HANDLE)bmp_infor;

    return OPRT_OK;
}
/**
* @function:tuya_bmp_handle_release
* @brief: free TY_BMP_HEADER_T in 'tuya_bmp_parse_head' fun
* @param[in]: handle
* @return: OPRT_OK
*/
OPERATE_RET tuya_bmp_handle_release(BMP_HANDLE handle) 
{
    if (NULL == handle) {
        return OPRT_INVALID_PARM;
    }

    Free(handle);

    return OPRT_OK;
}
/**
* @function:tuya_bmp_parse_data
* @brief: parse bmp file valid data
* @param[in]: data  -> valid data
* @param[in]: datalen -> valid data length
* @param[in]: handle -> bmp handle
* @param[out]: out_buf -> parsed data
* @param[out]: proc_offset -> valid data offset
* @return: success -> OPRT_OK fali -> else
*/
OPERATE_RET tuya_bmp_parse_data(IN BMP_HANDLE handle, IN UCHAR_T *data, IN UINT_T datalen, OUT UCHAR_T **out_buf, OUT INT_T *proc_offset)
{
    UCHAR_T tmp = 0;
    UINT_T  width_byte = 0;
    UCHAR_T *proc_buf = NULL;
    TY_BMP_INFOR_T *bmp_infor = NULL;
    UINT_T i = 0, j = 0, m = 0, line_pos = 0, proc_len = 0; 
    
    if (NULL == data || NULL == handle || 0 == datalen || NULL == out_buf || NULL == proc_offset) {
        PR_NOTICE("data%d  handle%d  datalen%d  out_buf%d  proc_offset%d", data, handle, datalen, out_buf, proc_offset);
        return OPRT_INVALID_PARM;
    }

    bmp_infor = (TY_BMP_INFOR_T *)handle;

    switch (bmp_infor->bitcount) {
        case _1Color_bmp:
            PR_INFO("This is 1_Color");	

            width_byte = bmp_infor->bmp_width / 8;  // per line bytes
            //prepare buf
            proc_len = datalen - (datalen % width_byte);    // Convert to the proc_len % width_byte = 0
            if (0 == proc_len) {
                return OPRT_COM_ERROR;
            }

            proc_buf = (UCHAR_T *)Malloc(proc_len);
            if (NULL == proc_buf) {
                return OPRT_MALLOC_FAILED;
            }
            memset(proc_buf, 0x00, proc_len);

            for (m = 0; m < proc_len; m++) {
                // Converts the black(0x00) and white(0xFF) color of a bmp image to a black(0x01) and white(0x00) pixel block for the printer
                proc_buf[m] = tuya_bmp_bit_reverse(0xff - data[m]);   
            }

            // swap cha_buf i_th line j <==> (48-j)
            for (i = 0; i < proc_len/width_byte; i++) {     
                line_pos = i * width_byte;
                for (j = 0; j < width_byte / 2; j++) {
                    // PR_NOTICE("s:%d e:%d", line_pos + j, line_pos + width_byte - j -1);
                    tmp = proc_buf[line_pos + j];
                    proc_buf[line_pos + j] = proc_buf[line_pos + width_byte - j - 1];
                    proc_buf[line_pos + width_byte - j - 1] = tmp;
                }
            }

            *out_buf = proc_buf;
            *proc_offset = proc_len;
        break;
        default:
        break;   
    }

    return OPRT_OK;
}
/**
* @function: tuya_bmp_out_buf_release
* @brief: free out_buf memory in 'tuya_bmp_parse_data' fun
* @param[in]: none
* @return: none
*/
OPERATE_RET tuya_bmp_out_buf_release(UCHAR_T *buf)
{
    if(NULL == buf) {
        return OPRT_INVALID_PARM;
    }

    Free(buf);

    return OPRT_OK;
}