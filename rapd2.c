#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>

#include "rapd2_ioctl.h"

#define INFO KERN_INFO "rapd2: "

static dev_t dev;
static struct cdev c_dev;
static struct class *cl;

static int my_open(struct inode *i, struct file *f) { return 0; }

static int my_close(struct inode *i, struct file *f) { return 0; }

static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg) {
  struct rapd2_msg msg;

  switch (cmd) {
  case RAPD2_SEND_MSG:
    if (copy_from_user(&msg, (struct rapd2_msg *)arg,
                       sizeof(struct rapd2_msg))) {
      return -EACCES;
    }

    msg.buffer[3] = 'A';

    printk(INFO "msg.length: %d\n", msg.length);
    printk(INFO "msg.buffer: %s\n", msg.buffer);

    if (copy_to_user((struct rapd2_msg *)arg, &msg, sizeof(struct rapd2_msg))) {
      return -EACCES;
    }
  }

  return 0;
}

static struct file_operations rapd2_fops = {.owner = THIS_MODULE,
                                            .open = my_open,
                                            .release = my_close,
                                            .unlocked_ioctl = my_ioctl};

static int __init rapd2_ioctl_init(void) {
  int ret;
  struct device *dev_ret;

  if ((ret = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "rapd2_ioctl")) <
      0) {
    return ret;
  }

  cdev_init(&c_dev, &rapd2_fops);

  if ((ret = cdev_add(&c_dev, dev, MINOR_CNT)) < 0) {
    return ret;
  }

  if (IS_ERR(cl = class_create(THIS_MODULE, "rapd2_char"))) {
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);
    return PTR_ERR(cl);
  }
  if (IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "rapd2"))) {
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);
    return PTR_ERR(dev_ret);
  }

  printk(INFO "Loaded\n");
  return 0;
}

static void __exit rapd2_ioctl_exit(void) {
  device_destroy(cl, dev);
  class_destroy(cl);
  cdev_del(&c_dev);
  unregister_chrdev_region(dev, MINOR_CNT);
  printk(INFO "Unloaded\n");
}

module_init(rapd2_ioctl_init);
module_exit(rapd2_ioctl_exit);

MODULE_LICENSE("GPL");
