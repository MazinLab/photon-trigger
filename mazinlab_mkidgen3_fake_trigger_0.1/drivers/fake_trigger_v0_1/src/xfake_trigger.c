// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
// Tool Version Limit: 2022.04
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xfake_trigger.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XFake_trigger_CfgInitialize(XFake_trigger *InstancePtr, XFake_trigger_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

u32 XFake_trigger_Get_threshoffs_BaseAddress(XFake_trigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Control_BaseAddress + XFAKE_TRIGGER_CONTROL_ADDR_THRESHOFFS_BASE);
}

u32 XFake_trigger_Get_threshoffs_HighAddress(XFake_trigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Control_BaseAddress + XFAKE_TRIGGER_CONTROL_ADDR_THRESHOFFS_HIGH);
}

u32 XFake_trigger_Get_threshoffs_TotalBytes(XFake_trigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (XFAKE_TRIGGER_CONTROL_ADDR_THRESHOFFS_HIGH - XFAKE_TRIGGER_CONTROL_ADDR_THRESHOFFS_BASE + 1);
}

u32 XFake_trigger_Get_threshoffs_BitWidth(XFake_trigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XFAKE_TRIGGER_CONTROL_WIDTH_THRESHOFFS;
}

u32 XFake_trigger_Get_threshoffs_Depth(XFake_trigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XFAKE_TRIGGER_CONTROL_DEPTH_THRESHOFFS;
}

u32 XFake_trigger_Write_threshoffs_Words(XFake_trigger *InstancePtr, int offset, word_type *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XFAKE_TRIGGER_CONTROL_ADDR_THRESHOFFS_HIGH - XFAKE_TRIGGER_CONTROL_ADDR_THRESHOFFS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(int *)(InstancePtr->Control_BaseAddress + XFAKE_TRIGGER_CONTROL_ADDR_THRESHOFFS_BASE + (offset + i)*4) = *(data + i);
    }
    return length;
}

u32 XFake_trigger_Read_threshoffs_Words(XFake_trigger *InstancePtr, int offset, word_type *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XFAKE_TRIGGER_CONTROL_ADDR_THRESHOFFS_HIGH - XFAKE_TRIGGER_CONTROL_ADDR_THRESHOFFS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(int *)(InstancePtr->Control_BaseAddress + XFAKE_TRIGGER_CONTROL_ADDR_THRESHOFFS_BASE + (offset + i)*4);
    }
    return length;
}

u32 XFake_trigger_Write_threshoffs_Bytes(XFake_trigger *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XFAKE_TRIGGER_CONTROL_ADDR_THRESHOFFS_HIGH - XFAKE_TRIGGER_CONTROL_ADDR_THRESHOFFS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(char *)(InstancePtr->Control_BaseAddress + XFAKE_TRIGGER_CONTROL_ADDR_THRESHOFFS_BASE + offset + i) = *(data + i);
    }
    return length;
}

u32 XFake_trigger_Read_threshoffs_Bytes(XFake_trigger *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XFAKE_TRIGGER_CONTROL_ADDR_THRESHOFFS_HIGH - XFAKE_TRIGGER_CONTROL_ADDR_THRESHOFFS_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(char *)(InstancePtr->Control_BaseAddress + XFAKE_TRIGGER_CONTROL_ADDR_THRESHOFFS_BASE + offset + i);
    }
    return length;
}

