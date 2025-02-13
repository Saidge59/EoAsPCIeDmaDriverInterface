#ifndef MY_DRIVER_H
#define MY_DRIVER_H

#define DEVICE_NAME "my_driver"
#define CLASS_NAME "my_driver_class"

struct my_dev
{
    dev_t devt;
    struct class *class;
    struct device *device;
    struct cdev cdev;
};

#endif // MY_DRIVER_H