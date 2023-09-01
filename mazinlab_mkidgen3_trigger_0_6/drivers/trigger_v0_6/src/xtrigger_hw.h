// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
// Tool Version Limit: 2022.04
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================
// control
// 0x0000 : reserved
// 0x0004 : reserved
// 0x0008 : reserved
// 0x000c : reserved
// 0x0010 : Data signal of desync
//          bit 0  - desync[0] (Read)
//          others - reserved
// 0x0014 : // 0x1000 ~
// 0x1fff : Memory 'threshoffs' (512 * 64b)
//          Word 2n   : bit [31:0] - threshoffs[n][31: 0]
//          Word 2n+1 : bit [31:0] - threshoffs[n][63:32]
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XTRIGGER_CONTROL_ADDR_DESYNC_DATA     0x0010
#define XTRIGGER_CONTROL_BITS_DESYNC_DATA     1
#define XTRIGGER_CONTROL_ADDR_THRESHOFFS_BASE 0x1000
#define XTRIGGER_CONTROL_ADDR_THRESHOFFS_HIGH 0x1fff
#define XTRIGGER_CONTROL_WIDTH_THRESHOFFS     64
#define XTRIGGER_CONTROL_DEPTH_THRESHOFFS     512

