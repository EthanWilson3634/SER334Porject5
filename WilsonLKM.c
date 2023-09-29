#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/sched/signal.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ethan Wilson");
MODULE_DESCRIPTION("This module displays information about every running proccesses with PID greater than user input");
MODULE_VERSION("1.6");

// This variable allows for tracking the version while debugging.
static const char* module_version = "1.6";

static int inp_pid = 0; // Default value
module_param(inp_pid, int, S_IRUSR | S_IWUSR); // Accept input through command line

int init_module(void){
	/************************************************
	* Display following information for a process:	*
	*   PROCESS NAME				*
	*   PID						*
	*   STATE					*
	*   PRIORITY					*
	*   STATIC-PRIORITY				*
	*   NORMAL-PRIORITY				*
	*************************************************/

	printk(KERN_INFO "Loading WilsonLKM v%s\n\n", module_version);
	struct task_struct* task;
	for_each_process(task){
		if(task->pid > inp_pid){
	        	printk(KERN_INFO " %-15s %-10s %-10s %-7s %-8s %-8s\n", "PROCESS", "PID", "STATE", "PRIO", "ST_PRIO", "NORM_PRIO");
		        printk(KERN_INFO " %-15s %-10d %-10d %-7d %-8d %-8d\n", task->comm, task->pid, task->__state, task->prio, task->static_prio, task->normal_prio);
				struct task_struct* child_task;
			list_for_each_entry(child_task, &task->children, sibling){ // list_for_each_entry list.h line 673
				printk(KERN_INFO " %-15s\n", "CHILD");
	                        printk(KERN_INFO " %-15s %-10d %-10d %-7d %-8d %-8d\n",
					child_task->comm, child_task->pid, child_task->__state, child_task->prio, child_task->static_prio, child_task->normal_prio);
			}

			if(task->parent){
				printk(KERN_INFO " %-15s\n", "PARENT");
	             		printk(KERN_INFO " %-15s %-10d %-10d %-7d %-8d %-8d\n",
					task->parent->comm, task->parent->pid, task->parent->__state, task->parent->prio, task->parent->static_prio, task->parent->normal_prio);
			}
			printk(KERN_INFO "\n");
		}
	}

        /*
         * A non 0 return means init_module failed; module can't be loaded.
         */
        return 0;
}

void cleanup_module(void){
	printk(KERN_INFO "Removing Module\n");
}
