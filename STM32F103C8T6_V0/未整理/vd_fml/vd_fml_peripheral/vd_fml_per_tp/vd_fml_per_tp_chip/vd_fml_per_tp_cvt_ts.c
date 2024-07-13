#include "vd_fml_per_tp_cvt_ts.h"
//#if (PROJECT_TP_EN == ON)
#include "vd_log.h"
#include "vd_bsp_gpio/vd_bsp_gpio.h"
#include "vd_bsp_i2c/vd_bsp_i2c.h"

#include "string.h"

#define CVT_TS_UPDATE 0
#if (CVT_TS_UPDATE)
	#include "vd_fml_per_tp_cvt_ts_cfg.h"
	#define CVT_FAMILY_OFFSET_CFG 	0x00
	#define CVT_VARIANT_OFFSET_CFG 	0x01
	#define CVT_VERSION_OFFSET_CFG 	0x02
	#define CVT_BUILD_OFFSET_CFG 	0x03

	#define CVT_CRC_LOW_CFG			0x0A
	#define CVT_CRC_MID_CFG			0x0B
	#define CVT_CRC_HI_CFG			0x0C

	#define CVT_MAX_BLOCK_WRITE		255
	#define CVT_INFO_CHECKSUM_SIZE  3

	/* CVT_GEN_COMMAND_T6 field */
	#define CVT_COMMAND_RESET		0
	#define CVT_COMMAND_BACKUPNV	1
	#define CVT_COMMAND_CALIBRATE	2
	#define CVT_COMMAND_REPORTALL	3
	#define CVT_COMMAND_DIAGNOSTIC	5

	#define CVT_OBJECT_START		0x07
	#define CVT_OBJECT_SIZE			6

	#define CVT_RESET_VALUE			0x01
	#define CVT_BACKUP_VALUE		0x55
#endif



/* Define for T6 status byte */
#define CVT_T6_STATUS_RESET		vd_bit(7)
#define CVT_T6_STATUS_OFL			vd_bit(6)
#define CVT_T6_STATUS_SIGERR	vd_bit(5)
#define CVT_T6_STATUS_CAL			vd_bit(4)
#define CVT_T6_STATUS_CFGERR	vd_bit(3)
#define CVT_T6_STATUS_COMSERR	vd_bit(2)

#define CVT_TS_IIC_SLAVE_ADDRESS        	(0x4A<<1)

static vd_void_t cvt_ts_exti_handler(vd_void_t *arg);//外部中断处理

struct cvt_info {
 uint8_t family_id;
 uint8_t variant_id;
 uint8_t version;
 uint8_t build;
 uint8_t matrix_xsize;
 uint8_t matrix_ysize;
 uint8_t object_num;  
};

struct cvt_object {
 uint8_t type;
 uint8_t start_addr_top;
 uint8_t start_addr_bottom;
 uint8_t size_minus_one;
 uint8_t instances_minus_one;
 uint8_t num_report_ids;
};

struct cvt_data {
 struct cvt_info *cvt_info;
 struct cvt_object *cvt_object;
 uint8_t T5_msg_size;
 uint16_t T5_address;
 uint8_t T6_reportid;
 uint16_t T6_address;
 uint8_t T100_reportid_min;
 uint8_t T100_reportid_max;
 uint8_t num_touchids;
 uint16_t T44_address;
 uint8_t *msg_buf;
 uint8_t max_reportid;
 uint32_t config_crc;
 uint16_t mem_size;
 uint8_t t6_status;
};

//cvt_ts驱动对象
typedef struct{
	vd_bool_t (*point_report)(tp_point_t *point);//tp数据上报函数
	struct cvt_data *d_data;
}cvt_ts_drv_t;

static cvt_ts_drv_t cvt_ts = {0};


