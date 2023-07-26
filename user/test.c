#include<stdio.h>

int main2(){
    printf("int size: %d\n",sizeof(int));
    printf("short size: %d\n",sizeof(short));
    printf("long size: %d\n",sizeof(long));
    printf("long long size: %d\n",sizeof(long long));
    printf("float size: %d\n",sizeof(float));
    printf("double size: %d\n",sizeof(double));
    printf("char size: %d\n",sizeof(char));
    printf("void size: %d\n",sizeof(void));
    printf("unsigned char size: %d\n",sizeof(unsigned char));
    printf("unsigned int size: %d\n",sizeof(unsigned int));
    printf("unsigned long size: %d\n",sizeof(unsigned long));
    printf("unsigned long long size: %d\n",sizeof(unsigned long long));
    printf("unsigned short size: %d\n",sizeof(unsigned short));
    printf("void * size: %d\n",sizeof(void *));
    printf("char * size: %d\n",sizeof(char *));
    printf("int * size: %d\n",sizeof(int *));
    printf("long * size: %d\n",sizeof(long *));
    return 0;
}