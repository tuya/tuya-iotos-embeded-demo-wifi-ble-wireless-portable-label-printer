# Tuya IoTOS Embedded Wi-Fi & Bluetooth LE Portable Label Printer

[English](./README.md) | [中文](./README_zh.md)

<br>

## Introduction

In this demo, we will show you how to prototype a portable label printer and make it IoT-enabled. Based on the [Tuya IoT Platform](https://iot.tuya.com/), we use Tuya's Wi-Fi and Bluetooth LE combo module, SDK, and the Tuya Smart app to connect the printer to the cloud. This smart printer can print characters and simple graphics.

<br>

## Get started

### Compile and flash

- Create a product on the [Tuya IoT Platform](https://iot.tuya.com/) and download the SDK in the third step of **Hardware Development**.

- Clone this demo to the `demos` folder in the downloaded SDK.

   ```bash
   $ cd demos
   $ git clone https://https://github.com/tuya/tuya-iotos-embeded-demo-wifi-ble-wireless-portable-label-printer.git
   ```

- Run the following command in the SDK root directory to start compiling.

   ```bash
   sh build_app.sh demos/tuya_demo_wireless-portable-label-printer tuya_demo_wireless-portable-label-printer 1.0.0
   ```

- For more information about flashing and authorization, see [General Wi-Fi SDK Burning and Authorization Instruction](https://developer.tuya.com/en/docs/iot/tuya-common-wifi-sdk-burning-and-authorization?id=K9ip0gbawnkn7).

<br>

### File introduction
```
├── include  /* Header file directory */
│   ├── common
│   ├── driver
│   ├── soc
│   ├── tuya_download_print.c  /* Download and print graphics */
│   ├── tuya_battery_monitor.h	 /* Detect battery level */
│   ├── tuya_bmp_decode.h	 /* Decode BMP files */
│   ├── tuya_device.h	 /* Entry file of application layer */
│   ├── tuya_motor.h	 /* Stepper motor driver */
│   ├── tuya_printer_dp_process.h	 /* DP data handler */
│   └── tuya_thermal_head.h	 /* Thermal head driver */
└── src	 /* Source code directory */
│   ├── common
│   ├── driver
│   ├── soc
│   ├── src
│   ├── tuya_download_print.c  /* Download and print graphics */
│   ├── tuya_battery_monitor.c	 /* Detect battery level */
│   ├── tuya_bmp_decode.c  /* Decode BMP files */
│   ├── tuya_device.c	 /* Entry file of application layer */
│   ├── tuya_motor.c	 /* Stepper motor driver */
│   ├── tuya_printer_dp_process.c	 /* DP data handler */
│   └── tuya_thermal_head.c	 /* Thermal head driver */
├── LICENSE
├── README.md
├── README_zh.md

```

<br>

### Entry to application
Entry file: `/demos/src/tuya_device.c`

Main function: `device_init()`

+ Call `tuya_iot_wf_soc_dev_init_param()` for SDK initialization to configure working mode and pairing mode, register callbacks, and save the firmware key and PID.
+ Call `tuya_iot_reg_get_wf_nw_stat_cb()` to register callback of device network status.

<br>

### Data point (DP)

- Report DP data: `dev_report_dp_json_async()`

   | Function | OPERATE_RET dev_report_dp_json_async(IN CONST CHAR_T *dev_id,IN CONST TY_OBJ_DP_S *dp_data,IN CONST UINT_T cnt) |
   | ------- | ------------------------------------------------------------ |
   | devid | For gateways and devices built with the MCU or SoC, the `devid` is NULL. For sub-devices, the `devid` is `sub-device_id`. |
   | dp_data | The name of DP struct array |
   | cnt | The number of elements in the DP struct array |
   | Return | `OPRT_OK`: success. Other values: failure. |

<br>

### Pin configuration

| Peripherals | I/O | Peripherals | I/O |
| ------------ | ---- | -------------- | ---- |
| Stepper motor IN1A | PA26 | Thermal head CLK | PA14 |
| Stepper motor IN1B | PA6 | Thermal head MOSI | PA16 |
| Stepper motor IN2A | PA7 | Lack of paper detection | PA15 |
| Stepper motor IN2B | PA8 | Thermal head DST | PA20 |
| Network status indicator | PA17 | Thermal head LAT | PA24 |
| Heating | PA9 |                |      |

<br>

## Reference

- [Wi-Fi SDK Guide](https://developer.tuya.com/en/docs/iot/tuya-common-wifi-sdk?id=K9glcmvw4u9ml)
- [Wi-Fi SDK Demo](https://developer.tuya.com/en/docs/iot/tuya-wifi-sdk-demo-instructions?id=K9oce5ayw5xem)
- [Tuya Project Hub](https://developer.tuya.com/demo)

<br>


## Technical support

You can get support from Tuya with the following methods:

- [Tuya Developer Platform](https://developer.tuya.com/en/)
- [Help Center](https://support.tuya.com/help)
- [Service & Support](https://service.console.tuya.com)

<br>
