#include "DriverInterface.h"

std::mutex DriverInterface::g_mutex;

DriverInterface::DriverInterface(const char* devicePath) {
    std::lock_guard<std::mutex> lock(g_mutex);
    driverHandle_ = open(devicePath, O_RDWR);
    if (driverHandle_ < 0) {
        throw std::runtime_error("Failed to open driver");
    }
}

DriverInterface::~DriverInterface() {
    std::lock_guard<std::mutex> lock(g_mutex);
    if (driverHandle_ >= 0) {
        close(driverHandle_);
        driverHandle_ = -1;
    }
}

bool DriverInterface::SendIoctl(unsigned long ioctlCode, void* inBuffer, size_t inBufferSize,
                                void* outBuffer, size_t outBufferSize) {
    std::lock_guard<std::mutex> lock(g_mutex);
    if (driverHandle_ < 0) {
        throw std::runtime_error("Invalid driver handle");
    }
    
    return ioctl(driverHandle_, ioctlCode, inBuffer) >= 0;
}

void DriverInterface::WriteRegister(uint8_t bar, uint64_t registerOffset, uint32_t value) {
    std::lock_guard<std::mutex> lock(g_mutex);
    if (driverHandle_ < 0) {
        throw std::runtime_error("Invalid driver handle");
    }
    
    REGESTRY_PARAMS info = {bar, registerOffset, value};
    if (ioctl(driverHandle_, IOCTL_SET_DMA_REG, &info) < 0) {
        throw std::runtime_error("Failed to write to register");
    }
}

uint32_t DriverInterface::ReadRegister(uint8_t bar, uint64_t registerOffset) {
    std::lock_guard<std::mutex> lock(g_mutex);
    if (driverHandle_ < 0) {
        throw std::runtime_error("Invalid driver handle");
    }
    
    REGESTRY_PARAMS info = {bar, registerOffset, 0};
    if (ioctl(driverHandle_, IOCTL_GET_DMA_REG, &info) < 0) {
        throw std::runtime_error("Failed to read from register");
    }
    return info.value;
}

int DriverInterface::GetHandle() const {
    return driverHandle_;
}
