# Task 06 description

----

Nice job with the module loading macros, those are tricky, but a very
valuable skill to know about, especially when running across them in
real kernel code.

Speaking of real kernel code, let's write some!

The task this time is this:
  - Take the kernel module you wrote for task 01, and modify it to be a
    misc char device driver.  The misc interface is a very simple way to
    be able to create a character device, without having to worry about
    all of the sysfs and character device registration mess.  And what a
    mess it is, so stick to the simple interfaces wherever possible.
  - The misc device should be created with a dynamic minor number, no
    need running off and trying to reserve a real minor number for your
    test module, that would be crazy.
  - The misc device should implement the read and write functions.
  - The misc device node should show up in /dev/eudyptula.
  - When the character device node is read from, your assigned id is
    returned to the caller.
  - When the character device node is written to, the data sent to the
    kernel needs to be checked.  If it matches your assigned id, then
    return a correct write return value.  If the value does not match
    your assigned id, return the "invalid value" error value.
  - The misc device should be registered when your module is loaded, and
    unregistered when it is unloaded.
  - Provide some "proof" this all works properly.

----

# My full answer

## Overview

There is a different kind of device drivers (DD) in the kernel. Usually, three
main categories are commonly cited: character, block, and network. Typically,
devices that require byte stream manipulation utilises Character DD. On the
other hand, devices that need manipulating a block of data with the requirement
to move forward and backward can uses Block DD. Finally, Network DD implement
the required manipulation for network devices.

In general, all DD has three shared steps:
1. Initializing/Freeing device resources: this sort of operation is better
summarized by `module_init` and `module_exit` (already described in
task 01);
2. Allocating/Freeing device registration: all DD requires some identification
in the system, and the mechanism for doing it is the MAJOR and MINOR number;
3. Fill out file_operations struct: All DD has a set of specific operations,
that is accesses for applications in user space via System Call (open, read,
write, etc.).

In this task, we work with another specific type of DD name misc char DD. Misc
char is very similar to character DD, with the exception that registration is
more straightforward than character DD.

## Basic of Device Driver

In this section, we expand the three steps described in the previous section to
provide an overview of DD.

## About misc

Miscellaneous Character Drivers is similar to a character DD, with a small
exception: the process of register a new device is simplified.

## Initializing/Freeing device resources

`module_init` macro orchestrates the first contact of the DD with the Kernel.
For doing the communication, it expects a function with the following
signature:

```
int name (void);
```

In the function definition, it is essential to do the required hardware
initialisation for putting the device in the correct state. In this function,
it is necessary to register the MAJOR and MINOR number. However, the
registration should be the last step of this function.

There is a kernel System Call that supports the process of load the module into
the memory.

Finally, there is a cleanup macro named `module_exit` which is responsible for
deallocating and undo anything made inside `module_init`. Follows the expected
signature for exit macro.

```
void name (void);
```

## Allocating/Freeing device registration

Register a device is based on the MAJOR and MINOR number. Former is responsible
for global identification of the Device in the system, and the kernel manages
it. The MINOR number identifies a different device related to the same MAJOR.
The DD has to obtain a MAJOR number, which could be statically acquired or
dynamically. Currently, the standard approach it is dynamically acquired by the
function:

```
void alloc_chrdev_region(dev_t *dev, unsigned int firstminor, unsigned int count, char *name);
```

Every time you allocate a MAJOR, you have to release it when you finish your
task with the function:

```
void unregister_chrdev_region(dev_t *dev, unsigned int count);
```

So, all the above approach represents the basic steps of common character DD
registration. On misc char DD, it is not required to handle all the stuff
related to DD identification. First, all the misc DD have a major number of 10
which requires the management of the minor number only. To do it, there is two
functions:

```
int misc_register(struct miscdevice * misc);
int misc_deregister(struct miscdevice * misc);
```

Finally, there is a simple data structure to be filled in the module_init function

```
struct miscdevice {
        int minor;
        const char *name;
        struct file_operations *fops;
        struct miscdevice *next, *prev;
};
```

# Reference

* [Linux Journal](http://www.linuxjournal.com/article/2920)
* [Linux Device Drivers, Third Edition - Chapter 3](https://lwn.net/Kernel/LDD3/)
