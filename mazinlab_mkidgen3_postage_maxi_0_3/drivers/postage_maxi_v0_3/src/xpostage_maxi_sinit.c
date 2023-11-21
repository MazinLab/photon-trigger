// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
// Tool Version Limit: 2022.04
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "xpostage_maxi.h"

extern XPostage_maxi_Config XPostage_maxi_ConfigTable[];

XPostage_maxi_Config *XPostage_maxi_LookupConfig(u16 DeviceId) {
	XPostage_maxi_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XPOSTAGE_MAXI_NUM_INSTANCES; Index++) {
		if (XPostage_maxi_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XPostage_maxi_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XPostage_maxi_Initialize(XPostage_maxi *InstancePtr, u16 DeviceId) {
	XPostage_maxi_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XPostage_maxi_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XPostage_maxi_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

