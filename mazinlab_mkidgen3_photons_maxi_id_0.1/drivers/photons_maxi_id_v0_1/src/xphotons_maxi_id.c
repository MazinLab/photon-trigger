// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
// Tool Version Limit: 2022.04
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xphotons_maxi_id.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XPhotons_maxi_id_CfgInitialize(XPhotons_maxi_id *InstancePtr, XPhotons_maxi_id_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XPhotons_maxi_id_Start(XPhotons_maxi_id *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPhotons_maxi_id_ReadReg(InstancePtr->Control_BaseAddress, XPHOTONS_MAXI_ID_CONTROL_ADDR_AP_CTRL) & 0x80;
    XPhotons_maxi_id_WriteReg(InstancePtr->Control_BaseAddress, XPHOTONS_MAXI_ID_CONTROL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XPhotons_maxi_id_IsDone(XPhotons_maxi_id *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPhotons_maxi_id_ReadReg(InstancePtr->Control_BaseAddress, XPHOTONS_MAXI_ID_CONTROL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XPhotons_maxi_id_IsIdle(XPhotons_maxi_id *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPhotons_maxi_id_ReadReg(InstancePtr->Control_BaseAddress, XPHOTONS_MAXI_ID_CONTROL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XPhotons_maxi_id_IsReady(XPhotons_maxi_id *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPhotons_maxi_id_ReadReg(InstancePtr->Control_BaseAddress, XPHOTONS_MAXI_ID_CONTROL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XPhotons_maxi_id_EnableAutoRestart(XPhotons_maxi_id *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPhotons_maxi_id_WriteReg(InstancePtr->Control_BaseAddress, XPHOTONS_MAXI_ID_CONTROL_ADDR_AP_CTRL, 0x80);
}

void XPhotons_maxi_id_DisableAutoRestart(XPhotons_maxi_id *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPhotons_maxi_id_WriteReg(InstancePtr->Control_BaseAddress, XPHOTONS_MAXI_ID_CONTROL_ADDR_AP_CTRL, 0);
}

void XPhotons_maxi_id_Set_photons_out(XPhotons_maxi_id *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPhotons_maxi_id_WriteReg(InstancePtr->Control_BaseAddress, XPHOTONS_MAXI_ID_CONTROL_ADDR_PHOTONS_OUT_DATA, (u32)(Data));
    XPhotons_maxi_id_WriteReg(InstancePtr->Control_BaseAddress, XPHOTONS_MAXI_ID_CONTROL_ADDR_PHOTONS_OUT_DATA + 4, (u32)(Data >> 32));
}

u64 XPhotons_maxi_id_Get_photons_out(XPhotons_maxi_id *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPhotons_maxi_id_ReadReg(InstancePtr->Control_BaseAddress, XPHOTONS_MAXI_ID_CONTROL_ADDR_PHOTONS_OUT_DATA);
    Data += (u64)XPhotons_maxi_id_ReadReg(InstancePtr->Control_BaseAddress, XPHOTONS_MAXI_ID_CONTROL_ADDR_PHOTONS_OUT_DATA + 4) << 32;
    return Data;
}

u32 XPhotons_maxi_id_Get_active_buffer(XPhotons_maxi_id *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPhotons_maxi_id_ReadReg(InstancePtr->Control_BaseAddress, XPHOTONS_MAXI_ID_CONTROL_ADDR_ACTIVE_BUFFER_DATA);
    return Data;
}

u32 XPhotons_maxi_id_Get_active_buffer_vld(XPhotons_maxi_id *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPhotons_maxi_id_ReadReg(InstancePtr->Control_BaseAddress, XPHOTONS_MAXI_ID_CONTROL_ADDR_ACTIVE_BUFFER_CTRL);
    return Data & 0x1;
}

u32 XPhotons_maxi_id_Get_n_photons_BaseAddress(XPhotons_maxi_id *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Control_BaseAddress + XPHOTONS_MAXI_ID_CONTROL_ADDR_N_PHOTONS_BASE);
}

u32 XPhotons_maxi_id_Get_n_photons_HighAddress(XPhotons_maxi_id *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Control_BaseAddress + XPHOTONS_MAXI_ID_CONTROL_ADDR_N_PHOTONS_HIGH);
}

u32 XPhotons_maxi_id_Get_n_photons_TotalBytes(XPhotons_maxi_id *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (XPHOTONS_MAXI_ID_CONTROL_ADDR_N_PHOTONS_HIGH - XPHOTONS_MAXI_ID_CONTROL_ADDR_N_PHOTONS_BASE + 1);
}

u32 XPhotons_maxi_id_Get_n_photons_BitWidth(XPhotons_maxi_id *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XPHOTONS_MAXI_ID_CONTROL_WIDTH_N_PHOTONS;
}

u32 XPhotons_maxi_id_Get_n_photons_Depth(XPhotons_maxi_id *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XPHOTONS_MAXI_ID_CONTROL_DEPTH_N_PHOTONS;
}

u32 XPhotons_maxi_id_Write_n_photons_Words(XPhotons_maxi_id *InstancePtr, int offset, word_type *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XPHOTONS_MAXI_ID_CONTROL_ADDR_N_PHOTONS_HIGH - XPHOTONS_MAXI_ID_CONTROL_ADDR_N_PHOTONS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(int *)(InstancePtr->Control_BaseAddress + XPHOTONS_MAXI_ID_CONTROL_ADDR_N_PHOTONS_BASE + (offset + i)*4) = *(data + i);
    }
    return length;
}

u32 XPhotons_maxi_id_Read_n_photons_Words(XPhotons_maxi_id *InstancePtr, int offset, word_type *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XPHOTONS_MAXI_ID_CONTROL_ADDR_N_PHOTONS_HIGH - XPHOTONS_MAXI_ID_CONTROL_ADDR_N_PHOTONS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(int *)(InstancePtr->Control_BaseAddress + XPHOTONS_MAXI_ID_CONTROL_ADDR_N_PHOTONS_BASE + (offset + i)*4);
    }
    return length;
}

u32 XPhotons_maxi_id_Write_n_photons_Bytes(XPhotons_maxi_id *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XPHOTONS_MAXI_ID_CONTROL_ADDR_N_PHOTONS_HIGH - XPHOTONS_MAXI_ID_CONTROL_ADDR_N_PHOTONS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(char *)(InstancePtr->Control_BaseAddress + XPHOTONS_MAXI_ID_CONTROL_ADDR_N_PHOTONS_BASE + offset + i) = *(data + i);
    }
    return length;
}

u32 XPhotons_maxi_id_Read_n_photons_Bytes(XPhotons_maxi_id *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XPHOTONS_MAXI_ID_CONTROL_ADDR_N_PHOTONS_HIGH - XPHOTONS_MAXI_ID_CONTROL_ADDR_N_PHOTONS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(char *)(InstancePtr->Control_BaseAddress + XPHOTONS_MAXI_ID_CONTROL_ADDR_N_PHOTONS_BASE + offset + i);
    }
    return length;
}

void XPhotons_maxi_id_InterruptGlobalEnable(XPhotons_maxi_id *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPhotons_maxi_id_WriteReg(InstancePtr->Control_BaseAddress, XPHOTONS_MAXI_ID_CONTROL_ADDR_GIE, 1);
}

void XPhotons_maxi_id_InterruptGlobalDisable(XPhotons_maxi_id *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPhotons_maxi_id_WriteReg(InstancePtr->Control_BaseAddress, XPHOTONS_MAXI_ID_CONTROL_ADDR_GIE, 0);
}

void XPhotons_maxi_id_InterruptEnable(XPhotons_maxi_id *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XPhotons_maxi_id_ReadReg(InstancePtr->Control_BaseAddress, XPHOTONS_MAXI_ID_CONTROL_ADDR_IER);
    XPhotons_maxi_id_WriteReg(InstancePtr->Control_BaseAddress, XPHOTONS_MAXI_ID_CONTROL_ADDR_IER, Register | Mask);
}

void XPhotons_maxi_id_InterruptDisable(XPhotons_maxi_id *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XPhotons_maxi_id_ReadReg(InstancePtr->Control_BaseAddress, XPHOTONS_MAXI_ID_CONTROL_ADDR_IER);
    XPhotons_maxi_id_WriteReg(InstancePtr->Control_BaseAddress, XPHOTONS_MAXI_ID_CONTROL_ADDR_IER, Register & (~Mask));
}

void XPhotons_maxi_id_InterruptClear(XPhotons_maxi_id *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    //XPhotons_maxi_id_WriteReg(InstancePtr->Control_BaseAddress, XPHOTONS_MAXI_ID_CONTROL_ADDR_ISR, Mask);
}

u32 XPhotons_maxi_id_InterruptGetEnabled(XPhotons_maxi_id *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XPhotons_maxi_id_ReadReg(InstancePtr->Control_BaseAddress, XPHOTONS_MAXI_ID_CONTROL_ADDR_IER);
}

u32 XPhotons_maxi_id_InterruptGetStatus(XPhotons_maxi_id *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    // Current Interrupt Clear Behavior is Clear on Read(COR).
    return XPhotons_maxi_id_ReadReg(InstancePtr->Control_BaseAddress, XPHOTONS_MAXI_ID_CONTROL_ADDR_ISR);
}

