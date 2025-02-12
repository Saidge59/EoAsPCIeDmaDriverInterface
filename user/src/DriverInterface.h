#ifndef DRIVER_INTERFACE_H
#define DRIVER_INTERFACE_H

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <cstring>

#define DMA_IOCTL_MAGIC 0x90
#define IOCTL_SET_DMA_REG _IOW(DMA_IOCTL_MAGIC, 1, struct REGESTRY_PARAMS)
#define IOCTL_GET_DMA_REG _IOR(DMA_IOCTL_MAGIC, 2, struct REGESTRY_PARAMS)

struct REGESTRY_PARAMS {
    uint8_t bar;
    uint64_t registerOffset;
    uint32_t value;
};

class DriverInterface {
public:
    explicit DriverInterface(const char* devicePath);
    ~DriverInterface();

    bool SendIoctl(unsigned long ioctlCode, void* inBuffer, size_t inBufferSize,
                   void* outBuffer, size_t outBufferSize);

    void WriteRegister(uint8_t bar, uint64_t registerOffset, uint32_t value);

    uint32_t ReadRegister(uint8_t bar, uint64_t registerOffset);

    int GetHandle() const;

private:
    int driverHandle_ = -1;
    static std::mutex g_mutex;
};

#endif // DRIVER_INTERFACE_H
