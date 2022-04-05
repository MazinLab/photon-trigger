// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2021.1 (64-bit)
// Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef XTRIGGER_H
#define XTRIGGER_H

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
#include "xtrigger_hw.h"

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
} XTrigger_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XTrigger;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XTrigger_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XTrigger_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XTrigger_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XTrigger_ReadReg(BaseAddress, RegOffset) \
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
int XTrigger_Initialize(XTrigger *InstancePtr, u16 DeviceId);
XTrigger_Config* XTrigger_LookupConfig(u16 DeviceId);
int XTrigger_CfgInitialize(XTrigger *InstancePtr, XTrigger_Config *ConfigPtr);
#else
int XTrigger_Initialize(XTrigger *InstancePtr, const char* InstanceName);
int XTrigger_Release(XTrigger *InstancePtr);
#endif


void XTrigger_Set_holdoff(XTrigger *InstancePtr, u32 Data);
u32 XTrigger_Get_holdoff(XTrigger *InstancePtr);
u32 XTrigger_Get_thresholds_BaseAddress(XTrigger *InstancePtr);
u32 XTrigger_Get_thresholds_HighAddress(XTrigger *InstancePtr);
u32 XTrigger_Get_thresholds_TotalBytes(XTrigger *InstancePtr);
u32 XTrigger_Get_thresholds_BitWidth(XTrigger *InstancePtr);
u32 XTrigger_Get_thresholds_Depth(XTrigger *InstancePtr);
u32 XTrigger_Write_thresholds_Words(XTrigger *InstancePtr, int offset, word_type *data, int length);
u32 XTrigger_Read_thresholds_Words(XTrigger *InstancePtr, int offset, word_type *data, int length);
u32 XTrigger_Write_thresholds_Bytes(XTrigger *InstancePtr, int offset, char *data, int length);
u32 XTrigger_Read_thresholds_Bytes(XTrigger *InstancePtr, int offset, char *data, int length);

#ifdef __cplusplus
}
#endif

#endif
