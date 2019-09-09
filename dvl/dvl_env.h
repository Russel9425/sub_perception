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

static const char* dvl_device = "/dev/ttyUSB1" ;

enum e_dvl_data_type
{
    DVL_SA = 0, //system attitude data
    DVL_TS = 1, //timing and scaling data
    DVL_WI = 2, //water-mass, instrument-referenced velocity data
    DVL_WS = 3, //water-mass, ship-referenced velocity data
    DVL_WE = 4, //water-mass, earth-referenced velocity data
    DVL_WD = 5, //water-mass, earth-referenced distance data
    DVL_BI = 6, //bottom-track, instrument-referenced velocity data
    DVL_BS = 7, //bottom-track, ship-referenced velocity data
    DVL_BE = 8, //bottom-track, earth-referenced velocity data
    DVL_BD = 9, //bottom-track, earth-referenced distance data
    DVL_END = 10
};

static const char dvl_prefixs[DVL_END][2] = {
    "SA",
    "TS",
    "WI",
    "WS",
    "WE",
    "WD",
    "BI",
    "BS",
    "BE",
    "BD"
};

#define GPS_STRING_LEN 50
#define GPS_STRING_MAX 48

#define DVL_DATA_TYPE_SEG_LEN 2
#define DVL_DATA_SEG_MAX_LEN 80
#define DVL_TIME_SEG_LEN     14

//For DVL_SA data
typedef struct _dvl_system_attitude_t
{
    float pitch_degree;
    float roll_degree;
    float heading_degree;
}dvl_system_attitude_t;

typedef struct _dvl_timing_scaling_t
{
    //timing
    unsigned char year;
    unsigned char month;
    unsigned char day;
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
    unsigned char ten_millisec;
    //scaling
    float salinity_ppt;         //salinity in parts per thousand (ppt)
    float temperature_c;        //temperature in C
    float depth_trans_m;        //depth of transducer face in meters
    float speed_sound_m_s;      //speed of sound in meters per second
    unsigned int bbb;           //built-in test result code
}dvl_timing_scaling_t;

typedef struct _dvl_instrument_ref_vel_t
{
    int vx_mm_s;                //X-axis vel. data in mm/s (+ = Bm1 Bm2 xdcr movement relative to water-mass/bottom)
    int vy_mm_s;                //Y-axis vel. data in mm/s (+ = Bm4 Bm3 xdcr movement relative to water-mass/bottom)
    int vz_mm_s;                //Z-axis vel. data in mm/s (+ = transducer movement away from water-mass/bottom)
    int ve_mm_s;                //Error velocity data in mm/s
    unsigned char status;       //Status of velocity data (A = good, V = bad)
}dvl_instrument_ref_vel_t;

typedef struct _dvl_ship_ref_vel_t
{
    int transverse_vel_mm_s;    //Transverse vel. data in mm/s (+ = Port Stbd ship movement relative to water-mass/bottom)
    int longitude_vel_mm_s;     //Longitudinal vel. data in mm/s (+ = Aft Fwd ship movement rel. to water-mass/bottom)
    int normal_vel_mm_s;        //Normal velocity data in mm/s (+ = ship movement away from water-mass/bottom)
    unsigned char status;       //Status of velocity data (A = good, V = bad)
}dvl_ship_ref_vel_t;

typedef struct _dvl_earth_ref_vel_t
{
    int east_vel_mm_s;          //East (u-axis) velocity data in mm/s (+ = DVL movement to east)
    int north_vel_mm_s;         //North (v-axis) velocity data in mm/s (+ = DVL movement to north)
    int upward_vel_mm_s;        //Upward (w-axis) velocity data in mm/s (+ = DVL movement to surface)
    unsigned char status;       //Status of velocity data (A = good, V = bad)
}dvl_earth_ref_vel_t;

typedef struct _dvl_earth_ref_dis_t
{
    float east_dis_m;                 //East (u-axis) distance data in meters
    float north_dis_m;                //North (v-axis) distance data in meters
    float upward_dis_m;               //Upward (w-axis) distance data in meters
    float range_to_center;            //Range to water-mass/bottom center in meters
    float time_to_last_good_vel_s;    //Time since last good-velocity estimate in seconds
}dvl_earth_ref_dis_t;

extern char g_data_type[DVL_DATA_TYPE_SEG_LEN + 1];
extern char g_data[DVL_DATA_SEG_MAX_LEN];

extern unsigned char g_data_len;

//DVL processing thread handler
extern pthread_t dvl_process_thread;

//DVL data meta ready event
extern pthread_cond_t  g_dvl_data_cv;
extern pthread_mutex_t g_dvl_data_mt;

//DVL global data
extern dvl_system_attitude_t    g_dvl_sa;
extern dvl_timing_scaling_t     g_dvl_ts;
extern dvl_instrument_ref_vel_t g_dvl_wi, g_dvl_bi;
extern dvl_ship_ref_vel_t       g_dvl_ws, g_dvl_bs;
extern dvl_earth_ref_vel_t      g_dvl_we, g_dvl_be;
extern dvl_earth_ref_dis_t      g_dvl_wd, g_dvl_bd;


void make_cond_timeout_ms(struct timespec *p_tsp, unsigned int ms);

#endif //SUB_PERCEPTION_GPS_ENV_H
