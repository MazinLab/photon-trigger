// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2021.1 (64-bit)
// Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "xtrigger.h"

extern XTrigger_Config XTrigger_ConfigTable[];

XTrigger_Config *XTrigger_LookupConfig(u16 DeviceId) {
	XTrigger_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XTRIGGER_NUM_INSTANCES; Index++) {
		if (XTrigger_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XTrigger_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XTrigger_Initialize(XTrigger *InstancePtr, u16 DeviceId) {
	XTrigger_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XTrigger_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XTrigger_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