static int cvt_ts_initialize(vd_bsp_port_e data_port)
{
	vd_int_t i, ret;
	struct cvt_info *id_buf = NULL;
	struct cvt_object* object_table = NULL;
	vd_uint32_t len, table_size;
	vd_uint8_t min_id, max_id, reportid = 1;
	vd_uint8_t *buff = NULL;
	vd_uint16_t end_address;
	
	len = sizeof(struct cvt_info);
	id_buf = (struct cvt_info *)PLATFORM_MALLOC(len);
	vd_check_return_val_error(id_buf == vd_null, -1, "[cvt] malloc cvt_info error, exit!\n");
	memset(id_buf, 0, len);

//	i2c_memory_read(&I2C0_Handle_CVT, CVT_TS_IIC_SLAVE_ADDRESS, 0x0000, (uint8_t *)id_buf, len);
	ret = vd_bsp_i2c_master_read(data_port, CVT_TS_IIC_SLAVE_ADDRESS, 0x0000, (uint8_t *)id_buf, len);
	vd_check_return_val_error(ret < 0, -1, "func:%s, line:%d, ret = %d\r\n", __func__, __LINE__, ret);

	log_debug("[cvt] 1111Read INFO: family_id[%X],variant_id[%X],"
			"version[%X],build[%X],matrix_xsize[%d],"
			"matrix_ysize[%d],object_num[%d]\n", 
			id_buf->family_id, id_buf->variant_id, 
			id_buf->version, id_buf->build, id_buf->matrix_xsize, 
			id_buf->matrix_ysize, id_buf->object_num);
	
	table_size = id_buf->object_num * sizeof(struct cvt_object);

	buff = PLATFORM_REALLOC(id_buf, table_size+len);
	vd_check_return_val_error(id_buf == vd_null, -1, "[cvt] realloc buff error, exit!\n");

	id_buf = (struct cvt_info *)buff;
	object_table = (struct cvt_object *)(buff + len);
	
	/* Object Table INFO */
//	i2c_memory_read(&I2C0_Handle_CVT, CVT_TS_IIC_SLAVE_ADDRESS, 0x00, buff, table_size+len);
	ret = vd_bsp_i2c_master_read(data_port, CVT_TS_IIC_SLAVE_ADDRESS, 0x00, buff, table_size+len);
	vd_check_return_val_error(ret < 0, -1, "func:%s, line:%d, ret = %d\r\n", __func__, __LINE__, ret);

	log_debug("[cvt] 2222Read INFO: family_id[%X],variant_id[%X],"
		"version[%X],build[%X],matrix_xsize[%d],"
		"matrix_ysize[%d],object_num[%d]\n", 
		id_buf->family_id, id_buf->variant_id, 
		id_buf->version, id_buf->build, id_buf->matrix_xsize, 
		id_buf->matrix_ysize, id_buf->object_num);
			
	/* Object Parse */
	for (i = 0; i < id_buf->object_num; i++) {
		struct cvt_object *object = (struct cvt_object *)(object_table + i);

		if (object->num_report_ids) 
		{
			min_id = reportid;
			reportid += object->num_report_ids * (object->instances_minus_one + 1);
			max_id = reportid - 1;
		} else {
			min_id = 0;
			max_id = 0;
		}
		log_debug("Type:T%d,  start_address:[0x%x],  size_minus_one:0x%02x, instances_minus_one:%d, num_report_ids:%d\r\n",
			object->type,(object->start_addr_bottom<<8)|object->start_addr_top,
			object->size_minus_one,
			object->instances_minus_one,object->num_report_ids);
		
		switch (object->type) {
			case 5:
				cvt_ts.d_data->T5_msg_size = object->size_minus_one;
				cvt_ts.d_data->T5_address = (object->start_addr_bottom<<8)|object->start_addr_top;
				break;
			case 6:
				cvt_ts.d_data->T6_reportid = min_id;
				cvt_ts.d_data->T6_address = (object->start_addr_bottom<<8)|object->start_addr_top;
				break;
			case 44:
				cvt_ts.d_data->T44_address = (object->start_addr_bottom<<8)|object->start_addr_top;
				log_debug("[cvt] d_data->T44_address = %x\n", (object->start_addr_bottom<<8)|object->start_addr_top);
				break;
			case 100:
				cvt_ts.d_data->T100_reportid_min = min_id;
				cvt_ts.d_data->T100_reportid_max = max_id;
				cvt_ts.d_data->num_touchids = object->num_report_ids;
				break;
		}
		end_address = (object->start_addr_bottom<<8)|object->start_addr_top + 
				(object->size_minus_one+1)*(object->instances_minus_one+1) - 1;
		if (end_address >= cvt_ts.d_data->mem_size)
			cvt_ts.d_data->mem_size = end_address + 1;
	}

	cvt_ts.d_data->max_reportid = reportid;

	if(cvt_ts.d_data->T44_address && (cvt_ts.d_data->T5_address != cvt_ts.d_data->T44_address + 1)) {
		log_debug("[cvt] Invalid T44 position!\n");
		PLATFORM_FREE(id_buf);
		return -1;
	}
	
	/* For message report */
	cvt_ts.d_data->msg_buf = (vd_uint8_t *)PLATFORM_MALLOC(cvt_ts.d_data->max_reportid * cvt_ts.d_data->T5_msg_size);
	if (cvt_ts.d_data->msg_buf == NULL) {
		log_debug("[cvt] malloc msg_buf error, exit!");
		PLATFORM_FREE(id_buf);
		return -1;
	}
	cvt_ts.d_data->cvt_info = id_buf;
	cvt_ts.d_data->cvt_object = object_table;
	
	return 0;
}


