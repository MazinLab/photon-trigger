// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2021.1 (64-bit)
// Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
// ==============================================================
// control
// 0x0000 : reserved
// 0x0004 : reserved
// 0x0008 : reserved
// 0x000c : reserved
// 0x0010 : Data signal of holdoff
//          bit 15~0 - holdoff[15:0] (Read/Write)
//          others   - reserved
// 0x0014 : reserved
// 0x1000 ~
// 0x1fff : Memory 'thresholds' (512 * 64b)
//          Word 2n   : bit [31:0] - thresholds[n][31: 0]
//          Word 2n+1 : bit [31:0] - thresholds[n][63:32]
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XTRIGGER_CONTROL_ADDR_HOLDOFF_DATA    0x0010
#define XTRIGGER_CONTROL_BITS_HOLDOFF_DATA    16
#define XTRIGGER_CONTROL_ADDR_THRESHOLDS_BASE 0x1000
#define XTRIGGER_CONTROL_ADDR_THRESHOLDS_HIGH 0x1fff
#define XTRIGGER_CONTROL_WIDTH_THRESHOLDS     64
#define XTRIGGER_CONTROL_DEPTH_THRESHOLDS     512

