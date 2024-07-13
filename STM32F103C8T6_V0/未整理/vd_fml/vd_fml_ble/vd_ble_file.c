#include "vd_ble_file.h"

/**======include =======*/
#include "os_timer.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "os_mem.h"
#include "plf.h"
#include "driver_system.h"
#include "gatt_api.h"
#include "gap_api.h"
#include "vd_ble.h"
#include "vd_bsp_global_switch/vd_bsp_global_switch.h"

/**======define =======*/
#define FLASH_MIN_SIZE 0x1000

os_timer_t ota_timer;

static uint8_t ota_tim_cnt = 0;
uint8_t  en =0;
uint32_t image_addr_base = 0,image_addr=0,image_indx=0,image_addr_start= 0 ,image_addr_end= 0 ;
uint32_t image_size = 0;
uint32_t image_save_en = 0 ;
uint8_t image_buffer[4096+250] = {0};

extern void app_boot_load_data(uint8_t *dest, uint32_t src, uint32_t len);


/*传输文件软件定时器中断函数*/
void ota_tim_fn(void *arg){

	if(ota_tim_cnt >10){
		en = 0;
		ota_tim_cnt = 0 ;
		os_timer_stop(&ota_timer);
	}
	ota_tim_cnt++;
}


/*蓝牙接收文件*/
void app_otas_recv_data(uint8_t conidx,uint8_t *p_data,uint16_t len)
{
	uint16_t flash_status;
	if(p_data != NULL){
		if(p_data[2] == 0x91 )
		{
			ble_iamge_updata_cmd_t *ble_iamge_updata_cmd = (ble_iamge_updata_cmd_t*)&p_data[2];
			gap_conn_param_update(conidx, 6, 6, 0, 500);
			gatt_mtu_exchange_req(conidx);	
//			co_printf("0x91:");
//			show_reg(p_data,len,1);
			image_addr_base = ble_iamge_updata_cmd->image_addr;
			image_size = ble_iamge_updata_cmd->iamge_size;
			image_indx = image_addr_base % FLASH_MIN_SIZE;
			image_addr = image_addr_base - image_indx;
			image_addr_start = image_addr;
			image_addr_end = (image_addr_base+ image_size-1)/FLASH_MIN_SIZE*FLASH_MIN_SIZE;
//		co_printf("read 0 addr:0x%x, addr_start 0x%x, addr_end 0x%x , addr_base 0x%x, image_size 0x%x, image_indx 0x%x \r",image_addr,image_addr_start,image_addr_end,image_addr_base,image_size,image_indx);
			
			flash_read(image_addr,FLASH_MIN_SIZE,image_buffer);
			ota_tim_cnt = 0;
			os_timer_init(&ota_timer,ota_tim_fn,NULL);
			os_timer_start(&ota_timer,1000,1);
			en = 1;
			uint8_t replay[9] ={0x7e ,0x07,0x91,0x81,0x01,0x1A,0x7e};
			vd_gatt_ntf(conidx,replay,7);
		}
		if( en ==1){			
			if(p_data[2] == 0x92){
				ble_iamge_updata_data_t *ble_iamge_updata_data = (ble_iamge_updata_data_t*)&p_data[2];
//				co_printf("0x92:");
//				show_reg(p_data,len,1);
				memcpy(&image_buffer[image_indx],ble_iamge_updata_data->iamge_data,ble_iamge_updata_data->image_rec_cnt);
//					memcpy(&image_buffer[image_indx],p_data,len);
				image_indx +=ble_iamge_updata_data->image_rec_cnt;
				ota_tim_cnt = 0;
//					image_indx +=len;
//			co_printf("image_indx:%d \r",image_indx);
				if(image_addr_start == image_addr_end){
					if(image_indx > (image_addr_base+ image_size-1)%FLASH_MIN_SIZE)
					{
//					co_printf("save 0 addr:0x%x, addr_start 0x%x, addr_end 0x%x , addr_base 0x%x, image_size 0x%x, image_indx 0x%x \r",image_addr,image_addr_start,image_addr_end,image_addr_base,image_size,image_indx);
						en = 0;
						os_timer_stop(&ota_timer);
	//					flash_write(image_addr,FLASH_MIN_SIZE,image_buffer);

						flash_status = flash_read_status(false);		

						vd_global_int_disable(); 

						flash_write_status(flash_status & (0xff), false);  //开锁
						flash_erase(image_addr, FLASH_MIN_SIZE);                      //擦除4KB
						flash_write_status(flash_status, false);           //关锁

						flash_write_status(flash_status & (0xff), false);  //开锁
						flash_write(image_addr,FLASH_MIN_SIZE, image_buffer );        
						flash_write_status(flash_status, false);           //关锁
						
						vd_global_int_restore();
					}
				}else {
					if(image_addr != image_addr_end){
						if(image_indx > FLASH_MIN_SIZE-1){

//						co_printf("save 1 addr:0x%x, addr_start 0x%x, addr_end 0x%x , addr_base 0x%x, image_size 0x%x, image_indx 0x%x \r",image_addr,image_addr_start,image_addr_end,image_addr_base,image_size,image_indx);

	//						flash_write(image_addr,FLASH_MIN_SIZE,image_buffer);
							flash_status = flash_read_status(false);		

							vd_global_int_disable(); 

							flash_write_status(flash_status & (0xff), false);  //开锁
							flash_erase(image_addr, FLASH_MIN_SIZE);                      //擦除4KB
							flash_write_status(flash_status, false);           //关锁

							flash_write_status(flash_status & (0xff), false);  //开锁
							flash_write(image_addr,FLASH_MIN_SIZE, image_buffer );        
							flash_write_status(flash_status, false);           //关锁
							
							vd_global_int_restore();
							image_addr+=FLASH_MIN_SIZE;
							image_indx-= FLASH_MIN_SIZE;
							if(image_addr == image_addr_end){
//							co_printf("read 1 addr:0x%x, addr_start 0x%x, addr_end 0x%x , addr_base 0x%x, image_size 0x%x, image_indx 0x%x \r",image_addr,image_addr_start,image_addr_end,image_addr_base,image_size,image_indx);

	 							flash_read(image_addr,FLASH_MIN_SIZE,image_buffer);
							}
							memcpy(image_buffer,&image_buffer[FLASH_MIN_SIZE],image_indx);
						}		
					}else{
						if(image_indx > (image_addr_base+ image_size-1)%FLASH_MIN_SIZE)
						{
//						co_printf("save 2 addr:0x%x, addr_start 0x%x, addr_end 0x%x , addr_base 0x%x, image_size 0x%x, image_indx 0x%x \r",image_addr,image_addr_start,image_addr_end,image_addr_base,image_size,image_indx);
							flash_status = flash_read_status(false);		

							vd_global_int_disable(); 

							flash_write_status(flash_status & (0xff), false);  //开锁
							flash_erase(image_addr, FLASH_MIN_SIZE);                      //擦除4KB
							flash_write_status(flash_status, false);           //关锁

							flash_write_status(flash_status & (0xff), false);  //开锁
							flash_write(image_addr,FLASH_MIN_SIZE, image_buffer );        
							flash_write_status(flash_status, false);           //关锁
							
							vd_global_int_restore();
							
							en = 0;
							os_timer_stop(&ota_timer);
						}
					}					
				}	
				uint8_t replay[9] ={0x7e ,0x07,0x92,0x81,0x01,0x1b,0x7e};
				vd_gatt_ntf(conidx,replay,7);				
			}	
		image_save_en++;
		}
	}else
	{
		if(p_data[2] == 0x91 )
		{
			uint8_t replay[9] ={0x7e ,0x07,0x91,0x81,0x02,0x1b,0x7e};
			vd_gatt_ntf(conidx,replay,7);
		}
			if(p_data[2] == 0x92 )
		{
			uint8_t replay[9] ={0x7e ,0x07,0x92,0x81,0x02,0x1c,0x7e};
			vd_gatt_ntf(conidx,replay,7);
		}
	}	
}   


uint16_t app_otas_read_data(uint8_t conidx,uint8_t *p_data)
{
    uint16_t length;
    switch(app_otas_status.read_opcode)
    {
        case OTA_CMD_READ_DATA:
            app_boot_load_data(p_data,app_otas_status.base_addr,app_otas_status.length);
            length = app_otas_status.length;
            break;
        case OTA_CMD_READ_MEM:
            memcpy(p_data, (uint8_t *)app_otas_status.base_addr, app_otas_status.length);
            length = app_otas_status.length;
            break;
        default:
            length = 0;
            break;
    }
    app_otas_status.read_opcode = OTA_CMD_NULL;
    return length;
}

