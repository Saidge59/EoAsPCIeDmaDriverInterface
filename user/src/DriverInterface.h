#ifndef DRIVER_INTERFACE_H
#define DRIVER_INTERFACE_H

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <cstring>
#include "Public.h"

class DriverInterface
{
public:
    explicit DriverInterface(const char *devicePath);
    ~DriverInterface();

    bool SendIoctl(unsigned long ioctlCode, void *inBuffer);

    void WriteRegister(uint8_t bar, uint64_t registerOffset, uint32_t value);

    uint32_t ReadRegister(uint8_t bar, uint64_t registerOffset);

    int GetHandle() const;

    void ReadDMAMemoryMapAndEventHandles(GLOBAL_DATA_DMA_PARAMETERS &dmaParam, GLOBAL_MEM_MAP_DATA &memoryData, GLOBAL_EVENT_HANDLE_DATA &eventData);
    void StartStopDmaChannel(uint8_t channel, bool isStartDmaChannel, bool isCycle);
    void StartStopDmaGlobal(bool isStartDmaGlobal, bool isRx);
    void StartDmaConfigure(GLOBAL_START_DMA_CONFIGURATION &startDmaConfiguration, GLOBAL_MEM_MAP_DATA &data);

private:
    int driverHandle_ = -1;
    int sharedEventHandle_[MAX_NUM_CHANNELS * MAX_NUM_DESCRIPTORS];
    static std::mutex g_mutex;
};

#endif // DRIVER_INTERFACE_H
