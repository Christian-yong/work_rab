/********************************************************************************************************
 * @file     user_app.c 
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
#include "proj/tl_common.h"
#if !WIN32
#include "proj/mcu/watchdog_i.h"
#include "proj_lib/mesh_crypto/mesh_md5.h"
#include "vendor/common/myprintf.h"
#endif 
#include "proj_lib/ble/ll/ll.h"
#include "proj_lib/ble/blt_config.h"
#include "vendor/common/user_config.h"
#include "proj_lib/ble/service/ble_ll_ota.h"
#include "vendor/common/app_health.h"
#include "proj_lib/sig_mesh/app_mesh.h"
#include "vendor/common/app_provison.h"
#include "vendor/common/lighting_model.h"
#include "vendor/common/sensors_model.h"
#include "vendor/common/remote_prov.h"
#include "proj_lib/mesh_crypto/sha256_telink.h"
#include "proj_lib/mesh_crypto/le_crypto.h"
#include "vendor/common/lighting_model_LC.h"
#include "vendor/common/mesh_ota.h"
#include "vendor/common/mesh_common.h"
#include "vendor/common/mesh_config.h"
#include "vendor/common/directed_forwarding.h"
#include "vendor/common/certify_base/certify_base_crypto.h"

#if(__TL_LIB_8258__ || (MCU_CORE_TYPE == MCU_CORE_8258))
#include "stack/ble/ble.h"
#elif(MCU_CORE_TYPE == MCU_CORE_8278)
#include "stack/ble_8278/ble.h"
#endif

#if FAST_PROVISION_ENABLE
#include "vendor/common/fast_provision_model.h"
#endif

#if (HCI_ACCESS==HCI_USE_UART)
#include "proj/drivers/uart.h"
#endif


void cb_user_factory_reset_additional()
{
    // TODO
}

void cb_user_proc_led_onoff_driver(int on)
{
    // TODO
}


//////////////////////////////////////////////////////////GY_Define////////////////////////////////////////////////////////////////////
/*gy_public*/
u8 gy_reboot_count = 0;
void gy_user_app_init(void)//��ʼ������user_init�е���
{
	gy_provision_automate_init();//�Զ�������ʼ��
	gy_button_init();//������ʼ��
	gy_led_init();
	gy_wifi_init();
	gy_ctl_light_info_init();
	return;
}

void gy_1s_timer_reboot(u32 count)
{
	if(gy_reboot_count)
	{
		if(gy_reboot_count == 1)
		{
			factory_reset();
			start_reboot();
		}
		gy_reboot_count--;
	}
	return;
}

/*scan_rsp*/
/*void mesh_scan_rsp_init()//ɨ��ظ������ض���
{
	mesh_scan_rsp_t tbl_scanRsp={0};
	tbl_scanRsp.vendor_id = g_vendor_id;
	tbl_scanRsp.adr_primary = ele_adr_primary;
	memcpy(tbl_scanRsp.mac_adr, tbl_mac, sizeof(tbl_scanRsp.mac_adr));
	memset(tbl_scanRsp.rsv_user, 0, sizeof(tbl_scanRsp.rsv_user));
	GY_SCAN_RSP_DEV_INFO gy_dev_info;
	gy_dev_info.version = 0x01;
	gy_dev_info.pid = GY_PID;
	gy_dev_info.vid = GY_VID;
	memset(tbl_scanRsp.rsv_user, 0x66, sizeof(tbl_scanRsp.rsv_user));
	memcpy(tbl_scanRsp.rsv_user, (u8*)&gy_dev_info, sizeof(gy_dev_info));
	tbl_scanRsp.type = GAP_ADTYPE_MANUFACTURER_SPECIFIC;	// manufacture data
	tbl_scanRsp.len = sizeof(mesh_scan_rsp_t) - 1;
	u8 rsp_len = tbl_scanRsp.len+1;
	bls_ll_setScanRspData((u8 *)&tbl_scanRsp, rsp_len);
	return;
}*/

/*scan_rsp*/
void mesh_scan_rsp_init()//ɨ��ظ������ض���
{
	 GY_MESH_SCAN_RSP_INFO gy_mesh_scan_rsp_info_t;
	 gy_mesh_scan_rsp_info_t.type = GAP_ADTYPE_LOCAL_NAME_COMPLETE;
	 memcpy(gy_mesh_scan_rsp_info_t.rsv_user, GY_DEV_NAME, sizeof(GY_DEV_NAME));
	 gy_mesh_scan_rsp_info_t.len = sizeof(GY_DEV_NAME)+1;
	 bls_ll_setScanRspData((u8 *)&gy_mesh_scan_rsp_info_t,  gy_mesh_scan_rsp_info_t.len+1);
	return;
}

/*timer*/
void gy_10ms_timer(u32 count)//10ms��ʱ������
{
	gy_10ms_timer_uart(count);//����10���붨ʱ��
	gy_10ms_timer_provision_automate(count);//�Զ�����10���붨ʱ��
	gy_10ms_timer_button(count);//��ť10���붨ʱ��
	return;
}

/*timer*/
void gy_1s_timer(u32 count)//1�붨ʱ������
{
	gy_1s_timer_reboot(count);
	gy_1s_timer_provision_automate(count);//�Զ�����1�붨ʱ��
	gy_1s_timer_button(count);//��ť1�붨ʱ��
	gy_1s_timer_led(count);//led 1�붨ʱ������
	gy_1s_timer_uart_get_wifi_state(count);//��WIFIģ�鷢��ѯ�ʵ�״̬��ָ���3�뿪ʼѯ�ʣ�ÿ���5��ѯ��һ�Σ�
	return;
}



/*uart*/
/*void gy_uart_test(u8 num)//���ڲ���ʹ��
{
	my_fifo_push_hci_tx_fifo((u8*)&num,sizeof(num), 0, 0);
}*/

/*uart*/
int gy_uart_receive_buff_handle(u8 *buff, int n)//����ӻ����ж�ȡ������
{
	int i = 0;
	for(;i<n;i++)
	{
		gy_uart_receive_one_byte_handle(buff[i]);
	}
	return 0;
}

