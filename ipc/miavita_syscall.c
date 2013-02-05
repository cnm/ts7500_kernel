#include <linux/module.h>
#include <linux/linkage.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/errno.h>
#include <linux/miavita_xtime.h>

uint64_t __miavita_elapsed_secs = 0, __miavita_elapsed_usecs = 0, __last_diff = 0;
EXPORT_SYMBOL(__miavita_elapsed_secs);
EXPORT_SYMBOL(__miavita_elapsed_usecs);

/**
 * @brief Initiates the miavita sec counter to the seconds in the kernel time
 * (not to be used by miavita)
 */
void init_miavita_xtime(void){
    struct timeval t;
    do_gettimeofday(&t);
    printk("FIRST MICROS %ld\n", t.tv_usec);
    __miavita_elapsed_usecs = t.tv_usec;
    printk("STORED MICROS %lld\n", __miavita_elapsed_usecs);
}
EXPORT_SYMBOL(init_miavita_xtime);

/**
 * @brief Adds one second to the mia vita seconds counter
 * To be called each time one receives a PPS interruption
 */
void pulse_miavita_xtime(void){
    struct timeval t;
    uint64_t temp_t;

    // Get's present kernel time
    do_gettimeofday(&t);

    /* If it is not the first second */
    if(__miavita_elapsed_secs != 0)
      {
        temp_t = t.tv_usec;

        /* Let's see if it was less than a second (accourding to the kernel) */
        if(temp_t < __miavita_elapsed_usecs)
          {
            temp_t += 1000000;                            /* Adds a second */
          }
        __last_diff = (temp_t - __miavita_elapsed_usecs); /* Stores the difference from the kernel usecs to miavita */
      }

    __miavita_elapsed_usecs = t.tv_usec;                  /* Mia Vita usecs set to kernel usecs */
    __miavita_elapsed_secs++;                             /* Increments a miavita sec           */
}
EXPORT_SYMBOL(pulse_miavita_xtime);                       /* Exporting symbol in the kernel     */

/**
 * @brief
 * Mia-Vita's syscall to know microseconds from gps.
 * This must return the seconds with microsecond precision.
 *
 * @param res
 *
 * @return 0 if OK, -ENOMEM if problem in copying result
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
    if(ret)
      {
        printk(KERN_EMERG "Copy_to_user failed with %lu not copied.\n", ret);
        return -ENOMEM;
      }

    return 0;
}

/**
 * @brief TODO - Is this called anywhere??
 *
 * @param res
 *
 * @return
 */
asmlinkage int sys_miavitameansyscall(uint64_t __user * res){
    unsigned long ret;
    ret = copy_to_user(res, &__last_diff, sizeof(uint64_t));
    if(ret)
      {
        printk(KERN_EMERG "Copy_to_user failed with %lu bytes not copied.\n", ret);
        return -ENOMEM;
      }

    return 0;
}


/**
 * @brief Sets the seconds in the miavita seconds variable
 * (TODO - Shouldn't this check if it is at the beginning of the seconds)
 *
 * @param secs to set
 *
 * @return Always 0
 */
asmlinkage int sys_miavitasetseconds(uint64_t secs){
    __miavita_elapsed_secs = secs;
    return 0;
}
