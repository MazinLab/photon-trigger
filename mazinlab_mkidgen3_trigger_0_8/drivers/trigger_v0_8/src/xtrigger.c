// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
// Tool Version Limit: 2022.04
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
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

u32 XTrigger_Get_threshoffs_BaseAddress(XTrigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Control_BaseAddress + XTRIGGER_CONTROL_ADDR_THRESHOFFS_BASE);
}

u32 XTrigger_Get_threshoffs_HighAddress(XTrigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Control_BaseAddress + XTRIGGER_CONTROL_ADDR_THRESHOFFS_HIGH);
}

u32 XTrigger_Get_threshoffs_TotalBytes(XTrigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (XTRIGGER_CONTROL_ADDR_THRESHOFFS_HIGH - XTRIGGER_CONTROL_ADDR_THRESHOFFS_BASE + 1);
}

u32 XTrigger_Get_threshoffs_BitWidth(XTrigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XTRIGGER_CONTROL_WIDTH_THRESHOFFS;
}

u32 XTrigger_Get_threshoffs_Depth(XTrigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XTRIGGER_CONTROL_DEPTH_THRESHOFFS;
}

u32 XTrigger_Write_threshoffs_Words(XTrigger *InstancePtr, int offset, word_type *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XTRIGGER_CONTROL_ADDR_THRESHOFFS_HIGH - XTRIGGER_CONTROL_ADDR_THRESHOFFS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(int *)(InstancePtr->Control_BaseAddress + XTRIGGER_CONTROL_ADDR_THRESHOFFS_BASE + (offset + i)*4) = *(data + i);
    }
    return length;
}

u32 XTrigger_Read_threshoffs_Words(XTrigger *InstancePtr, int offset, word_type *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XTRIGGER_CONTROL_ADDR_THRESHOFFS_HIGH - XTRIGGER_CONTROL_ADDR_THRESHOFFS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(int *)(InstancePtr->Control_BaseAddress + XTRIGGER_CONTROL_ADDR_THRESHOFFS_BASE + (offset + i)*4);
    }
    return length;
}

u32 XTrigger_Write_threshoffs_Bytes(XTrigger *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XTRIGGER_CONTROL_ADDR_THRESHOFFS_HIGH - XTRIGGER_CONTROL_ADDR_THRESHOFFS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(char *)(InstancePtr->Control_BaseAddress + XTRIGGER_CONTROL_ADDR_THRESHOFFS_BASE + offset + i) = *(data + i);
    }
    return length;
}

u32 XTrigger_Read_threshoffs_Bytes(XTrigger *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XTRIGGER_CONTROL_ADDR_THRESHOFFS_HIGH - XTRIGGER_CONTROL_ADDR_THRESHOFFS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(char *)(InstancePtr->Control_BaseAddress + XTRIGGER_CONTROL_ADDR_THRESHOFFS_BASE + offset + i);
    }
    return length;
}

