#ifndef _cplusplus 
#define _cplusplus
#include "rtpsend.h"

int rtp_params_init();
void rtp_send(unsigned char *buf, int len);
void checkerror(int err);
void nal_rtp_send(unsigned char* buf, int len);
int get_next_nalu(unsigned char* buf,int buf_len,unsigned char* outbuf);
int find_start_code(unsigned char*buf);
int rtp_params_stop();
#endif