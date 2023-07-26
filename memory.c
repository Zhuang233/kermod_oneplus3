#include "memory.h"
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/pid.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/highmem.h>


int read_proc_mem(unsigned long addr){
    memread_t memread;
    struct pid * p_pid_struct;
    struct task_struct * p_task_struct;
    struct mm_struct * p_mm_struct;
    struct page * p_page_read;
    void * map_addr_real;
    unsigned long offest = 0;
    copy_from_user(&memread,addr,sizeof(memread_t));
    printk(KERN_ALERT "memread :%d %p %d %p",memread.pid, memread.addr, memread.size, memread.buf_read_addr);

    p_pid_struct = find_get_pid(memread.pid);
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

    get_user_pages(p_task_struct, p_mm_struct, memread.addr, 1, 1, 1, &p_page_read, NULL);

    void* addr_mapp_to_ker = kmap(p_page_read);
    if(addr_mapp_to_ker == NULL){
        printk(KERN_EMERG "addr_mapp_to_ker is NULL");
        return 0;    
    }

    offest = (unsigned long)(memread.addr&(PAGE_SIZE-1));
    if(offest + memread.size >= PAGE_SIZE){
        printk(KERN_EMERG "offest too high : %x", offest);
        return 0;
    }
    map_addr_real = (void *)((unsigned long)addr_mapp_to_ker + offest);
    copy_to_user(memread.buf_read_addr, map_addr_real, memread.size);
    printk(KERN_EMERG "copy_to_user success!!");
    return 0;
}