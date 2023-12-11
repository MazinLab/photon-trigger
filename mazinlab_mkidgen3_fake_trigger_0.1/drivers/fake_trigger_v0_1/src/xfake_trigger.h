// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
// Tool Version Limit: 2022.04
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef XFAKE_TRIGGER_H
#define XFAKE_TRIGGER_H

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
#include "xfake_trigger_hw.h"

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
} XFake_trigger_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XFake_trigger;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XFake_trigger_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XFake_trigger_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XFake_trigger_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XFake_trigger_ReadReg(BaseAddress, RegOffset) \
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
int XFake_trigger_Initialize(XFake_trigger *InstancePtr, u16 DeviceId);
XFake_trigger_Config* XFake_trigger_LookupConfig(u16 DeviceId);
int XFake_trigger_CfgInitialize(XFake_trigger *InstancePtr, XFake_trigger_Config *ConfigPtr);
#else
int XFake_trigger_Initialize(XFake_trigger *InstancePtr, const char* InstanceName);
int XFake_trigger_Release(XFake_trigger *InstancePtr);
#endif


u32 XFake_trigger_Get_threshoffs_BaseAddress(XFake_trigger *InstancePtr);
u32 XFake_trigger_Get_threshoffs_HighAddress(XFake_trigger *InstancePtr);
u32 XFake_trigger_Get_threshoffs_TotalBytes(XFake_trigger *InstancePtr);
u32 XFake_trigger_Get_threshoffs_BitWidth(XFake_trigger *InstancePtr);
u32 XFake_trigger_Get_threshoffs_Depth(XFake_trigger *InstancePtr);
u32 XFake_trigger_Write_threshoffs_Words(XFake_trigger *InstancePtr, int offset, word_type *data, int length);
u32 XFake_trigger_Read_threshoffs_Words(XFake_trigger *InstancePtr, int offset, word_type *data, int length);
u32 XFake_trigger_Write_threshoffs_Bytes(XFake_trigger *InstancePtr, int offset, char *data, int length);
u32 XFake_trigger_Read_threshoffs_Bytes(XFake_trigger *InstancePtr, int offset, char *data, int length);

#ifdef __cplusplus
}
#endif

#endif
