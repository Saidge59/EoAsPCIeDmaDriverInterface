/*
 * @file Public.h
 * @brief Constants and structures for IOCTL and DMA data transfer in Linux.
 *
 * This section defines various constants, macros, data types, and structures
 * used for IOCTL commands and DMA data transfer between the driver and user applications.
 *
 */
#ifndef PUBLIC_H
#define PUBLIC_H

#ifdef __KERNEL__
#include <linux/ioctl.h>
#include <linux/types.h>
#else
#include <stdint.h>
#endif

#define DRV_VER 0x102

///< Maximum number of channels and descriptors
#define MAX_NUM_CHANNELS 20                           ///< Maximum number of DMA channels
#define MAX_NUM_CHANNELS_WITH_HEADER 8                ///< Maximum number of DMA channels for project with read size in header
#define MAX_NUM_DESCRIPTORS 8                         ///< Maximum number of DMA descriptors per channel
#define MAX_NUM_EVENTS_PER_DESCRIPTORS 3              ///< Maximum number of events per descriptor
#define DESCRIPTOR_BUFFER_SIZE (256ULL * 1024 * 1024) ///< Descriptor buffer size set to 1 GB

///< COMMON MACROS
///< Alignment for X is power of 2
#define ALIGN_X(value, x) (((value) + ((x) - 1)) & -(x)) ///< Align 'value' to the nearest multiple of 'x'
#define ALIGN_64(value) ALIGN_X((value), 64)             ///< Align 'value' to the nearest multiple of 64

#define TIMEOUT_MS_IOCTL 2000 ///< IOCTL call timeout in ms

#define DEVICE_GLOBAL_DRV_VER 0xB                        ///< Global enable Interrupt register
#define DEVICE_GLOBAL_INTERRUPT_FPGA_ENABLE 0x0003       ///< Global enable Interrupt register
#define DEVICE_GLOBAL_INTERRUPT_FPGA_STATUS 0x0005       ///< Global status Interrupt register (FIFO occupancy)
#define DEVICE_GLOBAL_INTERRUPT_FPGA_ACK 0x0004          ///< Global status Interrupt register
#define DEVICE_GLOBAL_INTERRUPT_FPGA_DATA 0x0006         ///< Global Interrupt register data (read before ACK)
#define DEVICE_GLOBAL_RX_DMA_ENABLE_FPGA_DATA 0x0008     ///< Global Rx DMA enable register data
#define DEVICE_GLOBAL_TX_DMA_ENABLE_FPGA_DATA 0x0009     ///< Global Tx DMA enable register data
#define DEVICE_GLOBAL_DMA_MAX_DMA_RX_CHANELS_DATA 0x0009 ///< Global DMA max channels register data
#define DEVICE_GLOBAL_DMA_MAX_DMA_TX_CHANELS_DATA 0x000A ///< Global DMA max channels register data
#define DEVICE_GLOBAL_DMA_PPS_TRIGER 0x0017              ///< Global PPS triger bit 0: Enable, bit 1: Rising/Fall

///< DMA control register address
#define DEVICE_GLOBAL_DMA_REG_CONTROL 0x0100 ///< Register address for DMA control operations.

///< DMA descriptors number register address
#define DEVICE_GLOBAL_DMA_REG_DESCRIPTORS_NUMBER 0x0101 ///< Register address for querying the number of DMA descriptors.

///< DMA get descriptor index register address
#define DEVICE_GLOBAL_DMA_REG_GET_DESCRIPTOR_INDEX 0x0102 ///< Register address for obtaining the current descriptor index.

///< DMA descriptors table base address
#define DEVICE_GLOBAL_DMA_REG_DESCRIPTORS_TABLE 0x0800 ///< Base address of the DMA descriptors table.

///< Function to transform FPGA address
static inline uint64_t TransFormFpgaAddress(uint64_t address)
{
    return address << 2; ///< Left shift by 2 positions to multiply by 4
}

///< Structures for IOCTL and DMA data transfer

typedef struct __attribute__((packed)) _REGESTRY_PARAMS
{
    uint8_t bar;      ///< Base address register number
    uint64_t address; ///< Address for the operation
    uint32_t value;   ///< Value for read/write operations
} REGESTRY_PARAMS;

