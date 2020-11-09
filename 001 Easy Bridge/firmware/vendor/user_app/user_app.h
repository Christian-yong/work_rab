/********************************************************************************************************
 * @file     user_app.h 
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

#include "proj/tl_common.h"
#include "vendor/mesh/app.h"
#include "vendor/mesh_lpn/app.h"
#include "vendor/mesh_provision/app.h"
#include "vendor/mesh_switch/app.h"

/** @addtogroup user_app
  * @{
  */
  
/** @defgroup user_app
  * @brief Common User Code.
  * @{
  */


// --------- function  ----------------
void cb_user_factory_reset_additional();
void cb_user_proc_led_onoff_driver(int on);


//////////////////////////////////////////////////////////GY_Define////////////////////////////////////////////////////////////////////
/*gy_public*/
enum{
    gy_disable,//禁用某项功能
    gy_enable,//启用某项功能
    gy_ready_enable,//预备启用某项功能，在长按按键执行蓝牙配网的时候使用
};
#define GY_REBOOT_COUNT_MAX	2//重启倒计时2秒
extern u8 gy_reboot_count;
void gy_1s_timer_reboot(u32 count);

void gy_user_app_init(void);//初始化，在user_init中调用

/*scan_rsp*/
typedef struct{
	u8 version;
	u16 pid;
	u16 vid;
}GY_SCAN_RSP_DEV_INFO;

#define GY_DEV_NAME "Smartshift Nano"
typedef struct{
 u8 len;
 u8 type;
 u8 rsv_user[29];
}GY_MESH_SCAN_RSP_INFO;

void mesh_scan_rsp_init();//扫描回复函数重定义

/*timer*/
void gy_10ms_timer(u32 count);//10ms定时器函数
void gy_1s_timer(u32 count);//1秒定时器函数



/*uart*/
#define GY_UART_RECEIVE_TIME_OUT	5//50ms超时

#define GY_NORMAL_CMD		0x60
#define GY_SIG_MODEL_CMD	0x61
#define GY_VENDER_MODEL_CMD	0x62
#define GY_GATT_CMD			0x63

#define GY_UART_CMD_HEAD_1	0x20
#define GY_UART_CMD_HEAD_2	0x20
#define GY_UART_CMD_END_1	0x02
#define GY_UART_CMD_END_2	0x02
typedef struct{
	u8 head[2];
	u8 len;
	u8 type;
	u8 data[200];
	u8 crc;
	u8 end[2];
}GY_UART_RECEIVE_CMD;
extern GY_UART_RECEIVE_CMD gy_uart_receive_cmd;
extern u8 gy_uart_receive_count;//接收一条完整串口指令的计数，如果接收超时，可选择计数清零
extern u8 gy_uart_receive_time_count;//串口接收超时计数
int gy_uart_receive_buff_handle(u8 *buff, int n);//处理从缓存中读取的数据
int gy_uart_receive_one_byte_handle(u8 data);//处理1个字节数据
int gy_uart_receive_one_cmd_handle(void);//处理一条完整的串口指令
int gy_uart_normal_cmd_handle(void);//普通串口指令处理
int gy_uart_sig_model_cmd_handle(void);//sig model指令处理
int gy_uart_vender_model_cmd_handle(void);//vender model指令处理
void gy_10ms_timer_uart(u32 count);//串口10毫秒定时器
void gy_10ms_timer_uart_receive(void);//处理接收数据超时，在串口10毫秒定时器函数中调用
//void gy_uart_test(u8 num);//串口测试使用
void gy_uart_send_cmd(u8 type, u8*data, u8 len);//向上位机发送串口协议指令
enum{
	gy_normal_cmd_opcode_wifi_state = 1,
	gy_normal_cmd_opcode_bt_state,
	gy_normal_cmd_opcode_wifi_reset,
	gy_normal_cmd_opcode_bt_reset,
	gy_normal_cmd_opcode_wifi_commissioning,
	gy_normal_cmd_opcode_bt_provision,
	gy_normal_cmd_opcode_wifi_stop_commission,
	gy_normal_cmd_opcode_bt_stop_provision,
	gy_normal_cmd_opcode_sig_model_req,
	gy_normal_cmd_opcode_vendor_model_req,
	gy_normal_cmd_opcode_del_one_mesh_node,
	gy_normal_cmd_opcode_sig_mesh_info,
	gy_normal_cmd_opcode_light_info,
	gy_normal_cmd_opcode_pass_through,
};
#define GY_UART_NORMAL_OPCODE_HIGH_VALUE	0x96
typedef struct{
	u8 opcode[2];
	u8 mark;
	u8 data[100];
}GY_NORMAL_CMD_DATA;//普通指令参数
void gy_uart_send_wifi_get_state_cmd(u8 mark);//发送获取wifi当前状态的指令，mark为标记，表示是否需要应答
void gy_uart_send_wifi_reset_cmd(u8 mark);//发送wifi恢复出厂设置的指令，mark为标记，表示是否需要应答
void gy_uart_send_wifi_commisioning_cmd(u8 mark);//发送wifi配网的指令，mark为标记，表示是否需要应答
void gy_uart_send_wifi_stop_commision_cmd(u8 mark);//发送wifi停止配网的指令，mark为标记，表示是否需要应答
void gy_1s_timer_uart_get_wifi_state(u32 count);//向WIFI模组发送询问灯状态的指令（第3秒开始询问，每间隔5秒询问一次）
void gy_1s_timer_uart(u32 count);//串口1秒定时器



