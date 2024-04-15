#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "virtual_keypad"

static int major_number;

static ssize_t virtual_keypad_write(struct file *file, const char *buffer, size_t length, loff_t *offset)
{
    // Process the input from userspace here
    char input[length];
    if (copy_from_user(input, buffer, length) != 0) {
        return -EFAULT;
    }
    input[length] = '\0'; // Ensure null-terminated string
    printk(KERN_INFO "Received input: %s\n", input);
    return length;
}

static struct file_operations fops = {
    .write = virtual_keypad_write,
};

static int __init virtual_keypad_init(void)
{
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "Failed to register a major number\n");
        return major_number;
    }
    printk(KERN_INFO "Virtual keypad module loaded with major number %d\n", major_number);
    return 0;
}

static void __exit virtual_keypad_exit(void)
{
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "Virtual keypad module unloaded\n");
}

module_init(virtual_keypad_init);
module_exit(virtual_keypad_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Virtual Keypad Module");