typedef struct __attribute__((packed)) _GLOBAL_DATA_DMA_PARAMETERS
{
    uint32_t DmaDescriptorsMaxCount;
    uint32_t DmaDescriptorMaxBufferSize;
    uint32_t DmaChannelsMaxCount;
} GLOBAL_DATA_DMA_PARAMETERS;

typedef struct __attribute__((packed)) _DATA_MEMORY_DMA_DESCRIPTOR
{
    uint64_t BufferVA;
    uint64_t BufferPA;
} DATA_MEMORY_DMA_DESCRIPTOR;

typedef struct __attribute__((packed)) _DATA_MEMORY_DMA_CHANNEL
{
    DATA_MEMORY_DMA_DESCRIPTOR DmaMemoryDescriptors[MAX_NUM_DESCRIPTORS];
} DATA_MEMORY_DMA_CHANNEL;

typedef struct __attribute__((packed)) _GLOBAL_MEM_MAP_DATA
{
    DATA_MEMORY_DMA_CHANNEL DataMemoryDmaChannels[MAX_NUM_CHANNELS];
} GLOBAL_MEM_MAP_DATA;

typedef struct __attribute__((packed)) _DATA_EVENT_HANDLE_DMA_DESCRIPTOR
{
    int DmaEventHandle;
} DATA_EVENT_HANDLE_DMA_DESCRIPTOR;

typedef struct __attribute__((packed)) _DATA_EVENT_HANDLE_DMA_CHANNEL
{
    DATA_EVENT_HANDLE_DMA_DESCRIPTOR DmaEventHandleDescriptors[MAX_NUM_DESCRIPTORS];
} DATA_EVENT_HANDLE_DMA_CHANNEL;

typedef struct __attribute__((packed)) _GLOBAL_EVENT_HANDLE_DATA
{
    DATA_EVENT_HANDLE_DMA_CHANNEL DataEventHandleDmaChannels[MAX_NUM_CHANNELS];
} GLOBAL_EVENT_HANDLE_DATA;

typedef struct __attribute__((packed)) _START_DMA_DESCRIPTORS_CONFIGURATION
{
    uint32_t DmaDescriptorBufferSize;
    uint32_t IsDescriptorInterruptEnable;
} START_DMA_DESCRIPTORS_CONFIGURATION;

typedef struct __attribute__((packed)) _START_DMA_CHANNEL_CONFIGURATION
{
    uint32_t DmaDescriptorsCount;
    START_DMA_DESCRIPTORS_CONFIGURATION StartDmaDescriptors[MAX_NUM_DESCRIPTORS];
} START_DMA_CHANNEL_CONFIGURATION;

typedef struct __attribute__((packed)) _GLOBAL_START_DMA_CONFIGURATION
{
    uint32_t DmaChannelsCount;
    uint32_t StartCycle;
    START_DMA_CHANNEL_CONFIGURATION StartDmaChannels[MAX_NUM_CHANNELS];
} GLOBAL_START_DMA_CONFIGURATION;

///< Device type definition for IOCTL
#define FILE_DEVICE_PCIE 0x9000 ///< Device type definition for IOCTL

// Define IOCTL command codes
#define IOCTL_SET_DMA_REG _IOW(FILE_DEVICE_PCIE, 0x701, REGESTRY_PARAMS)
#define IOCTL_GET_DMA_REG _IOR(FILE_DEVICE_PCIE, 0x702, REGESTRY_PARAMS)
#define IOCTL_GET_DMA_STATUS _IOR(FILE_DEVICE_PCIE, 0x703, uint32_t)
#define IOCTL_GLOBAL_DMA_CONFIGURATION_GET _IOR(FILE_DEVICE_PCIE, 0x704, GLOBAL_DATA_DMA_PARAMETERS)
#define IOCTL_GLOBAL_MEM_MAP_GET _IOR(FILE_DEVICE_PCIE, 0x705, GLOBAL_MEM_MAP_DATA)
#define IOCTL_GLOBAL_EVENT_HANDLE_SET _IOW(FILE_DEVICE_PCIE, 0x706, uint32_t)
#define IOCTL_GLOBAL_EVENT_HANDLE_GET _IOR(FILE_DEVICE_PCIE, 0x707, GLOBAL_EVENT_HANDLE_DATA)

#endif /* PUBLIC_H */
