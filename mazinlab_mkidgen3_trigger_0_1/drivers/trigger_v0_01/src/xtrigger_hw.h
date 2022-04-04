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

// control_r
// 0x0 : Control signals
//       bit 0  - ap_start (Read/Write/COH)
//       bit 1  - ap_done (Read/COR)
//       bit 2  - ap_idle (Read)
//       bit 3  - ap_ready (Read/COR)
//       bit 7  - auto_restart (Read/Write)
//       others - reserved
// 0x4 : Global Interrupt Enable Register
//       bit 0  - Global Interrupt Enable (Read/Write)
//       others - reserved
// 0x8 : IP Interrupt Enable Register (Read/Write)
//       bit 0  - enable ap_done interrupt (Read/Write)
//       bit 1  - enable ap_ready interrupt (Read/Write)
//       others - reserved
// 0xc : IP Interrupt Status Register (Read/TOW)
//       bit 0  - ap_done (COR/TOW)
//       bit 1  - ap_ready (COR/TOW)
//       others - reserved
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XTRIGGER_CONTROL_R_ADDR_AP_CTRL 0x0
#define XTRIGGER_CONTROL_R_ADDR_GIE     0x4
#define XTRIGGER_CONTROL_R_ADDR_IER     0x8
#define XTRIGGER_CONTROL_R_ADDR_ISR     0xc

