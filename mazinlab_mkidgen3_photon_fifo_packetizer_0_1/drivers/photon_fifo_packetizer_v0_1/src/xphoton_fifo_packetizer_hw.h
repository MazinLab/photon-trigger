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
// 0x10 : Data signal of max_photons_per_packet_minus2
//        bit 16~0 - max_photons_per_packet_minus2[16:0] (Read/Write)
//        others   - reserved
// 0x14 : reserved
// 0x18 : Data signal of approx_time_per_packet
//        bit 4~0 - approx_time_per_packet[4:0] (Read/Write)
//        others  - reserved
// 0x1c : reserved
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XPHOTON_FIFO_PACKETIZER_CONTROL_ADDR_MAX_PHOTONS_PER_PACKET_MINUS2_DATA 0x10
#define XPHOTON_FIFO_PACKETIZER_CONTROL_BITS_MAX_PHOTONS_PER_PACKET_MINUS2_DATA 17
#define XPHOTON_FIFO_PACKETIZER_CONTROL_ADDR_APPROX_TIME_PER_PACKET_DATA        0x18
#define XPHOTON_FIFO_PACKETIZER_CONTROL_BITS_APPROX_TIME_PER_PACKET_DATA        5

