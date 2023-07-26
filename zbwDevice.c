#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

#include "memory.h"

#define DEVICE_NAME "zbwDevice"// 自定义设备名
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

// 设备open函数(空实现)
int open_call_back(struct inode *node, struct file *file)
{
    return 0;
}

// 设备close函数(空实现)
int close_call_back(struct inode *node, struct file *file)
{
    return 0;
}

// 设备ioctl函数(接收指定命令)
long ioctl_call_back(struct file* const file, unsigned int const cmd, unsigned long const arg)
{
    switch (cmd)
    {
    case READ_PROC_MEM:
        read_proc_mem(arg);
        break;
    
    default:
        break;
    }
    
    return 0;
}

// 设备操作函数结构体，参考：https://blog.csdn.net/zqixiao_09/article/details/50850475
struct file_operations file_functions = {
    .owner   = THIS_MODULE,
    .open    = open_call_back,
    .release = close_call_back,
    .unlocked_ioctl = ioctl_call_back,
};

// 杂项设备结构体，可参考：https://blog.csdn.net/qq_20553613/article/details/103285204
struct miscdevice misc = {
    // 次设备号
	.minor = MISC_DYNAMIC_MINOR,
    // 设备名
	.name = DEVICE_NAME,
    // 设备文件操作接口
	.fops = &file_functions,
};

static int __init zbw_init(void){
    printk(KERN_ALERT "hello zbw module\n");
    return misc_register(&misc);
}

static void __exit zbw_exit(void){
    printk(KERN_ALERT "good bye zbw module\n");
    // 卸载杂项设备，参考同注册
	misc_deregister(&misc);
}

module_init(zbw_init);
module_exit(zbw_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kernel Memory Module");
MODULE_AUTHOR("ZBW");