/*uart*/
GY_UART_RECEIVE_CMD gy_uart_receive_cmd;
u8 gy_uart_receive_count = 0;
u8 gy_uart_receive_para_count = 0;
u8 gy_uart_receive_time_count = 0;
int gy_uart_receive_one_byte_handle(u8 data)//����1���ֽ�����
{
	if(gy_uart_receive_count==0 && data==GY_UART_CMD_HEAD_1)//��ͷ��һ���ֽ�
	{
		gy_uart_receive_time_count = 1;//���ô��ڽ��ճ�ʱ
		gy_uart_receive_cmd.head[0] = GY_UART_CMD_HEAD_1;
		gy_uart_receive_count++;
	}
	else if(gy_uart_receive_count==1 && data==GY_UART_CMD_HEAD_2)//��ͷ�ڶ����ֽ�
	{
		gy_uart_receive_cmd.head[1] = GY_UART_CMD_HEAD_2;
		gy_uart_receive_count++;
	}
	else if(gy_uart_receive_count==2 && data>0)//ָ���
	{
		gy_uart_receive_cmd.len = data;
		gy_uart_receive_count++;
	}
	else if(gy_uart_receive_count==3/* && (data==0x60 || data==0x61 || data==0x62)*/)//ָ�����ͣ�Ŀǰֻ�����֣�
	{
		gy_uart_receive_cmd.type = data;
		gy_uart_receive_cmd.crc = gy_uart_receive_cmd.type;
		gy_uart_receive_count++;
	}
	else if(gy_uart_receive_count==4)//ָ�����
	{
		//static u8 gy_uart_receive_para_count = 0;
		if(gy_uart_receive_para_count <= gy_uart_receive_cmd.len-1)
		{
			gy_uart_receive_cmd.data[gy_uart_receive_para_count] = data;

			gy_uart_receive_cmd.crc = (u8)(gy_uart_receive_cmd.crc+data);

			if(gy_uart_receive_para_count == gy_uart_receive_cmd.len-1-1)
			{
				gy_uart_receive_count++;
				gy_uart_receive_para_count = 0;
			}
			else
			{
				gy_uart_receive_para_count++;
			}
		}
		else
		{
			gy_uart_receive_count = 0;
			gy_uart_receive_para_count = 0;
		}
	}
	else if(gy_uart_receive_count==5)//У��λ
	{
		if(gy_uart_receive_cmd.crc==data)
		{
			gy_uart_receive_count++;
		}
		else
		{
			gy_uart_receive_count = 0;
		}
	}
	else if(gy_uart_receive_count==6 && data==GY_UART_CMD_END_1)//��β��һ���ֽ�
	{
		gy_uart_receive_cmd.end[0] = GY_UART_CMD_END_1;
		gy_uart_receive_count++;
	}
	else if(gy_uart_receive_count==7 && data==GY_UART_CMD_END_2)//��β�ڶ����ֽ�
	{
		gy_uart_receive_cmd.end[1] = GY_UART_CMD_END_2;
		gy_uart_receive_time_count = 0;//����ָ�������ϣ���ʱ����Ч����������
		gy_uart_receive_one_cmd_handle();//����һ�������Ĵ���ָ��
		gy_uart_receive_count = 0;
	}
	else if(gy_uart_receive_count != 0)
	{
		gy_uart_receive_count = 0;
	}
	return 0;
}

//void gy_all_light_on(void)
//{//E8 FF 00 00 00 00 02 01 FF FF 82 02 01 00
//	u8 gy_rx_data_1[] = {0xE8, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0xFF, 0xFF, 0x82, 0x02, 0x01, 0x00};
//	app_hci_cmd_from_usb_handle (gy_rx_data_1, sizeof(gy_rx_data_1)); // for both usb and uart
//	return;
//}
//
//void gy_all_light_off(void)
//{//E8 FF 00 00 00 00 02 01 FF FF 82 02 00 00
//	u8 gy_rx_data_1[] = {0xE8, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0xFF, 0xFF, 0x82, 0x02, 0x00, 0x00};
//	app_hci_cmd_from_usb_handle (gy_rx_data_1, sizeof(gy_rx_data_1)); // for both usb and uart
//	return;
//}

/*uart*/
void gy_uart_send_cmd(u8 type, u8*data, u8 len)//����λ�����ʹ���Э��ָ��
{
	u8 gy_uart_send_buff_t[200] = {0};
	gy_uart_send_buff_t[0] = 0x20;
	gy_uart_send_buff_t[1] = 0x20;
	gy_uart_send_buff_t[2] = 1+len;
	gy_uart_send_buff_t[3] = type;
	memcpy(&gy_uart_send_buff_t[4], data, len);
	gy_uart_send_buff_t[len+4] = gy_uart_send_buff_t[3];
	u8 i = 0;
	for(i = 0; i<len; i++)
	{
		gy_uart_send_buff_t[len+4] = (u8)(gy_uart_send_buff_t[len+4] + gy_uart_send_buff_t[i+4]);
	}
	gy_uart_send_buff_t[len+5] = 0x02;
	gy_uart_send_buff_t[len+6] = 0x02;
	my_fifo_push_hci_tx_fifo(gy_uart_send_buff_t,len+7, 0, 0);
	return;
}

/*uart*/
int gy_uart_receive_one_cmd_handle(void)//����һ�������Ĵ���ָ��
{
	switch(gy_uart_receive_cmd.type)
	{
	case GY_NORMAL_CMD:
	{
		gy_uart_normal_cmd_handle();//��ͨ����ָ���
		break;
	}
	case GY_SIG_MODEL_CMD:
	{
		gy_uart_sig_model_cmd_handle();//sig modelָ���
		break;
	}
	case GY_VENDER_MODEL_CMD:
	{
		break;
	}
	}
	return 0;
}

/*uart*/
int gy_uart_normal_cmd_handle(void)//��ͨ����ָ���
{
	GY_NORMAL_CMD_DATA * gy_normal_cmd_data_t = (GY_NORMAL_CMD_DATA *)gy_uart_receive_cmd.data;
	if(gy_normal_cmd_data_t->opcode[0] != GY_UART_NORMAL_OPCODE_HIGH_VALUE || (gy_normal_cmd_data_t->mark != 0x00 && gy_normal_cmd_data_t->mark != 0x01 && gy_normal_cmd_data_t->mark != 0x02))
	{
		return -1;
	}
	switch(gy_normal_cmd_data_t->opcode[1])
	{
	case gy_normal_cmd_opcode_wifi_state:
	{
		if(gy_normal_cmd_data_t->mark == 0x00 || gy_normal_cmd_data_t->mark == 0x02)
		{
			gy_wifi_info.mode = gy_normal_cmd_data_t->data[0];
			gy_wifi_info.state = gy_normal_cmd_data_t->data[1];
			if(gy_wifi_info.mode == gy_wifi_factory_test_mode && gy_wifi_info.state == gy_wifi_commissioned)
			{
				static u8 gy_factory_test_first = 0;
				if(gy_factory_test_first == 0)
				{
					gy_led_indicate_7();//ָʾ�Ƶ�����״̬���ƵƳ���
					gy_factory_test_first = 1;
				}
			}
		}

		break;
	}
//	case gy_normal_cmd_opcode_bt_state:
//	{
//		break;
//	}
	case gy_normal_cmd_opcode_wifi_reset:
	{
		if(gy_normal_cmd_data_t->mark == 0x00 || gy_normal_cmd_data_t->mark == 0x02)
		{
			gy_wifi_info.mode = gy_normal_cmd_data_t->data[0];
			gy_wifi_info.state = gy_normal_cmd_data_t->data[1];
		}
		break;
	}
//	case gy_normal_cmd_opcode_bt_reset:
//	{
//		break;
//	}
	case gy_normal_cmd_opcode_wifi_commissioning:
	{
		if(gy_normal_cmd_data_t->mark == 0x00 || gy_normal_cmd_data_t->mark == 0x02)
		{
			gy_wifi_info.mode = gy_normal_cmd_data_t->data[0];
			gy_wifi_info.state = gy_normal_cmd_data_t->data[1];
		}
		break;
	}
//	case gy_normal_cmd_opcode_bt_provision:
//	{
//		break;
//	}
	case gy_normal_cmd_opcode_wifi_stop_commission:
	{
		if(gy_normal_cmd_data_t->mark == 0x00 || gy_normal_cmd_data_t->mark == 0x02)
		{
			gy_wifi_info.mode = gy_normal_cmd_data_t->data[0];
			gy_wifi_info.state = gy_normal_cmd_data_t->data[1];
		}
		break;
	}
//	case gy_normal_cmd_opcode_bt_stop_provision:
//	{
//		break;
//	}
	case gy_normal_cmd_opcode_sig_model_req:
	{
		break;
	}
	case gy_normal_cmd_opcode_vendor_model_req:
	{
		break;
	}
	case gy_normal_cmd_opcode_del_one_mesh_node:
	{
		break;
	}
	case gy_normal_cmd_opcode_sig_mesh_info:
	{
		break;
	}
	case gy_normal_cmd_opcode_light_info:
	{
		break;
	}
	case gy_normal_cmd_opcode_pass_through:
	{
		break;
	}
	}
	return 0;
}

