/**************************************************************************

           Copyright(C), 2016-2026, tao.jing All rights reserved

 **************************************************************************
   File   : gps_env.c
   Author : tao.jing
   Date   : 19-9-1
   Brief  : 
**************************************************************************/
#include "gps_env.h"
#include "string.h"

struct gps_meta_data_t g_gps_meta_data;

void init_gps_meta_data(struct gps_meta_data_t* _meta)
{
    memset(_meta->Time, 0, GPS_STRING_LEN);
    memset(_meta->LastTime, 0, GPS_STRING_LEN);
    memset(_meta->Lat, 0, GPS_STRING_LEN);
    memset(_meta->Long, 0, GPS_STRING_LEN);
    memset(_meta->Sats, 0, GPS_STRING_LEN);
    _meta->LatDir  = 0;
    _meta->LongDir = 0;
}

void copy_gps_meta_data(struct gps_meta_data_t* dst_meta, struct gps_meta_data_t* src_meta)
{
    memcpy((void*)dst_meta, (void*)src_meta, sizeof(struct gps_meta_data_t));
}

//GPS processing thread handler
pthread_t gps_process_thread;

//GPS data meta ready event
pthread_cond_t  g_gps_data_cv = PTHREAD_COND_INITIALIZER;
pthread_mutex_t g_gps_data_mt = PTHREAD_MUTEX_INITIALIZER;

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