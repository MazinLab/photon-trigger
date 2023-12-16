// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
// Tool Version Limit: 2022.04
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================
// control
// 0x00 : reserved
// 0x04 : reserved
// 0x08 : reserved
// 0x0c : reserved
// 0x10 : Data signal of max_photons_per_packet_minus1
//        bit 16~0 - max_photons_per_packet_minus1[16:0] (Read/Write)
//        others   - reserved
// 0x14 : reserved
// 0x18 : Data signal of ticks_per_packet_minus1
//        bit 31~0 - ticks_per_packet_minus1[31:0] (Read/Write)
// 0x1c : reserved
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XPHOTON_FIFO_PACKETIZER_CONTROL_ADDR_MAX_PHOTONS_PER_PACKET_MINUS1_DATA 0x10
#define XPHOTON_FIFO_PACKETIZER_CONTROL_BITS_MAX_PHOTONS_PER_PACKET_MINUS1_DATA 17
#define XPHOTON_FIFO_PACKETIZER_CONTROL_ADDR_TICKS_PER_PACKET_MINUS1_DATA       0x18
#define XPHOTON_FIFO_PACKETIZER_CONTROL_BITS_TICKS_PER_PACKET_MINUS1_DATA       32

