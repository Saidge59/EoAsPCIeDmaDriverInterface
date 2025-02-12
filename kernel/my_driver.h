#ifndef MY_DRIVER_H
#define MY_DRIVER_H

#define DEVICE_NAME "my_driver"
#define CLASS_NAME "my_driver_class"
#define DMA_IOCTL_MAGIC 0x90
#define IOCTL_SET_DMA_REG _IOW(DMA_IOCTL_MAGIC, 1, struct reg_params)
#define IOCTL_GET_DMA_REG _IOR(DMA_IOCTL_MAGIC, 2, struct reg_params)

struct reg_params 
{
    uint8_t bar;
    uint64_t registerOffset;
    uint32_t value;
};

struct my_dev 
{
    dev_t devt;
    struct class *class;
    struct device *device;
    struct cdev cdev;
};

#endif //MY_DRIVER_H