/*uart*/
int gy_uart_sig_model_cmd_handle(void)//sig modelָ���
{
	mesh_bulk_cmd((mesh_bulk_cmd_par_t *)(gy_uart_receive_cmd.data), gy_uart_receive_cmd.len-1);
	return 0;
}

/*uart*/
int gy_uart_vender_model_cmd_handle(void)//vender modelָ���
{
	return 0;
}

/*uart*/
void gy_10ms_timer_uart_receive(void)//����������ݳ�ʱ���ڴ���10���붨ʱ�������е���
{
	if(gy_uart_receive_time_count)//���ڽ��մ�������
	{
		if(gy_uart_receive_time_count >= GY_UART_RECEIVE_TIME_OUT)//���մ������ݳ�ʱ
		{
			gy_uart_receive_time_count = 0;
			gy_uart_receive_count = 0;
			gy_uart_receive_para_count = 0;
		}
		else
		{
			gy_uart_receive_time_count++;
		}
	}
	return;
}

/*uart*/
void gy_10ms_timer_uart(u32 count)//����10���붨ʱ��
{
	gy_10ms_timer_uart_receive();//����������ݳ�ʱ���ڴ���10���붨ʱ�������е���
	return;
}

/*uart*/
void gy_uart_send_wifi_get_state_cmd(u8 mark)//���ͻ�ȡwifi��ǰ״̬��ָ�markΪ��ǣ���ʾ�Ƿ���ҪӦ��
{
	u8 gy_send_uart_data_t[] = {0x96, gy_normal_cmd_opcode_wifi_state, mark};
	gy_uart_send_cmd(GY_NORMAL_CMD, gy_send_uart_data_t, sizeof(gy_send_uart_data_t));//����λ��������ͨ����Э��ָ��
	return;
}

/*uart*/
void gy_uart_send_wifi_reset_cmd(u8 mark)//����wifi�ָ��������õ�ָ�markΪ��ǣ���ʾ�Ƿ���ҪӦ��
{
	u8 gy_send_uart_data_t[] = {0x96, gy_normal_cmd_opcode_wifi_reset, mark};
	gy_uart_send_cmd(GY_NORMAL_CMD, gy_send_uart_data_t, sizeof(gy_send_uart_data_t));//����λ��������ͨ����Э��ָ��
	return;
}

/*uart*/
void gy_uart_send_wifi_commisioning_cmd(u8 mark)//����wifi������ָ�markΪ��ǣ���ʾ�Ƿ���ҪӦ��
{
	u8 gy_send_uart_data_t[] = {0x96, gy_normal_cmd_opcode_wifi_commissioning, mark};
	gy_uart_send_cmd(GY_NORMAL_CMD, gy_send_uart_data_t, sizeof(gy_send_uart_data_t));//����λ��������ͨ����Э��ָ��
	return;
}

/*uart*/
void gy_uart_send_wifi_stop_commision_cmd(u8 mark)//����wifiֹͣ������ָ�markΪ��ǣ���ʾ�Ƿ���ҪӦ��
{
	u8 gy_send_uart_data_t[] = {0x96, gy_normal_cmd_opcode_wifi_stop_commission, mark};
	gy_uart_send_cmd(GY_NORMAL_CMD, gy_send_uart_data_t, sizeof(gy_send_uart_data_t));//����λ��������ͨ����Э��ָ��
	return;
}

/*uart*/
void gy_1s_timer_uart_get_wifi_state(u32 count)//��WIFIģ�鷢��ѯ�ʵ�״̬��ָ���3�뿪ʼѯ�ʣ�ÿ���5��ѯ��һ�Σ�
{
	if(count>=3 && ((count-3)%5==0) && gy_button.press_1s_count == 0)
	{
		gy_uart_send_wifi_get_state_cmd(0x01);//���ͻ�ȡwifi��ǰ״̬��ָ�markΪ��ǣ���ʾ�Ƿ���ҪӦ��
	}
	return;
}


/*uart*/
void gy_1s_timer_uart(u32 count)//����1�붨ʱ��
{
	gy_1s_timer_uart_get_wifi_state(count);//��WIFIģ�鷢��ѯ�ʵ�״̬��ָ���3�뿪ʼѯ�ʣ�ÿ���5��ѯ��һ�Σ�
	return;
}









/*provision automate*/
u8 gy_net_work_key[16] = {0x31,0x38,0x32,0x37,0x31,0x36,0x33,0x32,0x34,0x36,0x32,'G','Y','O','N','G'};
u16 gy_net_key_index = 0x0000;
u8 gy_iv_index[4] = {0x11,0x22,0x33,0x44};
u8 gy_app_key[16] = {0x32,0x30,0x32,0x30,0x31,0x30,0x32,0x32,0x30,0x30,0x32,0x30,'y','o','n','g'};
u8 gy_app_key_index = 0x0000;
u8 gy_local_dev_key[16] = {0x60,0x96,0x47,0x71,0x73,0x4f,0xbd,0x76,0xe3,0xb4,0x05,0x19,0xd1,0xd9,0x4a,0x48};
GY_PROVISION_AUTOMATE_INFO gy_provision_automate_info;

/*provision automate*/
void gy_provision_automate_init(void)//�Զ�������ʼ��
{
	memcpy(gy_net_work_key,tbl_mac,6);
	memcpy(gy_app_key,tbl_mac,6);
	memcpy(gy_local_dev_key,tbl_mac,6);

	gy_provision_automate_info.function_flag = gy_disable;
	gy_provision_automate_info.time_out_count = 0;
	return;
}

