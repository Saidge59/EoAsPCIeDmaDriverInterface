#include "DriverInterface.h"
#include <iostream>

int main() {
    try {
        const char* devicePath = "/dev/my_driver";
        DriverInterface driver(devicePath);

        uint8_t bar = 0;
        uint64_t offset = 0x100;
        uint32_t value = 42;

        driver.WriteRegister(bar, offset, value);
        std::cout << "Written value: " << value << " at offset " << offset << std::endl;

        uint32_t readValue = driver.ReadRegister(bar, offset);
        std::cout << "Read value: " << readValue << " from offset " << offset << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}