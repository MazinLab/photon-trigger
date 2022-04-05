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

void XPostage_Start(XPostage *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPostage_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_AP_CTRL) & 0x80;
    XPostage_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XPostage_IsDone(XPostage *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPostage_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XPostage_IsIdle(XPostage *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPostage_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XPostage_IsReady(XPostage *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPostage_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XPostage_EnableAutoRestart(XPostage *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPostage_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_AP_CTRL, 0x80);
}

void XPostage_DisableAutoRestart(XPostage *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPostage_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_AP_CTRL, 0);
}

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

void XPostage_InterruptGlobalEnable(XPostage *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPostage_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_GIE, 1);
}

void XPostage_InterruptGlobalDisable(XPostage *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPostage_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_GIE, 0);
}

void XPostage_InterruptEnable(XPostage *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XPostage_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_IER);
    XPostage_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_IER, Register | Mask);
}

void XPostage_InterruptDisable(XPostage *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XPostage_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_IER);
    XPostage_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_IER, Register & (~Mask));
}

void XPostage_InterruptClear(XPostage *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPostage_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_ISR, Mask);
}

u32 XPostage_InterruptGetEnabled(XPostage *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XPostage_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_IER);
}

u32 XPostage_InterruptGetStatus(XPostage *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XPostage_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_CONTROL_ADDR_ISR);
}