/*provision automate*/
void gy_10ms_timer_provision_automate_process(void)//�Զ������������Զ�����10ms��ʱ�������е���
{
	if(gy_provision_automate_info.function_flag == gy_enable)
	{
		switch(gy_provision_automate_info.provision_step)
		{
		case gy_scan://ɨ���豸
		{
			if(gy_provision_automate_info.current_step_state == gy_start)
			{
				set_provision_stop_flag_act(0);//��ʼɨ��
				gy_provision_automate_info.current_step_state = gy_ing;
			}
			else if(gy_provision_automate_info.current_step_state == gy_end)
			{
				set_gateway_adv_filter(gy_provision_automate_info.current_node_mac);
				//set_provision_stop_flag_act(1);//ֹͣɨ�裬�˴�ע�͵�����Ȼ�޷��ص�
				gy_provision_automate_info.provision_step = gy_set_node;
				gy_provision_automate_info.current_step_state = gy_start;
				gy_provision_automate_info.time_out_count = GY_SET_NODE_TIME_OUT_MAX;
			}
			break;
		}
		case gy_set_node://�����豸�ڵ���Ϣ
		{
			if(gy_provision_automate_info.current_step_state == gy_start)
			{
				gy_current_node_config();//�ڵ���������
				gy_provision_automate_info.current_step_state = gy_ing;
			}
			else if(gy_provision_automate_info.current_step_state == gy_end)
			{
				gy_provision_automate_info.provision_step = gy_bind_node;
				gy_provision_automate_info.current_step_state = gy_start;
				gy_provision_automate_info.time_out_count = GY_BIND_NODE_TIME_OUT_MAX;
			}
			break;
		}
		case gy_bind_node://���豸�ڵ�
		{
			if(gy_provision_automate_info.current_step_state == gy_start)
			{
				gy_current_node_bind();//��ǰ�ڵ��
				gy_provision_automate_info.current_step_state = gy_ing;
			}
			else if(gy_provision_automate_info.current_step_state == gy_end)
			{
				set_provision_stop_flag_act(1);//ֹͣɨ��
				gy_flash_node_info_write();//��flash��д��ڵ���Ϣ
				gy_provision_automate_info.provision_step = gy_scan;
				gy_provision_automate_info.current_step_state = gy_start;
				gy_provision_automate_info.time_out_count = GY_SCAN_TIME_OUT_MAX;
			}
			break;
		}
		}
	}
	return;
}

/*provision automate*/
void gy_local_provisioner_config(void)//������������
{
	set_provisioner_para(gy_net_work_key,gy_net_key_index,0,gy_iv_index,0x0001);
	set_dev_key(gy_local_dev_key);
	VC_node_dev_key_save(0x0001,gy_local_dev_key,2);
	memcpy(gy_node_info.mac, tbl_mac, 6);
	gy_node_info.primary_addr = 0x0001;
	memcpy(gy_node_info.dev_key, gy_local_dev_key, 16);
	gy_node_info.pid = GY_PID;
	gy_node_info.vid = GY_VID;
	gy_flash_node_info_write();//��flash��д��ڵ���Ϣ
	provision_mag.unicast_adr_last = g_ele_cnt+1;
	return;
}

/*provision automate*/
void gy_current_node_config(void)//�ڵ���������
{
	set_provisionee_para(gy_net_work_key,gy_net_key_index,0,gy_iv_index,provision_mag.unicast_adr_last);
	set_gateway_provision_sts(1);
	gy_node_info.primary_addr = provision_mag.unicast_adr_last;
	return;
}

/*provision automate*/
void gy_current_node_bind(void)//��ǰ�ڵ��
{
	extern u8 pro_dat[40];
	provison_net_info_str *p_str = (provison_net_info_str *)pro_dat;
	mesh_cfg_keybind_start_trigger_event((u8*)&gy_app_key_index,gy_app_key,
		p_str->unicast_address,p_str->key_index,0);
	return;
}

/*provision automate*/
void gy_10ms_timer_provision_automate(u32 count)//�Զ�����10���붨ʱ��
{
	gy_10ms_timer_provision_automate_process();//�Զ������������Զ�����10ms��ʱ�������е���
	return;
}

/*provision automate*/
void gy_1s_timer_provision_automate(u32 count)//�Զ���������1s��ʱ���е���
{
	gy_1s_timer_provision_automate_time_out_handler();//�Զ�������ʱ�����������Զ�����1�붨ʱ�������е���
	return;
}

/*provision automate*/
void gy_1s_timer_provision_automate_time_out_handler(void)//�Զ�������ʱ����������1s��ʱ���е���
{
	if(gy_provision_automate_info.function_flag == gy_enable)
	{
		if(gy_provision_automate_info.time_out_count)
		{
			if(gy_provision_automate_info.time_out_count == 1)//������ʱ
			{//***************ִ��������ʱ����
				gy_provision_automate_stop();//ֹͣ��������
				gy_control_light(0xFFFF, 65535, 3000);
				//gy_uart_test(7);
			}
			gy_provision_automate_info.time_out_count--;
		}
	}
	return;
}

/*provision automate*/
void gy_provision_automate_start(void)//��ʼ��������
{
	if(!is_provision_success())
	{
		gy_local_provisioner_config();//������������
	}
	gy_provision_automate_info.function_flag = gy_enable;
	gy_provision_automate_info.provision_step = gy_scan;
	gy_provision_automate_info.current_step_state = gy_start;
	gy_provision_automate_info.time_out_count = GY_SCAN_TIME_OUT_MAX;
	return;
}

/*provision automate*/
void gy_provision_automate_stop(void)//ֹͣ��������
{
	set_provision_stop_flag_act(1);//ֹͣɨ��
	gy_provision_automate_info.function_flag = gy_disable;
	return;
}




/*button*/
GY_BUTTON gy_button;
void gy_button_init(void)//������ʼ��
{
	gpio_set_func(GY_BUTTON_PIN, AS_GPIO);
	gpio_set_input_en(GY_BUTTON_PIN, 1);
	gpio_setup_up_down_resistor(GY_BUTTON_PIN, PM_PIN_PULLUP_1M);
	gy_button.state = gy_button_release;
	gy_button.press_10ms_count = 0;
	gy_button.press_1s_count = 0;
	gy_button.double_click_interval_count = 0;
	gy_button.double_click_flag = 0;
	return;
}

/*button*/
u8 gy_read_button(void)//��ȡ��ť״̬
{
	return !gpio_read(GY_BUTTON_PIN);
}

/*button*/
void gy_switch_temp_by_double_click_button(u8 flag)//˫���л�ɫ��
{
	u16 gy_set_temp_value;
	switch(flag)
	{
	case 0:
	{
		gy_set_temp_value = 2700;
		break;
	}
	case 1:
	{
		gy_set_temp_value = 3000;
		break;
	}
	case 2:
	{
		gy_set_temp_value = 3500;
		break;
	}
	case 3:
	{
		gy_set_temp_value = 4000;
		break;
	}
	case 4:
	{
		gy_set_temp_value = 5000;
		break;
	}
	case 5:
	{
		gy_set_temp_value = 6500;
		break;
	}
	default:
	{
		return;
	}
	}
	gy_control_light_temp(0xFFFF, gy_set_temp_value);
	return;
}

