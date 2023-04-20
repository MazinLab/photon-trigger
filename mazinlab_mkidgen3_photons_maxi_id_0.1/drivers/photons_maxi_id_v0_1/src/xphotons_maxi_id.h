// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
// Tool Version Limit: 2022.04
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef XPHOTONS_MAXI_ID_H
#define XPHOTONS_MAXI_ID_H

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
#include "xphotons_maxi_id_hw.h"

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
} XPhotons_maxi_id_Config;
#endif

typedef struct {
    u64 Control_BaseAddress;
    u32 IsReady;
} XPhotons_maxi_id;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XPhotons_maxi_id_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XPhotons_maxi_id_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XPhotons_maxi_id_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XPhotons_maxi_id_ReadReg(BaseAddress, RegOffset) \
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
int XPhotons_maxi_id_Initialize(XPhotons_maxi_id *InstancePtr, u16 DeviceId);
XPhotons_maxi_id_Config* XPhotons_maxi_id_LookupConfig(u16 DeviceId);
int XPhotons_maxi_id_CfgInitialize(XPhotons_maxi_id *InstancePtr, XPhotons_maxi_id_Config *ConfigPtr);
#else
int XPhotons_maxi_id_Initialize(XPhotons_maxi_id *InstancePtr, const char* InstanceName);
int XPhotons_maxi_id_Release(XPhotons_maxi_id *InstancePtr);
#endif

void XPhotons_maxi_id_Start(XPhotons_maxi_id *InstancePtr);
u32 XPhotons_maxi_id_IsDone(XPhotons_maxi_id *InstancePtr);
u32 XPhotons_maxi_id_IsIdle(XPhotons_maxi_id *InstancePtr);
u32 XPhotons_maxi_id_IsReady(XPhotons_maxi_id *InstancePtr);
void XPhotons_maxi_id_EnableAutoRestart(XPhotons_maxi_id *InstancePtr);
void XPhotons_maxi_id_DisableAutoRestart(XPhotons_maxi_id *InstancePtr);

void XPhotons_maxi_id_Set_photons_out(XPhotons_maxi_id *InstancePtr, u64 Data);
u64 XPhotons_maxi_id_Get_photons_out(XPhotons_maxi_id *InstancePtr);
u32 XPhotons_maxi_id_Get_active_buffer(XPhotons_maxi_id *InstancePtr);
u32 XPhotons_maxi_id_Get_active_buffer_vld(XPhotons_maxi_id *InstancePtr);
u32 XPhotons_maxi_id_Get_n_photons_BaseAddress(XPhotons_maxi_id *InstancePtr);
u32 XPhotons_maxi_id_Get_n_photons_HighAddress(XPhotons_maxi_id *InstancePtr);
u32 XPhotons_maxi_id_Get_n_photons_TotalBytes(XPhotons_maxi_id *InstancePtr);
u32 XPhotons_maxi_id_Get_n_photons_BitWidth(XPhotons_maxi_id *InstancePtr);
u32 XPhotons_maxi_id_Get_n_photons_Depth(XPhotons_maxi_id *InstancePtr);
u32 XPhotons_maxi_id_Write_n_photons_Words(XPhotons_maxi_id *InstancePtr, int offset, word_type *data, int length);
u32 XPhotons_maxi_id_Read_n_photons_Words(XPhotons_maxi_id *InstancePtr, int offset, word_type *data, int length);
u32 XPhotons_maxi_id_Write_n_photons_Bytes(XPhotons_maxi_id *InstancePtr, int offset, char *data, int length);
u32 XPhotons_maxi_id_Read_n_photons_Bytes(XPhotons_maxi_id *InstancePtr, int offset, char *data, int length);

void XPhotons_maxi_id_InterruptGlobalEnable(XPhotons_maxi_id *InstancePtr);
void XPhotons_maxi_id_InterruptGlobalDisable(XPhotons_maxi_id *InstancePtr);
void XPhotons_maxi_id_InterruptEnable(XPhotons_maxi_id *InstancePtr, u32 Mask);
void XPhotons_maxi_id_InterruptDisable(XPhotons_maxi_id *InstancePtr, u32 Mask);
void XPhotons_maxi_id_InterruptClear(XPhotons_maxi_id *InstancePtr, u32 Mask);
u32 XPhotons_maxi_id_InterruptGetEnabled(XPhotons_maxi_id *InstancePtr);
u32 XPhotons_maxi_id_InterruptGetStatus(XPhotons_maxi_id *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
