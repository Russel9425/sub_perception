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
#include "gps_api.h"
#include "gps_env.h"

void process_gps_meta_data(struct gps_meta_data_t* _meta);

int main(int argc, char **argv)
{
    printf(" ===== Hello GPS ===== \n");
    char cmd;
    printf("Press any key to continue gps init...\n");
    scanf("%c", &cmd);

    gps_init();

    printf("Press any key to continue gps processing thread...\n");
    scanf("%c", &cmd);

    int err = pthread_create(&gps_process_thread, NULL, gps_processing, NULL);

    if ( err != 0 )
    {
        printf("Cannot create gps process thread, err code %d...\n", err);
        printf("Main exit...\n");
        return 0;
    }

    //Wait for GPS data coming
    struct gps_meta_data_t cur_gps_meta_data;
    init_gps_meta_data(&cur_gps_meta_data);

    unsigned int wait_cnt = 0;
    while( wait_cnt < 5000 )
    {
        pthread_mutex_lock(&g_gps_data_mt);

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
        int wait_err = pthread_cond_wait(&g_gps_data_cv, &g_gps_data_mt);
        if ( wait_err != 0 )
        {
            printf("Wait GPS meta data timeout...\n");
            pthread_mutex_unlock(&g_gps_data_mt);
            continue;
        }

        copy_gps_meta_data(&cur_gps_meta_data, &g_gps_meta_data);

        pthread_mutex_unlock(&g_gps_data_mt);

        process_gps_meta_data(&cur_gps_meta_data);

        wait_cnt++;
    }

    return 0;
}

void process_gps_meta_data(struct gps_meta_data_t* _meta)
{
    char* Time = _meta->Time;
    char* LastTime = _meta->LastTime;
    char* Lat  = _meta->Lat;
    char* Long = _meta->Long;
    char* Sats = _meta->Sats;

    unsigned int hour, minute, second;
    unsigned int degree;
    unsigned long ulong;

    bool good_data = (strcmp(Time, LastTime) != 0);// && (LastTime[0] != '\0');

    if (good_data && (sscanf(strchr(Lat, '.') - 2, "%2u.%5lu", &minute, &ulong) == 2))
    {
        ulong *= 6;
        second = (unsigned short) (ulong / 10000);
        //printf("%lu %u ", ULong, Second);
        ulong -= ((unsigned long) second) * 10000l;
        //printf("%lu\n", ULong);
        *(strchr(Lat, '.') - 2) = '\0';
        if (sscanf(Lat, "%3u", &degree) == 1)
        {
            printf("Lat  %3.3u:%2.2u:%2.2u.%4.4lu%c\n", degree, minute, second, ulong, _meta->LatDir);
        } else
        {
            printf("Lat  ---:--:--.-----\n");
        }
    } else
    {
        printf("Lat  ---:--:--.-----\n");
    }
    Lat[0] = '\0';
    Lat[1] = '\0';
    _meta->LatDir = '\0';

    // Print the longitude nicely to the LCD console.
    if (good_data && (sscanf(strchr(Long, '.') - 2, "%2u.%5lu", &minute, &ulong) == 2))
    {
        ulong *= 6;
        second = (unsigned short) (ulong / 10000);
        ulong -= ((unsigned long) second) * 10000l;
        *(strchr(Long, '.') - 2) = '\0';
        if (sscanf(Long, "%3u", &degree) == 1)
        {
            printf("Long %3.3u:%2.2u:%2.2u.%4.4lu%c\n", degree, minute, second, ulong, _meta->LongDir);
        } else
        {
            printf("Long ---:--:--.-----\n");
        }
    } else
    {
        printf("Long ---:--:--.-----\n");
    }
    Long[0] = '\0';
    Long[1] = '\0';
    _meta->LongDir = '\0';

    // Print the time and active satellite count nicely
    if (good_data && (sscanf(Time, "%2u%2u%2u", &hour, &minute, &second) == 3) &&
        (sscanf(Sats, "%lu", &ulong) == 1))
    {
        printf("GMT %2.2u:%2.2u:%2.2u Sats:%2lu\n", hour, minute, second, ulong);
    } else
    {
        printf("GMT --:--:-- Sats:--\n");
    }
    strncpy(LastTime, Time, GPS_STRING_LEN - 1);
    Time[0] = '\0';
    Time[1] = '\0';
    Sats[0] = '\0';
    Sats[1] = '\0';
}
