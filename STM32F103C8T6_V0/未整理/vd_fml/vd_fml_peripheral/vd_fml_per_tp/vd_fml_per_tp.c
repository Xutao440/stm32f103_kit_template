#include "vd_fml_per_tp.h"

#include "vd_fml_per_tp_chip_config.h"
#if (PROJECT_TP_EN == ON)
#include "vd_log.h"
#include "queue/mq_queue.h"

#ifndef TP_CHIP_PORT_REGISTER
#define TP_CHIP_PORT_REGISTER vd_null
#endif

static vd_bool_t vd_tp_chip_point_report(tp_point_t *point);//tp�����ϱ�

//tp����
typedef struct{
	tp_chip_port_register_t port_register;	//ע��ӿ�
	tp_init_t								param;					//��ʼ������
	tp_chip_port_t 					port;						//tpоƬ�����ӿ�
	mqd_t            				point_queue;		//���ݶ���
}tp_drv_t;

//tp��������
static tp_drv_t tp = {
	.port_register = TP_CHIP_PORT_REGISTER,
#if (PROJECT_TP_EN == ON)
	.param = {PROJECT_TP_POWER_PORT, PROJECT_TP_RESET_PORT, PROJECT_TP_INTER_PORT, PROJECT_TP_PORT}
#endif
};
 	 


/**
 * @brief :TP�����ϱ�,��tpоƬ�ϱ����������ݴ������
 *
 * @param point: tp����
 * @return vd_bool_t
 */
static vd_bool_t vd_tp_chip_point_report(tp_point_t *point)
{
	int ret;
	
	vd_check_return_val_error(point == vd_null,  vd_false, "tp report point is null\n");
	vd_check_return_val_error(tp.point_queue <= 0, vd_false, "tp_queue error\n");
	
	ret = mq_send(tp.point_queue, (char*)point, sizeof(tp_point_t), 0);//�������
	vd_check_return_val_error(ret != sizeof(tp_point_t), vd_false, "tp mq_send error\n");
	
	return vd_true;
}

/**
 * @brief : tp��ʼ��
 *
 * @return vd_true
 * @return vd_false
 */
vd_bool_t vd_tp_init(vd_void_t)
{
	vd_bool_t ret = vd_false;
	struct mq_attr qattr = {0};
	
	/* tpע�� */
	vd_check_return_val_error(tp.port_register == vd_null, vd_false, "tp not bound\n");
	ret = tp.port_register(&tp.port);
	vd_check_return_val_error(ret == vd_false, vd_false, "tp register error\n");
	
	/* ���г�ʼ�� */
	qattr.mq_flags   = 0;
	qattr.mq_maxmsg  = PROJECT_TP_QUEUE_MSG_MAX;
	qattr.mq_msgsize = sizeof(tp_point_t);
	tp.point_queue   = mq_open("g_tp_queue", O_CREAT, 0644, &qattr);
	vd_check_return_val_error(tp.point_queue <= 0, vd_false, "mq_open tp_queue error\n");
	
	/* ������ʼ�� */
	vd_check_return_val_error(tp.port.init == vd_null,  vd_false, "tp init port is null\n");
	tp.param.point_report = vd_tp_chip_point_report;//tp�ϱ��ӿ�
	ret = tp.port.init(&tp.param);
	vd_check_return_val_error(ret == vd_false, vd_false, "tp init error\n");
	
	return vd_true;
}

/**
 * @brief : tp��ѯ
 *
 * @return vd_void_t
 */
vd_void_t vd_tp_loop(vd_void_t)
{
	vd_check_return_error(tp.port.loop == vd_null, "tp loop port is null\n");
	tp.port.loop();
}

/**
 * @brief :�Ӷ��л�ȡtp����,��Ӧ�ò���ã�
 *
 * @param point: tp����
 * @return vd_bool_t
 */
vd_bool_t vd_tp_read(tp_point_t* point)
{
	vd_bool_t ret = vd_false;
	ssize_t mq_ret;
	
	vd_check_return_val_error(point == vd_null,  vd_false, "tp read buf is null\n");
	vd_check_return_val_error(tp.point_queue <= 0, vd_false, "tp_queue error\n");
	
	mq_ret = mq_receive(tp.point_queue, (char *)point, sizeof(tp_point_t), NULL);//���ݳ���
	vd_check_return_val_error(mq_ret != sizeof(tp_point_t),  vd_false, "tp mq_receive error\n");
	
	return vd_true;
}
#endif

