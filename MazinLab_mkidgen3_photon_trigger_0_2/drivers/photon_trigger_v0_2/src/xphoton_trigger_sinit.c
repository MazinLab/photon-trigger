// ==============================================================
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2019.2.1 (64-bit)
// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "xphoton_trigger.h"

extern XPhoton_trigger_Config XPhoton_trigger_ConfigTable[];

XPhoton_trigger_Config *XPhoton_trigger_LookupConfig(u16 DeviceId) {
	XPhoton_trigger_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XPHOTON_TRIGGER_NUM_INSTANCES; Index++) {
		if (XPhoton_trigger_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XPhoton_trigger_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XPhoton_trigger_Initialize(XPhoton_trigger *InstancePtr, u16 DeviceId) {
	XPhoton_trigger_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XPhoton_trigger_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XPhoton_trigger_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

