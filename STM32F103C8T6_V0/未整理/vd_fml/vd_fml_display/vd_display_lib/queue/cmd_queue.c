/*
 * Project: mcu-kit
 * Module:
 * File: cmd_queue.c
 * Created Date: 2021-03-05 20:42:12
 * Author: meizhen
 * Description: ѭ������.
 * -----
 * todo: modified
 * -----
 * Copyright (c) 2021 - vDiscovery, Inc
 */

/* ======================================================================================
 * log
 */
#define LOG_ENABLE_DEBUG (0)

/* ======================================================================================
 * includes
 */
#include "cmd_queue.h"

#include "vd_log.h"

#include "vd_bsp_global_switch/vd_bsp_global_switch.h"
/* ======================================================================================
 * macros
 */

/* ======================================================================================
 * types
 */

/* ======================================================================================
 * declaration
 */

/* ======================================================================================
 * globals
 */

/* ======================================================================================
 * helper
 */

/* ======================================================================================
 * private implementation
 */

/* ======================================================================================
 * implementation
 */
/**
 * @brief : ����״̬��ȡ
 *
 * @param cmd_q
 * @return uint8_t
 */
enum cmd_queue_state vd_cmd_queue_state(struct vd_cmd_queue *cmd_q)
{
    if(cmd_q->read_index == cmd_q->write_index)
    {
        if(cmd_q->read_mirror == cmd_q->write_mirror)
            return vd_cmd_queue_empty;
            else
            return vd_cmd_queue_full;
    }
    return vd_cmd_queue_halffull;
}

/**
 * @brief : ��ʼ������
 *
 * @param queue
 * @return uint8_t
 */
uint8_t vd_queue_init(struct vd_cmd_queue* cmd_q,vd_uint8_t *pool,vd_int32_t size)
{
    cmd_q->read_mirror = cmd_q->read_index =0;
    cmd_q->write_mirror = cmd_q->write_index =0;
    cmd_q->buff_ptr = pool;
    cmd_q->buffer_size = size;
}

/**
 * @brief : ��ȡ���������ݳ���
 *
 * @param queue
 * @return uint32_t
 */
uint32_t vd_queue_size(struct vd_cmd_queue* cmd_q) 
{
    uint32_t write_i,read_i;
   switch (vd_cmd_queue_state(cmd_q))
   {
   case vd_cmd_queue_empty:
        return 0;
    break;
    case vd_cmd_queue_full:
        return cmd_q->buffer_size;
    break;
    case vd_cmd_queue_halffull:
        
   default:
   {
        write_i = cmd_q->write_index;
        read_i = cmd_q->read_index;
        if(write_i > read_i)
            return write_i - read_i;
        else
            return cmd_q->buffer_size - (read_i-write_i);
   }
    break;
   }
}

/**
 * @brief : ���data_len�ֽ�
 *
 * @param queue
 * @param data
 * @param data_len
 */
uint32_t vd_queue_push(struct vd_cmd_queue* cmd_q, uint8_t* data, uint32_t data_len) 
{
	vd_uint32_t sapce_length;
	
	if( vd_cmd_queue_state(cmd_q) == vd_cmd_queue_full)
	{
			 log_error("queue is full \n");
			 return 0;
	}		

	sapce_length = vd_cmd_queue_space_len(cmd_q);

	if(sapce_length < data_len){
		 return 0;
	}

	vd_global_int_disable();
		
	if (cmd_q->buffer_size - cmd_q->write_index > data_len)  //ʣ��ռ��㹻д��
    {
        /* read_index - write_index = empty space */
        memcpy(&cmd_q->buff_ptr[cmd_q->write_index], data, data_len);  //д������
        /* this should not cause overflow because there is enough space for
         * length of data in current mirror */
        cmd_q->write_index += data_len; //д����������Ӧ�ֽ�
				vd_global_int_restore();
        return data_len;
    }

	memcpy(&cmd_q->buff_ptr[cmd_q->write_index],&data[0],cmd_q->buffer_size - cmd_q->write_index); //���пռ䲻�㣬����д�������д�룬
	memcpy(&cmd_q->buff_ptr[0],&data[cmd_q->buffer_size - cmd_q->write_index], data_len - (cmd_q->buffer_size - cmd_q->write_index));//ʣ�����ݴ���д��ptr[0]��

	/* we are going into the other side of the mirror */
	cmd_q->write_mirror = ~cmd_q->write_mirror; //��Ϊд�����Ѿ��ػ��ˣ�����Ҫ��write_mirror��һ��ȡ������
	cmd_q->write_index = data_len - (cmd_q->buffer_size - cmd_q->write_index);// ����д����λ��
 
	vd_global_int_restore();

#if (CMD_QUEUE_DBG)
	log_debug("[zmz] size=%d, read_index=%d, write_index=%d\r\n", cmd_q->buffer_size,cmd_q->read_index,cmd_q->write_index);
#endif

	return data_len;
}


