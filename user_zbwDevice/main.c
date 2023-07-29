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
    OP_TEST_BP = _IOWR(MAGIC,op2,int),
    OP_TEST_FUN3 = _IOWR(MAGIC,op3,int),
    OP_TEST_FUN4 = _IOWR(MAGIC,op4,int),
};

typedef struct
{
    int pid;// 目标进程
    unsigned long addr;// 读取的地址
    void * buf_read_addr;// 用户空间读取的缓冲区指针
    int size;// 读取字节数
}memread_t;


int test_memory(){
    memread_t memread;
    unsigned char buf_read[1024] = {0};   

    printf("size of memread_t:%d\n",sizeof(memread));
    int fd = 0;
    fd = open(DEVICE_NAME, O_RDONLY);
    if (fd == -1) {
        printf("[-] open driver failed!!!\n");
        return 0;
    }
    else{
        printf("[+] open driver sucess\n");
    }

    memread.buf_read_addr = buf_read;

    printf("input pid addr size\n");
    scanf("%d %d %d", &memread.pid, &memread.addr, &memread.size);


    if(ioctl(fd, READ_PROC_MEM, &memread) != 0) {
        printf("ioctl err");
        return 0;
    }

    for(int i=0; i < memread.size; i++){
        printf("%x",buf_read[i]);
    }

    close(fd);
}

void test_break_point(){
    memread_t memread;
    int fd = 0;
    fd = open(DEVICE_NAME, O_RDONLY);
    if (fd == -1) {
        printf("[-] open driver failed!!!\n");
        return;
    }
    else{
        printf("[+] open driver sucess\n");
    }

    printf("input pid addr\n");
    scanf("%d %d", &memread.pid, &memread.addr);
    memread.buf_read_addr = NULL;
    memread.size = 0;

    if(ioctl(fd, OP_TEST_BP, &memread) != 0) {
        printf("ioctl err\n");
        return;
    }
    printf("regist success\n");
}

int main(){
    // test_memory();
    test_break_point();
    return 0;
}