#if (CVT_TS_UPDATE)

static int cvt_t6_command(uint16_t cmd_offset, uint8_t value, bool wait)
{
	uint16_t reg, reg_switch;
	uint8_t command_register;
	int timeout_counter = 0;
	int ret;

	reg = cvt_ts.d_data->T6_address + cmd_offset;
	reg_switch = ((reg & 0xff) << 8) | (reg >> 8);
	log_debug("reg_switch = %x \r\n", reg_switch);	
	/* Write CMD */
	i2c_memory_write(&I2C0_Handle_CVT, CVT_TS_IIC_SLAVE_ADDRESS, reg_switch, &value, 1);

	if (!wait) {	
		return 0;
	}
	
	return 0;
}

static void cvt_update_crc(uint8_t cmd, uint8_t value)
{
	int ret;
	
	cvt_ts.d_data->config_crc = 0;
	ret = cvt_t6_command(cmd, value, true);
	if (ret) {
		log_debug("[cvt] %s failed!!!!\n", __func__);
	}
}

static struct cvt_object *cvt_get_object(uint8_t type)
{
	struct cvt_object *object;
	int i;

	for (i = 0; i < cvt_ts.d_data->cvt_info->object_num; i++) {
		object = cvt_ts.d_data->cvt_object + i;
		if (object->type == type)
			return object;
	}

	log_debug("[cvt] Invalid object type T%u\n", type);
	return NULL;
}

