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

bool gps_init();

bool uw_gps_init();

int  uw_gps_read(char* buf, int len);

int  uw_gps_send(char* buf, int len);

bool uw_gps_read_char(char* p_read_char);

char uw_gps_read_byte(bool* state);

//State Machine for GPS processing
void* gps_processing(void* arg);

#endif //SUB_PERCEPTION_GPS_API_H
