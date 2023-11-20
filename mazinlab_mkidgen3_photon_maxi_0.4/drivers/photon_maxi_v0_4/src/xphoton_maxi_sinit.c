// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
// Tool Version Limit: 2022.04
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "xphoton_maxi.h"

extern XPhoton_maxi_Config XPhoton_maxi_ConfigTable[];

XPhoton_maxi_Config *XPhoton_maxi_LookupConfig(u16 DeviceId) {
	XPhoton_maxi_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XPHOTON_MAXI_NUM_INSTANCES; Index++) {
		if (XPhoton_maxi_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XPhoton_maxi_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XPhoton_maxi_Initialize(XPhoton_maxi *InstancePtr, u16 DeviceId) {
	XPhoton_maxi_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XPhoton_maxi_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XPhoton_maxi_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

