/**
 * Task 00
 **/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

/**
 * Modules need to be initialized, and here is the initialization for this
 * module. Notice:
 * 1. It is expected to initialize stuffs here, as register or start a device
 * 2. The return is important, 0 means everything OK
 * 3. The signature is mandatory
 * 4. In the beginning this function name was: init_module
 * 5. Finally, printk put a log information
 * Signature:
 *  int [WHATEVER_NAME_YOU_WANT] (void)
 **/
static int hello_init_module(void)
{
  printk(KERN_ALERT "Salut, Mundi! Oopss... Hello World!\n");
  return 0;
}

/**
* After the job, the module need to be removed from the Kernel. Notice:
* 1. Before leave, free stuff, stop things, etc...
* Signature:
*  void [WHATEVER_NAME_YOU_WANT] (void)
***/
static void hello_exit_module(void)
{
  printk(KERN_ALERT "It is time... good bye!\n");
}

/**
 * Here, these guys control the party. Basically, init register a new function
 * member to initialize device and exit register the cleanup function
 **/
module_init(hello_init_module);
module_exit(hello_exit_module);

/**
 * Here we can find some meta informations about the module.
 **/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("mE");
MODULE_DESCRIPTION("Hey, this is my first module toy :)");
