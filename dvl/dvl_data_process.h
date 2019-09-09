/**************************************************************************

           Copyright(C), 2016-2026, tao.jing All rights reserved

 **************************************************************************
   File   : dvl_data_package_process.h
   Author : tao.jing
   Date   : 19-9-6
   Brief  : 
**************************************************************************/

#ifndef SUB_PERCEPTION_DVL_DATA_PROCESS_H
#define SUB_PERCEPTION_DVL_DATA_PROCESS_H

typedef void(*dvl_process_func_t)(const char*, const char*, unsigned char);

void process_sa_data(const char* _data_type, const char* _data, unsigned char data_count);
void process_ts_data(const char* _data_type, const char* _data, unsigned char data_count);
void process_wi_data(const char* _data_type, const char* _data, unsigned char data_count);
void process_ws_data(const char* _data_type, const char* _data, unsigned char data_count);
void process_we_data(const char* _data_type, const char* _data, unsigned char data_count);
void process_wd_data(const char* _data_type, const char* _data, unsigned char data_count);
void process_bi_data(const char* _data_type, const char* _data, unsigned char data_count);
void process_bs_data(const char* _data_type, const char* _data, unsigned char data_count);
void process_be_data(const char* _data_type, const char* _data, unsigned char data_count);
void process_bd_data(const char* _data_type, const char* _data, unsigned char data_count);

extern dvl_process_func_t process_func_array[10];

#endif //SUB_PERCEPTION_DVL_DATA_PROCESS_H
