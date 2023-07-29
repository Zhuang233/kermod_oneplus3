#include "breakpoint.h"
#include <linux/hw_breakpoint.h>
#include <linux/uaccess.h>
#include "data_type.h"
#include <linux/pid.h>
#include <linux/perf_event.h>


struct perf_event_attr attr_bp;

void perf_overflow_callback(struct perf_event *perf_event, struct perf_sample_data *perf_sample_data, struct pt_regs *regs){
    printk(KERN_EMERG "touch zbw breakpoint\n");
}

void test_break_point(unsigned long addr){
    memread_t memread;
    struct perf_event* perf;

    
    copy_from_user(&memread,addr,sizeof(memread_t));
    printk(KERN_ALERT "memread :%d %p %d %p",memread.pid, memread.addr, memread.size, memread.buf_read_addr);

    //1.get mm_struct
    struct pid *p_pid_struct = find_get_pid(memread.pid);
    if(p_pid_struct == NULL){
        printk(KERN_EMERG "p_pid_struct is NULL");
        return;
    }

    struct task_struct *p_task_struct = get_pid_task(p_pid_struct, PIDTYPE_PID);
    if(p_task_struct == NULL){
        printk(KERN_EMERG "p_task_struct is NULL");
        return;
    }
    printk(KERN_EMERG "p_task_struct = %p\n",p_task_struct);


    //2.config breakpoint attribute
    memset(&attr_bp,0,sizeof(attr_bp));
    
    attr_bp.type = PERF_TYPE_BREAKPOINT;
    attr_bp.size = sizeof(attr_bp);
    attr_bp.pinned = 1;
    attr_bp.sample_period = 1;
    attr_bp.exclude_kernel = 1;
    attr_bp.bp_len = HW_BREAKPOINT_LEN_4;
    attr_bp.bp_type = HW_BREAKPOINT_RW;
    attr_bp.bp_addr = memread.addr;
    attr_bp.disabled = 0;
    

    //3.register breakpoint
    // struct perf_event *sample_hbp = register_user_hw_breakpoint(&attr_bp, perf_overflow_callback, NULL, p_task_struct);
    struct perf_event *sample_hbp = perf_event_create_kernel_counter(&attr_bp, -1, p_task_struct, perf_overflow_callback, NULL);
    if (IS_ERR((void __force *)sample_hbp)) {
        int ret = PTR_ERR((void __force *)sample_hbp);
        printk(KERN_INFO "register_user_hw_breakpoint failed: %d\n", ret);
        return;
    }
    else{
        printk(KERN_INFO "register_user_hw_breakpoint sucess %p", sample_hbp);
    }

}