/*provision automate*/
#define GY_PROVISION_TIME_OUT_MAX	10//设置10秒配网超时
extern u8 gy_net_work_key[16];
extern u16 gy_net_key_index;
extern u8 gy_iv_index[4];
extern u8 gy_app_key[16];
extern u8 gy_app_key_index;
extern u8 gy_local_dev_key[16];
enum{//当前配网执行步骤的状态current_step_state取值
	gy_start,
	gy_ing,
	gy_end,
};
enum{//配网执行步骤provision_step取值
	gy_idle,
	gy_scan,
	gy_set_node,
	gy_bind_node,
};
#define GY_SCAN_TIME_OUT_MAX	10//扫描设备10秒超时
#define GY_SET_NODE_TIME_OUT_MAX	20//设置未配网节点20秒超时
#define GY_BIND_NODE_TIME_OUT_MAX	20//节点绑定APP_KEY20秒超时
typedef struct{
	u8 function_flag;//自动配网功能使能或者禁用
	u8 time_out_count;//超时时间计数（倒计时）
	u8 provision_step;//配网执行步骤
	u8 current_step_state;//当前配网执行步骤的状态
	u8 current_node_mac[6];//当前配网节点的mac地址
}GY_PROVISION_AUTOMATE_INFO;
extern GY_PROVISION_AUTOMATE_INFO gy_provision_automate_info;
void gy_provision_automate_init(void);//自动配网初始化
void gy_local_provisioner_config(void);//本地配网设置
void gy_current_node_config(void);//节点网络设置
void gy_current_node_bind(void);//当前节点绑定
void gy_10ms_timer_provision_automate(u32 count);//自动配网10毫秒定时器
void gy_1s_timer_provision_automate(u32 count);//自动配网1秒定时器
void gy_10ms_timer_provision_automate_process(void);//自动配网程序，在自动配网10ms定时器函数中调用
void gy_1s_timer_provision_automate_time_out_handler(void);//自动配网超时处理函数，在自动配网1秒定时器函数中调用
void gy_provision_automate_start(void);//开始给灯配网
void gy_provision_automate_stop(void);//停止给灯配网


/*button*/
#define GY_DOUBLE_CLICK_INTERVAL_10MS_TIME_MAX	50//实现双击按钮的条件：第一次短按按键松开开始计时，50*10ms之内再次短按，则满足双击条件
#define GY_XIAODOU_10MS_TIME_MAX	5//5*10ms消抖时间
enum{
	gy_button_release,
	gy_button_press,
};
typedef struct{
	u8 state;
	u32 press_10ms_count;
	u16 press_1s_count;
	u16 double_click_interval_count;//双击按钮之间的时间间隔
	u8 double_click_flag;//1表示按键松开之后执行双击功能，0表示按键松开之后不执行双击功能
}GY_BUTTON;
extern GY_BUTTON gy_button;
void gy_button_init(void);//按键初始化
u8 gy_read_button(void);//获取按钮状态
void gy_10ms_timer_button(u32 count);//按钮10毫秒定时器
void gy_1s_timer_button(u32 count);//按钮1秒定时器
void gy_switch_temp_by_double_click_button(u8 flag);//双击切换色温




