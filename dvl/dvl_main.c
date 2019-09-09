/**************************************************************************

           Copyright(C), 2016-2026, tao.jing All rights reserved

 **************************************************************************
   File   : main.cpp
   Author : tao.jing
   Date   : 19-7-12
   Brief  : 
**************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include "dvl_api.h"
#include "dvl_env.h"
#include "dvl_data_process.h"

void process_dvl_meta_data(const char* _data_type, const char* _data, unsigned char data_count);

int main(int argc, char **argv)
{
    printf(" ===== Sensor DVL ===== \n");
    char cmd;
    printf("Press any key to continue dvl init...\n");
    scanf("%c", &cmd);

    dvl_init();

    printf("Press any key to continue dvl processing thread...\n");
    scanf("%c", &cmd);

    int err = pthread_create(&dvl_process_thread, NULL, dvl_processing, NULL);

    if ( err != 0 )
    {
        printf("Cannot create dvl process thread, err code %d...\n", err);
        printf("Main exit...\n");
        return 0;
    }

    //Wait for DVL data coming

    char cur_data_type[DVL_DATA_TYPE_SEG_LEN + 1] = {0};
    char cur_data[DVL_DATA_SEG_MAX_LEN] = {0};
    unsigned char cur_data_len = 0;


    unsigned int wait_cnt = 0;
    while( wait_cnt < 5000 )
    {
        pthread_mutex_lock(&g_dvl_data_mt);

        struct timespec tsp;
        make_cond_timeout_ms(&tsp, 2000);

        /*
        //time wait : set a timeout for data wait
        int wait_err = pthread_cond_timedwait(&g_gps_data_cv, &g_gps_data_mt, &tsp);
        if ( wait_err != 0 )
        {
            //printf("Wait GPS meta data timeout...\n");
            pthread_mutex_unlock(&g_gps_data_mt);
            continue;
        }
        */

        //block wait
        int wait_err = pthread_cond_wait(&g_dvl_data_cv, &g_dvl_data_mt);
        if ( wait_err != 0 )
        {
            printf("Wait DVL meta data timeout...\n");
            pthread_mutex_unlock(&g_dvl_data_mt);
            continue;
        }

        memcpy(cur_data_type, g_data_type, DVL_DATA_TYPE_SEG_LEN+1);
        memset(cur_data, 0, DVL_DATA_SEG_MAX_LEN);
        memcpy(cur_data, g_data, g_data_len);
        cur_data_len = g_data_len;

        pthread_mutex_unlock(&g_dvl_data_mt);

        process_dvl_meta_data(cur_data_type, cur_data, cur_data_len);

        wait_cnt++;
    }

    return 0;
}

void process_dvl_meta_data(const char* _data_type, const char* _data, unsigned char data_count)
{
    for ( unsigned int idx = 0; idx < DVL_END; idx ++ )
    {
        if ( strncmp(_data_type, dvl_prefixs[idx], 2) == 0 )
        {
            //Notify corresponding data coming - idx
            dvl_process_func_t p_dvl_func;
            p_dvl_func = process_func_array[idx];
            p_dvl_func(_data_type, _data, data_count);
        }
    }
}
