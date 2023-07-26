#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define DEVICE_NAME "/dev/ZbwDevice"// 自定义设备名

enum OPERATIONS {
    OP_TEST_PID = 0x800,
    OP_TEST_FUN2 = 0x801,
    OP_TEST_FUN3 = 0x802,
    OP_TEST_FUN4 = 0x803,
};

struct pid_addr
{
    __uint32_t pid;
    void* pid_struct_addr;
    __uint32_t test;
};

int pid = -1;
struct pid_addr pid_addr;


int main3(){
    // char string_buf[1024] = "zbw origin data";
    char* ptr;
    posix_memalign((void **)&ptr, 4096, 4096);
    memcpy(ptr, "zbw origin data", strlen("zbw origin data")); 

    int fd = 0;
    printf("size = %d\n",sizeof(size_t));
    printf("pid_addr_size = %d\n",sizeof(struct pid_addr));
    fd = open(DEVICE_NAME, O_RDONLY);
    if (fd == -1) {
        printf("[-] open driver failed!!!\n");
        return 0;
    }
    else{
        printf("[+] open driver sucess\n");
    }

    printf("PID ADDR:");
    scanf("%d %d",&pid,&ptr);
    pid_addr.pid = pid;
    pid_addr.test = 431847;
    pid_addr.pid_struct_addr = ptr;

    if(ioctl(fd, OP_TEST_FUN4, &pid_addr) != 0) {
        return 0;
    }

    printf("pid = %d\n",pid_addr.pid);
    printf("addr = %d\n",pid_addr.pid_struct_addr);
    printf("test = %d\n",pid_addr.test);

    // printf("string changed to : %s\n",ptr);

    close(fd);
}