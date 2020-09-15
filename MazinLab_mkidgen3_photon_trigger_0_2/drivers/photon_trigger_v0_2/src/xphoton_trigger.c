// ==============================================================
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2019.2.1 (64-bit)
// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xphoton_trigger.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XPhoton_trigger_CfgInitialize(XPhoton_trigger *InstancePtr, XPhoton_trigger_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XPhoton_trigger_Set_holdoff_V(XPhoton_trigger *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPhoton_trigger_WriteReg(InstancePtr->Control_BaseAddress, XPHOTON_TRIGGER_CONTROL_ADDR_HOLDOFF_V_DATA, Data);
}

u32 XPhoton_trigger_Get_holdoff_V(XPhoton_trigger *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPhoton_trigger_ReadReg(InstancePtr->Control_BaseAddress, XPHOTON_TRIGGER_CONTROL_ADDR_HOLDOFF_V_DATA);
    return Data;
}

void XPhoton_trigger_Set_baseline_shrink_V(XPhoton_trigger *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPhoton_trigger_WriteReg(InstancePtr->Control_BaseAddress, XPHOTON_TRIGGER_CONTROL_ADDR_BASELINE_SHRINK_V_DATA, Data);
}

u32 XPhoton_trigger_Get_baseline_shrink_V(XPhoton_trigger *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPhoton_trigger_ReadReg(InstancePtr->Control_BaseAddress, XPHOTON_TRIGGER_CONTROL_ADDR_BASELINE_SHRINK_V_DATA);
    return Data;
}

void XPhoton_trigger_Set_baseline_grow_V(XPhoton_trigger *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPhoton_trigger_WriteReg(InstancePtr->Control_BaseAddress, XPHOTON_TRIGGER_CONTROL_ADDR_BASELINE_GROW_V_DATA, Data);
}

u32 XPhoton_trigger_Get_baseline_grow_V(XPhoton_trigger *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPhoton_trigger_ReadReg(InstancePtr->Control_BaseAddress, XPHOTON_TRIGGER_CONTROL_ADDR_BASELINE_GROW_V_DATA);
    return Data;
}

void XPhoton_trigger_Set_dropped(XPhoton_trigger *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPhoton_trigger_WriteReg(InstancePtr->Control_BaseAddress, XPHOTON_TRIGGER_CONTROL_ADDR_DROPPED_DATA, Data);
}

u32 XPhoton_trigger_Get_dropped(XPhoton_trigger *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPhoton_trigger_ReadReg(InstancePtr->Control_BaseAddress, XPHOTON_TRIGGER_CONTROL_ADDR_DROPPED_DATA);
    return Data;
}

u32 XPhoton_trigger_Get_triggercfg_0_threshold_V_BaseAddress(XPhoton_trigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Control_BaseAddress + XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_0_THRESHOLD_V_BASE);
}

u32 XPhoton_trigger_Get_triggercfg_0_threshold_V_HighAddress(XPhoton_trigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Control_BaseAddress + XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_0_THRESHOLD_V_HIGH);
}

u32 XPhoton_trigger_Get_triggercfg_0_threshold_V_TotalBytes(XPhoton_trigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_0_THRESHOLD_V_HIGH - XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_0_THRESHOLD_V_BASE + 1);
}

u32 XPhoton_trigger_Get_triggercfg_0_threshold_V_BitWidth(XPhoton_trigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XPHOTON_TRIGGER_CONTROL_WIDTH_TRIGGERCFG_0_THRESHOLD_V;
}

u32 XPhoton_trigger_Get_triggercfg_0_threshold_V_Depth(XPhoton_trigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XPHOTON_TRIGGER_CONTROL_DEPTH_TRIGGERCFG_0_THRESHOLD_V;
}

u32 XPhoton_trigger_Write_triggercfg_0_threshold_V_Words(XPhoton_trigger *InstancePtr, int offset, int *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_0_THRESHOLD_V_HIGH - XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_0_THRESHOLD_V_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(int *)(InstancePtr->Control_BaseAddress + XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_0_THRESHOLD_V_BASE + (offset + i)*4) = *(data + i);
    }
    return length;
}

u32 XPhoton_trigger_Read_triggercfg_0_threshold_V_Words(XPhoton_trigger *InstancePtr, int offset, int *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_0_THRESHOLD_V_HIGH - XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_0_THRESHOLD_V_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(int *)(InstancePtr->Control_BaseAddress + XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_0_THRESHOLD_V_BASE + (offset + i)*4);
    }
    return length;
}

