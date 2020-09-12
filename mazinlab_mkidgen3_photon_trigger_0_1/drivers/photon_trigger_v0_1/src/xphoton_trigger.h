// ==============================================================
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2019.2.1 (64-bit)
// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef XPHOTON_TRIGGER_H
#define XPHOTON_TRIGGER_H

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
#include "xphoton_trigger_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
#else
typedef struct {
    u16 DeviceId;
    u32 Control_BaseAddress;
} XPhoton_trigger_Config;
#endif

typedef struct {
    u32 Control_BaseAddress;
    u32 IsReady;
} XPhoton_trigger;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XPhoton_trigger_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XPhoton_trigger_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XPhoton_trigger_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XPhoton_trigger_ReadReg(BaseAddress, RegOffset) \
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
int XPhoton_trigger_Initialize(XPhoton_trigger *InstancePtr, u16 DeviceId);
XPhoton_trigger_Config* XPhoton_trigger_LookupConfig(u16 DeviceId);
int XPhoton_trigger_CfgInitialize(XPhoton_trigger *InstancePtr, XPhoton_trigger_Config *ConfigPtr);
#else
int XPhoton_trigger_Initialize(XPhoton_trigger *InstancePtr, const char* InstanceName);
int XPhoton_trigger_Release(XPhoton_trigger *InstancePtr);
#endif


