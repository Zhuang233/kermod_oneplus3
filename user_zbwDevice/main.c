#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define DEVICE_NAME "/dev/zbwDevice"// 自定义设备名
#define MAGIC 0x5c //8bit

enum op_num{
    op1,
    op2,
    op3,
    op4
};

// 自定义设备ioctl命令
enum OPERATIONS {
    READ_PROC_MEM = _IOWR(MAGIC,op1,int),
    OP_TEST_FUN2 = _IOWR(MAGIC,op2,int),
    OP_TEST_FUN3 = _IOWR(MAGIC,op3,int),
    OP_TEST_FUN4 = _IOWR(MAGIC,op4,int),
};

typedef struct
{
    int a;
    char b;
    float c;
}data1_t;

typedef struct
{
    unsigned int len;
    data1_t data;
}origin_data_t;

origin_data_t origin_data;

int main(){
    origin_data.len = sizeof(data1_t);
    printf("size of data1_t:%d\n",origin_data.len);
    origin_data.data.a = 15527;
    origin_data.data.b = 'a';
    origin_data.data.c = 1.23;

    int fd = 0;
    fd = open(DEVICE_NAME, O_RDONLY);
    if (fd == -1) {
        printf("[-] open driver failed!!!\n");
        return 0;
    }
    else{
        printf("[+] open driver sucess\n");
    }

    if(ioctl(fd, READ_PROC_MEM, &origin_data) != 0) {
        printf("ioctl err");
        return 0;
    }

    close(fd);
}