#include<stdio.h>
#include <unistd.h>

int main2(){
    int pid = getpid();
    printf("My PID is: %d\n", pid);

    char* ptr;
    posix_memalign((void **)&ptr, 4096, 4096);
    memcpy(ptr, "zbw origin data", strlen("zbw origin data")); 
    printf("addr : %d\n",(int)ptr);
    printf("string_befor : %s\n",ptr);

    getchar();

    printf("string_after : %s\n",ptr);
    
    return 0;
}