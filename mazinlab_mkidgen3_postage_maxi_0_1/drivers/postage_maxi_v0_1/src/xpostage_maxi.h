// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
// Tool Version Limit: 2022.04
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef XPOSTAGE_MAXI_H
#define XPOSTAGE_MAXI_H

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
#include "xpostage_maxi_hw.h"

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
} XPostage_maxi_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XPostage_maxi;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XPostage_maxi_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XPostage_maxi_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XPostage_maxi_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XPostage_maxi_ReadReg(BaseAddress, RegOffset) \
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
int XPostage_maxi_Initialize(XPostage_maxi *InstancePtr, u16 DeviceId);
XPostage_maxi_Config* XPostage_maxi_LookupConfig(u16 DeviceId);
int XPostage_maxi_CfgInitialize(XPostage_maxi *InstancePtr, XPostage_maxi_Config *ConfigPtr);
#else
int XPostage_maxi_Initialize(XPostage_maxi *InstancePtr, const char* InstanceName);
int XPostage_maxi_Release(XPostage_maxi *InstancePtr);
#endif

void XPostage_maxi_Start(XPostage_maxi *InstancePtr);
u32 XPostage_maxi_IsDone(XPostage_maxi *InstancePtr);
u32 XPostage_maxi_IsIdle(XPostage_maxi *InstancePtr);
u32 XPostage_maxi_IsReady(XPostage_maxi *InstancePtr);
void XPostage_maxi_EnableAutoRestart(XPostage_maxi *InstancePtr);
void XPostage_maxi_DisableAutoRestart(XPostage_maxi *InstancePtr);

void XPostage_maxi_Set_iq(XPostage_maxi *InstancePtr, u64 Data);
u64 XPostage_maxi_Get_iq(XPostage_maxi *InstancePtr);
u32 XPostage_maxi_Get_event_count_BaseAddress(XPostage_maxi *InstancePtr);
u32 XPostage_maxi_Get_event_count_HighAddress(XPostage_maxi *InstancePtr);
u32 XPostage_maxi_Get_event_count_TotalBytes(XPostage_maxi *InstancePtr);
u32 XPostage_maxi_Get_event_count_BitWidth(XPostage_maxi *InstancePtr);
u32 XPostage_maxi_Get_event_count_Depth(XPostage_maxi *InstancePtr);
u32 XPostage_maxi_Write_event_count_Words(XPostage_maxi *InstancePtr, int offset, word_type *data, int length);
u32 XPostage_maxi_Read_event_count_Words(XPostage_maxi *InstancePtr, int offset, word_type *data, int length);
u32 XPostage_maxi_Write_event_count_Bytes(XPostage_maxi *InstancePtr, int offset, char *data, int length);
u32 XPostage_maxi_Read_event_count_Bytes(XPostage_maxi *InstancePtr, int offset, char *data, int length);

void XPostage_maxi_InterruptGlobalEnable(XPostage_maxi *InstancePtr);
void XPostage_maxi_InterruptGlobalDisable(XPostage_maxi *InstancePtr);
void XPostage_maxi_InterruptEnable(XPostage_maxi *InstancePtr, u32 Mask);
void XPostage_maxi_InterruptDisable(XPostage_maxi *InstancePtr, u32 Mask);
void XPostage_maxi_InterruptClear(XPostage_maxi *InstancePtr, u32 Mask);
u32 XPostage_maxi_InterruptGetEnabled(XPostage_maxi *InstancePtr);
u32 XPostage_maxi_InterruptGetStatus(XPostage_maxi *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
