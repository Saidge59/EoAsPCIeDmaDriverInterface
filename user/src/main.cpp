#include "DriverInterface.h"
#include <iostream>

int main()
{
    try
    {
        const char *devicePath = "/dev/my_driver";
        DriverInterface driver(devicePath);

        uint8_t bar = 0;
        uint64_t offset = 0x100;
        uint32_t value = 42;

        driver.WriteRegister(bar, offset, value);
        std::cout << "Written value: " << value << " at offset " << offset << std::endl;

        uint32_t readValue = driver.ReadRegister(bar, offset);
        std::cout << "Read value: " << readValue << " from offset " << offset << std::endl;

        GLOBAL_DATA_DMA_PARAMETERS dmaParams = {};
        GLOBAL_MEM_MAP_DATA memoryData = {};
        GLOBAL_EVENT_HANDLE_DATA eventData = {};

        driver.ReadDMAMemoryMapAndEventHandles(dmaParams, memoryData, eventData);
        std::cout << "DMA configuration received: " << dmaParams.DmaChannelsMaxCount << " channels" << std::endl;

        uint8_t channel = 0;
        bool isStartDmaChannel = true;
        bool isCycle = false;
        driver.StartStopDmaChannel(channel, isStartDmaChannel, isCycle);
        std::cout << "DMA channel " << static_cast<int>(channel) << " run" << std::endl;

        bool isStartDmaGlobal = true;
        bool isRx = true;
        driver.StartStopDmaGlobal(isStartDmaGlobal, isRx);
        std::cout << "Global DMA " << (isRx ? "receive" : "transmit") << " run" << std::endl;

        GLOBAL_START_DMA_CONFIGURATION startDmaConfig = {};
        driver.StartDmaConfigure(startDmaConfig, memoryData);
        std::cout << "DMA configured" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}