#include "DriverInterface.h"
#include <sys/eventfd.h>
#include <sstream>

std::mutex DriverInterface::g_mutex;

DriverInterface::DriverInterface(const char *devicePath)
{
    std::lock_guard<std::mutex> lock(g_mutex);
    driverHandle_ = open(devicePath, O_RDWR);
    if (driverHandle_ < 0)
    {
        throw std::runtime_error("Failed to open driver");
    }
}

DriverInterface::~DriverInterface()
{
    std::lock_guard<std::mutex> lock(g_mutex);
    if (driverHandle_ >= 0)
    {
        close(driverHandle_);
        driverHandle_ = -1;
    }
}

bool DriverInterface::SendIoctl(unsigned long ioctlCode, void *arg)
{
    if (driverHandle_ < 0)
    {
        throw std::runtime_error("Invalid driver handle");
    }

    return ioctl(driverHandle_, ioctlCode, arg) >= 0;
}

void DriverInterface::WriteRegister(uint8_t bar, uint64_t registerOffset, uint32_t value)
{
    REGESTRY_PARAMS info = {bar, registerOffset, value};
    if (!SendIoctl(IOCTL_SET_DMA_REG, &info))
    {
        throw std::runtime_error("Failed to write to register");
    }
}

uint32_t DriverInterface::ReadRegister(uint8_t bar, uint64_t registerOffset)
{
    REGESTRY_PARAMS info = {bar, registerOffset, 0};
    if (!SendIoctl(IOCTL_GET_DMA_REG, &info))
    {
        throw std::runtime_error("Failed to read from register");
    }
    return info.value;
}

int DriverInterface::GetHandle() const
{
    return driverHandle_;
}

void DriverInterface::ReadDMAMemoryMapAndEventHandles(GLOBAL_DATA_DMA_PARAMETERS &dmaParam, GLOBAL_MEM_MAP_DATA &memoryData, GLOBAL_EVENT_HANDLE_DATA &eventData)
{
    std::lock_guard<std::mutex> lock(g_mutex);

    auto readMemoryMap = [&]()
    {
        if (driverHandle_ < 0)
        {
            throw std::runtime_error("Invalid driver handle");
        }

        if (!SendIoctl(IOCTL_GLOBAL_DMA_CONFIGURATION_GET, &dmaParam))
        {
            throw std::runtime_error("Failed to call IOCTL_GLOBAL_DMA_CONFIGURATION_GET");
        }

        pid_t processID = getpid();
        for (uint32_t channel = 0; channel < dmaParam.DmaChannelsMaxCount; ++channel)
        {
            for (uint32_t descriptor = 0; descriptor < dmaParam.DmaDescriptorsMaxCount; ++descriptor)
            {
                std::stringstream ss;
                ss << "/dev/shm/dma_event_" << processID << "_C" << channel << "_D" << descriptor;
                std::string eventName = ss.str();

                int index = channel * dmaParam.DmaDescriptorsMaxCount + descriptor;
                sharedEventHandle_[index] = eventfd(0, EFD_NONBLOCK);
                if (sharedEventHandle_[index] == -1)
                {
                    throw std::runtime_error("Cannot create event for channel " + std::to_string(channel));
                }

                std::cout << "Created event: " << eventName << " for channel = " << channel << " and descriptor = " << descriptor << std::endl;
            }
        }

        if (!SendIoctl(IOCTL_GLOBAL_EVENT_HANDLE_SET, sharedEventHandle_))
        {
            throw std::runtime_error("Failed to call IOCTL_GLOBAL_EVENT_HANDLE_SET");
        }

        if (!SendIoctl(IOCTL_GLOBAL_EVENT_HANDLE_GET, &eventData))
        {
            throw std::runtime_error("Failed to call IOCTL_GLOBAL_EVENT_HANDLE_GET");
        }

        if (!SendIoctl(IOCTL_GLOBAL_MEM_MAP_GET, &memoryData))
        {
            throw std::runtime_error("Failed to call IOCTL_GLOBAL_MEM_MAP_GET");
        }
    };

    readMemoryMap();
}

void DriverInterface::StartStopDmaChannel(uint8_t channel, bool isStartDmaChannel, bool isCycle)
{
    std::lock_guard<std::mutex> lock(g_mutex);

    auto startStopDmaChannel = [&]()
    {
        if (driverHandle_ < 0)
        {
            throw std::runtime_error("Invalid driver handle");
        }
        if (channel >= MAX_NUM_CHANNELS)
        {
            throw std::runtime_error("Invalid channel parameter");
        }

        uint32_t DmaControlValue = isStartDmaChannel ? 0x00000003 : 0x00000000;
        if (isCycle)
        {
            DmaControlValue |= 0x00000008;
        }

        std::cout << "StartStopDmaChannel" << std::endl;
        WriteRegister(0, TransFormFpgaAddress(DEVICE_GLOBAL_DMA_REG_CONTROL) + 0x40 * channel, DmaControlValue);
    };

    startStopDmaChannel();
}

