// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
// Tool Version Limit: 2022.04
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef XPHOTON_FIFO_PACKETIZER_H
#define XPHOTON_FIFO_PACKETIZER_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "xphoton_fifo_packetizer_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
#else
typedef struct {
    u16 DeviceId;
    u64 Control_BaseAddress;
} XPhoton_fifo_packetizer_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XPhoton_fifo_packetizer;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XPhoton_fifo_packetizer_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XPhoton_fifo_packetizer_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XPhoton_fifo_packetizer_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XPhoton_fifo_packetizer_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
int XPhoton_fifo_packetizer_Initialize(XPhoton_fifo_packetizer *InstancePtr, u16 DeviceId);
XPhoton_fifo_packetizer_Config* XPhoton_fifo_packetizer_LookupConfig(u16 DeviceId);
int XPhoton_fifo_packetizer_CfgInitialize(XPhoton_fifo_packetizer *InstancePtr, XPhoton_fifo_packetizer_Config *ConfigPtr);
#else
int XPhoton_fifo_packetizer_Initialize(XPhoton_fifo_packetizer *InstancePtr, const char* InstanceName);
int XPhoton_fifo_packetizer_Release(XPhoton_fifo_packetizer *InstancePtr);
#endif


void XPhoton_fifo_packetizer_Set_max_photons_per_packet_minus2(XPhoton_fifo_packetizer *InstancePtr, u32 Data);
u32 XPhoton_fifo_packetizer_Get_max_photons_per_packet_minus2(XPhoton_fifo_packetizer *InstancePtr);
void XPhoton_fifo_packetizer_Set_approx_time_per_packet(XPhoton_fifo_packetizer *InstancePtr, u32 Data);
u32 XPhoton_fifo_packetizer_Get_approx_time_per_packet(XPhoton_fifo_packetizer *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
