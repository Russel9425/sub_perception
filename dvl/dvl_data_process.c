/**************************************************************************

           Copyright(C), 2016-2026, tao.jing All rights reserved

 **************************************************************************
   File   : dvl_data_process.c
   Author : tao.jing
   Date   : 19-9-6
   Brief  : 
**************************************************************************/
#include <stdio.h>
#include <string.h>
#include "dvl_data_process.h"
#include "dvl_env.h"

dvl_process_func_t process_func_array[10] = {
    process_sa_data,
    process_ts_data,
    process_wi_data,
    process_ws_data,
    process_we_data,
    process_wd_data,
    process_bi_data,
    process_bs_data,
    process_be_data,
    process_bd_data,
};

void process_sa_data(const char* _data_type, const char* _data, unsigned char data_count)
{
    printf(" === process_sa_data === \n");
    int ret = sscanf(_data, "%f,%f,%f", &g_dvl_sa.pitch_degree,
                                        &g_dvl_sa.roll_degree,
                                        &g_dvl_sa.heading_degree);

    if ( ret != 3 )
    {
        printf("SA Error: sscanf data resolved count %d\n", ret);
        return;
    }

    printf("SA - Pitch: %f, Roll: %f, Head: %f\n", g_dvl_sa.pitch_degree,
                                                 g_dvl_sa.roll_degree,
                                                 g_dvl_sa.heading_degree);
}

void process_ts_data(const char* _data_type, const char* _data, unsigned char data_count)
{
    printf(" === process_ts_data === \n");
    if ( data_count < DVL_TIME_SEG_LEN + 1 )
    {
        printf("Error: ts length error %s\n", _data);
        return;
    }

    char time[DVL_TIME_SEG_LEN + 1] = {0};
    char data[DVL_DATA_SEG_MAX_LEN] = {0};

    //Resolve time seg -- len : 14
    for (unsigned char idx = 0; idx < DVL_TIME_SEG_LEN; idx ++)
    {
        if ( _data[idx] <= '9' && _data[idx] >= '0' )
        {
            time[idx] = _data[idx];
        }
        else
        {
            printf("Error: time seg resolve failed %s\n", _data);
            return;
        }
    }

    if ( _data[DVL_TIME_SEG_LEN] != ',' )
    {
        printf("Error: time seg resolve failed %s\n", _data);
    }
    printf("TS - Time String : %s\n", time);

    //Resolve data seg
    strncpy(data, _data + DVL_TIME_SEG_LEN + 1, data_count - DVL_TIME_SEG_LEN - 1);
    int ret = sscanf(data, "%f,%f,%f,%f,%d", &g_dvl_ts.salinity_ppt,
                                              &g_dvl_ts.temperature_c,
                                              &g_dvl_ts.depth_trans_m,
                                              &g_dvl_ts.speed_sound_m_s,
                                              &g_dvl_ts.bbb);

    if ( ret != 5 )
    {
        printf("TS Error: sscanf data resolved count %d\n", ret);
        return;
    }

    printf("TS - salinity_ppt: %f \n"
           "     temperature_c: %f \n"
           "     depth_trans_m: %f \n"
           "     speed_sound_m_s: %f \n"
           "     built-in-test: %d \n",
           g_dvl_ts.salinity_ppt,
           g_dvl_ts.temperature_c,
           g_dvl_ts.depth_trans_m,
           g_dvl_ts.speed_sound_m_s,
           g_dvl_ts.bbb);
}

void process_wi_data(const char* _data_type, const char* _data, unsigned char data_count)
{
    printf("process_wi_data\n");
}

void process_ws_data(const char* _data_type, const char* _data, unsigned char data_count)
{
    printf("process_ws_data\n");
}

void process_we_data(const char* _data_type, const char* _data, unsigned char data_count)
{
    printf("process_we_data\n");
}

void process_wd_data(const char* _data_type, const char* _data, unsigned char data_count)
{
    printf("process_wd_data\n");
}

void process_bi_data(const char* _data_type, const char* _data, unsigned char data_count)
{
    printf(" === process_bi_data === \n");
    int ret = sscanf(_data, "%d,%d,%d,%d,%c", &g_dvl_bi.vx_mm_s,
                                              &g_dvl_bi.vy_mm_s,
                                              &g_dvl_bi.vz_mm_s,
                                              &g_dvl_bi.ve_mm_s,
                                              &g_dvl_bi.status);

    if ( ret != 5 )
    {
        printf("BI Error: sscanf data resolved count %d\n", ret);
        return;
    }

    printf("BI - Vx: %d, Vy: %d, Vz: %d, \n"
           "Ve: %d, Status: %c\n", g_dvl_bi.vx_mm_s,
                                   g_dvl_bi.vy_mm_s,
                                   g_dvl_bi.vz_mm_s,
                                   g_dvl_bi.ve_mm_s,
                                   g_dvl_bi.status);
}

void process_bs_data(const char* _data_type, const char* _data, unsigned char data_count)
{
    printf(" === process_bs_data === \n");
    int ret = sscanf(_data, "%d,%d,%d,%c", &g_dvl_bs.transverse_vel_mm_s,
                                           &g_dvl_bs.longitude_vel_mm_s,
                                           &g_dvl_bs.normal_vel_mm_s,
                                           &g_dvl_bs.status);

    if ( ret != 4 )
    {
        printf("BS Error: sscanf data resolved count %d\n", ret);
        return;
    }

    printf("BS - TV: %d, LV: %d, NV: %d, Status: %c\n", g_dvl_bs.transverse_vel_mm_s,
                                                        g_dvl_bs.longitude_vel_mm_s,
                                                        g_dvl_bs.normal_vel_mm_s,
                                                        g_dvl_bs.status);
}

void process_be_data(const char* _data_type, const char* _data, unsigned char data_count)
{
    printf(" === process_be_data === \n");
    int ret = sscanf(_data, "%d,%d,%d,%c", &g_dvl_be.east_vel_mm_s,
                                           &g_dvl_be.north_vel_mm_s,
                                           &g_dvl_be.upward_vel_mm_s,
                                           &g_dvl_be.status);

    if ( ret != 4 )
    {
        printf("BE Error: sscanf data resolved count %d\n", ret);
        return;
    }

    printf("BS - EV: %d, NV: %d, UV: %d, Status: %c\n", g_dvl_be.east_vel_mm_s,
                                                        g_dvl_be.north_vel_mm_s,
                                                        g_dvl_be.upward_vel_mm_s,
                                                        g_dvl_be.status);
}

void process_bd_data(const char* _data_type, const char* _data, unsigned char data_count)
{
    printf(" === process_bd_data === \n");
    int ret = sscanf(_data, "%f,%f,%f,%f,%f", &g_dvl_bd.east_dis_m,
                                              &g_dvl_bd.north_dis_m,
                                              &g_dvl_bd.upward_dis_m,
                                              &g_dvl_bd.range_to_center,
                                              &g_dvl_bd.time_to_last_good_vel_s);

    if ( ret != 5 )
    {
        printf("BE Error: sscanf data resolved count %d\n", ret);
        return;
    }

    printf("BS - ED: %f, \n"
           "     ND: %f, \n"
           "     UD: %f, \n"
           "     RC: %f, \n"
           "     TL: %f, \n",
           g_dvl_bd.east_dis_m,
           g_dvl_bd.north_dis_m,
           g_dvl_bd.upward_dis_m,
           g_dvl_bd.range_to_center,
           g_dvl_bd.time_to_last_good_vel_s);
}