/*button*/
void gy_10ms_timer_button(u32 count)//��ť10���붨ʱ��
{
	static u8 gy_button_state;//��ȡ��ť״̬
	gy_button_state = gy_read_button();//��ȡ��ť״̬
	if(gy_button.state != gy_button_state)
	{
		gy_button.state = gy_button_state;
		if(gy_button_state == gy_button_press)//�����ոհ���
		{
			gy_button.press_10ms_count = 1;
		}
		else if(gy_button_state == gy_button_release)//�����ո�̧��
		{
			if(gy_button.press_10ms_count >= GY_XIAODOU_10MS_TIME_MAX && gy_button.press_10ms_count <= 100)//�̰����ܣ����°�ť1��֮���ɿ���Ϊ�̰�
			{
				if(gy_button.double_click_interval_count == 0 && gy_button.double_click_flag == 0)//��һ�ζ̰�
				{
					gy_button.double_click_interval_count = GY_DOUBLE_CLICK_INTERVAL_10MS_TIME_MAX;//����ʱ�������500����֮��û���ٴζ̰�����ִ�п��ص��������ִ��˫������
				}
				else if(gy_button.double_click_flag == 1)//˫������
				{///////////////////ִ��˫������
					if(gy_wifi_info.mode == gy_wifi_factory_test_mode && gy_wifi_info.state == gy_wifi_commissioned)
					{
						static u8 gy_factory_led_flag = 0;
						if(gy_factory_led_flag == 0)
						{
							gy_led_indicate_8();//ָʾ�Ƶ�����״̬����Ƴ���
							gy_factory_led_flag = 1;
						}
						else if(gy_factory_led_flag == 1)
						{
							gy_led_indicate_1();//ָʾ�Ƶ�һ��״̬�����Ƴ���
							gy_factory_led_flag = 2;
						}
						else if(gy_factory_led_flag == 2)
						{
							gy_led_indicate_3();//ָʾ�Ƶ�����״̬���̵Ƴ���
							gy_factory_led_flag = 0;
						}
					}
					else if(gy_wifi_info.mode == gy_wifi_work_mode)
					{
						static u8 gy_double_click_count = 0;
						if(gy_double_click_count >= 6)
						{
							gy_double_click_count = 0;
						}
						gy_switch_temp_by_double_click_button(gy_double_click_count);
						gy_double_click_count++;
					}
					//////////////////////////////////////////////////////////
					gy_button.double_click_flag = 0;
				}
			}
			else if(gy_button.press_1s_count >= 3 && gy_button.press_1s_count < 6)//��������3-6��
			{
				if(gy_provision_automate_info.function_flag == gy_disable)
				{
					if(gy_wifi_info.state == gy_wifi_commissioning && gy_wifi_info.state_back != gy_wifi_commissioning)
					{
						gy_uart_send_wifi_commisioning_cmd(0x01);//����wifi������ָ�markΪ��ǣ���ʾ�Ƿ���ҪӦ��
						//gy_wifi_info.state = gy_wifi_commissioning;
					}
				}
				//gy_uart_test(0x95);
			}
			else if(gy_button.press_1s_count >= 6 && gy_button.press_1s_count < 10)//��������6-10��
			{
				if(gy_provision_automate_info.function_flag == gy_ready_enable)
				{
					gy_provision_automate_start();//��ʼ��������
				}
				//gy_uart_test(0x96);
			}
			else if(gy_button.press_1s_count >=10)//��������10������
			{
				//start_reboot();
			}
			gy_button.press_10ms_count = 0;
			gy_button.press_1s_count = 0;
		}
	}
	else
	{
		if(gy_button.press_10ms_count)//�����������
		{
			gy_button.press_10ms_count++;
			if(gy_button.press_10ms_count == GY_XIAODOU_10MS_TIME_MAX)//�����������
			{////////////////////////ִ���������ʱ�Ĳ���
				gy_button.press_1s_count = 1;
				if(gy_button.double_click_interval_count)//�����˫��
				{
					gy_button.double_click_flag = 1;
					gy_button.double_click_interval_count = 0;
				}
			}
		}
		else//��������ɿ�
		{
			if(gy_button.double_click_interval_count)//˫����������ʱ
			{
				if(gy_button.double_click_interval_count == 1)//����ʱ������ִ�п��صƹ���
				{
					if(gy_provision_automate_info.function_flag == gy_enable)//�������ģ��������������ֹͣ����
					{
						gy_provision_automate_stop();//ֹͣ��������
					}
					else if(gy_wifi_info.state == gy_wifi_commissioning)
					{
						gy_uart_send_wifi_stop_commision_cmd(0x01);//����wifiֹͣ������ָ�markΪ��ǣ���ʾ�Ƿ���ҪӦ��
						gy_wifi_info.state = gy_wifi_info.state_back;
					}
					else
					{
						if(gy_ctl_light_info.state == gy_ctl_light_off)
						{
							gy_turn_on_light(0xFFFF);//����
						}
						else if(gy_ctl_light_info.state == gy_ctl_light_on)
						{
							gy_turn_off_light(0xFFFF);//�ص�
						}
					}

					if(gy_wifi_info.mode == gy_wifi_factory_test_mode && gy_wifi_info.state == gy_wifi_commissioned)
					{
						gy_led_indicate_3();//ָʾ�Ƶ�����״̬���̵Ƴ���
					}
				}
				gy_button.double_click_interval_count--;
			}
		}
	}
	return;
}

/*button*/
void gy_1s_timer_button(u32 count)//��ť1�붨ʱ��
{
	if(gy_button.press_1s_count)//�����������
	{
		if(gy_button.press_1s_count == 3)//��������ʱ��Ϊ3��
		{
			if(gy_provision_automate_info.function_flag == gy_enable)//�������ģ��������������ֹͣ����
			{
				gy_provision_automate_stop();//ֹͣ��������
			}

			gy_wifi_info.state_back = gy_wifi_info.state;
			if(gy_wifi_info.state != gy_wifi_commissioning)
			{
				gy_wifi_info.state = gy_wifi_commissioning;
			}
			//gy_uart_test(3);
		}
		else if(gy_button.press_1s_count == 6)//��������ʱ��Ϊ6��
		{
			gy_wifi_info.state = gy_wifi_info.state_back;
			if(gy_wifi_info.state == gy_wifi_commissioning)
			{
				gy_uart_send_wifi_stop_commision_cmd(0x01);//����wifiֹͣ������ָ�markΪ��ǣ���ʾ�Ƿ���ҪӦ��
				gy_wifi_info.state = gy_wifi_uncommission;
			}
			if(gy_provision_automate_info.function_flag == gy_disable)
			{
				gy_provision_automate_info.function_flag = gy_ready_enable;
			}
			//gy_uart_test(6);
		}
		else if(gy_button.press_1s_count == 10)//��������ʱ��Ϊ10��
		{
			//gy_uart_test(10);
			gy_uart_send_wifi_reset_cmd(0x01);
			gy_provision_automate_stop();//ֹͣ��������
			gy_flash_node_info_clear();//���flash�еĽڵ���Ϣ
			//factory_reset();
			gy_reboot_count = GY_REBOOT_COUNT_MAX;
			gy_led_r_blink(5, 300*1000);

		}
		gy_button.press_1s_count++;
	}
	return;
}








/*led*/
GY_LED gy_led;
unsigned short gy_calc_pwm_duty_by_100_level(u8 level)
{
	return (unsigned short)(level*PWM_MAX_TICK/100);
}

/*led*/
void gy_led_init(void)
{
	gy_led.indicate_type = gy_indicate_1;
	//R
	pwm_set(GY_PWMID_R, PWM_MAX_TICK, GY_PWM_INV_R ? PWM_MAX_TICK - 0 : 0);
	pwm_start(GY_PWMID_R);
	gpio_set_func(GY_LED_R_PIN, GY_PWM_FUNC_R);
	gy_led.r_state = gy_led_off;

	//G
	pwm_set(GY_PWMID_G, PWM_MAX_TICK, GY_PWM_INV_G ? PWM_MAX_TICK - 0 : 0);
	pwm_start(GY_PWMID_G);
	gpio_set_func(GY_LED_G_PIN, GY_PWM_FUNC_G);
	gy_led.g_state = gy_led_off;

	//B
	pwm_set(GY_PWMID_B, PWM_MAX_TICK, GY_PWM_INV_B ? PWM_MAX_TICK - 0 : 0);
	pwm_start(GY_PWMID_B);
	gpio_set_func(GY_LED_B_PIN, GY_PWM_FUNC_B);
	gy_led.b_state = gy_led_off;

	gy_led_indicate_1();//ָʾ�Ƶ�һ��״̬�����Ƴ���
	return;
}

