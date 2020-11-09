/********************************************************************************************************
 * @file     user_app_config.h 
 *
 * @brief    for TLSR chips
 *
 * @author	 telink
 * @date     Sep. 30, 2010
 *
 * @par      Copyright (c) 2010, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *           
 *			 The information contained herein is confidential and proprietary property of Telink 
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms 
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai) 
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in. 
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this 
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided. 
 *           
 *******************************************************************************************************/
#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

/*
    Note: only can use "#define", "#if .. #elif..#endif", etc. here. Don't use "enum" "typedef".
*/
#define GY_EXIST_CODE	1
#define GY_NEW_ADD_CODE	1

#define GY_EASY_BRIDGE_PRODUCT	0//********************Easy bridge
#define GY_DONGLE_PRODUCT		1//********************Dongle
#define GY_TURING_BOARD			2//********************Turing开发板

#define GY_PRODUCT	GY_EASY_BRIDGE_PRODUCT//***************编译出来的文件适用于哪种产品

#define GY_CID	0x07A5

#define GY_VID	0x0103//v1.3//产品版本高字节为主版本号，低字节为次版本号

#define GY_PRO_OOB 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F

#if(GY_PRODUCT == GY_EASY_BRIDGE_PRODUCT)//Easy bridge  __PROJECT_MESH_GW_NODE__=1  CHIP_TYPE=CHIP_TYPE_8258
#define GY_UART_TX_PIN	GPIO_PC2
#define GY_UART_RX_PIN	GPIO_PC3
#define GY_BUTTON_PIN	GPIO_PC1
#define GY_LED_R_PIN	GPIO_PB4
#define GY_LED_G_PIN	GPIO_PB5
#define GY_LED_B_PIN	GPIO_PC4
#define GY_PWM_FUNC_R  AS_PWM  // AS_PWM_SECOND
#define GY_PWM_FUNC_G  AS_PWM  // AS_PWM_SECOND
#define GY_PWM_FUNC_B  AS_PWM  // AS_PWM_SECOND
#define GY_PWMID_R     (GET_PWMID(GY_LED_R_PIN, GY_PWM_FUNC_R))
#define GY_PWMID_G     (GET_PWMID(GY_LED_G_PIN, GY_PWM_FUNC_G))
#define GY_PWMID_B     (GET_PWMID(GY_LED_B_PIN, GY_PWM_FUNC_B))
#define GY_PWM_INV_R   (GET_PWM_INVERT_VAL(GY_LED_R_PIN, GY_PWM_FUNC_R))
#define GY_PWM_INV_G   (GET_PWM_INVERT_VAL(GY_LED_G_PIN, GY_PWM_FUNC_G))
#define GY_PWM_INV_B   (GET_PWM_INVERT_VAL(GY_LED_B_PIN, GY_PWM_FUNC_B))
#define GY_PID	6666

#elif(GY_PRODUCT == GY_DONGLE_PRODUCT)//Dongle  __PROJECT_MESH_PRO__=1  CHIP_TYPE=CHIP_TYPE_8258
#define GY_UART_TX_PIN	GPIO_PC2
#define GY_UART_RX_PIN	GPIO_PC3
#define GY_BUTTON_PIN	GPIO_PC1
#define GY_PID	6667

#elif(GY_PRODUCT == GY_TURING_BOARD)//Turing开发板   __PROJECT_MESH_PRO__=1   CHIP_TYPE=CHIP_TYPE_8258
#define GY_UART_TX_PIN	GPIO_PB7
#define GY_UART_RX_PIN	GPIO_PD7
#define GY_BUTTON_PIN	GPIO_PC1
#define GY_PID	6668

#endif

// ------- function Macro ---------
// #define CB_USER_FACTORY_RESET_ADDITIONAL()   cb_user_factory_reset_additional()
// #define CB_USER_PROC_LED_ONOFF_DRIVER(on)    cb_user_proc_led_onoff_driver(on)


#include "user_app_default.h"   // must at the end of this file

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
