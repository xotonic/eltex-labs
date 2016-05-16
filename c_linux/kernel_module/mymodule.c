//use dev(?) proc sys ioctl
/*
  root# insmod mymodule.ko
  root# dmesg | tail -1
  root# rmmod mymodule.ko
  root# dmesg | tail -1
*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dmitry Kuzmin");
MODULE_DESCRIPTION("Calculating average of float array");

static int __init my_init(void)
{
  printk(KERN_DEBUG "-= My module initialized =-\n");
  return 0;
}

static void __exit my_exit(void)
{
  printk(KERN_DEBUG "-= My module exit =-\n");
}

module_init(my_init);
module_exit(my_exit);
