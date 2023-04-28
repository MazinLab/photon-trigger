// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
// Tool Version Limit: 2022.04
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xpostage_maxi.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XPostage_maxi_CfgInitialize(XPostage_maxi *InstancePtr, XPostage_maxi_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XPostage_maxi_Start(XPostage_maxi *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPostage_maxi_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_MAXI_CONTROL_ADDR_AP_CTRL) & 0x80;
    XPostage_maxi_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_MAXI_CONTROL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XPostage_maxi_IsDone(XPostage_maxi *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPostage_maxi_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_MAXI_CONTROL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XPostage_maxi_IsIdle(XPostage_maxi *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPostage_maxi_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_MAXI_CONTROL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XPostage_maxi_IsReady(XPostage_maxi *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPostage_maxi_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_MAXI_CONTROL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XPostage_maxi_EnableAutoRestart(XPostage_maxi *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPostage_maxi_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_MAXI_CONTROL_ADDR_AP_CTRL, 0x80);
}

void XPostage_maxi_DisableAutoRestart(XPostage_maxi *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPostage_maxi_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_MAXI_CONTROL_ADDR_AP_CTRL, 0);
}

void XPostage_maxi_Set_iq(XPostage_maxi *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPostage_maxi_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_MAXI_CONTROL_ADDR_IQ_DATA, (u32)(Data));
    XPostage_maxi_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_MAXI_CONTROL_ADDR_IQ_DATA + 4, (u32)(Data >> 32));
}

u64 XPostage_maxi_Get_iq(XPostage_maxi *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPostage_maxi_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_MAXI_CONTROL_ADDR_IQ_DATA);
    Data += (u64)XPostage_maxi_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_MAXI_CONTROL_ADDR_IQ_DATA + 4) << 32;
    return Data;
}

void XPostage_maxi_Set_event_count(XPostage_maxi *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPostage_maxi_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_MAXI_CONTROL_ADDR_EVENT_COUNT_DATA, Data);
}

u32 XPostage_maxi_Get_event_count(XPostage_maxi *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPostage_maxi_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_MAXI_CONTROL_ADDR_EVENT_COUNT_DATA);
    return Data;
}

void XPostage_maxi_Set_max_events(XPostage_maxi *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPostage_maxi_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_MAXI_CONTROL_ADDR_MAX_EVENTS_DATA, Data);
}

u32 XPostage_maxi_Get_max_events(XPostage_maxi *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPostage_maxi_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_MAXI_CONTROL_ADDR_MAX_EVENTS_DATA);
    return Data;
}

void XPostage_maxi_InterruptGlobalEnable(XPostage_maxi *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPostage_maxi_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_MAXI_CONTROL_ADDR_GIE, 1);
}

void XPostage_maxi_InterruptGlobalDisable(XPostage_maxi *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPostage_maxi_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_MAXI_CONTROL_ADDR_GIE, 0);
}

void XPostage_maxi_InterruptEnable(XPostage_maxi *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XPostage_maxi_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_MAXI_CONTROL_ADDR_IER);
    XPostage_maxi_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_MAXI_CONTROL_ADDR_IER, Register | Mask);
}

void XPostage_maxi_InterruptDisable(XPostage_maxi *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XPostage_maxi_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_MAXI_CONTROL_ADDR_IER);
    XPostage_maxi_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_MAXI_CONTROL_ADDR_IER, Register & (~Mask));
}

void XPostage_maxi_InterruptClear(XPostage_maxi *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    //XPostage_maxi_WriteReg(InstancePtr->Control_BaseAddress, XPOSTAGE_MAXI_CONTROL_ADDR_ISR, Mask);
}

u32 XPostage_maxi_InterruptGetEnabled(XPostage_maxi *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XPostage_maxi_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_MAXI_CONTROL_ADDR_IER);
}

u32 XPostage_maxi_InterruptGetStatus(XPostage_maxi *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    // Current Interrupt Clear Behavior is Clear on Read(COR).
    return XPostage_maxi_ReadReg(InstancePtr->Control_BaseAddress, XPOSTAGE_MAXI_CONTROL_ADDR_ISR);
}