void DriverInterface::StartStopDmaGlobal(bool isStartDmaGlobal, bool isRx)
{
    std::lock_guard<std::mutex> lock(g_mutex);

    auto startStopDmaGlobal = [&]()
    {
        if (driverHandle_ < 0)
        {
            throw std::runtime_error("Invalid driver handle");
        }

        uint32_t value = isStartDmaGlobal ? 0x00000001 : 0x00000000;
        uint64_t address = isRx ? DEVICE_GLOBAL_RX_DMA_ENABLE_FPGA_DATA : DEVICE_GLOBAL_TX_DMA_ENABLE_FPGA_DATA;

        WriteRegister(0, TransFormFpgaAddress(address), value);
    };

    startStopDmaGlobal();
}

void DriverInterface::StartDmaConfigure(GLOBAL_START_DMA_CONFIGURATION &startDmaConfiguration, GLOBAL_MEM_MAP_DATA &data)
{
    std::lock_guard<std::mutex> lock(g_mutex);

    auto configurationStartDma = [&]()
    {
        if (driverHandle_ < 0)
        {
            throw std::runtime_error("Invalid driver handle");
        }

        uint32_t InterruptStatusValue = ReadRegister(0, TransFormFpgaAddress(DEVICE_GLOBAL_INTERRUPT_FPGA_STATUS));
        WriteRegister(0, TransFormFpgaAddress(DEVICE_GLOBAL_INTERRUPT_FPGA_DATA), 0x00FF);

        int NumberOfChannels = std::min(startDmaConfiguration.DmaChannelsCount, static_cast<uint32_t>(MAX_NUM_CHANNELS));
        for (int channel = 0; channel < NumberOfChannels; channel++)
        {
            int NumberOfDescriptors = std::min(startDmaConfiguration.StartDmaChannels[channel].DmaDescriptorsCount, static_cast<uint32_t>(MAX_NUM_DESCRIPTORS));
            if (NumberOfDescriptors == 0)
                continue;

            WriteRegister(0, TransFormFpgaAddress(DEVICE_GLOBAL_DMA_REG_CONTROL) + 0x40 * channel, 0x00000000);
            WriteRegister(0, TransFormFpgaAddress(DEVICE_GLOBAL_DMA_REG_DESCRIPTORS_NUMBER) + 0x40 * channel, NumberOfDescriptors);

            for (int descriptor = 0; descriptor < NumberOfDescriptors; descriptor++)
            {
                uint32_t PaLowValue = data.DataMemoryDmaChannels[channel].DmaMemoryDescriptors[descriptor].BufferPA & 0xFFFFFFFF;
                uint32_t PaHighValue = (data.DataMemoryDmaChannels[channel].DmaMemoryDescriptors[descriptor].BufferPA >> 32) & 0xFFFFFFFF;

                WriteRegister(0, TransFormFpgaAddress(DEVICE_GLOBAL_DMA_REG_DESCRIPTORS_TABLE) + 0x400 * channel + 0x10 * descriptor, PaLowValue);
                WriteRegister(0, TransFormFpgaAddress(DEVICE_GLOBAL_DMA_REG_DESCRIPTORS_TABLE) + 0x400 * channel + 0x10 * descriptor + 0x4, PaHighValue);

                uint32_t DmaBufferSize = std::min(startDmaConfiguration.StartDmaChannels[channel].StartDmaDescriptors[descriptor].DmaDescriptorBufferSize, static_cast<uint32_t>(DESCRIPTOR_BUFFER_SIZE));
                WriteRegister(0, TransFormFpgaAddress(DEVICE_GLOBAL_DMA_REG_DESCRIPTORS_TABLE) + 0x400 * channel + 0x10 * descriptor + 0x8, DmaBufferSize | (1 << 31));

                uint32_t IsDescriptorInterruptEnable = std::min(startDmaConfiguration.StartDmaChannels[channel].StartDmaDescriptors[descriptor].IsDescriptorInterruptEnable, static_cast<uint32_t>(true));
                WriteRegister(0, TransFormFpgaAddress(DEVICE_GLOBAL_DMA_REG_DESCRIPTORS_TABLE) + 0x400 * channel + 0x10 * descriptor + 0xc, IsDescriptorInterruptEnable);
            }
        }

        WriteRegister(0, TransFormFpgaAddress(DEVICE_GLOBAL_DMA_PPS_TRIGER), 0x00000000);
    };

    configurationStartDma();
}