/*led*/
unsigned short gy_calc_pwm_duty_by_100_level(u8 level);//设置PWM百分比
#define GY_PWM_DUTY(a)	gy_calc_pwm_duty_by_100_level(a)//设置PWM百分比
#define GY_LED_ON	GY_PWM_DUTY(20)
#define GY_LED_OFF	GY_PWM_DUTY(0)
enum{
	gy_led_off,
	gy_led_on,
};
enum{
	gy_indicate_1,//指示灯第一种状态：蓝灯常亮
	gy_indicate_2,//指示灯第二种状态：黄灯闪烁
	gy_indicate_3,//指示灯第三种状态：绿灯常亮
	gy_indicate_4,//指示灯第三种状态：绿灯闪烁
	gy_indicate_5,//指示灯第五种状态：红蓝交替闪烁
	gy_indicate_6,//指示灯第六种状态：绿蓝交替闪烁
	gy_indicate_7,//指示灯第七种状态：黄灯常亮
	gy_indicate_8,//指示灯第七种状态：红灯常亮
};
typedef struct{
	u8 r_state;
	u8 g_state;
	u8 b_state;
	u8 indicate_type;//当前led执行哪种指示状态
}GY_LED;
extern GY_LED gy_led;
void gy_led_init(void);
void gy_set_led_r_state(u8 state);
void gy_set_led_g_state(u8 state);
void gy_set_led_b_state(u8 state);
void gy_led_indicate_1(void);//指示灯第一种状态：蓝灯常亮
void gy_led_indicate_2(void);//指示灯第二种状态：黄灯闪烁
void gy_led_indicate_3(void);//指示灯第三种状态：绿灯常亮
void gy_led_indicate_4(void);//指示灯第四种状态：绿灯闪烁
void gy_led_indicate_5(void);//指示灯第五种状态：红蓝交替闪烁
void gy_led_indicate_6(void);//指示灯第六种状态：绿蓝交替闪烁
void gy_led_indicate_7(void);//指示灯第七种状态：黄灯常亮
void gy_led_indicate_8(void);//指示灯第七种状态：红灯常亮
void gy_1s_timer_led(u32 count);//led 1秒定时器函数
void gy_1s_timer_led_process(u8 indicate_type);//led执行状态指示，在led 1秒定时器函数中执行
void gy_led_r_blink(u8 count, u32 half_cycle_us);
void gy_led_g_blink(u8 count, u32 half_cycle_us);




//***gy_flash_gatt***//
#define GY_FLASH_NODE_INFO_ADDR	0x78000//节点信息存储位置
#define GY_FLASH_NODE_INFO_LEN	0x1000	//预留0x1000个字节存储节点信息
#define GY_FLASH_NODE_INFO_MAX_NUM	100	//最大存储100个节点信息
#define GY_FLASH_NODE_INFO_FLAG		0x66//flag存储该值
#define GY_GATT_SEND_ONE_PACKET_DATA_MAX		11//GATT当前发送一个包有效数据的最大字节数
typedef struct{
	u8 flag;
	//u8 uuid[16];
	u8 mac[6];
	u16 primary_addr;
	u8 dev_key[16];
	u16 pid;
	u16 vid;
}GY_NODE_INFO;
extern GY_NODE_INFO gy_node_info;
void gy_flash_node_info_write(void);//向flash中写入节点信息
void gy_flash_node_info_clear(void);//清空flash中的节点信息
void gy_gatt_send(u8 type, u8* buff, u8 len);//向GATT中发送数据
void gy_gatt_send_mesh_info(void);//向GATT中发送mesh网络信息
void gy_gatt_send_node_info(void);//向GATT中发送节点信息



//***gy_wifi***//
enum{//mode
	gy_wifi_factory_test_mode,
	gy_wifi_work_mode,
};
enum{//state
	gy_wifi_uncommission,
	gy_wifi_commissioning,
	gy_wifi_rejoining,
	gy_wifi_commissioned,
};
typedef struct{
	u8 mode;
	u8 state;
	u8 state_back;//记住WIFI模组的前一个状态，在长安按键执行WIFI加网的过程中使用
}GY_WIFI_INFO;
extern GY_WIFI_INFO gy_wifi_info;
void gy_wifi_init(void);



//***gy_ctl_light***//
enum{//state
	gy_ctl_light_off,
	gy_ctl_light_on,
};
typedef struct{
	u8 state;
}GY_CTL_LIGHT_INFO;
extern GY_CTL_LIGHT_INFO gy_ctl_light_info;
void gy_ctl_light_info_init(void);
void gy_control_light(u16 device_addr, u16 dim_value, u16 temp_value);
void gy_turn_on_light(u16 device_addr);//开灯
void gy_turn_off_light(u16 device_addr);//关灯
void gy_control_light_temp(u16 device_addr, u16 temp_value);

/**
  * @}
  */

/**
  * @}
  */


