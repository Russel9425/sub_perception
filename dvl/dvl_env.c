/**************************************************************************

           Copyright(C), 2016-2026, tao.jing All rights reserved

 **************************************************************************
   File   : gps_env.c
   Author : tao.jing
   Date   : 19-9-1
   Brief  : 
**************************************************************************/
#include "dvl_env.h"
#include "string.h"

char g_data_type[DVL_DATA_TYPE_SEG_LEN + 1] = {0};
char g_data[DVL_DATA_SEG_MAX_LEN] = {0};

unsigned char g_data_len = 0;

//GPS processing thread handler
pthread_t dvl_process_thread;

//DVL data meta ready event
pthread_cond_t  g_dvl_data_cv = PTHREAD_COND_INITIALIZER;
pthread_mutex_t g_dvl_data_mt = PTHREAD_MUTEX_INITIALIZER;

//DVL global data
dvl_system_attitude_t    g_dvl_sa;
dvl_timing_scaling_t     g_dvl_ts;
dvl_instrument_ref_vel_t g_dvl_wi, g_dvl_bi;
dvl_ship_ref_vel_t       g_dvl_ws, g_dvl_bs;
dvl_earth_ref_vel_t      g_dvl_we, g_dvl_be;
dvl_earth_ref_dis_t      g_dvl_wd, g_dvl_bd;

void make_cond_timeout_ms(struct timespec *p_tsp, unsigned int ms)
{
    struct timeval now;

    //get current time
    gettimeofday(&now, NULL);

    //convert current time to timespec
    p_tsp->tv_sec  = now.tv_sec;
    p_tsp->tv_nsec = now.tv_usec * 1000;

    //add wait time
    p_tsp->tv_nsec += ms * 1000 * 1000;
}