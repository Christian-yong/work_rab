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
    gy_disable,//����ĳ���
    gy_enable,//����ĳ���
    gy_ready_enable,//Ԥ������ĳ��ܣ��ڳ�������ִ������������ʱ��ʹ��
};
#define GY_REBOOT_COUNT_MAX	2//��������ʱ2��
extern u8 gy_reboot_count;
void gy_1s_timer_reboot(u32 count);

void gy_user_app_init(void);//��ʼ������user_init�е���

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

void mesh_scan_rsp_init();//ɨ��ظ������ض���

/*timer*/
void gy_10ms_timer(u32 count);//10ms��ʱ������
void gy_1s_timer(u32 count);//1�붨ʱ������



/*uart*/
#define GY_UART_RECEIVE_TIME_OUT	5//50ms��ʱ

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
extern u8 gy_uart_receive_count;//����һ����������ָ��ļ�����������ճ�ʱ����ѡ���������
extern u8 gy_uart_receive_time_count;//���ڽ��ճ�ʱ����
int gy_uart_receive_buff_handle(u8 *buff, int n);//����ӻ����ж�ȡ������
int gy_uart_receive_one_byte_handle(u8 data);//����1���ֽ�����
int gy_uart_receive_one_cmd_handle(void);//����һ�������Ĵ���ָ��
int gy_uart_normal_cmd_handle(void);//��ͨ����ָ���
int gy_uart_sig_model_cmd_handle(void);//sig modelָ���
int gy_uart_vender_model_cmd_handle(void);//vender modelָ���
void gy_10ms_timer_uart(u32 count);//����10���붨ʱ��
void gy_10ms_timer_uart_receive(void);//����������ݳ�ʱ���ڴ���10���붨ʱ�������е���
//void gy_uart_test(u8 num);//���ڲ���ʹ��
void gy_uart_send_cmd(u8 type, u8*data, u8 len);//����λ�����ʹ���Э��ָ��
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
}GY_NORMAL_CMD_DATA;//��ָͨ�����
void gy_uart_send_wifi_get_state_cmd(u8 mark);//���ͻ�ȡwifi��ǰ״̬��ָ�markΪ��ǣ���ʾ�Ƿ���ҪӦ��
void gy_uart_send_wifi_reset_cmd(u8 mark);//����wifi�ָ��������õ�ָ�markΪ��ǣ���ʾ�Ƿ���ҪӦ��
void gy_uart_send_wifi_commisioning_cmd(u8 mark);//����wifi������ָ�markΪ��ǣ���ʾ�Ƿ���ҪӦ��
void gy_uart_send_wifi_stop_commision_cmd(u8 mark);//����wifiֹͣ������ָ�markΪ��ǣ���ʾ�Ƿ���ҪӦ��
void gy_1s_timer_uart_get_wifi_state(u32 count);//��WIFIģ�鷢��ѯ�ʵ�״̬��ָ���3�뿪ʼѯ�ʣ�ÿ���5��ѯ��һ�Σ�
void gy_1s_timer_uart(u32 count);//����1�붨ʱ��



/*provision automate*/
#define GY_PROVISION_TIME_OUT_MAX	10//����10��������ʱ
extern u8 gy_net_work_key[16];
extern u16 gy_net_key_index;
extern u8 gy_iv_index[4];
extern u8 gy_app_key[16];
extern u8 gy_app_key_index;
extern u8 gy_local_dev_key[16];
enum{//��ǰ����ִ�в����״̬current_step_stateȡֵ
	gy_start,
	gy_ing,
	gy_end,
};
enum{//����ִ�в���provision_stepȡֵ
	gy_idle,
	gy_scan,
	gy_set_node,
	gy_bind_node,
};
#define GY_SCAN_TIME_OUT_MAX	10//ɨ���豸10�볬ʱ
#define GY_SET_NODE_TIME_OUT_MAX	20//����δ�����ڵ�20�볬ʱ
#define GY_BIND_NODE_TIME_OUT_MAX	20//�ڵ��APP_KEY20�볬ʱ
typedef struct{
	u8 function_flag;//�Զ���������ʹ�ܻ��߽���
	u8 time_out_count;//��ʱʱ�����������ʱ��
	u8 provision_step;//����ִ�в���
	u8 current_step_state;//��ǰ����ִ�в����״̬
	u8 current_node_mac[6];//��ǰ�����ڵ��mac��ַ
}GY_PROVISION_AUTOMATE_INFO;
extern GY_PROVISION_AUTOMATE_INFO gy_provision_automate_info;
void gy_provision_automate_init(void);//�Զ�������ʼ��
void gy_local_provisioner_config(void);//������������
void gy_current_node_config(void);//�ڵ���������
void gy_current_node_bind(void);//��ǰ�ڵ��
void gy_10ms_timer_provision_automate(u32 count);//�Զ�����10���붨ʱ��
void gy_1s_timer_provision_automate(u32 count);//�Զ�����1�붨ʱ��
void gy_10ms_timer_provision_automate_process(void);//�Զ������������Զ�����10ms��ʱ�������е���
void gy_1s_timer_provision_automate_time_out_handler(void);//�Զ�������ʱ�����������Զ�����1�붨ʱ�������е���
void gy_provision_automate_start(void);//��ʼ��������
void gy_provision_automate_stop(void);//ֹͣ��������


