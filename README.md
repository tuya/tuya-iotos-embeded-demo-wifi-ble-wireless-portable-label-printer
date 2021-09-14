# Tuya IoTOS Embeded Demo WiFi & BLE Wireless Portable Label Printer 

[English](./README.md) | [中文](./README_zh.md) 

<br>

## Introduction 

This demo is based on [Tuya IoT Platform](https://iot.tuya.com/), Tuya Smart APP, IoTOS Embeded WiFi&Ble SDK, using Tuya WiFi/WiFi+Ble series modules quickly build a wireless printer application. With the function of printing text and simple pictures.

<br>


## Quick start 

### Compile & Burn

- After creating a product on [Tuya IoT Platform](https://iot.tuya.com/), download the SDK from the development materials of hardware development.

- Download the demo to the demos directory of the SDK:

  ```bash
  $ cd demos
  $ git clone https://https://github.com/tuya/tuya-iotos-embeded-demo-wifi-ble-wireless-portable-label-printer.git
  ```

  (Note: The build.sh in the Demo is only required to be used in the SDK below version 2.0.3.)

- Execute the following command in the SDK root directory to start compiling:

  ```bash
  sh build_app.sh demos/tuya_demo_wireless-portable-label-printer tuya_demo_wireless-portable-label-printer 1.0.0
  ```

- Firmware burn-in license information please refer to: [General Wi-Fi SDK programming authorization instructions](https://developer.tuya.com/cn/docs/iot/tuya-common-wifi-sdk-burning-and-authorization?id=K9ip0gbawnkn7)  

<br>

### File description 

```
├── include/* Header files */
│   ├── common
│   ├── driver
│   ├── soc
│   ├── tuya_download_print.c	/* image download and print */
│   ├── tuya_battery_monitor.h	/* Battery level detection */
│   ├── tuya_bmp_decode.h	/* Bmp image decode */
│   ├── tuya_device.h	/* Entry file of application layer */
│   ├── tuya_motor.h	/* Stepper motor drive */
│   ├── tuya_printer_dp_process.h	/* DP process */
│   └── tuya_thermal_head.h	/* Thermal head drive */
└── src	/* Source code files */
│   ├── common
│   ├── driver
│   ├── soc
│   ├── src
│   ├── tuya_download_print.c	/* image download and print */
│   ├── tuya_battery_monitor.c	/* Battery level detection */
│   ├── tuya_bmp_decode.c	/* Bmp image decode */
│   ├── tuya_device.c	/* Entry file of application layer */
│   ├── tuya_motor.c	/* Stepper motor drive */
│   ├── tuya_printer_dp_process.c	/* DP process */
│   └── tuya_thermal_head.c	/* Thermal head drive */
├── LICENSE
├── README.md
├── README_zh.md
```

<br>

### Entry to application

Entry file: /demos/src/tuya_device.c

Important function：`device_init()`

- Call the `tuya_iot_wf_soc_dev_init_param()` interface to initialize the SDK, configure the working mode, network distribution mode, and register various callback functions and save the firmware key and PID.
- Call the `tuya_iot_reg_get_wf_nw_stat_cb()` interface to register the device network status callback function.

<br>

### Data point (DP)

- Report dp point interface: dev_report_dp_json_async()

| Function name | OPERATE_RET dev_report_dp_json_async(IN CONST CHAR_T *dev_id,IN CONST TY_OBJ_DP_S *dp_data,IN CONST UINT_T cnt) |
| ------------- | ------------------------------------------------------------ |
| devid         | device id (if gateway, MCU, SOC class device then devid = NULL; if sub-device, then devid = sub-device_id) |
| dp_data       | dp structure array name                                      |
| cnt           | Number of elements of the dp structure array                 |
| Return        | OPRT_OK: Success Other: Failure                              |

<br>

### I/O list

| 外设                   | I/O  | 外设                                | I/O  |
| ---------------------- | ---- | ----------------------------------- | ---- |
| stepper motor IN1A     | PA26 | thermal head CLK                    | PA14 |
| stepper motor IN1B     | PA6  | thermal head MOSI                   | PA16 |
| stepper motor IN2A     | PA7  | thermal head 0ut-of-paper detection | PA15 |
| stepper motor IN2B     | PA8  | thermal head DST                    | PA20 |
| distribution Indicator | PA17 | thermal head LAT                    | PA24 |
| heat power             | PA9  |                                     |      |

<br>

## Reference

- [General Wi-Fi SDK Instruction](https://developer.tuya.com/en/docs/iot/tuya-common-wifi-sdk?id=K9glcmvw4u9ml) 
- [General Wi-Fi SDK Demo Instruction](https://developer.tuya.com/en/docs/iot/tuya-wifi-sdk-demo-instructions?id=K9oce5ayw5xem) 
- [Tuya Project Hub](https://developer.tuya.com/demo)

<br>

## Technical Support

You can get support from Tuya with the following methods:

- [Tuya IoT Developer Platform](https://developer.tuya.com/en/)
- [Help Center](https://support.tuya.com/en/help)
- [Service & Support](https://service.console.tuya.com)

<br>