static int cvt_update_cfg_data_test()
{
	struct cvt_object *object;
	int ret = 0;
	int data_pos = 0;
	int byte_offset;
	int i;
	int cfg_start_ofs;
	uint32_t info_crc, config_crc;
	uint32_t info_crc_tmp[3];
	uint32_t cfg_crc_tmp[3];
	uint8_t cfg_info_tmp[7];
	uint8_t *config_mem;
	uint32_t config_mem_size;
	unsigned int type, instance, size;
	unsigned int type_tmp[2];
	unsigned int instance_tmp[2];
	unsigned int size_tmp[2];
	uint8_t val;
	uint16_t reg, write_addr;
	uint32_t len = 0;;

	log_debug("[cvt] Start CVT Update!!!!!\n");

	len = sizeof(cvt_cfg_data);	/* config length */

	/* Load information block and check */
	for (i = 0; i < sizeof(struct cvt_info); i++) {
		cfg_info_tmp[i] = *(uint8_t*)(cvt_cfg_data + data_pos);
		data_pos++;
	}
	
	/* Read Info CRCs */
	for(i=0;i<3;i++){
		info_crc_tmp[i] = *(uint8_t*)(cvt_cfg_data + data_pos);
	   	data_pos ++;
	}
	info_crc = info_crc_tmp[2]|(info_crc_tmp[1]<<8)|(info_crc_tmp[0]<<16);
	
	/* Read Config CRCs*/
	for(i=0;i<3;i++){
		cfg_crc_tmp[i] = *(uint8_t*)(cvt_cfg_data + data_pos);
		data_pos ++;
	}
	config_crc = cfg_crc_tmp[2]|(cfg_crc_tmp[1]<<8)|(cfg_crc_tmp[0]<<16);
	
	/* Malloc memory to store configuration */
	cfg_start_ofs = CVT_OBJECT_START +
			cvt_ts.d_data->cvt_info->object_num * CVT_OBJECT_SIZE +
			CVT_INFO_CHECKSUM_SIZE;
	config_mem_size = cvt_ts.d_data->mem_size - cfg_start_ofs;
	log_debug("[cvt] %s()11111111111111111config_mem_size = %d\n", __func__, config_mem_size);

	config_mem = PLATFORM_MALLOC(config_mem_size);
	if (!config_mem) {
		log_debug("[cvt] Failed to allocate memory\n");
		ret = -1;
		goto release;
	}
	
	log_debug("[cvt] %s()222222222222222\n", __func__);
	while (data_pos < len) {
		for(i=0;i<2;i++){	/* Type --> 2byte */
			type_tmp[i] = *(uint8_t*)(cvt_cfg_data + data_pos);
			data_pos++;
		}
		
		for(i=0;i<2;i++){	/* Instance --> 2byte */
			instance_tmp[i] = *(uint8_t*)(cvt_cfg_data + data_pos);
			data_pos++;
		}

		for(i=0;i<2;i++){	/* SIZE */
			size_tmp[i] = *(uint8_t*)(cvt_cfg_data + data_pos);
			data_pos++;
		}
		type = type_tmp[1]|(type_tmp[0]<<8);
		instance = instance_tmp[1]|(instance_tmp[0])<<8;
		size = size_tmp[1]|(size_tmp[0]<<8);

		object = cvt_get_object(type);	/* Get Object */
		if (!object) {
			/* Skip object */
			for (i = 0; i < size; i++) {
				val = *(uint8_t*)(cvt_cfg_data + data_pos);
				data_pos ++;
			}
			continue;
		}
		
		if (instance > (object->instances_minus_one+1)) {
			log_debug("[cvt] Object instances exceeded!\n");
			ret = -1;
			goto release_mem;
		}

		if (size > (object->size_minus_one+1)) {
			log_debug("[cvt] Discarding %lu byte(s) in T%u\n",
				 size - (object->size_minus_one+1), type);
		}

		reg = (object->start_addr_bottom<<8)|object->start_addr_top + 
									(object->size_minus_one+1) * instance;

		for (i = 0; i < size; i++) {

			val = *(uint8_t*)(cvt_cfg_data + data_pos);

			data_pos++;			

			if (i > (object->size_minus_one+1))
				continue;

			byte_offset = reg + i - cfg_start_ofs;

			if ((byte_offset >= 0)
			    && (byte_offset <= config_mem_size)) {
				*(config_mem + byte_offset) = val;	/* Copy data to config_mem */
			} else {
				log_debug("[cvt] Bad object: reg:%d, T%d, ofs=%d\n",
					reg, object->type, byte_offset);
				ret = -1;
				goto release_mem;
			}
		}
	}
	
	/* Write configuration as blocks */
	byte_offset = 0;
	while (byte_offset < config_mem_size) {
		size = config_mem_size - byte_offset;

		if (size > CVT_MAX_BLOCK_WRITE)
			size = CVT_MAX_BLOCK_WRITE;
		/* Change register address */
		write_addr = (((cfg_start_ofs + byte_offset) & 0xff) << 8) | 
					((cfg_start_ofs + byte_offset) >> 8);
		/* Write register */
		i2c_memory_write(&I2C0_Handle_CVT, CVT_TS_IIC_SLAVE_ADDRESS, write_addr, 
					config_mem+byte_offset, size);

		byte_offset += size;
	}

	/* Backup to memory */
	cvt_update_crc(CVT_COMMAND_BACKUPNV, CVT_BACKUP_VALUE);
	PLATFORM_DELAY_MS(50);
	log_debug("[cvt] Config successfully updated\n");
	
release_mem:
	PLATFORM_FREE(config_mem);
release:
	return ret;
}

