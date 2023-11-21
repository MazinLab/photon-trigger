// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
// Tool Version Limit: 2022.04
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "xpostage_filter_w_interconn.h"

extern XPostage_filter_w_interconn_Config XPostage_filter_w_interconn_ConfigTable[];

XPostage_filter_w_interconn_Config *XPostage_filter_w_interconn_LookupConfig(u16 DeviceId) {
	XPostage_filter_w_interconn_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XPOSTAGE_FILTER_W_INTERCONN_NUM_INSTANCES; Index++) {
		if (XPostage_filter_w_interconn_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XPostage_filter_w_interconn_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XPostage_filter_w_interconn_Initialize(XPostage_filter_w_interconn *InstancePtr, u16 DeviceId) {
	XPostage_filter_w_interconn_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XPostage_filter_w_interconn_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XPostage_filter_w_interconn_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

