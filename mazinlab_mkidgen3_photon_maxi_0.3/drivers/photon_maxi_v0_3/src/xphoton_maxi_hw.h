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
// 0x10 : Data signal of photons_out
//        bit 31~0 - photons_out[31:0] (Read/Write)
// 0x14 : Data signal of photons_out
//        bit 31~0 - photons_out[63:32] (Read/Write)
// 0x18 : reserved
// 0x28 : Data signal of active_buffer
//        bit 7~0 - active_buffer[7:0] (Read)
//        others  - reserved
// 0x2c : Control signal of active_buffer
//        bit 0  - active_buffer_ap_vld (Read/COR)
//        others - reserved
// 0x38 : Data signal of photons_per_buf
//        bit 16~0 - photons_per_buf[16:0] (Read/Write)
//        others   - reserved
// 0x3c : reserved
// 0x40 : Data signal of time_shift
//        bit 4~0 - time_shift[4:0] (Read/Write)
//        others  - reserved
// 0x44 : reserved
// 0x20 ~
// 0x27 : Memory 'n_photons' (2 * 17b)
//        Word n : bit [16:0] - n_photons[n]
//                 others     - reserved
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XPHOTON_MAXI_CONTROL_ADDR_AP_CTRL              0x00
#define XPHOTON_MAXI_CONTROL_ADDR_GIE                  0x04
#define XPHOTON_MAXI_CONTROL_ADDR_IER                  0x08
#define XPHOTON_MAXI_CONTROL_ADDR_ISR                  0x0c
#define XPHOTON_MAXI_CONTROL_ADDR_PHOTONS_OUT_DATA     0x10
#define XPHOTON_MAXI_CONTROL_BITS_PHOTONS_OUT_DATA     64
#define XPHOTON_MAXI_CONTROL_ADDR_ACTIVE_BUFFER_DATA   0x28
#define XPHOTON_MAXI_CONTROL_BITS_ACTIVE_BUFFER_DATA   8
#define XPHOTON_MAXI_CONTROL_ADDR_ACTIVE_BUFFER_CTRL   0x2c
#define XPHOTON_MAXI_CONTROL_ADDR_PHOTONS_PER_BUF_DATA 0x38
#define XPHOTON_MAXI_CONTROL_BITS_PHOTONS_PER_BUF_DATA 17
#define XPHOTON_MAXI_CONTROL_ADDR_TIME_SHIFT_DATA      0x40
#define XPHOTON_MAXI_CONTROL_BITS_TIME_SHIFT_DATA      5
#define XPHOTON_MAXI_CONTROL_ADDR_N_PHOTONS_BASE       0x20
#define XPHOTON_MAXI_CONTROL_ADDR_N_PHOTONS_HIGH       0x27
#define XPHOTON_MAXI_CONTROL_WIDTH_N_PHOTONS           17
#define XPHOTON_MAXI_CONTROL_DEPTH_N_PHOTONS           2