static int cvt_ts_upgrade(void)
{
	vd_int_t rc = 0;
	vd_uint8_t family,variant,version,build;
	vd_uint8_t i;
	vd_uint8_t readData[13];
	vd_uint32_t cfgCrc;

	for(i=0;i<sizeof(readData);i++){
		readData[i] = *(uint8_t*)(cvt_cfg_data+i);
	}
	
	family = readData[CVT_FAMILY_OFFSET_CFG];
	variant = readData[CVT_VARIANT_OFFSET_CFG];
	version = readData[CVT_VERSION_OFFSET_CFG];
	build = readData[CVT_BUILD_OFFSET_CFG];
	/* CheckSum */
	cfgCrc = (readData[CVT_CRC_HI_CFG])|(readData[CVT_CRC_MID_CFG]<<8)|(readData[CVT_CRC_LOW_CFG]<<16);

	/* Compare ID INFO */
	if(family != cvt_ts.d_data->cvt_info->family_id || variant != cvt_ts.d_data->cvt_info->variant_id)
	{
		log_debug("[cvt]%s(), the chip type doesn't match, exit update!\n", 
				__func__);
		return 1;
	}

	if (version != cvt_ts.d_data->cvt_info->version || build != cvt_ts.d_data->cvt_info->build) {
		log_debug("[cvt]%s(), config corresponding fw version doesn't match, exit update!\n", 
				__func__);
		return 1;
	}

	/* Get d_data->config_crc */
	cvt_update_crc(CVT_COMMAND_REPORTALL, 1);
	PLATFORM_DELAY_MS(50);

	log_debug("565656565656 \r\n");
	if(cfgCrc == cvt_ts.d_data->config_crc){
		log_debug("[cvt]%s(), config crc is same CRC = 0x%X, No need update, exit!\n", 
				__func__, cfgCrc);
		return 1;
	}
	
	log_debug("[cvt]%s(), config crc in file CRC = 0x%X, config crc in ic CRC = 0x%X\n",
				__func__, cfgCrc, cvt_ts.d_data->config_crc);

	/* Update Config */
	rc = cvt_update_cfg_data_test();
	if(rc){
		log_debug("[cvt]%s(), Failed to update config!\n",__func__);
	}

	return rc;
}
#endif

static void cvt_proc_t6_msg(uint8_t *msg)
{
	uint8_t status = msg[1];
	uint32_t crc = msg[2] | (msg[3] << 8) | (msg[4] << 16);
	int ret;

	log_debug("[cvt] %s(), status = %x!!!!\n", __func__, status);
	
	if (crc != cvt_ts.d_data->config_crc) {
		cvt_ts.d_data->config_crc = crc;
		log_debug("[cvt] T6 Config CheckSum: 0x%06X\n", crc);
	}
	
	if (status != cvt_ts.d_data->t6_status) {
		log_debug("[cvt] T6 Status 0x%02X%s%s%s%s%s%s%s\n",
			status,
			status == 0 ? " OK" : "",
			status & CVT_T6_STATUS_RESET ? " RESET" : "",
			status & CVT_T6_STATUS_OFL ? " OFL" : "",
			status & CVT_T6_STATUS_SIGERR ? " SIGERR" : "",
			status & CVT_T6_STATUS_CAL ? " CAL" : "",
			status & CVT_T6_STATUS_CFGERR ? " CFGERR" : "",
			status & CVT_T6_STATUS_COMSERR ? " COMSERR" : "");
	}
	cvt_ts.d_data->t6_status = status;
}