/*led*/
void gy_set_led_r_state(u8 state)
{
	if(gy_led.r_state != state)
	{
		if(state == gy_led_off)
		{
			pwm_set_cmp (GY_PWMID_R, GY_PWM_INV_R ? PWM_MAX_TICK - GY_LED_OFF : GY_LED_OFF);
		}
		else if(state == gy_led_on)
		{
			pwm_set_cmp (GY_PWMID_R, GY_PWM_INV_R ? PWM_MAX_TICK - GY_LED_ON : GY_LED_ON);
		}
		gy_led.r_state = state;
	}
	return;
}

/*led*/
void gy_set_led_g_state(u8 state)
{
	if(gy_led.g_state != state)
	{
		if(state == gy_led_off)
		{
			pwm_set_cmp (GY_PWMID_G, GY_PWM_INV_G ? PWM_MAX_TICK - GY_LED_OFF : GY_LED_OFF);
		}
		else if(state == gy_led_on)
		{
			pwm_set_cmp (GY_PWMID_G, GY_PWM_INV_G ? PWM_MAX_TICK - GY_LED_ON : GY_LED_ON);
		}
		gy_led.g_state = state;
	}
	return;
}

/*led*/
void gy_set_led_b_state(u8 state)
{
	if(gy_led.b_state != state)
	{
		if(state == gy_led_off)
		{
			pwm_set_cmp (GY_PWMID_B, GY_PWM_INV_B ? PWM_MAX_TICK - GY_LED_OFF : GY_LED_OFF);
		}
		else if(state == gy_led_on)
		{
			pwm_set_cmp (GY_PWMID_B, GY_PWM_INV_B ? PWM_MAX_TICK - GY_LED_ON : GY_LED_ON);
		}
		gy_led.b_state = state;
	}
	return;
}

/*led*/
void gy_led_indicate_1(void)//ָʾ�Ƶ�һ��״̬�����Ƴ���
{
	gy_set_led_r_state(gy_led_off);
	gy_set_led_g_state(gy_led_off);
	gy_set_led_b_state(gy_led_on);
	return;
}

/*led*/
void gy_led_indicate_2(void)//ָʾ�Ƶڶ���״̬���Ƶ���˸
{
	gy_set_led_b_state(gy_led_off);
	gy_set_led_r_state(!gy_led.g_state);
	gy_set_led_g_state(!gy_led.g_state);
	return;
}

/*led*/
void gy_led_indicate_3(void)//ָʾ�Ƶ�����״̬���̵Ƴ���
{
	gy_set_led_r_state(gy_led_off);
	gy_set_led_g_state(gy_led_on);
	gy_set_led_b_state(gy_led_off);
	return;
}

/*led*/
void gy_led_indicate_4(void)//ָʾ�Ƶ�����״̬���̵���˸
{
	gy_set_led_r_state(gy_led_off);
	gy_set_led_b_state(gy_led_off);
	gy_set_led_g_state(!gy_led.g_state);
	return;
}

/*led*/
void gy_led_indicate_5(void)//ָʾ�Ƶ�����״̬������������˸
{
	gy_set_led_g_state(gy_led_off);
	gy_set_led_r_state(gy_led.b_state);
	gy_set_led_b_state(!gy_led.b_state);
	return;
}

/*led*/
void gy_led_indicate_6(void)//ָʾ�Ƶ�����״̬������������˸
{
	gy_set_led_r_state(gy_led_off);
	gy_set_led_g_state(gy_led.b_state);
	gy_set_led_b_state(!gy_led.b_state);
	return;
}

/*led*/
void gy_led_indicate_7(void)//ָʾ�Ƶ�����״̬���ƵƳ���
{
	gy_set_led_r_state(gy_led_on);
	gy_set_led_g_state(gy_led_on);
	gy_set_led_b_state(gy_led_off);
	return;
}

/*led*/
void gy_led_indicate_8(void)//ָʾ�Ƶ�����״̬����Ƴ���
{
	gy_set_led_r_state(gy_led_on);
	gy_set_led_g_state(gy_led_off);
	gy_set_led_b_state(gy_led_off);
	return;
}

void gy_1s_timer_led(u32 count)//led 1�붨ʱ������
{
	if(gy_wifi_info.mode == gy_wifi_work_mode)
	{
		if((gy_wifi_info.state == gy_wifi_uncommission) && (gy_provision_automate_info.function_flag == gy_disable))
		{
			gy_led.indicate_type = gy_indicate_1;
		}
		else if((gy_wifi_info.state == gy_wifi_commissioning) && (gy_provision_automate_info.function_flag == gy_disable))
		{
			gy_led.indicate_type = gy_indicate_2;
		}
		else if((gy_wifi_info.state == gy_wifi_commissioned) &&	(gy_provision_automate_info.function_flag == gy_disable))
		{
			gy_led.indicate_type = gy_indicate_3;
		}
		else if((gy_wifi_info.state == gy_wifi_rejoining) && (gy_provision_automate_info.function_flag == gy_disable))
		{
			gy_led.indicate_type = gy_indicate_4;
		}
		if((gy_wifi_info.state == gy_wifi_uncommission) &&
				(gy_provision_automate_info.function_flag == gy_enable || gy_provision_automate_info.function_flag == gy_ready_enable))
		{
			gy_led.indicate_type = gy_indicate_5;
		}
		if((gy_wifi_info.state == gy_wifi_commissioned) &&
				(gy_provision_automate_info.function_flag == gy_enable || gy_provision_automate_info.function_flag == gy_ready_enable))
		{
			gy_led.indicate_type = gy_indicate_6;
		}
		if((gy_wifi_info.state == gy_wifi_rejoining) &&
				(gy_provision_automate_info.function_flag == gy_enable || gy_provision_automate_info.function_flag == gy_ready_enable))
		{
			gy_led.indicate_type = gy_indicate_5;
		}
		gy_1s_timer_led_process(gy_led.indicate_type);//ledִ��״ָ̬ʾ����led 1�붨ʱ��������ִ��
	}
	return;
}

//***gy_led***//
void gy_led_r_blink(u8 count, u32 half_cycle_us)
{
	gy_set_led_g_state(gy_led_off);
	gy_set_led_b_state(gy_led_off);
	for(u32 i=0; i< count;i++)
	{
		wd_clear();
		gy_set_led_r_state(gy_led_off);
		sleep_us(half_cycle_us);
		wd_clear();
		gy_set_led_r_state(gy_led_on);
		sleep_us(half_cycle_us);
	}
	gy_set_led_r_state(gy_led_off);
	return;
}

//***gy_led***//
void gy_led_g_blink(u8 count, u32 half_cycle_us)
{
	gy_set_led_r_state(gy_led_off);
	gy_set_led_b_state(gy_led_off);
	for(u32 i=0; i< count;i++)
	{
		wd_clear();
		gy_set_led_g_state(gy_led_off);
		sleep_us(half_cycle_us);
		wd_clear();
		gy_set_led_g_state(gy_led_on);
		sleep_us(half_cycle_us);
	}
	gy_set_led_g_state(gy_led_off);
	return;
}

