/**
* @file tuya_thermal_head.c
* @author zzw
* @email: huanling.zhang@tuya.com
* @brief tuya_thermal_head module is used to printer driver
* @version 0.1
* @date 2021-08-26
*
* @copyright Copyright (c) tuya.inc 2021
*
*/

/*********************************************************************
********************************includes******************************
*********************************************************************/
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "spi_pub.h"
#include "http_inf.h"
#include "tuya_timer.h"
/*********************************************************************
****************************private includes**************************
*********************************************************************/
#include "tuya_motor.h"
#include "tuya_device.h"
#include "tuya_bmp_decode.h"
#include "tuya_thermal_head.h"
#include "tuya_battery_monitor.h"
#include "tuya_printer_dp_process.h"
/*********************************************************************
******************************macro define****************************
*********************************************************************/
#define SPI_BPS	    	1000000		// SPI baud 
/*********************************************************************
****************************typedef define****************************
*********************************************************************/


/*********************************************************************
****************************variable define***************************
*********************************************************************/
/*SPI struct init*/
STATIC UCHAR_T g_spi_txbuf[48];
STATIC UCHAR_T g_spi_rxbuf[48];
STATIC struct spi_message g_spi_msg = {
	.send_buf = g_spi_txbuf,
	.send_len = 48,
	.recv_buf = g_spi_rxbuf,
	.recv_len = 1,
};
UINT8_T thermal_buf[4] = {0};
/*********************************************************************
****************************function define***************************
*********************************************************************/
/**
* @function:tuya_tmlhead_init
* @brief: printer ports init
* @param[in]: none
* @return: none
*/
VOID tuya_tmlhead_init(IN CONST TY_GPIO_PORT_E port_1, IN CONST TY_GPIO_PORT_E port_2, \
							IN CONST TY_GPIO_PORT_E port_3, IN CONST TY_GPIO_PORT_E port_4)
{
	thermal_buf[0] = port_1;	// LAT
	thermal_buf[1] = port_2;	// DST
	thermal_buf[2] = port_3;	// PRINT_POWER
	thermal_buf[3] = port_4;	// PRINTER_SENSOR

	/* thermal head ports init */
	tuya_gpio_inout_set(thermal_buf[0], FALSE);
	tuya_gpio_write(thermal_buf[0], TRUE);
	tuya_gpio_inout_set(thermal_buf[1], FALSE);
	tuya_gpio_write(thermal_buf[1], FALSE);
	tuya_gpio_inout_set(thermal_buf[2], FALSE);
	tuya_gpio_write(thermal_buf[2], TRUE);		//调试完成后 此处初始化拉低 开关按键事件拉高	
		
	/* SPI init */
	bk_spi_master_dma_tx_init(SPI_MODE_0, SPI_BPS, &g_spi_msg);
	tuya_gpio_inout_set(thermal_buf[3], TRUE);
}
/**
* @function:tuya_tmlhead_reset
* @brief: Stop heating and idle the step motor
* @param[in]: none
* @return: none
*/
STATIC VOID tuya_tmlhead_reset(VOID)
{
	memset(g_spi_txbuf, 0, sizeof(g_spi_txbuf));

	/* clear printing data after powered on */
	tuya_gpio_write(thermal_buf[1], FALSE);
	tuya_gpio_write(thermal_buf[0], FALSE);
	bk_spi_master_dma_send(&g_spi_msg);
	tuya_gpio_write(thermal_buf[0], TRUE);
}

/**
* @function:tuya_tmlhead_begin_print_line
* @brief: send 1bpp data to printhead and begin heating
* @param[in]: data -> The data to be printed
* @return: none
*/
STATIC VOID tuya_tmlhead_begin_print_line(UINT8_T* data)
{
	if (1 == tuya_gpio_read(thermal_buf[3])) {
		return;
	}
	bk_spi_master_dma_send(&g_spi_msg);
	tuya_hal_system_sleep(5);
	tuya_gpio_write(thermal_buf[1], FALSE);
	tuya_gpio_write(thermal_buf[0], FALSE);
	tuya_gpio_write(thermal_buf[0], TRUE);
	tuya_gpio_write(thermal_buf[1], TRUE);
}
/**
* @function:tuya_tmlhead_end_printLine
* @brief:end heating 
* @param[in]: none
* @return: none
*/
STATIC VOID tuya_tmlhead_end_printLine(VOID)
{
	tuya_gpio_write(thermal_buf[1], FALSE);
}
/**
* @function: tuya_tmlhead_print_1bLine
* @brief: Print one line of data
* @param[in]: data -> Data to be printed
* @return: none
*/
STATIC VOID tuya_tmlhead_print_1bLine(UINT8_T* data)
{
	if (1 == tuya_gpio_read(thermal_buf[3])) {
		return;
	}
	tuya_tmlhead_begin_print_line(data);
	tuya_motor_feedPaper_line(1, FORWARD, 2);   
	tuya_tmlhead_end_printLine();	
}
/**
* @function:tuya_tmlhead_start_print
* @brief: drive the motor and the thermal head to start printing
* @param[in]: print_bytes	start print bytes point
* @param[in]: line_bytes	printer can print bytes per line
* @param[in]: data			print data
* @param[in]: data_len		print data length				  
* @param[in]: paper_cb		callback handler
* @return: success	OPRT_OK 
*/
INT_T tuya_tmlhead_start_print(IN UINT8_T start_bytes, IN UINT8_T line_bytes, IN UINT8_T *data, IN UINT_T data_len)
{
	BOOL_T paper_info = 0;
	UINT16_T start_point = 0;
	UINT16_T m, motor_num = 0;
	static UCHAR_T *p_temp = NULL;
	
	if (NULL == data || 0 == data_len) {
		return OPRT_INVALID_PARM;
	}    

	/* out of paper check */
	if (1 == tuya_gpio_read(thermal_buf[3])) {
		tuya_update_paper_alarm(1);
		PR_ERR("out_of_paper!!!");
		return out_of_paper;
	} else {
		tuya_update_paper_alarm(0);
	}

	p_temp = data;
	motor_num = data_len / line_bytes;  // print rows number
	start_point = line_bytes - ((line_bytes - start_bytes) * SIZEOF(g_spi_txbuf[0]));	// The starting point of data 

    for (m = 0; m < motor_num; m++) {
		tuya_tmlhead_reset();	// clear last time LAT register data
		if (m > 0) {	
			p_temp += line_bytes;
		}
		memset(g_spi_txbuf, 0, start_point);
		memcpy(&g_spi_txbuf[start_bytes], p_temp, (line_bytes - start_bytes));
		tuya_tmlhead_print_1bLine(g_spi_msg.send_buf);
		
	}

	return print_success;
}