u32 XPhoton_trigger_Write_triggercfg_0_threshold_V_Bytes(XPhoton_trigger *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_0_THRESHOLD_V_HIGH - XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_0_THRESHOLD_V_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(char *)(InstancePtr->Control_BaseAddress + XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_0_THRESHOLD_V_BASE + offset + i) = *(data + i);
    }
    return length;
}

u32 XPhoton_trigger_Read_triggercfg_0_threshold_V_Bytes(XPhoton_trigger *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_0_THRESHOLD_V_HIGH - XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_0_THRESHOLD_V_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(char *)(InstancePtr->Control_BaseAddress + XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_0_THRESHOLD_V_BASE + offset + i);
    }
    return length;
}

u32 XPhoton_trigger_Get_triggercfg_1_threshold_V_BaseAddress(XPhoton_trigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Control_BaseAddress + XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_1_THRESHOLD_V_BASE);
}

u32 XPhoton_trigger_Get_triggercfg_1_threshold_V_HighAddress(XPhoton_trigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Control_BaseAddress + XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_1_THRESHOLD_V_HIGH);
}

u32 XPhoton_trigger_Get_triggercfg_1_threshold_V_TotalBytes(XPhoton_trigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_1_THRESHOLD_V_HIGH - XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_1_THRESHOLD_V_BASE + 1);
}

u32 XPhoton_trigger_Get_triggercfg_1_threshold_V_BitWidth(XPhoton_trigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XPHOTON_TRIGGER_CONTROL_WIDTH_TRIGGERCFG_1_THRESHOLD_V;
}

u32 XPhoton_trigger_Get_triggercfg_1_threshold_V_Depth(XPhoton_trigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XPHOTON_TRIGGER_CONTROL_DEPTH_TRIGGERCFG_1_THRESHOLD_V;
}

u32 XPhoton_trigger_Write_triggercfg_1_threshold_V_Words(XPhoton_trigger *InstancePtr, int offset, int *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_1_THRESHOLD_V_HIGH - XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_1_THRESHOLD_V_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(int *)(InstancePtr->Control_BaseAddress + XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_1_THRESHOLD_V_BASE + (offset + i)*4) = *(data + i);
    }
    return length;
}

u32 XPhoton_trigger_Read_triggercfg_1_threshold_V_Words(XPhoton_trigger *InstancePtr, int offset, int *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_1_THRESHOLD_V_HIGH - XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_1_THRESHOLD_V_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(int *)(InstancePtr->Control_BaseAddress + XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_1_THRESHOLD_V_BASE + (offset + i)*4);
    }
    return length;
}

u32 XPhoton_trigger_Write_triggercfg_1_threshold_V_Bytes(XPhoton_trigger *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_1_THRESHOLD_V_HIGH - XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_1_THRESHOLD_V_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(char *)(InstancePtr->Control_BaseAddress + XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_1_THRESHOLD_V_BASE + offset + i) = *(data + i);
    }
    return length;
}

u32 XPhoton_trigger_Read_triggercfg_1_threshold_V_Bytes(XPhoton_trigger *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_1_THRESHOLD_V_HIGH - XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_1_THRESHOLD_V_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(char *)(InstancePtr->Control_BaseAddress + XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_1_THRESHOLD_V_BASE + offset + i);
    }
    return length;
}

u32 XPhoton_trigger_Get_triggercfg_2_threshold_V_BaseAddress(XPhoton_trigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Control_BaseAddress + XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_2_THRESHOLD_V_BASE);
}

u32 XPhoton_trigger_Get_triggercfg_2_threshold_V_HighAddress(XPhoton_trigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Control_BaseAddress + XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_2_THRESHOLD_V_HIGH);
}

u32 XPhoton_trigger_Get_triggercfg_2_threshold_V_TotalBytes(XPhoton_trigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_2_THRESHOLD_V_HIGH - XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_2_THRESHOLD_V_BASE + 1);
}

u32 XPhoton_trigger_Get_triggercfg_2_threshold_V_BitWidth(XPhoton_trigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XPHOTON_TRIGGER_CONTROL_WIDTH_TRIGGERCFG_2_THRESHOLD_V;
}

