// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2021.1 (64-bit)
// Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
// ==============================================================
// control
// 0x00 : Control signals
//        bit 0  - ap_start (Read/Write/COH)
//        bit 1  - ap_done (Read/COR)
//        bit 2  - ap_idle (Read)
//        bit 3  - ap_ready (Read/COR)
//        bit 7  - auto_restart (Read/Write)
//        others - reserved
// 0x04 : Global Interrupt Enable Register
//        bit 0  - Global Interrupt Enable (Read/Write)
//        others - reserved
// 0x08 : IP Interrupt Enable Register (Read/Write)
//        bit 0  - enable ap_done interrupt (Read/Write)
//        bit 1  - enable ap_ready interrupt (Read/Write)
//        others - reserved
// 0x0c : IP Interrupt Status Register (Read/TOW)
//        bit 0  - ap_done (COR/TOW)
//        bit 1  - ap_ready (COR/TOW)
//        others - reserved
// 0x10 : Data signal of monitor_0
//        bit 11~0 - monitor_0[11:0] (Read/Write)
//        others   - reserved
// 0x14 : reserved
// 0x18 : Data signal of monitor_1
//        bit 11~0 - monitor_1[11:0] (Read/Write)
//        others   - reserved
// 0x1c : reserved
// 0x20 : Data signal of monitor_2
//        bit 11~0 - monitor_2[11:0] (Read/Write)
//        others   - reserved
// 0x24 : reserved
// 0x28 : Data signal of monitor_3
//        bit 11~0 - monitor_3[11:0] (Read/Write)
//        others   - reserved
// 0x2c : reserved
// 0x30 : Data signal of monitor_4
//        bit 11~0 - monitor_4[11:0] (Read/Write)
//        others   - reserved
// 0x34 : reserved
// 0x38 : Data signal of monitor_5
//        bit 11~0 - monitor_5[11:0] (Read/Write)
//        others   - reserved
// 0x3c : reserved
// 0x40 : Data signal of monitor_6
//        bit 11~0 - monitor_6[11:0] (Read/Write)
//        others   - reserved
// 0x44 : reserved
// 0x48 : Data signal of monitor_7
//        bit 11~0 - monitor_7[11:0] (Read/Write)
//        others   - reserved
// 0x4c : reserved
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XPOSTAGE_CONTROL_ADDR_AP_CTRL        0x00
#define XPOSTAGE_CONTROL_ADDR_GIE            0x04
#define XPOSTAGE_CONTROL_ADDR_IER            0x08
#define XPOSTAGE_CONTROL_ADDR_ISR            0x0c
#define XPOSTAGE_CONTROL_ADDR_MONITOR_0_DATA 0x10
#define XPOSTAGE_CONTROL_BITS_MONITOR_0_DATA 12
#define XPOSTAGE_CONTROL_ADDR_MONITOR_1_DATA 0x18
#define XPOSTAGE_CONTROL_BITS_MONITOR_1_DATA 12
#define XPOSTAGE_CONTROL_ADDR_MONITOR_2_DATA 0x20
#define XPOSTAGE_CONTROL_BITS_MONITOR_2_DATA 12
#define XPOSTAGE_CONTROL_ADDR_MONITOR_3_DATA 0x28
#define XPOSTAGE_CONTROL_BITS_MONITOR_3_DATA 12
#define XPOSTAGE_CONTROL_ADDR_MONITOR_4_DATA 0x30
#define XPOSTAGE_CONTROL_BITS_MONITOR_4_DATA 12
#define XPOSTAGE_CONTROL_ADDR_MONITOR_5_DATA 0x38
#define XPOSTAGE_CONTROL_BITS_MONITOR_5_DATA 12
#define XPOSTAGE_CONTROL_ADDR_MONITOR_6_DATA 0x40
#define XPOSTAGE_CONTROL_BITS_MONITOR_6_DATA 12
#define XPOSTAGE_CONTROL_ADDR_MONITOR_7_DATA 0x48
#define XPOSTAGE_CONTROL_BITS_MONITOR_7_DATA 12