/**
 * @brief
 *
 * @param queue : ��ȡ������max_size���ֽڣ�������
 * @param value
 * @param max_size
 * @return uint32_t
 */
uint32_t vd_queue_pull(struct vd_cmd_queue* cmd_q, uint8_t* value, uint32_t read_len)
{
	vd_uint32_t read_right_len;
	vd_uint32_t read_left_len;
	vd_uint32_t queue_data_len;//����������ݳ���

	if( vd_cmd_queue_state(cmd_q) == vd_cmd_queue_empty)
	{
			 return 0;
	}

	queue_data_len = vd_queue_size(cmd_q);
	if(queue_data_len == 0) return 0;
	
	vd_global_int_disable();
	
	if(queue_data_len < read_len){
		read_len = queue_data_len;
	}
		
	read_right_len = cmd_q->buffer_size - cmd_q->read_index;	
	
	if(read_len < read_right_len){//�ұ߳������Զ�ȡ
		if(value){
			memcpy(value, &cmd_q->buff_ptr[cmd_q->read_index], read_len);
		}
		cmd_q->read_index += read_len;
	}else{//�ұ߳��Ȳ����Զ�ȡ
		read_left_len = read_len - read_right_len;
		
		if(value){
			//�ȿ����ұ�����
			memcpy(value, &cmd_q->buff_ptr[cmd_q->read_index], read_right_len);
			//�����������
			memcpy(value + read_right_len, cmd_q->buff_ptr, read_left_len);
		}

		cmd_q->read_index = read_left_len;
		cmd_q->read_mirror =~cmd_q->read_mirror;
	}

	vd_global_int_restore();
	
	return read_len;
}


/**
 * @brief
 *
* @param queue : ��ȡ������max_size���ֽڣ�������
 * @param value
 * @param max_size
 * @return uint32_t
 */
uint32_t vd_queue_pull_notout(struct vd_cmd_queue* cmd_q, uint8_t* value, uint32_t read_len)
{
	vd_uint32_t read_right_len;
	vd_uint32_t read_left_len;
	vd_uint32_t queue_data_len;//����������ݳ���

	if( vd_cmd_queue_state(cmd_q) == vd_cmd_queue_empty)
	{
		return 0;
	}

	queue_data_len = vd_queue_size(cmd_q);
	if(queue_data_len == 0) return 0;
	
	if(queue_data_len < read_len){
		read_len = queue_data_len;
	}
		
	read_right_len = cmd_q->buffer_size - cmd_q->read_index;	
	
	if(read_len < read_right_len){//�ұ߳������Զ�ȡ
		if(value){
			memcpy(value, &cmd_q->buff_ptr[cmd_q->read_index], read_len);
		}
	}else{//�ұ߳��Ȳ����Զ�ȡ
		read_left_len = read_len - read_right_len;
		
		if(value){
			//�ȿ����ұ�����
			memcpy(value, &cmd_q->buff_ptr[cmd_q->read_index], read_right_len);
			//�����������
			memcpy(value + read_right_len, cmd_q->buff_ptr, read_left_len);
		}
	}
	
	return read_len;
}



/**
 * @brief : ��ն���
 *
 * @param queue
 * @return uint8_t
 */
uint8_t vd_queue_clear(struct vd_cmd_queue* cmd_q) {
    if (cmd_q) 
    {
      memset(cmd_q->buff_ptr, 0, cmd_q->buffer_size);
      cmd_q->read_index = 0;
      cmd_q->read_mirror = 0;
      cmd_q->write_index = 0;
      cmd_q->write_mirror = 0;
    }
    return 0;
}
