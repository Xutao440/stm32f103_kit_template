#ifndef _LOG_H_
#define _LOG_H_

#include "bsp_cfg.h"

#define MAX_STRING_SIZE         300  //×Ö·û´®³¤¶È

//DEBUG(fmt, args...)  	printf("[ABORT][FILE = %s][FUNCTION = %s][LINE = %d] ["format"]\n",__FILE__,__FUNCTION__,__LINE__,##args)
//log_printf("[ABORT] [%s %d]: "format"",__FUNCTION__,__LINE__,##args)
#if (ABORT_LOG && LOG_ENABLE)
#define log_abort(format, args...) log_printf("[ABORT]: "format"",##args)
#else
#define log_abort(format, args...)
#endif

#if (ERROR_LOG && LOG_ENABLE)
#define log_error(format, args...) log_printf("[ERRO]: "format"",##args)
#else
#define log_error(format, args...)
#endif

#if (INFO_LOG && LOG_ENABLE)
#define log_info(format, args...) log_printf("[INFO]: "format"",##args)
#else
#define log_info(format, args...)
#endif

#if (DEBUG_LOG && LOG_ENABLE)
#define log_debug(format, args...) log_printf("[DEBUG]: "format"",##args)
#else
#define log_debug(format, args...)
#endif

#if (FATAL_LOG && LOG_ENABLE)
#define log_fatal(format, args...) log_printf("[FATAL]: "format"",##args)
#else
#define log_fatal(format, args...)
#endif

#if (WARN_LOG && LOG_ENABLE)
#define log_warn(format, args...) log_printf("[WARN]: "format"",##args)
#else
#define log_warn(format, args...)
#endif

#if LOG_ENABLE
	#define log_array(tip, array, size)      \
		{                                    \
			log_printf("%s[%d]:", tip, size);    \
			for (int i = 0; i < size; i++) { \
				log_printf("%02X ", array[i]);    \
			}                                \
			log_printf("\r\n");                  \
		}
	#endif

void log_init(VD_USARTx_E USARTx);		
void log_printf(char* format, ...);	
		
#endif  //_DT_LOG_H_

