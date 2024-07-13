/*
 * Project: WH001
 * Module: 
 * File: pid.c
 * Created Date: 2022-11-26 10:05:23
 * Author: your_name
 * Description: description
 * -----
 * todo: modified 
 * -----
 * Copyright (c) 2022 - vDiscovery, Inc
 */

/* ======================================================================================
 * log
 */
#define LOG_ENABLE_DEBUG (1)

/* ======================================================================================
 * includes
 */
#include "vd_fml_per_ntc_pid.h"

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

/**
 * @brief 
 * 
 * @param calibration 
 * @param state 
 * @return pid_state 
 */
pid_state vd_fml_per_ntc_temp_set(pid_calibration calibration, pid_state state) {
    vd_int16_t error = state.target - state.actual;
//    co_printf("error=%d\n",error);
    state.integral += (error * state.time_delta);
//    co_printf("state.integral=%d\n",state.integral);
    vd_int16_t derivative = (error - state.previous_error) / state.time_delta;
//    co_printf("derivative=%d\n",derivative);
    state.output = (
        (calibration.kp * error) + (calibration.ki * state.integral) + (calibration.kd * derivative)
    );
//    co_printf("state.output=%d\n",state.output);

    state.previous_error = error;
    return state;
}
/* ======================================================================================
 * implementation
 */
