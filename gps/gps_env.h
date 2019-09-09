/**************************************************************************

           Copyright(C), 2016-2026, tao.jing All rights reserved

 **************************************************************************
   File   : gps_env.h
   Author : tao.jing
   Date   : 19-9-1
   Brief  : 
**************************************************************************/
#ifndef SUB_PERCEPTION_GPS_ENV_H
#define SUB_PERCEPTION_GPS_ENV_H

#include <pthread.h>
#include <sys/time.h>

static const char* gps_device = "/dev/ttyUSB0" ;
static const char GGAMessage[] = "$GPGGA,";
static const char VTGMessage[] = "$GPVTG,";

#define GPS_STRING_LEN 50
#define GPS_STRING_MAX 48

struct gps_meta_data_t
{
    char Time[GPS_STRING_LEN];
    char LastTime[GPS_STRING_LEN];

    char Lat[GPS_STRING_LEN];
    char Long[GPS_STRING_LEN];
    char Sats[GPS_STRING_LEN];
    char LatDir;
    char LongDir;
};

extern struct gps_meta_data_t g_gps_meta_data;

void init_gps_meta_data(struct gps_meta_data_t* _meta);
void copy_gps_meta_data(struct gps_meta_data_t* dst_meta, struct gps_meta_data_t* src_meta);

//GPS processing thread handler
extern pthread_t gps_process_thread;

//GPS data meta ready event
extern pthread_cond_t  g_gps_data_cv;
extern pthread_mutex_t g_gps_data_mt;

void make_cond_timeout_ms(struct timespec *p_tsp, unsigned int ms);

#endif //SUB_PERCEPTION_GPS_ENV_H
