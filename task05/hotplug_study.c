#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/hid.h>

static int hotplug_study_probe(struct usb_interface * interface,
                               const struct usb_device_id * id)
{
  pr_debug("USB device connected");
  return 0;
}

static void hotplug_study_disconnect(struct usb_interface * interface)
{
  pr_debug("USB device disconnected");
}

static struct usb_device_id hotplug_study_table [] = {
  {USB_INTERFACE_INFO
	 (USB_INTERFACE_CLASS_HID, USB_INTERFACE_SUBCLASS_BOOT,
	  USB_INTERFACE_PROTOCOL_MOUSE)},
	{},
};
MODULE_DEVICE_TABLE(usb, hotplug_study_table);

static struct usb_driver hotplug_study = {
  .name = "hotplug_study",
  .probe = hotplug_study_probe,
  .disconnect = hotplug_study_disconnect,
  .id_table = hotplug_study_table,
};

static int hotplug_initialize(void)
{
  int result = 0;

  result = usb_register(&hotplug_study);
  if (result < 0)
  {
    pr_err("usb_register failed for the " __FILE__ "driver. Error number %d, ",
            result);
    return -1;
  }

  pr_debug("Eudyptula hotplug inserted\n");
  return 0;
}

static void hotplug_cleanup(void)
{
  usb_deregister(&hotplug_study);
  pr_debug("Eudyptula hotplug removed\n");
}

module_init(hotplug_initialize);
module_exit(hotplug_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Siqueira");
MODULE_DESCRIPTION("Play with hotplug devices");
