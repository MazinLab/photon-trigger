// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
// Tool Version Limit: 2022.04
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xpostage_filter_w_interconn.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XPostage_filter_w_interconn_CfgInitialize(XPostage_filter_w_interconn *InstancePtr, XPostage_filter_w_interconn_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XPostage_filter_w_interconn_Set_monitor_0(XPostage_filter_w_interconn *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPostage_filter_w_interconn_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_FILTER_W_INTERCONN_CONTROL_ADDR_MONITOR_0_DATA, Data);
}

u32 XPostage_filter_w_interconn_Get_monitor_0(XPostage_filter_w_interconn *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPostage_filter_w_interconn_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_FILTER_W_INTERCONN_CONTROL_ADDR_MONITOR_0_DATA);
    return Data;
}

void XPostage_filter_w_interconn_Set_monitor_1(XPostage_filter_w_interconn *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPostage_filter_w_interconn_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_FILTER_W_INTERCONN_CONTROL_ADDR_MONITOR_1_DATA, Data);
}

u32 XPostage_filter_w_interconn_Get_monitor_1(XPostage_filter_w_interconn *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPostage_filter_w_interconn_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_FILTER_W_INTERCONN_CONTROL_ADDR_MONITOR_1_DATA);
    return Data;
}

void XPostage_filter_w_interconn_Set_monitor_2(XPostage_filter_w_interconn *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPostage_filter_w_interconn_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_FILTER_W_INTERCONN_CONTROL_ADDR_MONITOR_2_DATA, Data);
}

u32 XPostage_filter_w_interconn_Get_monitor_2(XPostage_filter_w_interconn *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPostage_filter_w_interconn_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_FILTER_W_INTERCONN_CONTROL_ADDR_MONITOR_2_DATA);
    return Data;
}

void XPostage_filter_w_interconn_Set_monitor_3(XPostage_filter_w_interconn *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPostage_filter_w_interconn_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_FILTER_W_INTERCONN_CONTROL_ADDR_MONITOR_3_DATA, Data);
}

u32 XPostage_filter_w_interconn_Get_monitor_3(XPostage_filter_w_interconn *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPostage_filter_w_interconn_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_FILTER_W_INTERCONN_CONTROL_ADDR_MONITOR_3_DATA);
    return Data;
}

void XPostage_filter_w_interconn_Set_monitor_4(XPostage_filter_w_interconn *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPostage_filter_w_interconn_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_FILTER_W_INTERCONN_CONTROL_ADDR_MONITOR_4_DATA, Data);
}

u32 XPostage_filter_w_interconn_Get_monitor_4(XPostage_filter_w_interconn *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPostage_filter_w_interconn_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_FILTER_W_INTERCONN_CONTROL_ADDR_MONITOR_4_DATA);
    return Data;
}

void XPostage_filter_w_interconn_Set_monitor_5(XPostage_filter_w_interconn *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPostage_filter_w_interconn_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_FILTER_W_INTERCONN_CONTROL_ADDR_MONITOR_5_DATA, Data);
}

u32 XPostage_filter_w_interconn_Get_monitor_5(XPostage_filter_w_interconn *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPostage_filter_w_interconn_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_FILTER_W_INTERCONN_CONTROL_ADDR_MONITOR_5_DATA);
    return Data;
}

void XPostage_filter_w_interconn_Set_monitor_6(XPostage_filter_w_interconn *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPostage_filter_w_interconn_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_FILTER_W_INTERCONN_CONTROL_ADDR_MONITOR_6_DATA, Data);
}

u32 XPostage_filter_w_interconn_Get_monitor_6(XPostage_filter_w_interconn *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPostage_filter_w_interconn_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_FILTER_W_INTERCONN_CONTROL_ADDR_MONITOR_6_DATA);
    return Data;
}

void XPostage_filter_w_interconn_Set_monitor_7(XPostage_filter_w_interconn *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPostage_filter_w_interconn_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_FILTER_W_INTERCONN_CONTROL_ADDR_MONITOR_7_DATA, Data);
}

u32 XPostage_filter_w_interconn_Get_monitor_7(XPostage_filter_w_interconn *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPostage_filter_w_interconn_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_FILTER_W_INTERCONN_CONTROL_ADDR_MONITOR_7_DATA);
    return Data;
}

