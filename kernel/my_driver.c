#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/ioctl.h>
#include "my_driver.h"

static struct my_dev *my_dev;

static int my_driver_open(struct inode *inodep, struct file *filep) 
{
    pr_info("my_driver: Device opened\n");
    return 0;
}

static int my_driver_release(struct inode *inodep, struct file *filep) 
{
    pr_info("my_driver: Device closed\n");
    return 0;
}

static long my_driver_ioctl(struct file *filep, unsigned int cmd, unsigned long arg) 
{
    struct reg_params user_params;
    
    if (copy_from_user(&user_params, (struct reg_params *)arg, sizeof(struct reg_params))) 
    {
        return -EFAULT;
    }

    switch (cmd) 
    {
        case IOCTL_SET_DMA_REG:
            pr_info("my_driver: Writing value %u to BAR %u, offset %llu\n",
                   user_params.value, user_params.bar, user_params.registerOffset);
            break;
        
        case IOCTL_GET_DMA_REG:
            user_params.value = 1234; // Simulate reading hardware register
            if (copy_to_user((struct reg_params *)arg, &user_params, sizeof(struct reg_params))) 
            {
                return -EFAULT;
            }
            pr_info("my_driver: Read value %u from BAR %u, offset %llu\n",
                   user_params.value, user_params.bar, user_params.registerOffset);
            break;

        default:
            pr_info("IOCTL: default");
            return -EINVAL;
    }

    return 0;
}

static struct file_operations fops = {
    .open = my_driver_open,
    .release = my_driver_release,
    .unlocked_ioctl = my_driver_ioctl,
};

static int __init my_driver_init(void) 
{
    int ret;

    pr_info("%s: Initializing module\n", DEVICE_NAME);

    my_dev = kzalloc(sizeof(struct my_dev), GFP_KERNEL);
    if (!my_dev) {
        pr_err("%s: Failed to allocate memory for device\n", DEVICE_NAME);
        return -ENOMEM;
    }

    // Select the area for the device
    ret = alloc_chrdev_region(&my_dev->devt, 0, 1, DEVICE_NAME);
    if (ret) {
        pr_err("%s: Failed to allocate chrdev region\n", DEVICE_NAME);
        goto err_free_dev;
    }

    // Initialize character device
    cdev_init(&my_dev->cdev, &fops);
    my_dev->cdev.owner = THIS_MODULE;
    ret = cdev_add(&my_dev->cdev, my_dev->devt, 1);
    if (ret) {
        pr_err("%s: Failed to add cdev\n", DEVICE_NAME);
        goto err_unreg_chrdev;
    }

    // Create device class
    my_dev->class = class_create(DEVICE_NAME);
    if (IS_ERR(my_dev->class)) {
        pr_err("%s: Failed to create class\n", DEVICE_NAME);
        ret = PTR_ERR(my_dev->class);
        goto err_del_cdev;
    }

    // Create device
    my_dev->device = device_create(my_dev->class, NULL, my_dev->devt, NULL, DEVICE_NAME);
    if (IS_ERR(my_dev->device)) {
        pr_err("%s: Failed to create device\n", DEVICE_NAME);
        ret = PTR_ERR(my_dev->device);
        goto err_destroy_class;
    }

    pr_info("%s: Module loaded successfully\n", DEVICE_NAME);
    return 0;

err_destroy_class:
    class_destroy(my_dev->class);
err_del_cdev:
    cdev_del(&my_dev->cdev);
err_unreg_chrdev:
    unregister_chrdev_region(my_dev->devt, 1);
err_free_dev:
    kfree(my_dev);
    return ret;
}

static void __exit my_driver_exit(void) 
{
    device_destroy(my_dev->class, my_dev->devt);
    class_destroy(my_dev->class);
    cdev_del(&my_dev->cdev);
    unregister_chrdev_region(my_dev->devt, 1);
    kfree(my_dev);
    pr_info("%s: Module unloaded successfully\n", DEVICE_NAME);
}

module_init(my_driver_init);
module_exit(my_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aplit Soft Ltd.");
MODULE_DESCRIPTION("Linux kernel driver");
MODULE_VERSION("1.0");
