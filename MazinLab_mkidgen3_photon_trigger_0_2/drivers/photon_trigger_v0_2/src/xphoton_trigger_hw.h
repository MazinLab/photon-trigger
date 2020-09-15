// ==============================================================
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2019.2.1 (64-bit)
// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// ==============================================================
// control
// 0x0000 : reserved
// 0x0004 : reserved
// 0x0008 : reserved
// 0x000c : reserved
// 0x1400 : Data signal of holdoff_V
//          bit 9~0 - holdoff_V[9:0] (Read/Write)
//          others  - reserved
// 0x1404 : reserved
// 0x1408 : Data signal of baseline_shrink_V
//          bit 8~0 - baseline_shrink_V[8:0] (Read/Write)
//          others  - reserved
// 0x140c : reserved
// 0x1410 : Data signal of baseline_grow_V
//          bit 8~0 - baseline_grow_V[8:0] (Read/Write)
//          others  - reserved
// 0x1414 : reserved
// 0x1418 : Data signal of dropped
//          bit 31~0 - dropped[31:0] (Read/Write)
// 0x141c : reserved
// 0x0400 ~
// 0x07ff : Memory 'triggercfg_0_threshold_V' (512 * 16b)
//          Word n : bit [15: 0] - triggercfg_0_threshold_V[2n]
//                   bit [31:16] - triggercfg_0_threshold_V[2n+1]
// 0x0800 ~
// 0x0bff : Memory 'triggercfg_1_threshold_V' (512 * 16b)
//          Word n : bit [15: 0] - triggercfg_1_threshold_V[2n]
//                   bit [31:16] - triggercfg_1_threshold_V[2n+1]
// 0x0c00 ~
// 0x0fff : Memory 'triggercfg_2_threshold_V' (512 * 16b)
//          Word n : bit [15: 0] - triggercfg_2_threshold_V[2n]
//                   bit [31:16] - triggercfg_2_threshold_V[2n+1]
// 0x1000 ~
// 0x13ff : Memory 'triggercfg_3_threshold_V' (512 * 16b)
//          Word n : bit [15: 0] - triggercfg_3_threshold_V[2n]
//                   bit [31:16] - triggercfg_3_threshold_V[2n+1]
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XPHOTON_TRIGGER_CONTROL_ADDR_HOLDOFF_V_DATA                0x1400
#define XPHOTON_TRIGGER_CONTROL_BITS_HOLDOFF_V_DATA                10
#define XPHOTON_TRIGGER_CONTROL_ADDR_BASELINE_SHRINK_V_DATA        0x1408
#define XPHOTON_TRIGGER_CONTROL_BITS_BASELINE_SHRINK_V_DATA        9
#define XPHOTON_TRIGGER_CONTROL_ADDR_BASELINE_GROW_V_DATA          0x1410
#define XPHOTON_TRIGGER_CONTROL_BITS_BASELINE_GROW_V_DATA          9
#define XPHOTON_TRIGGER_CONTROL_ADDR_DROPPED_DATA                  0x1418
#define XPHOTON_TRIGGER_CONTROL_BITS_DROPPED_DATA                  32
#define XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_0_THRESHOLD_V_BASE 0x0400
#define XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_0_THRESHOLD_V_HIGH 0x07ff
#define XPHOTON_TRIGGER_CONTROL_WIDTH_TRIGGERCFG_0_THRESHOLD_V     16
#define XPHOTON_TRIGGER_CONTROL_DEPTH_TRIGGERCFG_0_THRESHOLD_V     512
#define XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_1_THRESHOLD_V_BASE 0x0800
#define XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_1_THRESHOLD_V_HIGH 0x0bff
#define XPHOTON_TRIGGER_CONTROL_WIDTH_TRIGGERCFG_1_THRESHOLD_V     16
#define XPHOTON_TRIGGER_CONTROL_DEPTH_TRIGGERCFG_1_THRESHOLD_V     512
#define XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_2_THRESHOLD_V_BASE 0x0c00
#define XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_2_THRESHOLD_V_HIGH 0x0fff
#define XPHOTON_TRIGGER_CONTROL_WIDTH_TRIGGERCFG_2_THRESHOLD_V     16
#define XPHOTON_TRIGGER_CONTROL_DEPTH_TRIGGERCFG_2_THRESHOLD_V     512
#define XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_3_THRESHOLD_V_BASE 0x1000
#define XPHOTON_TRIGGER_CONTROL_ADDR_TRIGGERCFG_3_THRESHOLD_V_HIGH 0x13ff
#define XPHOTON_TRIGGER_CONTROL_WIDTH_TRIGGERCFG_3_THRESHOLD_V     16
#define XPHOTON_TRIGGER_CONTROL_DEPTH_TRIGGERCFG_3_THRESHOLD_V     512

