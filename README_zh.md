# Tuya IoTOS Embeded Demo WiFi & BLE Wireless Portable Label Printer 

[English](./README.md) | [中文](./README_zh.md)

<br>

## 简介 

本 demo 基于 [涂鸦IoT平台](https://iot.tuya.com/) 、涂鸦智能APP、IoTOS Embeded WiFi & Ble SDK，使用涂鸦WiFi/WiFi+BLE系列模组快速组建一个无线打印机应用。具有打印文字和简单图片的功能。

<br>

## 快速上手

### 编译与烧录

-  在 [涂鸦IoT平台](https://iot.tuya.com/) 上创建产品后，在硬件开发的开发资料中下载 SDK。

- 下载Demo至SDK目录的demos目录下：

  ```bash
  $ cd demos
  $ git clone https://https://github.com/tuya/tuya-iotos-embeded-demo-wifi-ble-wireless-portable-label-printer.git
  ```

- 在SDK根目录下执行以下命令开始编译：

  ```bash
  sh build_app.sh demos/tuya_demo_wireless-portable-label-printer tuya_demo_wireless-portable-label-printer 1.0.0
  ```

- 固件烧录授权相关信息请参考：[通用 Wi-Fi SDK 烧录授权说明](https://developer.tuya.com/cn/docs/iot/tuya-common-wifi-sdk-burning-and-authorization?id=K9ip0gbawnkn7) 

<br>

### 文件介绍
```
├── include	/* 头文件目录 */
│   ├── common
│   ├── driver
│   ├── soc
│   ├── tuya_download_print.c	/* 图片下载和打印 */
│   ├── tuya_battery_monitor.h	/* 电池电量检测 */
│   ├── tuya_bmp_decode.h	/* bmp图片解码 */
│   ├── tuya_device.h	/* 应用层入口文件 */
│   ├── tuya_motor.h	/* 步进电机驱动 */
│   ├── tuya_printer_dp_process.h	/* dp数据处理 */
│   └── tuya_thermal_head.h	/* 打印机热敏头驱动 */
└── src	/* 源文件目录 */
│   ├── common
│   ├── driver
│   ├── soc
│   ├── src
│   ├── tuya_download_print.c	/* 图片下载和打印 */
│   ├── tuya_battery_monitor.c	/* 电池电量检测 */
│   ├── tuya_bmp_decode.c	/* bmp图片解码 */
│   ├── tuya_device.c	/* 应用层入口文件 */
│   ├── tuya_motor.c	/* 打印机步进电机驱动 */
│   ├── tuya_printer_dp_process.c	/* dp数据处理 */
│   └── tuya_thermal_head.c	/* 打印机热敏头驱动 */
├── LICENSE
├── README.md
├── README_zh.md

```

<br>

### 应用入口
入口文件：`/demos/src/tuya_device.c`

重要函数：`device_init()`

+ 调用 `tuya_iot_wf_soc_dev_init_param()` 接口进行SDK初始化，配置了工作模式、配网模式，同时注册了各种回调函数并存入了固件key和PID。
+ 调用 `tuya_iot_reg_get_wf_nw_stat_cb()`接口注册设备网络状态回调函数。

<br>

### DP点相关

- 上报dp点接口：`dev_report_dp_json_async()`

| 函数名  | OPERATE_RET dev_report_dp_json_async(IN CONST CHAR_T *dev_id,IN CONST TY_OBJ_DP_S *dp_data,IN CONST UINT_T cnt) |
| ------- | ------------------------------------------------------------ |
| devid   | 设备id（若为网关、MCU、SOC类设备则devid = NULL；若为子设备，则devid = sub-device_id) |
| dp_data | dp结构体数组名                                               |
| cnt     | dp结构体数组的元素个数                                       |
| Return  | OPRT_OK：成功  Other：失败                                   |

<br>

### I/O 列表

| 外设         | I/O  | 外设           | I/O  |
| ------------ | ---- | -------------- | ---- |
| 步进电机IN1A | PA26 | 热敏头CLK      | PA14 |
| 步进电机IN1B | PA6  | 热敏头MOSI     | PA16 |
| 步进电机IN2A | PA7  | 热敏头缺纸检测 | PA15 |
| 步进电机IN2B | PA8  | 热敏头DST      | PA20 |
| 配网指示灯   | PA17 | 热敏头LAT      | PA24 |
| 加热供电     | PA9  |                |      |

<br>

## 相关文档

- [通用 Wi-Fi SDK 说明](https://developer.tuya.com/cn/docs/iot/tuya-common-wifi-sdk?id=K9glcmvw4u9ml) 
- [通用 Wi-Fi SDK Demo 说明](https://developer.tuya.com/cn/docs/iot/tuya-wifi-sdk-demo-instructions?id=K9oce5ayw5xem) 
- [涂鸦 Demo 中心](https://developer.tuya.com/demo) 

<br>


## 技术支持

您可以通过以下方法获得涂鸦的支持:

- [涂鸦 AI+IoT 开发者平台](https://developer.tuya.com)
- [帮助中心](https://support.tuya.com/help)
- [服务与支持](https://service.console.tuya.com)

<br>