static void cvt_proc_t100_msg(uint8_t *msg)
{
	int id;
	uint8_t status, type = 0;
	uint8_t touch_num;
	tp_point_t point;
	
	id = msg[0] - cvt_ts.d_data->T100_reportid_min;
	if (id < 0 || id > cvt_ts.d_data->num_touchids) {
		log_debug("[cvt] invaild touch id[%d], total num touch is %d \n", id, cvt_ts.d_data->num_touchids);
		return;
	}
	
	if (id == 0 || id == 1) {
		touch_num = msg[2];
		log_debug("[cvt] Reserve id[%d], touch num = %d\n", id, touch_num);
		return;
	}
	
	status = msg[1];	/* Finger Status */
	point.x = (msg[3] << 8) | (msg[2] & 0xff);
	point.y = (msg[5] << 8) | (msg[4] & 0xff);
	
	if (status & vd_bit(7)) {	/* Down */
		type = (status & 0x70) >> 4;	/* TouchType: Hover/Finger/Glove/PassiveStylus... */
		if(cvt_ts.point_report){
			cvt_ts.point_report(&point);
		}
	}
}

static void cvt_proc_message(uint8_t *msg)
{
	uint8_t report_id = msg[0];
	
	if (report_id == 0xff)
		return;
	
	if (report_id == cvt_ts.d_data->T6_reportid)
		cvt_proc_t6_msg(msg);
	else if (report_id >= cvt_ts.d_data->T100_reportid_min && report_id <= cvt_ts.d_data->T100_reportid_max)
		cvt_proc_t100_msg(msg);
	
}

/**
 * @brief : tp初始化
 *
 * @return vd_true
 * @return vd_false
 */
static vd_bool_t cvt_ts_init(tp_init_t *tp_init)
{
	vd_int32_t ret;
	
	log_debug("[cvt] --------%s() Start!---------\n", __func__);
	
	vd_check_return_val_error(tp_init == vd_null,  vd_false, "tp_init is null\n");
	vd_check_return_val_error(tp_init->reset_port < 0,  vd_false, "tp reset_port is null\n");
	vd_check_return_val_error(tp_init->inter_port < 0,  vd_false, "tp inter_port is null\n");
	vd_check_return_val_error(tp_init->data_port < 0,  vd_false, "tp data_port is null\n");
	
	//获取数据上报函数
	cvt_ts.point_report = tp_init->point_report;
	
	//引脚初始化
	ret = vd_bsp_gpio_init(tp_init->reset_port, VD_GPIO_MODE_OUTPUT);//复位引脚初始化
	vd_check_return_val_error(ret < 0,  vd_false, "tp reset_port init error\n");
	ret = vd_bsp_gpio_requst_irq(tp_init->inter_port, VD_GPIO_IRQ_MODE_FALLING, cvt_ts_exti_handler, tp_init);//中断引脚初始化
	vd_check_return_val_error(ret < 0,  vd_false, "tp inter_port init error\n");
	
	//i2c初始化
	ret = vd_bsp_i2c_init(tp_init->data_port, CVT_TS_IIC_SLAVE_ADDRESS, PLATFORM_I2C_REG_BIT_16BIT);
	vd_check_return_val_error(ret < 0,  vd_false, "tp data_port init error\n");

	/* Reset Chip */
	ret = vd_bsp_gpio_write(tp_init->reset_port, VD_GPIO_HIGH);
	vd_check_return_val_error(ret < 0,  vd_false, "tp reset high error\n");
	PLATFORM_DELAY_US(100);
	ret = vd_bsp_gpio_write(tp_init->reset_port, VD_GPIO_LOW);
	vd_check_return_val_error(ret < 0,  vd_false, "tp reset low error\n");
	PLATFORM_DELAY_US(50);
	ret = vd_bsp_gpio_write(tp_init->reset_port, VD_GPIO_HIGH);
	vd_check_return_val_error(ret < 0,  vd_false, "tp reset high error\n");
	PLATFORM_DELAY_US(2000);

	cvt_ts.d_data = (struct cvt_data *)PLATFORM_MALLOC(sizeof(struct cvt_data));
	vd_check_return_val_error(cvt_ts.d_data == vd_null, vd_false, "[cvt] malloc cvt_data error, exit!\n");

	/* cvt ts initialize */
	ret = cvt_ts_initialize(tp_init->data_port);
	if (ret) {
		log_debug("[cvt] cvt_ts_initialize failed, eixt\n");
		PLATFORM_FREE(cvt_ts.d_data);
		return vd_false;
	}

#if (CVT_TS_UPDATE)
	log_debug("[cvt] CVT TS UPDATE\n");
	ret = cvt_ts_upgrade();
	if (ret < 0) {
		log_debug("[cvt] cvt_ts_upgrade failed!!!!!\n");
	}
#endif	

	/* IRQ Enable */
	ret = vd_bsp_gpio_irq_enable(tp_init->inter_port, VD_GPIO_IRQ_ENABLE);
	
	log_debug("cvt_ts_initialize sucess \r\n");  
}