/*button*/
#define GY_DOUBLE_CLICK_INTERVAL_10MS_TIME_MAX	50//ʵ��˫����ť����������һ�ζ̰������ɿ���ʼ��ʱ��50*10ms֮���ٴζ̰���������˫������
#define GY_XIAODOU_10MS_TIME_MAX	5//5*10ms����ʱ��
enum{
	gy_button_release,
	gy_button_press,
};
typedef struct{
	u8 state;
	u32 press_10ms_count;
	u16 press_1s_count;
	u16 double_click_interval_count;//˫����ť֮���ʱ����
	u8 double_click_flag;//1��ʾ�����ɿ�֮��ִ��˫�����ܣ�0��ʾ�����ɿ�֮��ִ��˫������
}GY_BUTTON;
extern GY_BUTTON gy_button;
void gy_button_init(void);//������ʼ��
u8 gy_read_button(void);//��ȡ��ť״̬
void gy_10ms_timer_button(u32 count);//��ť10���붨ʱ��
void gy_1s_timer_button(u32 count);//��ť1�붨ʱ��
void gy_switch_temp_by_double_click_button(u8 flag);//˫���л�ɫ��




/*led*/
unsigned short gy_calc_pwm_duty_by_100_level(u8 level);//����PWM�ٷֱ�
#define GY_PWM_DUTY(a)	gy_calc_pwm_duty_by_100_level(a)//����PWM�ٷֱ�
#define GY_LED_ON	GY_PWM_DUTY(20)
#define GY_LED_OFF	GY_PWM_DUTY(0)
enum{
	gy_led_off,
	gy_led_on,
};
enum{
	gy_indicate_1,//ָʾ�Ƶ�һ��״̬�����Ƴ���
	gy_indicate_2,//ָʾ�Ƶڶ���״̬���Ƶ���˸
	gy_indicate_3,//ָʾ�Ƶ�����״̬���̵Ƴ���
	gy_indicate_4,//ָʾ�Ƶ�����״̬���̵���˸
	gy_indicate_5,//ָʾ�Ƶ�����״̬������������˸
	gy_indicate_6,//ָʾ�Ƶ�����״̬������������˸
	gy_indicate_7,//ָʾ�Ƶ�����״̬���ƵƳ���
	gy_indicate_8,//ָʾ�Ƶ�����״̬����Ƴ���
};
typedef struct{
	u8 r_state;
	u8 g_state;
	u8 b_state;
	u8 indicate_type;//��ǰledִ������ָʾ״̬
}GY_LED;
extern GY_LED gy_led;
void gy_led_init(void);
void gy_set_led_r_state(u8 state);
void gy_set_led_g_state(u8 state);
void gy_set_led_b_state(u8 state);
void gy_led_indicate_1(void);//ָʾ�Ƶ�һ��״̬�����Ƴ���
void gy_led_indicate_2(void);//ָʾ�Ƶڶ���״̬���Ƶ���˸
void gy_led_indicate_3(void);//ָʾ�Ƶ�����״̬���̵Ƴ���
void gy_led_indicate_4(void);//ָʾ�Ƶ�����״̬���̵���˸
void gy_led_indicate_5(void);//ָʾ�Ƶ�����״̬������������˸
void gy_led_indicate_6(void);//ָʾ�Ƶ�����״̬������������˸
void gy_led_indicate_7(void);//ָʾ�Ƶ�����״̬���ƵƳ���
void gy_led_indicate_8(void);//ָʾ�Ƶ�����״̬����Ƴ���
void gy_1s_timer_led(u32 count);//led 1�붨ʱ������
void gy_1s_timer_led_process(u8 indicate_type);//ledִ��״ָ̬ʾ����led 1�붨ʱ��������ִ��
void gy_led_r_blink(u8 count, u32 half_cycle_us);
void gy_led_g_blink(u8 count, u32 half_cycle_us);




//***gy_flash_gatt***//
#define GY_FLASH_NODE_INFO_ADDR	0x78000//�ڵ���Ϣ�洢λ��
#define GY_FLASH_NODE_INFO_LEN	0x1000	//Ԥ��0x1000���ֽڴ洢�ڵ���Ϣ
#define GY_FLASH_NODE_INFO_MAX_NUM	100	//���洢100���ڵ���Ϣ
#define GY_FLASH_NODE_INFO_FLAG		0x66//flag�洢��ֵ
#define GY_GATT_SEND_ONE_PACKET_DATA_MAX		11//GATT��ǰ����һ������Ч���ݵ�����ֽ���
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
void gy_flash_node_info_write(void);//��flash��д��ڵ���Ϣ
void gy_flash_node_info_clear(void);//���flash�еĽڵ���Ϣ
void gy_gatt_send(u8 type, u8* buff, u8 len);//��GATT�з�������
void gy_gatt_send_mesh_info(void);//��GATT�з���mesh������Ϣ
void gy_gatt_send_node_info(void);//��GATT�з��ͽڵ���Ϣ



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
	u8 state_back;//��סWIFIģ���ǰһ��״̬���ڳ�������ִ��WIFI�����Ĺ�����ʹ��
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
void gy_turn_on_light(u16 device_addr);//����
void gy_turn_off_light(u16 device_addr);//�ص�
void gy_control_light_temp(u16 device_addr, u16 temp_value);

/**
  * @}
  */

/**
  * @}
  */


