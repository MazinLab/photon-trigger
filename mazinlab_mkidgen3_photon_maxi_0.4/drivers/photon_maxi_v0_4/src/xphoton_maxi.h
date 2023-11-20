// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
// Tool Version Limit: 2022.04
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef XPHOTON_MAXI_H
#define XPHOTON_MAXI_H

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
#include "xphoton_maxi_hw.h"

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
} XPhoton_maxi_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XPhoton_maxi;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XPhoton_maxi_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XPhoton_maxi_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XPhoton_maxi_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XPhoton_maxi_ReadReg(BaseAddress, RegOffset) \
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
int XPhoton_maxi_Initialize(XPhoton_maxi *InstancePtr, u16 DeviceId);
XPhoton_maxi_Config* XPhoton_maxi_LookupConfig(u16 DeviceId);
int XPhoton_maxi_CfgInitialize(XPhoton_maxi *InstancePtr, XPhoton_maxi_Config *ConfigPtr);
#else
int XPhoton_maxi_Initialize(XPhoton_maxi *InstancePtr, const char* InstanceName);
int XPhoton_maxi_Release(XPhoton_maxi *InstancePtr);
#endif

void XPhoton_maxi_Start(XPhoton_maxi *InstancePtr);
u32 XPhoton_maxi_IsDone(XPhoton_maxi *InstancePtr);
u32 XPhoton_maxi_IsIdle(XPhoton_maxi *InstancePtr);
u32 XPhoton_maxi_IsReady(XPhoton_maxi *InstancePtr);
void XPhoton_maxi_EnableAutoRestart(XPhoton_maxi *InstancePtr);
void XPhoton_maxi_DisableAutoRestart(XPhoton_maxi *InstancePtr);

void XPhoton_maxi_Set_photons_out(XPhoton_maxi *InstancePtr, u64 Data);
u64 XPhoton_maxi_Get_photons_out(XPhoton_maxi *InstancePtr);
u32 XPhoton_maxi_Get_active_buffer(XPhoton_maxi *InstancePtr);
u32 XPhoton_maxi_Get_active_buffer_vld(XPhoton_maxi *InstancePtr);
void XPhoton_maxi_Set_photons_per_buf(XPhoton_maxi *InstancePtr, u32 Data);
u32 XPhoton_maxi_Get_photons_per_buf(XPhoton_maxi *InstancePtr);
void XPhoton_maxi_Set_time_shift(XPhoton_maxi *InstancePtr, u32 Data);
u32 XPhoton_maxi_Get_time_shift(XPhoton_maxi *InstancePtr);
u32 XPhoton_maxi_Get_n_photons_BaseAddress(XPhoton_maxi *InstancePtr);
u32 XPhoton_maxi_Get_n_photons_HighAddress(XPhoton_maxi *InstancePtr);
u32 XPhoton_maxi_Get_n_photons_TotalBytes(XPhoton_maxi *InstancePtr);
u32 XPhoton_maxi_Get_n_photons_BitWidth(XPhoton_maxi *InstancePtr);
u32 XPhoton_maxi_Get_n_photons_Depth(XPhoton_maxi *InstancePtr);
u32 XPhoton_maxi_Write_n_photons_Words(XPhoton_maxi *InstancePtr, int offset, word_type *data, int length);
u32 XPhoton_maxi_Read_n_photons_Words(XPhoton_maxi *InstancePtr, int offset, word_type *data, int length);
u32 XPhoton_maxi_Write_n_photons_Bytes(XPhoton_maxi *InstancePtr, int offset, char *data, int length);
u32 XPhoton_maxi_Read_n_photons_Bytes(XPhoton_maxi *InstancePtr, int offset, char *data, int length);

void XPhoton_maxi_InterruptGlobalEnable(XPhoton_maxi *InstancePtr);
void XPhoton_maxi_InterruptGlobalDisable(XPhoton_maxi *InstancePtr);
void XPhoton_maxi_InterruptEnable(XPhoton_maxi *InstancePtr, u32 Mask);
void XPhoton_maxi_InterruptDisable(XPhoton_maxi *InstancePtr, u32 Mask);
void XPhoton_maxi_InterruptClear(XPhoton_maxi *InstancePtr, u32 Mask);
u32 XPhoton_maxi_InterruptGetEnabled(XPhoton_maxi *InstancePtr);
u32 XPhoton_maxi_InterruptGetStatus(XPhoton_maxi *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
