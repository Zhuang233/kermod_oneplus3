#include "memory.h"
#include <linux/uaccess.h>
#include <linux/slab.h>



int read_proc_mem(unsigned long addr){
    // 获取结构体大小
    unsigned int struct_len = 0;
    data1_t data_test;

    union {
    float f;
    unsigned int u;
    } float_to_int;
    
    copy_from_user(&struct_len,addr,sizeof(unsigned int));
    printk(KERN_EMERG "struct_len = %d\n",struct_len);

    copy_from_user(&data_test,addr+sizeof(unsigned int),sizeof(data1_t));

    printk(KERN_EMERG "a = %d\n",data_test.a);
    printk(KERN_EMERG "b = %c\n",data_test.b);

    float_to_int.f = data_test.c;

    printk(KERN_EMERG "c = %x\n",float_to_int.u);

    

    return 0;
}