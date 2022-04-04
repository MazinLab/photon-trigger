// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2021.1 (64-bit)
// Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "xpostage.h"

extern XPostage_Config XPostage_ConfigTable[];

XPostage_Config *XPostage_LookupConfig(u16 DeviceId) {
	XPostage_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XPOSTAGE_NUM_INSTANCES; Index++) {
		if (XPostage_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XPostage_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XPostage_Initialize(XPostage *InstancePtr, u16 DeviceId) {
	XPostage_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XPostage_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XPostage_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

