#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/highmem.h>
#include <linux/uaccess.h>

// static int pid_to_read = 0x1234; // 要读取的进程的 PID
// static unsigned long user_addr = 0x5678; // 要读取的用户空间地址
#define DEVICE_NAME "ZbwDevice"// 自定义设备名
// 自定义设备ioctl命令
enum OPERATIONS {
    OP_TEST_FUN1 = 0x800,
    OP_TEST_FUN2 = 0x801,
    OP_TEST_FUN3 = 0x802,
    OP_TEST_FUN4 = 0x803,
};

struct pid_addr
{
    int pid;
    void* addr;
    int test;
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
    struct pid_addr pid_addr; 
    struct task_struct *target_task;
    struct page *page;
    unsigned long kernel_addr;
    void *ptr;

    copy_from_user(&pid_addr,arg,sizeof(struct pid_addr));

    // 获取要读取的进程的 task_struct
    rcu_read_lock();
    target_task = pid_task(find_vpid(pid_addr.pid), PIDTYPE_PID);
    rcu_read_unlock();

    if (!target_task) {
        printk(KERN_INFO "Cannot find the target process.\n");
        return -EINVAL;
    }

    // 获取用户空间地址对应的 page
    page = get_user_pages(target_task, target_task->mm, pid_addr.addr, 1, 1, 0, NULL, NULL);
    if (IS_ERR(page)) {
        printk(KERN_INFO "Failed to get user page.\n");
        return PTR_ERR(page);
    }

    // 将 page 映射到内核空间
    kernel_addr = (unsigned long)kmap(page);
    if (!kernel_addr) {
        printk(KERN_INFO "Failed to map the page.\n");
        put_page(page);
        return -ENOMEM;
    }

    // 在内核空间中访问用户空间内存
    ptr = (void *)(kernel_addr + ((int)pid_addr.addr & (PAGE_SIZE - 1)));
    printk(KERN_INFO "Value at user address 0x%lx: %p\n", pid_addr.addr, *((unsigned long *)ptr));

    // 解除映射并释放 page
    kunmap(page);
    put_page(page);

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

static int __init hello_init(void){
    printk(KERN_ALERT "hello zbw module\n");
    return misc_register(&misc);
}

static void __exit hello_exit(void){
    printk(KERN_ALERT "good bye zbw module\n");
    // 卸载杂项设备，参考同注册
	misc_deregister(&misc);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Read User Memory Kernel Module");
MODULE_AUTHOR("Your Name");