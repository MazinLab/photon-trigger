// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2021.1 (64-bit)
// Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef XPOSTAGE_H
#define XPOSTAGE_H

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
#include "xpostage_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
#else
typedef struct {
    u16 DeviceId;
    u32 Control_BaseAddress;
} XPostage_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XPostage;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XPostage_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XPostage_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XPostage_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XPostage_ReadReg(BaseAddress, RegOffset) \
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
int XPostage_Initialize(XPostage *InstancePtr, u16 DeviceId);
XPostage_Config* XPostage_LookupConfig(u16 DeviceId);
int XPostage_CfgInitialize(XPostage *InstancePtr, XPostage_Config *ConfigPtr);
#else
int XPostage_Initialize(XPostage *InstancePtr, const char* InstanceName);
int XPostage_Release(XPostage *InstancePtr);
#endif

void XPostage_Start(XPostage *InstancePtr);
u32 XPostage_IsDone(XPostage *InstancePtr);
u32 XPostage_IsIdle(XPostage *InstancePtr);
u32 XPostage_IsReady(XPostage *InstancePtr);
void XPostage_EnableAutoRestart(XPostage *InstancePtr);
void XPostage_DisableAutoRestart(XPostage *InstancePtr);

void XPostage_Set_monitor_0(XPostage *InstancePtr, u32 Data);
u32 XPostage_Get_monitor_0(XPostage *InstancePtr);
void XPostage_Set_monitor_1(XPostage *InstancePtr, u32 Data);
u32 XPostage_Get_monitor_1(XPostage *InstancePtr);
void XPostage_Set_monitor_2(XPostage *InstancePtr, u32 Data);
u32 XPostage_Get_monitor_2(XPostage *InstancePtr);
void XPostage_Set_monitor_3(XPostage *InstancePtr, u32 Data);
u32 XPostage_Get_monitor_3(XPostage *InstancePtr);
void XPostage_Set_monitor_4(XPostage *InstancePtr, u32 Data);
u32 XPostage_Get_monitor_4(XPostage *InstancePtr);
void XPostage_Set_monitor_5(XPostage *InstancePtr, u32 Data);
u32 XPostage_Get_monitor_5(XPostage *InstancePtr);
void XPostage_Set_monitor_6(XPostage *InstancePtr, u32 Data);
u32 XPostage_Get_monitor_6(XPostage *InstancePtr);
void XPostage_Set_monitor_7(XPostage *InstancePtr, u32 Data);
u32 XPostage_Get_monitor_7(XPostage *InstancePtr);

void XPostage_InterruptGlobalEnable(XPostage *InstancePtr);
void XPostage_InterruptGlobalDisable(XPostage *InstancePtr);
void XPostage_InterruptEnable(XPostage *InstancePtr, u32 Mask);
void XPostage_InterruptDisable(XPostage *InstancePtr, u32 Mask);
void XPostage_InterruptClear(XPostage *InstancePtr, u32 Mask);
u32 XPostage_InterruptGetEnabled(XPostage *InstancePtr);
u32 XPostage_InterruptGetStatus(XPostage *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
