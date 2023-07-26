#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/mm.h>
#include <linux/highmem.h>
#include <linux/pagemap.h>

#include <asm/pgtable.h>

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
    char buf[8] = { 0 };
    // struct task_struct* task_struct;
    struct pid * p_pid_struct;
    struct task_struct * p_task_struct;
    struct mm_struct * p_mm_struct;
    pgd_t * p_pgd;
    struct page * p_page_read;
    // pud_t * p_pud;

    printk(KERN_EMERG "zbw ioctl\n");
    copy_from_user(&pid_addr,arg,sizeof(struct pid_addr));
    printk(KERN_EMERG "size = %d\n",sizeof(size_t));
    printk(KERN_EMERG "pid_addr_size = %d\n",sizeof(struct pid_addr));
    printk(KERN_EMERG "rcv_pid = %d\n",pid_addr.pid);
    printk(KERN_EMERG "rcv_addr= %d\n",pid_addr.addr);

    // task_struct = get_pid_task(p1.pid[0],PIDTYPE_PID);
    // p1.pid[0] = task_struct->pid;
    // p1.pid[1] = task_struct->real_parent->pid;
    p_pid_struct = find_get_pid(pid_addr.pid);
    if(p_pid_struct == NULL){
        printk(KERN_EMERG "p_pid_struct is NULL");
        return 0;
    }
    printk(KERN_EMERG "p_pid_struct = %p\n",p_pid_struct);


    p_task_struct = pid_task(p_pid_struct,PIDTYPE_PID);
    if(p_task_struct == NULL){
        printk(KERN_EMERG "p_task_struct is NULL");
        return 0;
    }
    printk(KERN_EMERG "p_task_struct = %p\n",p_task_struct);

    p_mm_struct = get_task_mm(p_task_struct);
        if(p_mm_struct == NULL){
        printk(KERN_EMERG "p_mm_struct is NULL");
        return 0;
    }
    printk(KERN_EMERG "p_mm_struct = %p\n",p_mm_struct);

    p_pgd = p_mm_struct->pgd;
    if(p_pgd == NULL){
        printk(KERN_EMERG "p_pgd is NULL");
        return 0;
    }
    printk(KERN_EMERG "p_pgd = %p\n", p_pgd);

    printk(KERN_EMERG "addr_p_pgd = %p\n",&p_pgd);

    printk(KERN_EMERG "addr_start_code = %p\n",p_mm_struct->start_code);
    printk(KERN_EMERG "addr_end_code = %p\n",p_mm_struct->end_code);
    printk(KERN_EMERG "addr_start_data = %p\n",p_mm_struct->start_data);
    printk(KERN_EMERG "addr_end_data = %p\n",p_mm_struct->end_data);
    printk(KERN_EMERG "addr_start_brk = %p\n",p_mm_struct->start_brk);
    printk(KERN_EMERG "addr_start_code = %p\n",p_mm_struct->brk);
    printk(KERN_EMERG "addr_mmap_base= %p\n",p_mm_struct->mmap_base);
    printk(KERN_EMERG "addr_start_stack = %p\n",p_mm_struct->start_stack);


//----------------------------
    down_read(&current->mm->mmap_sem);

    get_user_pages(p_task_struct,p_mm_struct,pid_addr.addr,1,1,1,&p_page_read,NULL);
    char* addr_mapp_to_ker = kmap(p_page_read);
    if(addr_mapp_to_ker == NULL){
        printk(KERN_EMERG "addr_mapp_to_ker is NULL");
        return 0;    
    }

    printk(KERN_EMERG "read_string = %s\n", addr_mapp_to_ker);

    strcpy(addr_mapp_to_ker, "1552798761");
    page_cache_release(addr_mapp_to_ker);

    up_read(&current->mm->mmap_sem);
//---------------------------------

    // pud_offset(p_pgd,pid_addr.addr);

    copy_to_user(arg,&pid_addr,sizeof(struct pid_addr));
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


// 模块信息
MODULE_DESCRIPTION("Zbw Linux Kernel.");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ZBW");