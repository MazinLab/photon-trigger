// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
// Tool Version Limit: 2022.04
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xphoton_fifo_packetizer.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XPhoton_fifo_packetizer_CfgInitialize(XPhoton_fifo_packetizer *InstancePtr, XPhoton_fifo_packetizer_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Control_BaseAddress = ConfigPtr->Control_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XPhoton_fifo_packetizer_Set_max_photons_per_packet_minus1(XPhoton_fifo_packetizer *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPhoton_fifo_packetizer_WriteReg(InstancePtr->Control_BaseAddress, XPHOTON_FIFO_PACKETIZER_CONTROL_ADDR_MAX_PHOTONS_PER_PACKET_MINUS1_DATA, Data);
}

u32 XPhoton_fifo_packetizer_Get_max_photons_per_packet_minus1(XPhoton_fifo_packetizer *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPhoton_fifo_packetizer_ReadReg(InstancePtr->Control_BaseAddress, XPHOTON_FIFO_PACKETIZER_CONTROL_ADDR_MAX_PHOTONS_PER_PACKET_MINUS1_DATA);
    return Data;
}

void XPhoton_fifo_packetizer_Set_ticks_per_packet_minus1(XPhoton_fifo_packetizer *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XPhoton_fifo_packetizer_WriteReg(InstancePtr->Control_BaseAddress, XPHOTON_FIFO_PACKETIZER_CONTROL_ADDR_TICKS_PER_PACKET_MINUS1_DATA, Data);
}

u32 XPhoton_fifo_packetizer_Get_ticks_per_packet_minus1(XPhoton_fifo_packetizer *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XPhoton_fifo_packetizer_ReadReg(InstancePtr->Control_BaseAddress, XPHOTON_FIFO_PACKETIZER_CONTROL_ADDR_TICKS_PER_PACKET_MINUS1_DATA);
    return Data;
}