u32 XPhoton_trigger_Get_triggercfg_2_threshold_V_Depth(XPhoton_trigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XPHOTON_TRIGGER_CONTROL_DEPTH_TRIGGERCFG_2_THRESHOLD_V;
}

u32 XPhoton_trigger_Write_triggercfg_2_threshold_V_Words(XPhoton_trigger *InstancePtr, int offset, int *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_2_THRESHOLD_V_HIGH - XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_2_THRESHOLD_V_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(int *)(InstancePtr->Control_BaseAddress + XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_2_THRESHOLD_V_BASE + (offset + i)*4) = *(data + i);
    }
    return length;
}

u32 XPhoton_trigger_Read_triggercfg_2_threshold_V_Words(XPhoton_trigger *InstancePtr, int offset, int *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_2_THRESHOLD_V_HIGH - XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_2_THRESHOLD_V_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(int *)(InstancePtr->Control_BaseAddress + XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_2_THRESHOLD_V_BASE + (offset + i)*4);
    }
    return length;
}

u32 XPhoton_trigger_Write_triggercfg_2_threshold_V_Bytes(XPhoton_trigger *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_2_THRESHOLD_V_HIGH - XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_2_THRESHOLD_V_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(char *)(InstancePtr->Control_BaseAddress + XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_2_THRESHOLD_V_BASE + offset + i) = *(data + i);
    }
    return length;
}

u32 XPhoton_trigger_Read_triggercfg_2_threshold_V_Bytes(XPhoton_trigger *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_2_THRESHOLD_V_HIGH - XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_2_THRESHOLD_V_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(char *)(InstancePtr->Control_BaseAddress + XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_2_THRESHOLD_V_BASE + offset + i);
    }
    return length;
}

u32 XPhoton_trigger_Get_triggercfg_3_threshold_V_BaseAddress(XPhoton_trigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Control_BaseAddress + XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_3_THRESHOLD_V_BASE);
}

u32 XPhoton_trigger_Get_triggercfg_3_threshold_V_HighAddress(XPhoton_trigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Control_BaseAddress + XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_3_THRESHOLD_V_HIGH);
}

u32 XPhoton_trigger_Get_triggercfg_3_threshold_V_TotalBytes(XPhoton_trigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_3_THRESHOLD_V_HIGH - XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_3_THRESHOLD_V_BASE + 1);
}

u32 XPhoton_trigger_Get_triggercfg_3_threshold_V_BitWidth(XPhoton_trigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XPHOTON_TRIGGER_CONTROL_WIDTH_TRIGGERCFG_3_THRESHOLD_V;
}

u32 XPhoton_trigger_Get_triggercfg_3_threshold_V_Depth(XPhoton_trigger *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XPHOTON_TRIGGER_CONTROL_DEPTH_TRIGGERCFG_3_THRESHOLD_V;
}

u32 XPhoton_trigger_Write_triggercfg_3_threshold_V_Words(XPhoton_trigger *InstancePtr, int offset, int *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_3_THRESHOLD_V_HIGH - XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_3_THRESHOLD_V_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(int *)(InstancePtr->Control_BaseAddress + XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_3_THRESHOLD_V_BASE + (offset + i)*4) = *(data + i);
    }
    return length;
}

u32 XPhoton_trigger_Read_triggercfg_3_threshold_V_Words(XPhoton_trigger *InstancePtr, int offset, int *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_3_THRESHOLD_V_HIGH - XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_3_THRESHOLD_V_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(int *)(InstancePtr->Control_BaseAddress + XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_3_THRESHOLD_V_BASE + (offset + i)*4);
    }
    return length;
}

u32 XPhoton_trigger_Write_triggercfg_3_threshold_V_Bytes(XPhoton_trigger *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_3_THRESHOLD_V_HIGH - XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_3_THRESHOLD_V_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(char *)(InstancePtr->Control_BaseAddress + XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_3_THRESHOLD_V_BASE + offset + i) = *(data + i);
    }
    return length;
}

u32 XPhoton_trigger_Read_triggercfg_3_threshold_V_Bytes(XPhoton_trigger *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_3_THRESHOLD_V_HIGH - XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_3_THRESHOLD_V_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(char *)(InstancePtr->Control_BaseAddress + XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_3_THRESHOLD_V_BASE + offset + i);
    }
    return length;
}

