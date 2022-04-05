// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2021.1 (64-bit)
// Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xpostage.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XPostage_CfgInitialize(XPostage *InstancePtr, XPostage_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XPostage_Set_monitor_0(XPostage *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPostage_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_MONITOR_0_DATA, Data);
}

u32 XPostage_Get_monitor_0(XPostage *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPostage_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_MONITOR_0_DATA);
    return Data;
}

void XPostage_Set_monitor_1(XPostage *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPostage_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_MONITOR_1_DATA, Data);
}

u32 XPostage_Get_monitor_1(XPostage *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPostage_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_MONITOR_1_DATA);
    return Data;
}

void XPostage_Set_monitor_2(XPostage *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPostage_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_MONITOR_2_DATA, Data);
}

u32 XPostage_Get_monitor_2(XPostage *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPostage_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_MONITOR_2_DATA);
    return Data;
}

void XPostage_Set_monitor_3(XPostage *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPostage_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_MONITOR_3_DATA, Data);
}

u32 XPostage_Get_monitor_3(XPostage *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPostage_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_MONITOR_3_DATA);
    return Data;
}

void XPostage_Set_monitor_4(XPostage *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPostage_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_MONITOR_4_DATA, Data);
}

u32 XPostage_Get_monitor_4(XPostage *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPostage_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_MONITOR_4_DATA);
    return Data;
}

void XPostage_Set_monitor_5(XPostage *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPostage_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_MONITOR_5_DATA, Data);
}

u32 XPostage_Get_monitor_5(XPostage *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPostage_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_MONITOR_5_DATA);
    return Data;
}

void XPostage_Set_monitor_6(XPostage *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPostage_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_MONITOR_6_DATA, Data);
}

u32 XPostage_Get_monitor_6(XPostage *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPostage_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_MONITOR_6_DATA);
    return Data;
}

void XPostage_Set_monitor_7(XPostage *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPostage_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_MONITOR_7_DATA, Data);
}

u32 XPostage_Get_monitor_7(XPostage *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPostage_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_MONITOR_7_DATA);
    return Data;
}