void gy_1s_timer_led_process(u8 indicate_type)//ledִ��״ָ̬ʾ����led 1�붨ʱ��������ִ��
{
	switch(indicate_type)
	{
	case gy_indicate_1:
	{
		gy_led_indicate_1();//ָʾ�Ƶ�һ��״̬����Ƴ���
		break;
	}
	case gy_indicate_2:
	{
		gy_led_indicate_2();//ָʾ�Ƶڶ���״̬���Ƶ���˸
		break;
	}
	case gy_indicate_3:
	{
		gy_led_indicate_3();//ָʾ�Ƶ�����״̬���̵Ƴ���
		break;
	}
	case gy_indicate_4:
	{
		gy_led_indicate_4();//ָʾ�Ƶ�����״̬�������˸
		break;
	}
	case gy_indicate_5:
	{
		gy_led_indicate_5();//ָʾ�Ƶ�����״̬������������˸
		break;
	}
	case gy_indicate_6:
	{
		gy_led_indicate_6();//ָʾ�Ƶ�����״̬������������˸
		break;
	}
	}
	return;
}









//***gy_flash_gatt***//
GY_NODE_INFO gy_node_info;
void gy_flash_node_info_write(void)//��flash��д��ڵ���Ϣ
{
	u8 gy_buff[GY_FLASH_NODE_INFO_LEN] = {0xff};
	flash_read_page(GY_FLASH_NODE_INFO_ADDR, GY_FLASH_NODE_INFO_LEN, gy_buff);
	GY_NODE_INFO *gy_node_p = (GY_NODE_INFO*)gy_buff;
	u8 i = 0;
	for(;i<GY_FLASH_NODE_INFO_MAX_NUM && (i+1)*sizeof(GY_NODE_INFO)<=GY_FLASH_NODE_INFO_LEN;i++)
	{
		if(gy_node_p[i].flag==0x66)
		{
			if(!memcmp(gy_node_p[i].mac, gy_node_info.mac, 6)/* && !memcmp(gy_node_p[i].uuid, gy_node_info.uuid, 16)*/)
			{//���flash֮ǰ�洢�˸ýڵ���Ϣ
				break;
			}
		}
		else if(gy_node_p[i].flag==0xff)
		{
			break;
		}
	}
	gy_node_info.flag = GY_FLASH_NODE_INFO_FLAG;
	memcpy(gy_node_info.dev_key,VC_master_get_other_node_dev_key(gy_node_info.primary_addr),16);
	if(i>=GY_FLASH_NODE_INFO_MAX_NUM || (i+1)*sizeof(GY_NODE_INFO)>GY_FLASH_NODE_INFO_LEN)//�������򶼴洢����Ч�ڵ���Ϣ
	{
		memcpy((u8*)&gy_buff[sizeof(GY_NODE_INFO)],(u8*)&gy_buff[0],GY_FLASH_NODE_INFO_LEN-sizeof(GY_NODE_INFO));
		memcpy((u8*)&gy_buff[0],(u8*)&gy_node_info,sizeof(GY_NODE_INFO));
	}
	else if(gy_node_p[i].flag==0x66)//���flash֮ǰ�洢�˸ýڵ���Ϣ
	{
		gy_node_p[i].primary_addr = gy_node_info.primary_addr;
	}
	else if(gy_node_p[i].flag==0xff)//�洢������λ�ô洢�µĽڵ���Ϣ
	{
		//memcpy((u8*)&gy_node_p[i],(u8*)&gy_node_info,sizeof(GY_NODE_INFO));
		memcpy((u8*)&gy_buff[sizeof(GY_NODE_INFO)],(u8*)&gy_buff[0],GY_FLASH_NODE_INFO_LEN-sizeof(GY_NODE_INFO));
		memcpy((u8*)&gy_buff[0],(u8*)&gy_node_info,sizeof(GY_NODE_INFO));
	}
	flash_erase_sector(GY_FLASH_NODE_INFO_ADDR);
	flash_write_page(GY_FLASH_NODE_INFO_ADDR, GY_FLASH_NODE_INFO_LEN, gy_buff);
	return;
}

void gy_flash_node_info_clear(void)//���flash�еĽڵ���Ϣ
{
	flash_erase_sector(GY_FLASH_NODE_INFO_ADDR);
	return;
}
/*

void gy_flash_node_info_read(u8* buff)//��ȡflash�нڵ���Ϣ
{
	//flash_write_page(GY_FLASH_NODE_INFO_ADDR, GY_FLASH_NODE_INFO_LEN, buf);
	return;
}
*/

//***gy_flash_gatt***//
void gy_gatt_send(u8 type, u8* buff, u8 len)//��GATT�з�������
{
	u8 i = 0;
	u8 gy_uart_send_buff_t[20] = {0};
	for(i = 0; i<len/GY_GATT_SEND_ONE_PACKET_DATA_MAX; i++)
	{
		gy_uart_send_buff_t[0] = 0x20;
		gy_uart_send_buff_t[1] = 0x20;
		gy_uart_send_buff_t[2] = GY_GATT_SEND_ONE_PACKET_DATA_MAX+3;//1+len;
		gy_uart_send_buff_t[3] = type;
		if(len%GY_GATT_SEND_ONE_PACKET_DATA_MAX)
		{
			gy_uart_send_buff_t[4] = len/GY_GATT_SEND_ONE_PACKET_DATA_MAX + 1;
		}
		else
		{
			gy_uart_send_buff_t[4] = len/GY_GATT_SEND_ONE_PACKET_DATA_MAX;
		}
		gy_uart_send_buff_t[5] = i+1;
		memcpy(&gy_uart_send_buff_t[6], &buff[GY_GATT_SEND_ONE_PACKET_DATA_MAX*i], GY_GATT_SEND_ONE_PACKET_DATA_MAX);
		gy_uart_send_buff_t[6+GY_GATT_SEND_ONE_PACKET_DATA_MAX] = gy_uart_send_buff_t[3];
		u8 j = 0;
		for(j = 0; j<gy_uart_send_buff_t[2]-1; j++)
		{
			gy_uart_send_buff_t[6+GY_GATT_SEND_ONE_PACKET_DATA_MAX] = (u8)(gy_uart_send_buff_t[6+GY_GATT_SEND_ONE_PACKET_DATA_MAX] + gy_uart_send_buff_t[4+j]);
		}
		gy_uart_send_buff_t[GY_GATT_SEND_ONE_PACKET_DATA_MAX+7] = 0x02;
		gy_uart_send_buff_t[GY_GATT_SEND_ONE_PACKET_DATA_MAX+8] = 0x02;
		bls_att_pushNotifyData (PROVISION_ATT_HANDLE, gy_uart_send_buff_t, GY_GATT_SEND_ONE_PACKET_DATA_MAX+9);
		//my_fifo_push_hci_tx_fifo(gy_uart_send_buff_t,GY_GATT_SEND_ONE_PACKET_DATA_MAX+9, 0, 0);
	}
	if(len%GY_GATT_SEND_ONE_PACKET_DATA_MAX)
	{
		gy_uart_send_buff_t[0] = 0x20;
		gy_uart_send_buff_t[1] = 0x20;
		gy_uart_send_buff_t[2] = len%GY_GATT_SEND_ONE_PACKET_DATA_MAX+3;//1+len;
		gy_uart_send_buff_t[3] = type;
		gy_uart_send_buff_t[4] = len/GY_GATT_SEND_ONE_PACKET_DATA_MAX + 1;
		gy_uart_send_buff_t[5] = i+1;
		memcpy(&gy_uart_send_buff_t[6], &buff[GY_GATT_SEND_ONE_PACKET_DATA_MAX*i], len%GY_GATT_SEND_ONE_PACKET_DATA_MAX);
		gy_uart_send_buff_t[6+len%GY_GATT_SEND_ONE_PACKET_DATA_MAX] = gy_uart_send_buff_t[3];
		u8 j = 0;
		for(j = 0; j<gy_uart_send_buff_t[2]-1; j++)
		{
			gy_uart_send_buff_t[6+len%GY_GATT_SEND_ONE_PACKET_DATA_MAX] = (u8)(gy_uart_send_buff_t[6+len%GY_GATT_SEND_ONE_PACKET_DATA_MAX] + gy_uart_send_buff_t[4+j]);
		}
		gy_uart_send_buff_t[len%GY_GATT_SEND_ONE_PACKET_DATA_MAX+7] = 0x02;
		gy_uart_send_buff_t[len%GY_GATT_SEND_ONE_PACKET_DATA_MAX+8] = 0x02;
		bls_att_pushNotifyData (PROVISION_ATT_HANDLE, gy_uart_send_buff_t, len%GY_GATT_SEND_ONE_PACKET_DATA_MAX+9);
		//my_fifo_push_hci_tx_fifo(gy_uart_send_buff_t,len%GY_GATT_SEND_ONE_PACKET_DATA_MAX+9, 0, 0);
	}
	return;
}

