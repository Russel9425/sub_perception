/**************************************************************************

           Copyright(C), 2016-2026, tao.jing All rights reserved

 **************************************************************************
   File   : gps_api.c
   Author : tao.jing
   Date   : 19-7-12
   Brief  : 
**************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "dvl_env.h"
#include "dvl_api.h"

serial_t dvl_serial;

bool dvl_init()
{
    return dvl_open_device(&dvl_serial, dvl_device);
}

bool dvl_open_device(serial_t *p_serial, const char* serial_port)
{
    if (serial_open(p_serial, serial_port, 19200) != 0)
    {
        printf("GPS device open %s failed...\n", serial_port);
        return false;
    }

    printf("GPS device open %s successfully...\n", serial_port);
    return true;
}

char dvl_read_char(serial_t *p_serial)
{
    char read_char;
    int ret = serial_read(p_serial, (uint8_t*)&read_char, sizeof(read_char), -1);
    if (ret != sizeof(read_char))
    {
        printf("Serial read char error...\n");
        return 0xFF;
    }

    return read_char;
}

//State Machine for GPS processing
void* dvl_processing(void* arg)
{
    char read_char;
    unsigned char read_state = 0; //index for each serial read decoding
    unsigned char count = 0;      //index for each information segment internal use

    bool ready_to_use = false;

    /*** Declare result strings  ***/
    char DataTypeSeg[DVL_DATA_TYPE_SEG_LEN + 1] = {0};
    char DataSeg[DVL_DATA_SEG_MAX_LEN] = {0};

    // Loop until the user hits a key
    printf(" Starting DVL information processing ... \n");
    while (1)
    {
        if ((unsigned char)(read_char = dvl_read_char(&dvl_serial)) == 0xFF)
        {
            break;
        }

        if ( (char)read_char == ' ' )
        {
            //Skip space char
            continue;
        }
        else if ( (char)read_char == ':' )
        {
            //Force to start a new data package
            read_state = 0;
            count = 0;
        }

        //State 0 : init state
        if ( read_state == 0 )
        {
            //Check the ':' prefix
            if ( (char)read_char == ':' )
            {
                read_state = 1; //Switch to Determine data type state
                continue;
            }
        }
        //State 1 : Determine data type state
        else if ( read_state == 1 )
        {
            // Check if we got the ',' indicating the end of the time field
            if ( (char)read_char == ',' )
            {
                // Terminate the time string properly
                DataTypeSeg[count] = '\0';
                // Set the count for the next read_state machine read_state
                count = 0;
                // Go to relative switch state
                read_state++;
            }
            else
            {
                // Store the character we got
                if (count < DVL_DATA_TYPE_SEG_LEN)
                {
                    DataTypeSeg[count] = read_char;
                    count++;
                }
                else
                {
                    //DataType Seg Overflow
                    read_state = 0;
                    count = 0;
                    continue;
                }
            }
        }
        //State 2 : Data acquire state waiting char <CR>
        else if( read_state == 2 )
        {
            // Check if we got the ':' indicating a new data seg coming
            if ( (char)read_char == ':' )
            {
                // Terminate the time string properly
                DataTypeSeg[count] = '\0';
                count = 0;
                read_state = 0;
                continue;
            }
            else if ( (char)read_char == 0x0D )
            {
                DataSeg[count] = read_char;
                count++;
                read_state ++;
            }
            else
            {
                // Store the character we got
                if (count < DVL_DATA_SEG_MAX_LEN)
                {
                    DataSeg[count] = read_char;
                    count++;
                }
                else
                {
                    //DataType Seg Overflow
                    read_state = 0;
                    count = 0;
                    continue;
                }
            }
        }
        //State 3 : Data acquire state waiting char <LF>
        else if ( read_state == 3 )
        {
            if ( (char)read_char == 0x0A )
            {
                if (count < DVL_DATA_SEG_MAX_LEN)
                {
                    DataSeg[count] = read_char;
                    count++;

                    pthread_mutex_lock(&g_dvl_data_mt);
                    memcpy(g_data_type, DataTypeSeg, DVL_DATA_TYPE_SEG_LEN+1);
                    memcpy(g_data, DataSeg, DVL_DATA_SEG_MAX_LEN);
                    g_data_len = count;
                    pthread_mutex_unlock(&g_dvl_data_mt);

                    pthread_cond_signal(&g_dvl_data_cv);
                    continue;
                }
                else
                {
                    //DataType Seg Overflow
                    read_state = 0;
                    count = 0;
                    continue;
                }
            }
            else
            {
                count = 0;
                // Go to init state
                read_state = 0;
            }
        }
    }
    return ((void*)0);
}


