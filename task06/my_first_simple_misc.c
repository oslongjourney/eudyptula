#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>

#define MODULE_NAME "eudyptula"
#define ME "siqueira"
#define BUFFER_SIZE 128

static ssize_t read_misc(struct file *filp, char __user *buff, size_t count,
			 loff_t *offp)
{
	int retval = 0;
	retval = copy_to_user(buff, ME, sizeof(ME));
	if (retval != sizeof(ME))
		pr_info("Weird, just %ld byte was copied ;(", sizeof(ME));

	return retval;
}

static ssize_t write_misc(struct file *filp, const char __user *buff,
			  size_t count, loff_t *offp)
{
	char buffer[BUFFER_SIZE];
	int retval = 0;

	retval = copy_from_user(buffer, buff, BUFFER_SIZE);
	if (retval < 0) {
		pr_info("Something happened during write :(");
		return retval;
	}

	if (strcmp(ME, buffer))
		return -EINVAL;
	else
		return 0;
}

static struct file_operations misc_char_fops = {
	.owner = THIS_MODULE,
	.read = read_misc,
	.write = write_misc,
};

static struct miscdevice misc_char;

static int eudyptula_char_misc_init(void)
{
	int retval = 0;

	misc_char.minor = MISC_DYNAMIC_MINOR;
	misc_char.name = MODULE_NAME;
	misc_char.fops = &misc_char_fops;

	retval = misc_register(&misc_char);
	if (retval) {
		pr_info("Ohhh dear! Why you failed ;(");
		return retval;
	}

	pr_info("Salut Misc %d! :)", misc_char.minor);

	return 0;
}

static void eudyptula_char_misc_release(void)
{
	misc_deregister(&misc_char);
	pr_info("Adios Eudyptula misc! I really enjoyed your company");
}

module_init(eudyptula_char_misc_init);
module_exit(eudyptula_char_misc_release);
MODULE_LICENSE("GPL");
MODULE_AUTHOR(ME);
