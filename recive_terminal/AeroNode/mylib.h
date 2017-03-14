 #ifdef _cplusplus 
 extern "C"  
    {  
    #endif  
    int rtp_params_init();
    void rtp_send(unsigned char *buf, int len);
    void nal_rtp_send(unsigned char* buf, int len);
    int get_next_nalu(unsigned char* buf,int buf_len,unsigned char* outbuf);
    int find_start_code(unsigned char*buf);
    void checkerror(int err);
    #ifdef _cplusplus  
    }  
    #endif  