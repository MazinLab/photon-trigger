// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
// Tool Version Limit: 2022.04
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================
// control
// 0x00 : Control signals
//        bit 0  - ap_start (Read/Write/COH)
//        bit 1  - ap_done (Read/COR)
//        bit 2  - ap_idle (Read)
//        bit 3  - ap_ready (Read/COR)
//        bit 7  - auto_restart (Read/Write)
//        bit 9  - interrupt (Read)
//        others - reserved
// 0x04 : Global Interrupt Enable Register
//        bit 0  - Global Interrupt Enable (Read/Write)
//        others - reserved
// 0x08 : IP Interrupt Enable Register (Read/Write)
//        bit 0 - enable ap_done interrupt (Read/Write)
//        bit 1 - enable ap_ready interrupt (Read/Write)
//        others - reserved
// 0x0c : IP Interrupt Status Register (Read/COR)
//        bit 0 - ap_done (Read/COR)
//        bit 1 - ap_ready (Read/COR)
//        others - reserved
// 0x10 : Data signal of iq
//        bit 31~0 - iq[31:0] (Read/Write)
// 0x14 : Data signal of iq
//        bit 31~0 - iq[63:32] (Read/Write)
// 0x18 : reserved
// 0x1c : Data signal of event_count
//        bit 15~0 - event_count[15:0] (Read)
//        others   - reserved
// 0x20 : Control signal of event_count
//        bit 0  - event_count_ap_vld (Read/COR)
//        others - reserved
// 0x2c : Data signal of max_events
//        bit 15~0 - max_events[15:0] (Read/Write)
//        others   - reserved
// 0x30 : reserved
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XPOSTAGE_MAXI_CONTROL_ADDR_AP_CTRL          0x00
#define XPOSTAGE_MAXI_CONTROL_ADDR_GIE              0x04
#define XPOSTAGE_MAXI_CONTROL_ADDR_IER              0x08
#define XPOSTAGE_MAXI_CONTROL_ADDR_ISR              0x0c
#define XPOSTAGE_MAXI_CONTROL_ADDR_IQ_DATA          0x10
#define XPOSTAGE_MAXI_CONTROL_BITS_IQ_DATA          64
#define XPOSTAGE_MAXI_CONTROL_ADDR_EVENT_COUNT_DATA 0x1c
#define XPOSTAGE_MAXI_CONTROL_BITS_EVENT_COUNT_DATA 16
#define XPOSTAGE_MAXI_CONTROL_ADDR_EVENT_COUNT_CTRL 0x20
#define XPOSTAGE_MAXI_CONTROL_ADDR_MAX_EVENTS_DATA  0x2c
#define XPOSTAGE_MAXI_CONTROL_BITS_MAX_EVENTS_DATA  16

