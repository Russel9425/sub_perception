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

bool dvl_init();

bool uw_dvl_init();

int  uw_dvl_read(char* buf, int len);

int  uw_dvl_send(char* buf, int len);

bool uw_dvl_read_char(char* p_read_char);

char uw_dvl_read_byte(bool* state);

//State Machine for GPS processing
void* dvl_processing(void* arg);

#endif //SUB_PERCEPTION_GPS_API_H
