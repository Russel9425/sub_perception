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
#include <unistd.h>
#include "gps_env.h"
#include "gps_api.h"

serial_t gps_serial;

bool gps_init()
{
    return gps_open_device(&gps_serial, gps_device);
}

bool gps_open_device(serial_t *p_serial, const char* serial_port)
{
    if (serial_open(p_serial, serial_port, 19200) != 0)
    {
        printf("GPS device open %s failed...\n", serial_port);
        return false;
    }

    printf("GPS device open %s successfully...\n", serial_port);
    return true;
}

char gps_read_char(serial_t *p_serial)
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
void* gps_processing(void* arg)
{
    char read_char;
    unsigned char read_state = 0; //index for each serial read decoding
    unsigned char count = 0;      //index for each information segment internal use

    bool ready_to_use = false;

    /*** Declare result strings  ***/
    struct gps_meta_data_t cur_gps_meta_data;

    char* Time = cur_gps_meta_data.Time;
    char* LastTime = cur_gps_meta_data.LastTime;
    char* Lat  = cur_gps_meta_data.Lat;
    char* Long = cur_gps_meta_data.Long;
    char* Sats = cur_gps_meta_data.Sats;

    //unsigned short hour, minute, second;
    //unsigned short degree;
    unsigned int hour, minute, second;
    unsigned int degree;
    unsigned long ulong;

    // Loop until the user hits a key
    printf(" Starting gps information processing ... \n");
    while (1)
    {
        if ((unsigned char)(read_char = gps_read_char(&gps_serial)) == 0xFF)
        {
            break;
        }
        // We got a character so now we have to decode it
        if (read_state < 7)
        {
            //printf("State 1\n");
            // read_states 0-6 correspond to characters 0-6 in the $GPGGA, status message
            // header
            if ((char) read_char == GGAMessage[read_state])
            {
                // We got the right character so expect the next one
                read_state++;
                // If we moved past the $GPGGA, header we need to zero count for the
                // next part of the read_state machine
                if (read_state == 7) count = 0;
            }
                // if one of them gets lost or is wrong then just wait for the next
                // $GPGGA, message
            else 
            {
                read_state = 0;
                printf("No start char---$GPGGA!\n");
            }
        }
        else if (read_state == 7) // We are getting the UTC (TIME) part of the message
        {
            //printf("State 2\n");
            // Check if we got the ',' indicating the end of the time field
            if (read_char == ',')
            {
                // Terminate the time string properly
                Time[count] = '\0';
                // Set the count for the next read_state machine read_state
                count = 0;
                // Go to the next read_state machine read_state
                read_state++;
            } else
            {
                // Store the character we got
                if (count <= GPS_STRING_MAX) Time[count] = read_char;
                // Get ready to store the next character
                count++;
            }
        }
        else if (read_state == 8) // We're getting the latitude (LAT) information
        {
            //printf("State 3\n");
            // Check if we got the ',' indicating the end of the latitude field
            if (read_char == ',')
            {
                // Terminate the latitude string properly
                Lat[count] = '\0';
                // Set the count for the next read_state machine read_state
                count = 0;
                // Go to the next read_state machine read_state
                read_state++;
            } else
            {
                // Store the character we got
                if (count <= GPS_STRING_MAX) Lat[count] = read_char;
                // Get ready to store the next character
                count++;
            }
        }
        else if (read_state == 9) // We're getting the latitude direction ('N' or 'S')
        {
            //printf("State 4\n");
            // Check if we got the ',' indicating the end of the field
            if (read_char == ',') read_state++;
            else cur_gps_meta_data.LatDir = read_char;
        }
        else if (read_state == 10) // LONG
        {
            // Check if we got the ',' indicating the end of the longitude field
            if (read_char == ',')
            {
                // Terminate the longitude string properly
                Long[count] = '\0';
                // Set the count for the next read_state machine read_state
                count = 0;
                // Go to the next read_state machine read_state
                read_state++;
            } else
            {
                // Store the character we got
                if (count <= GPS_STRING_MAX) Long[count] = read_char;
                // Get ready to store the next character
                count++;
            }
        }
        else if (read_state == 11) // We're getting the longitude direction ('E' or 'W')
        {
            // Check if we got the ',' indicating the end of the field
            if (read_char == ',') read_state++;
            else cur_gps_meta_data.LongDir = read_char;
        }
        else if (read_state == 12) // We're getting the position type...
        {
            // Check if we got the ',' indicating the end of the field
            if (read_char == ',') read_state++;
        }
        else if (read_state == 13) // Getting the number of active satellites...
        {
            // Check if we got the ',' indicating the end of the field
            if (read_char == ',')
            {
                // If we didn't actually get any characters between the commas
                // then store a space rather than a '\0' so that we can still print
                // the actual latitude, which is right after this one character
                Sats[count] = '\0';
                // Set the count for the next read_state machine read_state
                count = 0;
                // Reset the read_state machine for the next status message
                //read_state++;
                // Got enough information for use
                ready_to_use = true;
                read_state = 0;
            } else
            {
                // Store the character we got
                if (count <= GPS_STRING_MAX) Sats[count] = read_char;
                // Get ready to store the next character
                count++;
            }
        }
            /*** GPVTG Speed and Course Information ***/
        else if ((read_state >= 14) && (read_state <= 20)) // Getting the "$GPVTG," header
        {
            // read_states 14-20 correspond to characters 0-6 in the $GPVTG, status message
            // header
            if ((char) read_char == VTGMessage[read_state - 14])
            {
                // We got the right character so expect the next one
                read_state++;
                // If we moved past the $GPVTG, header we need to zero count for the
                // next part of the read_state machine
                if (read_state == 21) count = 0;
            }
                // if one of them gets lost or is wrong then just wait for the next
                // $GPGGA, message
            else if (read_state > 14) read_state = 0;
        }
        else if (read_state == 21) // Course Over Ground
        {
            // Check if we got the ',' indicating the end of the COG field
            if (read_char == ',')
            {
                // Terminate the COG string properly
                //COG[count] = '\0';
                // Set the count for the next read_state machine read_state
                count = 0;
                // Go to the next read_state machine read_state
                read_state++;
            }
            else
            {
                // Store the character we got
                //if (count <= STRING_MAX) COG[count] = read_char;
                // Get ready to store the next character
                count++;
            }
        }
        else if ((read_state >= 22) && (read_state <= 26)) // We're getting the position type...
        {
            // Check if we got the ',' indicating the end of the field
            if (read_char == ',') read_state++;
        }
        else if (read_state == 27) // We're getting speed over ground
        {
            if (read_char == ',')
            {
                // Terminate the SOG string properly
                //SOG[count] = '\0';
                // Set the count for the next read_state machine read_state
                count = 0;
                // Go to the next read_state machine read_state
                read_state = 0;
            }
            else
            {
                // Store the character we got
                //if (count <= STRING_MAX) SOG[count] = read_char;
                // Get ready to store the next character
                count++;
            }
        }

        if (ready_to_use)
        {
            ready_to_use = false;
            // Make sure we don't display garbage data from the GPS (it
            // returns garbage when it loses satellite lock or has just been
            // turned on)  We know the data is garbage when the time is the same
            // as the last time returned
            //bool good_data = (strcmp(Time, LastTime) != 0) && (LastTime[0] != '\0');

            // We don't judge the data, just send the data to process thread( this app is the main thread)
            //if (good_data && (sscanf(strchr(Lat, '.') - 2, "%2u.%5lu", &minute, &ulong) == 2))
            {
                pthread_mutex_lock(&g_gps_data_mt);
                copy_gps_meta_data(&g_gps_meta_data, &cur_gps_meta_data);
                pthread_mutex_unlock(&g_gps_data_mt);

                pthread_cond_signal(&g_gps_data_cv);

                continue;
            }
/*
            // Wait before we fiddle with IO stuff so that the serial port
            // can settle down
            for (unsigned char i = 0; i < 0xFF; i++);
            // Do another wait while the LCD thinks about things
            for (unsigned char i = 0; i < 0xFF; i++);
            // Print the time, latitude and longitude to the LCD and the console
            // Print the latitude nicely to the console.
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
                    printf("Lat  %3.3u:%2.2u:%2.2u.%4.4lu%c\n", degree, minute, second, ulong, cur_gps_meta_data.LatDir);
                } else
                {
                    printf("Lat  ---:--:--.-----\n");
                }
            }
            else
            {
                printf("Lat  ---:--:--.-----\n");
            }
            Lat[0] = '\0';
            Lat[1] = '\0';
            cur_gps_meta_data.LatDir = '\0';

            // Print the longitude nicely to the LCD console.
            if (good_data && (sscanf(strchr(Long, '.') - 2, "%2u.%5lu", &minute, &ulong) == 2))
            {
                ulong *= 6;
                second = (unsigned short) (ulong / 10000);
                ulong -= ((unsigned long) second) * 10000l;
                *(strchr(Long, '.') - 2) = '\0';
                if (sscanf(Long, "%3u", &degree) == 1)
                {
                    printf("Long %3.3u:%2.2u:%2.2u.%4.4lu%c\n", degree, minute, second, ulong, cur_gps_meta_data.LongDir);
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
            cur_gps_meta_data.LongDir = '\0';

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
            Sats[1] = '\0';*/
        }
        usleep(10);
    }
    return ((void*)0);
}


