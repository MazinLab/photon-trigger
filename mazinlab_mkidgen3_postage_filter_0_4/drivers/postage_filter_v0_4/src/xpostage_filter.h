// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
// Tool Version Limit: 2022.04
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef XPOSTAGE_FILTER_H
#define XPOSTAGE_FILTER_H

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
#include "xpostage_filter_hw.h"

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
} XPostage_filter_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XPostage_filter;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XPostage_filter_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XPostage_filter_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XPostage_filter_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XPostage_filter_ReadReg(BaseAddress, RegOffset) \
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
int XPostage_filter_Initialize(XPostage_filter *InstancePtr, u16 DeviceId);
XPostage_filter_Config* XPostage_filter_LookupConfig(u16 DeviceId);
int XPostage_filter_CfgInitialize(XPostage_filter *InstancePtr, XPostage_filter_Config *ConfigPtr);
#else
int XPostage_filter_Initialize(XPostage_filter *InstancePtr, const char* InstanceName);
int XPostage_filter_Release(XPostage_filter *InstancePtr);
#endif


void XPostage_filter_Set_monitor_0(XPostage_filter *InstancePtr, u32 Data);
u32 XPostage_filter_Get_monitor_0(XPostage_filter *InstancePtr);
void XPostage_filter_Set_monitor_1(XPostage_filter *InstancePtr, u32 Data);
u32 XPostage_filter_Get_monitor_1(XPostage_filter *InstancePtr);
void XPostage_filter_Set_monitor_2(XPostage_filter *InstancePtr, u32 Data);
u32 XPostage_filter_Get_monitor_2(XPostage_filter *InstancePtr);
void XPostage_filter_Set_monitor_3(XPostage_filter *InstancePtr, u32 Data);
u32 XPostage_filter_Get_monitor_3(XPostage_filter *InstancePtr);
void XPostage_filter_Set_monitor_4(XPostage_filter *InstancePtr, u32 Data);
u32 XPostage_filter_Get_monitor_4(XPostage_filter *InstancePtr);
void XPostage_filter_Set_monitor_5(XPostage_filter *InstancePtr, u32 Data);
u32 XPostage_filter_Get_monitor_5(XPostage_filter *InstancePtr);
void XPostage_filter_Set_monitor_6(XPostage_filter *InstancePtr, u32 Data);
u32 XPostage_filter_Get_monitor_6(XPostage_filter *InstancePtr);
void XPostage_filter_Set_monitor_7(XPostage_filter *InstancePtr, u32 Data);
u32 XPostage_filter_Get_monitor_7(XPostage_filter *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