void XPhoton_trigger_Set_holdoff_V(XPhoton_trigger *InstancePtr, u32 Data);
u32 XPhoton_trigger_Get_holdoff_V(XPhoton_trigger *InstancePtr);
void XPhoton_trigger_Set_baseline_shrink_V(XPhoton_trigger *InstancePtr, u32 Data);
u32 XPhoton_trigger_Get_baseline_shrink_V(XPhoton_trigger *InstancePtr);
void XPhoton_trigger_Set_baseline_grow_V(XPhoton_trigger *InstancePtr, u32 Data);
u32 XPhoton_trigger_Get_baseline_grow_V(XPhoton_trigger *InstancePtr);
void XPhoton_trigger_Set_dropped_i(XPhoton_trigger *InstancePtr, u32 Data);
u32 XPhoton_trigger_Get_dropped_i(XPhoton_trigger *InstancePtr);
u32 XPhoton_trigger_Get_dropped_o(XPhoton_trigger *InstancePtr);
u32 XPhoton_trigger_Get_dropped_o_vld(XPhoton_trigger *InstancePtr);
u32 XPhoton_trigger_Get_triggercfg_0_threshold_V_BaseAddress(XPhoton_trigger *InstancePtr);
u32 XPhoton_trigger_Get_triggercfg_0_threshold_V_HighAddress(XPhoton_trigger *InstancePtr);
u32 XPhoton_trigger_Get_triggercfg_0_threshold_V_TotalBytes(XPhoton_trigger *InstancePtr);
u32 XPhoton_trigger_Get_triggercfg_0_threshold_V_BitWidth(XPhoton_trigger *InstancePtr);
u32 XPhoton_trigger_Get_triggercfg_0_threshold_V_Depth(XPhoton_trigger *InstancePtr);
u32 XPhoton_trigger_Write_triggercfg_0_threshold_V_Words(XPhoton_trigger *InstancePtr, int offset, int *data, int length);
u32 XPhoton_trigger_Read_triggercfg_0_threshold_V_Words(XPhoton_trigger *InstancePtr, int offset, int *data, int length);
u32 XPhoton_trigger_Write_triggercfg_0_threshold_V_Bytes(XPhoton_trigger *InstancePtr, int offset, char *data, int length);
u32 XPhoton_trigger_Read_triggercfg_0_threshold_V_Bytes(XPhoton_trigger *InstancePtr, int offset, char *data, int length);
u32 XPhoton_trigger_Get_triggercfg_1_threshold_V_BaseAddress(XPhoton_trigger *InstancePtr);
u32 XPhoton_trigger_Get_triggercfg_1_threshold_V_HighAddress(XPhoton_trigger *InstancePtr);
u32 XPhoton_trigger_Get_triggercfg_1_threshold_V_TotalBytes(XPhoton_trigger *InstancePtr);
u32 XPhoton_trigger_Get_triggercfg_1_threshold_V_BitWidth(XPhoton_trigger *InstancePtr);
u32 XPhoton_trigger_Get_triggercfg_1_threshold_V_Depth(XPhoton_trigger *InstancePtr);
u32 XPhoton_trigger_Write_triggercfg_1_threshold_V_Words(XPhoton_trigger *InstancePtr, int offset, int *data, int length);
u32 XPhoton_trigger_Read_triggercfg_1_threshold_V_Words(XPhoton_trigger *InstancePtr, int offset, int *data, int length);
u32 XPhoton_trigger_Write_triggercfg_1_threshold_V_Bytes(XPhoton_trigger *InstancePtr, int offset, char *data, int length);
u32 XPhoton_trigger_Read_triggercfg_1_threshold_V_Bytes(XPhoton_trigger *InstancePtr, int offset, char *data, int length);
u32 XPhoton_trigger_Get_triggercfg_2_threshold_V_BaseAddress(XPhoton_trigger *InstancePtr);
u32 XPhoton_trigger_Get_triggercfg_2_threshold_V_HighAddress(XPhoton_trigger *InstancePtr);
u32 XPhoton_trigger_Get_triggercfg_2_threshold_V_TotalBytes(XPhoton_trigger *InstancePtr);
u32 XPhoton_trigger_Get_triggercfg_2_threshold_V_BitWidth(XPhoton_trigger *InstancePtr);
u32 XPhoton_trigger_Get_triggercfg_2_threshold_V_Depth(XPhoton_trigger *InstancePtr);
u32 XPhoton_trigger_Write_triggercfg_2_threshold_V_Words(XPhoton_trigger *InstancePtr, int offset, int *data, int length);
u32 XPhoton_trigger_Read_triggercfg_2_threshold_V_Words(XPhoton_trigger *InstancePtr, int offset, int *data, int length);
u32 XPhoton_trigger_Write_triggercfg_2_threshold_V_Bytes(XPhoton_trigger *InstancePtr, int offset, char *data, int length);
u32 XPhoton_trigger_Read_triggercfg_2_threshold_V_Bytes(XPhoton_trigger *InstancePtr, int offset, char *data, int length);
u32 XPhoton_trigger_Get_triggercfg_3_threshold_V_BaseAddress(XPhoton_trigger *InstancePtr);
u32 XPhoton_trigger_Get_triggercfg_3_threshold_V_HighAddress(XPhoton_trigger *InstancePtr);
u32 XPhoton_trigger_Get_triggercfg_3_threshold_V_TotalBytes(XPhoton_trigger *InstancePtr);
u32 XPhoton_trigger_Get_triggercfg_3_threshold_V_BitWidth(XPhoton_trigger *InstancePtr);
u32 XPhoton_trigger_Get_triggercfg_3_threshold_V_Depth(XPhoton_trigger *InstancePtr);
u32 XPhoton_trigger_Write_triggercfg_3_threshold_V_Words(XPhoton_trigger *InstancePtr, int offset, int *data, int length);
u32 XPhoton_trigger_Read_triggercfg_3_threshold_V_Words(XPhoton_trigger *InstancePtr, int offset, int *data, int length);
u32 XPhoton_trigger_Write_triggercfg_3_threshold_V_Bytes(XPhoton_trigger *InstancePtr, int offset, char *data, int length);
u32 XPhoton_trigger_Read_triggercfg_3_threshold_V_Bytes(XPhoton_trigger *InstancePtr, int offset, char *data, int length);

#ifdef __cplusplus
}
#endif

#endif
