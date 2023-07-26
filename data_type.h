#ifndef __ZBW_DATA_TYPE
#define __ZBW_DATA_TYPE


typedef struct
{
    int a;
    char b;
    float c;
}data1_t;

typedef struct
{
    int pid;// 目标进程
    unsigned int addr;// 读取的地址
    void * buf_read_addr;// 用户空间读取的缓冲区指针
    int size;// 读取字节数
}memread_t;



#endif