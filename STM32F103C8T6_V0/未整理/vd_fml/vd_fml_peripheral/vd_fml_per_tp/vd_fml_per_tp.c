#include "vd_fml_per_tp.h"

#include "vd_fml_per_tp_chip_config.h"
#if (PROJECT_TP_EN == ON)
#include "vd_log.h"
#include "queue/mq_queue.h"

#ifndef TP_CHIP_PORT_REGISTER
#define TP_CHIP_PORT_REGISTER vd_null
#endif

static vd_bool_t vd_tp_chip_point_report(tp_point_t *point);//tp数据上报

//tp驱动
typedef struct{
	tp_chip_port_register_t port_register;	//注册接口
	tp_init_t								param;					//初始化参数
	tp_chip_port_t 					port;						//tp芯片操作接口
	mqd_t            				point_queue;		//数据队列
}tp_drv_t;

//tp驱动对象
static tp_drv_t tp = {
	.port_register = TP_CHIP_PORT_REGISTER,
#if (PROJECT_TP_EN == ON)
	.param = {PROJECT_TP_POWER_PORT, PROJECT_TP_RESET_PORT, PROJECT_TP_INTER_PORT, PROJECT_TP_PORT}
#endif
};
 	 


/**
 * @brief :TP数据上报,将tp芯片上报上来的数据存入队列
 *
 * @param point: tp坐标
 * @return vd_bool_t
 */
static vd_bool_t vd_tp_chip_point_report(tp_point_t *point)
{
	int ret;
	
	vd_check_return_val_error(point == vd_null,  vd_false, "tp report point is null\n");
	vd_check_return_val_error(tp.point_queue <= 0, vd_false, "tp_queue error\n");
	
	ret = mq_send(tp.point_queue, (char*)point, sizeof(tp_point_t), 0);//数据入队
	vd_check_return_val_error(ret != sizeof(tp_point_t), vd_false, "tp mq_send error\n");
	
	return vd_true;
}

/**
 * @brief : tp初始化
 *
 * @return vd_true
 * @return vd_false
 */
vd_bool_t vd_tp_init(vd_void_t)
{
	vd_bool_t ret = vd_false;
	struct mq_attr qattr = {0};
	
	/* tp注册 */
	vd_check_return_val_error(tp.port_register == vd_null, vd_false, "tp not bound\n");
	ret = tp.port_register(&tp.port);
	vd_check_return_val_error(ret == vd_false, vd_false, "tp register error\n");
	
	/* 队列初始化 */
	qattr.mq_flags   = 0;
	qattr.mq_maxmsg  = PROJECT_TP_QUEUE_MSG_MAX;
	qattr.mq_msgsize = sizeof(tp_point_t);
	tp.point_queue   = mq_open("g_tp_queue", O_CREAT, 0644, &qattr);
	vd_check_return_val_error(tp.point_queue <= 0, vd_false, "mq_open tp_queue error\n");
	
	/* 驱动初始化 */
	vd_check_return_val_error(tp.port.init == vd_null,  vd_false, "tp init port is null\n");
	tp.param.point_report = vd_tp_chip_point_report;//tp上报接口
	ret = tp.port.init(&tp.param);
	vd_check_return_val_error(ret == vd_false, vd_false, "tp init error\n");
	
	return vd_true;
}

/**
 * @brief : tp轮询
 *
 * @return vd_void_t
 */
vd_void_t vd_tp_loop(vd_void_t)
{
	vd_check_return_error(tp.port.loop == vd_null, "tp loop port is null\n");
	tp.port.loop();
}

/**
 * @brief :从队列获取tp坐标,供应用层调用，
 *
 * @param point: tp坐标
 * @return vd_bool_t
 */
vd_bool_t vd_tp_read(tp_point_t* point)
{
	vd_bool_t ret = vd_false;
	ssize_t mq_ret;
	
	vd_check_return_val_error(point == vd_null,  vd_false, "tp read buf is null\n");
	vd_check_return_val_error(tp.point_queue <= 0, vd_false, "tp_queue error\n");
	
	mq_ret = mq_receive(tp.point_queue, (char *)point, sizeof(tp_point_t), NULL);//数据出队
	vd_check_return_val_error(mq_ret != sizeof(tp_point_t),  vd_false, "tp mq_receive error\n");
	
	return vd_true;
}
#endif

