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
#include "uw_api.h"
#include "uw_device.h"

bool dvl_init()
{
    imx6Init();
    return uw_dvl_init();
}

bool uw_dvl_init()
{
    int ret = 0;
    ret = rs232_rs422_power_set(UW_DEVICE_DVL_PORT, SERIAL_POWER_ON);
    if (ret)
    {
        printf("Power set error...");
        return false;
    }

    ret = rs232_rs422_mode_switch(UW_DEVICE_DVL_PORT, SERIAL_RS232_MODE);
    if (ret)
    {
        printf("Mode switch error...");
        return false;
    }

    ret = rs232_rs422_cfg(UW_DEVICE_DVL_PORT, 19200, 8, 1, 0);
    if (ret)
    {
        printf("Config error...");
        return false;
    }
    return true;
}


int uw_dvl_send(char *buf, int len)
{
    int ret = rs232_rs422_send(UW_DEVICE_DVL_PORT, buf, len);
    if (ret != len)
    {
        printf("Send error...%d\n", ret);
    }
    return ret;
}

int uw_dvl_recv(char *buf, int len)
{
    int ret = rs232_rs422_recv(UW_DEVICE_DVL_PORT, buf, len);

    if (ret == 69651)
    {
        printf("Recv error...%d\n", ret);
    }

    return ret;
}

bool uw_dvl_read_char(char *p_read_char)
{
    int ret = uw_dvl_recv(p_read_char, sizeof(char));
    if (ret != sizeof(char))
    {
        //printf("Serial read char error...\n");
        return false;
    }

    return true;
}

char uw_dvl_read_byte(bool *state)
{
    char read_byte;
    int ret = rs232_rs422_recv(UW_DEVICE_DVL_PORT, &read_byte, 1);

    if (ret == 69651)
    {
        //printf("Recv error...%d\n", ret);
        *state = false;
        return 0xFF;
    }

    *state = true;
    return read_byte;
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
        bool read_ret = false;
        read_char = uw_dvl_read_byte(&read_ret);
        if (!read_ret)
        {
            //break;    //for clion static check only
            continue;
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