/**
 * @brief : tp轮询，TP读取数据,数据存入队列
 *
 * @return vd_void_t
 */
static vd_void_t cvt_ts_loop(vd_void_t)
{

}

/**
 * @brief : 外部中断处理
 *
 * @return vd_void_t
 */
static vd_void_t cvt_ts_exti_handler(vd_void_t *arg)
{
	vd_uint16_t msg_addr;
	vd_uint8_t count, num_left, i;
	vd_int32_t ret;
	tp_init_t *tp_init = (tp_init_t *)arg;
	
	msg_addr = ((cvt_ts.d_data->T44_address & 0xff) << 8) | (cvt_ts.d_data->T44_address >> 8);

 // i2c_memory_read(&I2C0_Handle_CVT, CVT_TS_IIC_SLAVE_ADDRESS, msg_addr, cvt_ts.d_data->msg_buf, cvt_ts.d_data->T5_msg_size + 1);
	ret = vd_bsp_i2c_master_read(tp_init->data_port, CVT_TS_IIC_SLAVE_ADDRESS, msg_addr, cvt_ts.d_data->msg_buf, cvt_ts.d_data->T5_msg_size + 1);
	vd_check_return_error(ret < 0, "func:%s, line:%d, ret = %d\r\n", __func__, __LINE__, ret);
	
	count = cvt_ts.d_data->msg_buf[0];
	if (count == 0) {
		log_debug("[cvt] Zero message!\n");
		return;
	} else if (count > cvt_ts.d_data->max_reportid) {
		log_debug("[cvt] count bigger than max_reportid, msg_buf[0] = %d\n", cvt_ts.d_data->msg_buf[0]);
		count = cvt_ts.d_data->max_reportid;
	}
	
	/* Process first message */
	cvt_proc_message(cvt_ts.d_data->msg_buf + 1);
	
	num_left = count - 1;
	if (num_left) {
		log_debug("[cvt] Remaining message to process\n");
		for (i = 0; i < count; i++) {
//			i2c_memory_read(&I2C0_Handle_CVT, CVT_TS_IIC_SLAVE_ADDRESS, cvt_ts.d_data->T5_address, cvt_ts.d_data->msg_buf, cvt_ts.d_data->T5_msg_size);
			ret = vd_bsp_i2c_master_read(tp_init->data_port, CVT_TS_IIC_SLAVE_ADDRESS, cvt_ts.d_data->T5_address, cvt_ts.d_data->msg_buf, cvt_ts.d_data->T5_msg_size);
			vd_check_return_error(ret < 0, "func:%s, line:%d, ret = %d\r\n", __func__, __LINE__, ret);
			cvt_proc_message(cvt_ts.d_data->msg_buf);
		}
	}
}

/**
 * @brief : tp端口注册
 *
 * @return port
 * @return vd_bool_t
 */
vd_bool_t cvt_ts_port_register(tp_chip_port_t *port)
{
	vd_check_return_val_error(port == vd_null,  vd_false, "tp port is null\n");
	
	port->init = cvt_ts_init;
	port->loop = cvt_ts_loop;
	
	return vd_true;
}
//#endif