//***gy_flash_gatt***//
void gy_gatt_send_mesh_info(void)//��GATT�з���mesh������Ϣ
{
	u8 gy_mesh_info_buff[34];//net_key + app_key
	gy_mesh_info_buff[0] = 0x02;
	gy_mesh_info_buff[1] = 0x01;
	memcpy(&gy_mesh_info_buff[2], gy_net_work_key, 16);
	memcpy(&gy_mesh_info_buff[18], gy_app_key, 16);
	gy_gatt_send(GY_GATT_CMD, gy_mesh_info_buff, sizeof(gy_mesh_info_buff));//��GATT�з�������
	return;
}

//***gy_flash_gatt***//
void gy_gatt_send_node_info(void)//��GATT�з��ͽڵ���Ϣ
{
	u8 gy_buff[GY_FLASH_NODE_INFO_LEN] = {0xff};
	flash_read_page(GY_FLASH_NODE_INFO_ADDR, GY_FLASH_NODE_INFO_LEN, gy_buff);
	GY_NODE_INFO *gy_node_p = (GY_NODE_INFO*)gy_buff;
	u8 i = 0;
	for(;i<GY_FLASH_NODE_INFO_MAX_NUM && (i+1)*sizeof(GY_NODE_INFO)<=GY_FLASH_NODE_INFO_LEN;i++)
	{
		if(gy_node_p[i].flag==0x66)
		{
			u8 gy_node_info_t[sizeof(GY_NODE_INFO)+1] = {0xFF};
			gy_node_info_t[0] = 0x02;
			gy_node_info_t[1] = 0x02;
			memcpy(&gy_node_info_t[2], (u8*)&gy_node_p[i].mac[0], sizeof(GY_NODE_INFO)-1);
			gy_gatt_send(GY_GATT_CMD, gy_node_info_t, sizeof(GY_NODE_INFO)+1);//��GATT�з�������
		}
		else if(gy_node_p[i].flag==0xff)
		{
			break;
		}
	}
	gy_buff[0] = 0x02;
	gy_buff[1] = 0x03;
	gy_gatt_send(GY_GATT_CMD, gy_buff, 2);//��GATT�з�������
	return;
}





//***gy_wifi***//
GY_WIFI_INFO gy_wifi_info;
void gy_wifi_init(void)
{
	gy_wifi_info.mode = gy_wifi_work_mode;
	gy_wifi_info.state = gy_wifi_uncommission;
	gy_wifi_info.state_back = gy_wifi_info.state;
	return;
}





//***gy_ctl_light***//
GY_CTL_LIGHT_INFO gy_ctl_light_info;
void gy_ctl_light_info_init(void)
{
	gy_ctl_light_info.state = gy_ctl_light_on;
	return;
}

//***gy_ctl_light***//
void gy_control_light(u16 device_addr, u16 dim_value, u16 temp_value)
{//00 00 00 00 02 01 FF FF 82 5E 20 00 20 20 00 00 00 00 00
	u8 gy_uart_send_data[] = { 0x00, 0x00,0x00,0x00,0x02,0x01,(u8)device_addr,(u8)(device_addr>>8),0x82,0x5E,(u8)dim_value,(u8)(dim_value >> 8), (u8)temp_value,(u8)(temp_value >> 8),0X00,0X00,0X00/*,0X00,0X00*/ };
	mesh_bulk_cmd((mesh_bulk_cmd_par_t *)(gy_uart_send_data), sizeof(gy_uart_send_data));
	if(dim_value>0)
	{
		gy_ctl_light_info.state = gy_ctl_light_on;
	}
	else
	{
		gy_ctl_light_info.state = gy_ctl_light_off;
	}
	return;
}

//***gy_ctl_light***//
void gy_turn_on_light(u16 device_addr)//����
{//20 20 0F 61 00 00 00 00 00 00 FF FF 82 03 01 00 00 00 E4 02 02
	u8 gy_uart_send_data[] = { 0x00,0x00,0x00,0x00,0x02,0x01,(u8)device_addr,(u8)(device_addr >> 8),0x82,0x02,0x01,0x00/*,0x00,0x00*/ };
	mesh_bulk_cmd((mesh_bulk_cmd_par_t *)(gy_uart_send_data), sizeof(gy_uart_send_data));
	gy_ctl_light_info.state = gy_ctl_light_on;
	return;
}

//***gy_ctl_light***//
void gy_turn_off_light(u16 device_addr)//�ص�
{
	u8 gy_uart_send_data[] = { 0x00,0x00,0x00,0x00,0x02,0x01,(u8)device_addr,(u8)(device_addr >> 8),0x82,0x02,0x00,0x00/*,0x00,0x00*/ };
	mesh_bulk_cmd((mesh_bulk_cmd_par_t *)(gy_uart_send_data), sizeof(gy_uart_send_data));
	gy_ctl_light_info.state = gy_ctl_light_off;
	return;
}

//***gy_ctl_light***//
void gy_control_light_temp(u16 device_addr, u16 temp_value)
{
	u8 gy_uart_send_data[] = {0x00, 0x00, 0x00, 0x00, 0x02, 0x00, (u8)device_addr, (u8)(device_addr >> 8), 0x82, 0x64, (u8)temp_value, (u8)(temp_value>>8), 0x00/*, 0x00, 0x00*/};
	mesh_bulk_cmd((mesh_bulk_cmd_par_t *)(gy_uart_send_data), sizeof(gy_uart_send_data));
	return;
}









