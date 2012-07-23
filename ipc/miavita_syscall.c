#include <linux/module.h>
#include <linux/linkage.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/errno.h>
#include <linux/miavita_xtime.h>

uint64_t __miavita_elapsed_secs = 0, __miavita_elapsed_usecs = 0, __last_diff = 0;
EXPORT_SYMBOL(__miavita_elapsed_secs);
EXPORT_SYMBOL(__miavita_elapsed_usecs);

void init_miavita_xtime(void){
  struct timeval t;
  /* Get the current kernel time */
  do_gettimeofday(&t);
  printk("FIRST MICROS %ld\n", t.tv_usec);
  /* Save the current usec that have passed in the second */
  __miavita_elapsed_usecs = t.tv_usec;
  printk("STORED MICROS %lld\n", __miavita_elapsed_usecs);
}
EXPORT_SYMBOL(init_miavita_xtime);

void pulse_miavita_xtime(void){
  struct timeval t;
  struct timespec ts;
  
  do_gettimeofday(&t);


  ts.tv_sec = t.tv_sec;
  /* Reset the nsec because there is a new second */
  ts.tv_nsec = 0;
  
  do_settimeofday(&ts);
  
  __miavita_elapsed_secs++;
}
EXPORT_SYMBOL(pulse_miavita_xtime);

/*
 * Mia-Vita's syscall to know microseconds from gps.
 * This must return the seconds with microsecond precision.
 */
asmlinkage int sys_miavitasyscall(uint64_t __user * res){
  struct timeval t;
  int64_t check;
  unsigned long ret;
  do_gettimeofday(&t);
  if( ((uint64_t) t.tv_usec) < __miavita_elapsed_usecs)
    t.tv_usec += 1000000;    

  check = __miavita_elapsed_secs * 1000000 + (( (uint64_t)t.tv_usec ) - __miavita_elapsed_usecs) ;
  if(check < 0)
    printk(KERN_EMERG "Don't know how but i'm returnning a negative value %lld = %llu + (%llu - %llu)\n", check, __miavita_elapsed_secs * 1000000, (uint64_t)t.tv_usec, __miavita_elapsed_usecs);

  ret = copy_to_user(res, &check, sizeof(uint64_t));
  if(ret){
    printk(KERN_EMERG "Copy_to_user failed with %lu not copied.\n", ret);
    return -ENOMEM;
  }

  return 0;
}

asmlinkage int sys_miavitameansyscall(uint64_t __user * res){
  unsigned long ret;
  ret = copy_to_user(res, &__last_diff, sizeof(uint64_t));
  if(ret){
    printk(KERN_EMERG "Copy_to_user failed with %lu bytes not copied.\n", ret);
    return -ENOMEM;
  }

  return 0; 
}

asmlinkage int sys_miavitasetseconds(struct timeval __user * gps_time){
  struct timeval kernel_time;
  struct timespec new_kernel_time;

  do_gettimeofday(&kernel_time);
  if(((uint64_t) kernel_time.tv_usec - (uint64_t) (*gps_time).tv_usec) < 350){
    printk(KERN_DEBUG "Cannot set seconds, maximum delay not respected.\n");
    /* Indicate that the supplied data is not inside the required interval */
    return -EINVAL;
  }

  new_kernel_time.tv_sec = (*gps_time).tv_sec;
  new_kernel_time.tv_nsec = 0;
  /* Use the kernel to keep the usec for the samples up to date */
  do_settimeofday(&new_kernel_time);

  __miavita_elapsed_secs = (uint64_t) (*gps_time).tv_sec;
  return 0;
}
