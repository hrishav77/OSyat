#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/input.h>

#define DEVICE_NAME "virtual_keypad"

static int major_number;
static struct input_dev *virt_keyboard_dev;

static ssize_t virtual_keypad_write(struct file *file, const char *buffer, size_t length, loff_t *offset)
{
    char input[length + 1];

    if (copy_from_user(input, buffer, length) != 0)
    {
        return -EFAULT;
    }
    input[length] = '\0';

    // Simulate typing based on input
    struct input_dev *dev = virt_keyboard_dev;
    int i;
    for (i = 0; i < length; i++)
    {
        input_event(dev, EV_KEY, input[i], 1); // Simulate press of the current character
        input_sync(dev);                       // Send the input event
        input_event(dev, EV_KEY, input[i], 0);
        input_sync(dev);
    }

    return length;
}

static struct file_operations fops = {
    .write = virtual_keypad_write,
};

static int __init virtual_keypad_init(void)
{
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0)
    {
        printk(KERN_ALERT "Failed to register a major number\n");
        return major_number;
    }

    // Initialize input device
    virt_keyboard_dev = input_allocate_device();
    if (!virt_keyboard_dev)
    {
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "Failed to allocate input device\n");
        return -ENOMEM;
    }

    virt_keyboard_dev->name = "Virtual Keypad";
    virt_keyboard_dev->id.bustype = BUS_USB;
    virt_keyboard_dev->id.vendor = 0x0001;
    virt_keyboard_dev->id.product = 0x0001;
    virt_keyboard_dev->id.version = 0x0100;
    set_bit(EV_KEY, virt_keyboard_dev->evbit);
    set_bit(EV_REP, virt_keyboard_dev->evbit); // Enable key repetition
    input_set_capability(virt_keyboard_dev, EV_KEY, KEY_MAX);
    input_set_all_keycodes(virt_keyboard_dev, NULL);

    if (input_register_device(virt_keyboard_dev))
    {
        input_free_device(virt_keyboard_dev);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "Failed to register input device\n");
        return -ENOMEM;
    }

    printk(KERN_INFO "Virtual keypad module loaded with major number %d\n", major_number);
    return 0;
}

static void __exit virtual_keypad_exit(void)
{
    input_unregister_device(virt_keyboard_dev);
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "Virtual keypad module unloaded\n");
}

module_init(virtual_keypad_init);
module_exit(virtual_keypad_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Virtual Keypad Module");
