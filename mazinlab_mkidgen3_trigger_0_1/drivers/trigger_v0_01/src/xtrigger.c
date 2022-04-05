// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2021.1 (64-bit)
// Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xtrigger.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XTrigger_CfgInitialize(XTrigger *InstancePtr, XTrigger_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XTrigger_Start(XTrigger *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTrigger_ReadReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_AP_CTRL) & 0x80;
    XTrigger_WriteReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XTrigger_IsDone(XTrigger *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTrigger_ReadReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XTrigger_IsIdle(XTrigger *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTrigger_ReadReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XTrigger_IsReady(XTrigger *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTrigger_ReadReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XTrigger_EnableAutoRestart(XTrigger *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTrigger_WriteReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_AP_CTRL, 0x80);
}

void XTrigger_DisableAutoRestart(XTrigger *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTrigger_WriteReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_AP_CTRL, 0);
}

void XTrigger_Set_holdoff(XTrigger *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTrigger_WriteReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_HOLDOFF_DATA, Data);
}

u32 XTrigger_Get_holdoff(XTrigger *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTrigger_ReadReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_HOLDOFF_DATA);
    return Data;
}

u32 XTrigger_Get_thresholds_BaseAddress(XTrigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Control_BaseAddress + XTRIGGER_CONTROL_ADDR_THRESHOLDS_BASE);
}

u32 XTrigger_Get_thresholds_HighAddress(XTrigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Control_BaseAddress + XTRIGGER_CONTROL_ADDR_THRESHOLDS_HIGH);
}

u32 XTrigger_Get_thresholds_TotalBytes(XTrigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (XTRIGGER_CONTROL_ADDR_THRESHOLDS_HIGH - XTRIGGER_CONTROL_ADDR_THRESHOLDS_BASE + 1);
}

u32 XTrigger_Get_thresholds_BitWidth(XTrigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XTRIGGER_CONTROL_WIDTH_THRESHOLDS;
}

u32 XTrigger_Get_thresholds_Depth(XTrigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XTRIGGER_CONTROL_DEPTH_THRESHOLDS;
}

u32 XTrigger_Write_thresholds_Words(XTrigger *InstancePtr, int offset, word_type *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XTRIGGER_CONTROL_ADDR_THRESHOLDS_HIGH - XTRIGGER_CONTROL_ADDR_THRESHOLDS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(int *)(InstancePtr->Control_BaseAddress + XTRIGGER_CONTROL_ADDR_THRESHOLDS_BASE + (offset + i)*4) = *(data + i);
    }
    return length;
}

u32 XTrigger_Read_thresholds_Words(XTrigger *InstancePtr, int offset, word_type *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XTRIGGER_CONTROL_ADDR_THRESHOLDS_HIGH - XTRIGGER_CONTROL_ADDR_THRESHOLDS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(int *)(InstancePtr->Control_BaseAddress + XTRIGGER_CONTROL_ADDR_THRESHOLDS_BASE + (offset + i)*4);
    }
    return length;
}

u32 XTrigger_Write_thresholds_Bytes(XTrigger *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XTRIGGER_CONTROL_ADDR_THRESHOLDS_HIGH - XTRIGGER_CONTROL_ADDR_THRESHOLDS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(char *)(InstancePtr->Control_BaseAddress + XTRIGGER_CONTROL_ADDR_THRESHOLDS_BASE + offset + i) = *(data + i);
    }
    return length;
}

u32 XTrigger_Read_thresholds_Bytes(XTrigger *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XTRIGGER_CONTROL_ADDR_THRESHOLDS_HIGH - XTRIGGER_CONTROL_ADDR_THRESHOLDS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(char *)(InstancePtr->Control_BaseAddress + XTRIGGER_CONTROL_ADDR_THRESHOLDS_BASE + offset + i);
    }
    return length;
}

void XTrigger_InterruptGlobalEnable(XTrigger *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTrigger_WriteReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_GIE, 1);
}

void XTrigger_InterruptGlobalDisable(XTrigger *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTrigger_WriteReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_GIE, 0);
}

void XTrigger_InterruptEnable(XTrigger *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XTrigger_ReadReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_IER);
    XTrigger_WriteReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_IER, Register | Mask);
}

void XTrigger_InterruptDisable(XTrigger *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XTrigger_ReadReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_IER);
    XTrigger_WriteReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_IER, Register & (~Mask));
}

void XTrigger_InterruptClear(XTrigger *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTrigger_WriteReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_ISR, Mask);
}

u32 XTrigger_InterruptGetEnabled(XTrigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XTrigger_ReadReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_IER);
}

u32 XTrigger_InterruptGetStatus(XTrigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XTrigger_ReadReg(InstancePtr->Control_BaseAddress, XTRIGGER_CONTROL_ADDR_ISR);
}

