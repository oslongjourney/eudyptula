/**
 * Task 09
 **/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sysfs.h>

static struct kobject * eudyptula_kobject;
static int value;

static ssize_t eudyptula_show(struct kobject* kobj, struct kobj_attribute* attr,
          char* buffer)
{
  return sprintf(buffer, "%d\n", value);
}

static ssize_t eudyptula_store(struct kobject* kobj,
                               struct kobj_attribute* attr,
                               const char* buffer, size_t count)
{
  sscanf(buffer, "%du", &value);
  return count;
}

static struct kobj_attribute eudyptula_attribute = __ATTR(value, 0660,
                                                          eudyptula_show,
                                                          eudyptula_store);

static int eudyptula_init_module(void)
{
   int status = 0;

    pr_debug("Eudyptula sysfs try to get alive!\n");

    eudyptula_kobject = kobject_create_and_add("eudyptula_sysfs",
                                               kernel_kobj);
    if (!eudyptula_kobject)
      return -ENOMEM;

    status = sysfs_create_file(eudyptula_kobject, &eudyptula_attribute.attr);
    if (status)
      pr_debug("Cannot create /sys/kernel/eudyptula_kobject \n");

    pr_debug("Yes! Eudyptula sysfs is alive!\n");

    return status;
}

static void eudyptula_exit_module(void)
{
  pr_debug("Yes! Eudyptula sysfs is alive!\n");
  kobject_put(eudyptula_kobject);
}

module_init(eudyptula_init_module);
module_exit(eudyptula_exit_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Siqueira");
MODULE_DESCRIPTION("Play with sysfs");
