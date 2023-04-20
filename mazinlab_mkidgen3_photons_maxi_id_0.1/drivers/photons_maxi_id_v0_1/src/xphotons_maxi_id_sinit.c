// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
// Tool Version Limit: 2022.04
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "xphotons_maxi_id.h"

extern XPhotons_maxi_id_Config XPhotons_maxi_id_ConfigTable[];

XPhotons_maxi_id_Config *XPhotons_maxi_id_LookupConfig(u16 DeviceId) {
	XPhotons_maxi_id_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XPHOTONS_MAXI_ID_NUM_INSTANCES; Index++) {
		if (XPhotons_maxi_id_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XPhotons_maxi_id_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XPhotons_maxi_id_Initialize(XPhotons_maxi_id *InstancePtr, u16 DeviceId) {
	XPhotons_maxi_id_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XPhotons_maxi_id_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XPhotons_maxi_id_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

