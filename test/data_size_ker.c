#include <linux/init.h>
#include <linux/module.h>
    

static void __init data_size_init(){
    printk(KERN_ALERT "int size: %d\n",sizeof(int));
    printk(KERN_ALERT "short size: %d\n",sizeof(short));
    printk(KERN_ALERT "long size: %d\n",sizeof(long));
    printk(KERN_ALERT "long long size: %d\n",sizeof(long long));
    printk(KERN_ALERT "float size: %d\n",sizeof(float));
    printk(KERN_ALERT "double size: %d\n",sizeof(double));
    printk(KERN_ALERT "char size: %d\n",sizeof(char));
    printk(KERN_ALERT "void size: %d\n",sizeof(void));
    printk(KERN_ALERT "unsigned char size: %d\n",sizeof(unsigned char));
    printk(KERN_ALERT "unsigned int size: %d\n",sizeof(unsigned int));
    printk(KERN_ALERT "unsigned long size: %d\n",sizeof(unsigned long));
    printk(KERN_ALERT "unsigned long long size: %d\n",sizeof(unsigned long long));
    printk(KERN_ALERT "unsigned short size: %d\n",sizeof(unsigned short));
    printk(KERN_ALERT "void * size: %d\n",sizeof(void *));
    printk(KERN_ALERT "char * size: %d\n",sizeof(char *));
    printk(KERN_ALERT "int * size: %d\n",sizeof(int *));
    printk(KERN_ALERT "long * size: %d\n",sizeof(long *));
}
static void __exit data_size_exit(){
    return 0;
}




module_init(data_size_init);
module_exit(data_size_exit);


// 模块信息
MODULE_DESCRIPTION("Zbw Linux Kernel.");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ZBW");