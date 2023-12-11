// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
// Tool Version Limit: 2022.04
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "xfake_trigger.h"

extern XFake_trigger_Config XFake_trigger_ConfigTable[];

XFake_trigger_Config *XFake_trigger_LookupConfig(u16 DeviceId) {
	XFake_trigger_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XFAKE_TRIGGER_NUM_INSTANCES; Index++) {
		if (XFake_trigger_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XFake_trigger_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XFake_trigger_Initialize(XFake_trigger *InstancePtr, u16 DeviceId) {
	XFake_trigger_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XFake_trigger_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XFake_trigger_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

