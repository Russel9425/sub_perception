/**************************************************************************

           Copyright(C), 2016-2026, tao.jing All rights reserved

 **************************************************************************
   File   : gps_apt.h
   Author : tao.jing
   Date   : 19-7-12
   Brief  : 
**************************************************************************/

#ifndef SUB_PERCEPTION_GPS_API_H
#define SUB_PERCEPTION_GPS_API_H

#include "serial.h"

struct gpgga_t
{

};

bool gps_init();

bool gps_open_device(serial_t* p_serial, const char* serial_port);

char gps_read_char(serial_t* p_serial);

//State Machine for GPS processing
void* gps_processing(void* arg);

#endif //SUB_PERCEPTION_GPS_API_H
