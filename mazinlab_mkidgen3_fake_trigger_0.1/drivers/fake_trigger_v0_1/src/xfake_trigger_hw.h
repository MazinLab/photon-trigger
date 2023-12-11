// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
// Tool Version Limit: 2022.04
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================
// control
// 0x1000 ~
// 0x1fff : Memory 'threshoffs' (512 * 64b)
//          Word 2n   : bit [31:0] - threshoffs[n][31: 0]
//          Word 2n+1 : bit [31:0] - threshoffs[n][63:32]
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XFAKE_TRIGGER_CONTROL_ADDR_THRESHOFFS_BASE 0x1000
#define XFAKE_TRIGGER_CONTROL_ADDR_THRESHOFFS_HIGH 0x1fff
#define XFAKE_TRIGGER_CONTROL_WIDTH_THRESHOFFS     64
#define XFAKE_TRIGGER_CONTROL_DEPTH_THRESHOFFS     512

