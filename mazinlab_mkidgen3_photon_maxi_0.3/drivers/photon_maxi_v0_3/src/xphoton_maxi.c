// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
// Tool Version Limit: 2022.04
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xphoton_maxi.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XPhoton_maxi_CfgInitialize(XPhoton_maxi *InstancePtr, XPhoton_maxi_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XPhoton_maxi_Start(XPhoton_maxi *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPhoton_maxi_ReadReg(InstancePtr->Control_BaseAddress, XPHOTON_MAXI_CONTROL_ADDR_AP_CTRL) & 0x80;
    XPhoton_maxi_WriteReg(InstancePtr->Control_BaseAddress, XPHOTON_MAXI_CONTROL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XPhoton_maxi_IsDone(XPhoton_maxi *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPhoton_maxi_ReadReg(InstancePtr->Control_BaseAddress, XPHOTON_MAXI_CONTROL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XPhoton_maxi_IsIdle(XPhoton_maxi *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPhoton_maxi_ReadReg(InstancePtr->Control_BaseAddress, XPHOTON_MAXI_CONTROL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XPhoton_maxi_IsReady(XPhoton_maxi *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPhoton_maxi_ReadReg(InstancePtr->Control_BaseAddress, XPHOTON_MAXI_CONTROL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XPhoton_maxi_EnableAutoRestart(XPhoton_maxi *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPhoton_maxi_WriteReg(InstancePtr->Control_BaseAddress, XPHOTON_MAXI_CONTROL_ADDR_AP_CTRL, 0x80);
}

void XPhoton_maxi_DisableAutoRestart(XPhoton_maxi *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPhoton_maxi_WriteReg(InstancePtr->Control_BaseAddress, XPHOTON_MAXI_CONTROL_ADDR_AP_CTRL, 0);
}

void XPhoton_maxi_Set_photons_out(XPhoton_maxi *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPhoton_maxi_WriteReg(InstancePtr->Control_BaseAddress, XPHOTON_MAXI_CONTROL_ADDR_PHOTONS_OUT_DATA, (u32)(Data));
    XPhoton_maxi_WriteReg(InstancePtr->Control_BaseAddress, XPHOTON_MAXI_CONTROL_ADDR_PHOTONS_OUT_DATA + 4, (u32)(Data >> 32));
}

u64 XPhoton_maxi_Get_photons_out(XPhoton_maxi *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPhoton_maxi_ReadReg(InstancePtr->Control_BaseAddress, XPHOTON_MAXI_CONTROL_ADDR_PHOTONS_OUT_DATA);
    Data += (u64)XPhoton_maxi_ReadReg(InstancePtr->Control_BaseAddress, XPHOTON_MAXI_CONTROL_ADDR_PHOTONS_OUT_DATA + 4) << 32;
    return Data;
}

u32 XPhoton_maxi_Get_active_buffer(XPhoton_maxi *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPhoton_maxi_ReadReg(InstancePtr->Control_BaseAddress, XPHOTON_MAXI_CONTROL_ADDR_ACTIVE_BUFFER_DATA);
    return Data;
}

u32 XPhoton_maxi_Get_active_buffer_vld(XPhoton_maxi *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPhoton_maxi_ReadReg(InstancePtr->Control_BaseAddress, XPHOTON_MAXI_CONTROL_ADDR_ACTIVE_BUFFER_CTRL);
    return Data & 0x1;
}

void XPhoton_maxi_Set_photons_per_buf(XPhoton_maxi *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPhoton_maxi_WriteReg(InstancePtr->Control_BaseAddress, XPHOTON_MAXI_CONTROL_ADDR_PHOTONS_PER_BUF_DATA, Data);
}

u32 XPhoton_maxi_Get_photons_per_buf(XPhoton_maxi *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPhoton_maxi_ReadReg(InstancePtr->Control_BaseAddress, XPHOTON_MAXI_CONTROL_ADDR_PHOTONS_PER_BUF_DATA);
    return Data;
}

void XPhoton_maxi_Set_time_shift(XPhoton_maxi *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPhoton_maxi_WriteReg(InstancePtr->Control_BaseAddress, XPHOTON_MAXI_CONTROL_ADDR_TIME_SHIFT_DATA, Data);
}

u32 XPhoton_maxi_Get_time_shift(XPhoton_maxi *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPhoton_maxi_ReadReg(InstancePtr->Control_BaseAddress, XPHOTON_MAXI_CONTROL_ADDR_TIME_SHIFT_DATA);
    return Data;
}

u32 XPhoton_maxi_Get_n_photons_BaseAddress(XPhoton_maxi *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Control_BaseAddress + XPHOTON_MAXI_CONTROL_ADDR_N_PHOTONS_BASE);
}

u32 XPhoton_maxi_Get_n_photons_HighAddress(XPhoton_maxi *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Control_BaseAddress + XPHOTON_MAXI_CONTROL_ADDR_N_PHOTONS_HIGH);
}

u32 XPhoton_maxi_Get_n_photons_TotalBytes(XPhoton_maxi *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (XPHOTON_MAXI_CONTROL_ADDR_N_PHOTONS_HIGH - XPHOTON_MAXI_CONTROL_ADDR_N_PHOTONS_BASE + 1);
}

u32 XPhoton_maxi_Get_n_photons_BitWidth(XPhoton_maxi *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XPHOTON_MAXI_CONTROL_WIDTH_N_PHOTONS;
}

u32 XPhoton_maxi_Get_n_photons_Depth(XPhoton_maxi *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XPHOTON_MAXI_CONTROL_DEPTH_N_PHOTONS;
}

u32 XPhoton_maxi_Write_n_photons_Words(XPhoton_maxi *InstancePtr, int offset, word_type *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XPHOTON_MAXI_CONTROL_ADDR_N_PHOTONS_HIGH - XPHOTON_MAXI_CONTROL_ADDR_N_PHOTONS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(int *)(InstancePtr->Control_BaseAddress + XPHOTON_MAXI_CONTROL_ADDR_N_PHOTONS_BASE + (offset + i)*4) = *(data + i);
    }
    return length;
}

u32 XPhoton_maxi_Read_n_photons_Words(XPhoton_maxi *InstancePtr, int offset, word_type *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XPHOTON_MAXI_CONTROL_ADDR_N_PHOTONS_HIGH - XPHOTON_MAXI_CONTROL_ADDR_N_PHOTONS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(int *)(InstancePtr->Control_BaseAddress + XPHOTON_MAXI_CONTROL_ADDR_N_PHOTONS_BASE + (offset + i)*4);
    }
    return length;
}

u32 XPhoton_maxi_Write_n_photons_Bytes(XPhoton_maxi *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XPHOTON_MAXI_CONTROL_ADDR_N_PHOTONS_HIGH - XPHOTON_MAXI_CONTROL_ADDR_N_PHOTONS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(char *)(InstancePtr->Control_BaseAddress + XPHOTON_MAXI_CONTROL_ADDR_N_PHOTONS_BASE + offset + i) = *(data + i);
    }
    return length;
}

u32 XPhoton_maxi_Read_n_photons_Bytes(XPhoton_maxi *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XPHOTON_MAXI_CONTROL_ADDR_N_PHOTONS_HIGH - XPHOTON_MAXI_CONTROL_ADDR_N_PHOTONS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(char *)(InstancePtr->Control_BaseAddress + XPHOTON_MAXI_CONTROL_ADDR_N_PHOTONS_BASE + offset + i);
    }
    return length;
}

void XPhoton_maxi_InterruptGlobalEnable(XPhoton_maxi *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPhoton_maxi_WriteReg(InstancePtr->Control_BaseAddress, XPHOTON_MAXI_CONTROL_ADDR_GIE, 1);
}

void XPhoton_maxi_InterruptGlobalDisable(XPhoton_maxi *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPhoton_maxi_WriteReg(InstancePtr->Control_BaseAddress, XPHOTON_MAXI_CONTROL_ADDR_GIE, 0);
}

void XPhoton_maxi_InterruptEnable(XPhoton_maxi *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XPhoton_maxi_ReadReg(InstancePtr->Control_BaseAddress, XPHOTON_MAXI_CONTROL_ADDR_IER);
    XPhoton_maxi_WriteReg(InstancePtr->Control_BaseAddress, XPHOTON_MAXI_CONTROL_ADDR_IER, Register | Mask);
}

void XPhoton_maxi_InterruptDisable(XPhoton_maxi *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XPhoton_maxi_ReadReg(InstancePtr->Control_BaseAddress, XPHOTON_MAXI_CONTROL_ADDR_IER);
    XPhoton_maxi_WriteReg(InstancePtr->Control_BaseAddress, XPHOTON_MAXI_CONTROL_ADDR_IER, Register & (~Mask));
}

void XPhoton_maxi_InterruptClear(XPhoton_maxi *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    //XPhoton_maxi_WriteReg(InstancePtr->Control_BaseAddress, XPHOTON_MAXI_CONTROL_ADDR_ISR, Mask);
}

u32 XPhoton_maxi_InterruptGetEnabled(XPhoton_maxi *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XPhoton_maxi_ReadReg(InstancePtr->Control_BaseAddress, XPHOTON_MAXI_CONTROL_ADDR_IER);
}

u32 XPhoton_maxi_InterruptGetStatus(XPhoton_maxi *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    // Current Interrupt Clear Behavior is Clear on Read(COR).
    return XPhoton_maxi_ReadReg(InstancePtr->Control_BaseAddress, XPHOTON_MAXI_CONTROL_ADDR_ISR);
}

