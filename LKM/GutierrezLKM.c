/**
 * Runs through each process greater than the input pid and displays some information about it 
 * 
 * To compile, run makefile by entering "make"
 *
 * @time : 4 
 *
 *
 * @authors: Gutierrez, Operating System Concepts - 10th Edition
 * @version 1.0 
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>

#include <linux/sched.h>
#include <linux/sched/signal.h>

static int inp_pid = 0;
char buf[TASK_COMM_LEN];

module_param(inp_pid, int, S_IRUGO);
MODULE_PARM_DESC(inp_pid, "An integer");

void displayTask(struct task_struct* t) {

    get_task_comm(buf, t);
    
    printk(KERN_INFO "PROCESS         \t PID\t STATE\t PRIO\t ST_PRIO NORM_PRIO\n");
    printk(KERN_INFO "%-16s\t %u\t %ld\t %d\t %d\t %d\n", 
	    buf, 
	    t->pid, 
	    t->state, 
	    t->prio, 
	    t->static_prio, 
	    t->normal_prio);
    printk(KERN_INFO);
}

/* This function is called when the module is loaded. */
int lkm_init(void)
{

    struct task_struct* task;
    struct 

    for_each_process(task) {
    	if (task->pid > inp_pid) {
    		displayTask(task);
    	}
    }
    
    return 0;
}

/* This function is called when the module is removed. */
void lkm_exit(void) {
	printk(KERN_INFO "Removing Module\n");
}

/* Macros for registering module entry and exit points. */
module_init( lkm_init );
module_exit( lkm_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LKM Module");
MODULE_AUTHOR("Gutierrez");

