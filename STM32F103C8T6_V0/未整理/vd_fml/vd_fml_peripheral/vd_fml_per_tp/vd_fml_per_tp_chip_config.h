#ifndef __VD_FML_PER_TP_CHIP_CONFIG_H
#define __VD_FML_PER_TP_CHIP_CONFIG_H

#include "project_config.h"


//���� project_config.h ��ѡ��Ĳ�ͬTPоƬ����Ӧע�ắ��
#if (PROJECT_TP_CHIP == TP_CHIP_ICN85XX)
#elif (PROJECT_TP_CHIP == TP_CHIP_ICNGT911)
#elif (PROJECT_TP_CHIP == TP_CHIP_CVT_TS)
	#include "vd_fml_per_tp_chip/vd_fml_per_tp_cvt_ts.h"
	#define	TP_CHIP_PORT_REGISTER cvt_ts_port_register
#endif

#endif
