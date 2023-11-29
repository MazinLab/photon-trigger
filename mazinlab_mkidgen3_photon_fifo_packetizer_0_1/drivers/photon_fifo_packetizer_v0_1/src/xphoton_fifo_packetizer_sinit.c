// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
// Tool Version Limit: 2022.04
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "xphoton_fifo_packetizer.h"

extern XPhoton_fifo_packetizer_Config XPhoton_fifo_packetizer_ConfigTable[];

XPhoton_fifo_packetizer_Config *XPhoton_fifo_packetizer_LookupConfig(u16 DeviceId) {
	XPhoton_fifo_packetizer_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XPHOTON_FIFO_PACKETIZER_NUM_INSTANCES; Index++) {
		if (XPhoton_fifo_packetizer_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XPhoton_fifo_packetizer_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XPhoton_fifo_packetizer_Initialize(XPhoton_fifo_packetizer *InstancePtr, u16 DeviceId) {
	XPhoton_fifo_packetizer_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XPhoton_fifo_packetizer_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XPhoton_fifo_packetizer_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

