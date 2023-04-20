// ==============================================================
// RTL generated by Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
// Version: 2022.1
// Copyright (C) Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// 
// ===========================================================

`timescale 1 ns / 1 ps 

(* CORE_GENERATION_INFO="photons_maxi_id_photons_maxi_id,hls_ip_2022_1,{HLS_INPUT_TYPE=cxx,HLS_INPUT_FLOAT=0,HLS_INPUT_FIXED=0,HLS_INPUT_PART=xczu28dr-ffvg1517-2-e,HLS_INPUT_CLOCK=3.906000,HLS_INPUT_ARCH=others,HLS_SYN_CLOCK=2.851380,HLS_SYN_LAT=-1,HLS_SYN_TPT=none,HLS_SYN_MEM=0,HLS_SYN_DSP=0,HLS_SYN_FF=1361,HLS_SYN_LUT=3602,HLS_VERSION=2022_1}" *)

module photons_maxi_id (
        ap_clk,
        ap_rst_n,
        m_axi_gmem_AWVALID,
        m_axi_gmem_AWREADY,
        m_axi_gmem_AWADDR,
        m_axi_gmem_AWID,
        m_axi_gmem_AWLEN,
        m_axi_gmem_AWSIZE,
        m_axi_gmem_AWBURST,
        m_axi_gmem_AWLOCK,
        m_axi_gmem_AWCACHE,
        m_axi_gmem_AWPROT,
        m_axi_gmem_AWQOS,
        m_axi_gmem_AWREGION,
        m_axi_gmem_AWUSER,
        m_axi_gmem_WVALID,
        m_axi_gmem_WREADY,
        m_axi_gmem_WDATA,
        m_axi_gmem_WSTRB,
        m_axi_gmem_WLAST,
        m_axi_gmem_WID,
        m_axi_gmem_WUSER,
        m_axi_gmem_ARVALID,
        m_axi_gmem_ARREADY,
        m_axi_gmem_ARADDR,
        m_axi_gmem_ARID,
        m_axi_gmem_ARLEN,
        m_axi_gmem_ARSIZE,
        m_axi_gmem_ARBURST,
        m_axi_gmem_ARLOCK,
        m_axi_gmem_ARCACHE,
        m_axi_gmem_ARPROT,
        m_axi_gmem_ARQOS,
        m_axi_gmem_ARREGION,
        m_axi_gmem_ARUSER,
        m_axi_gmem_RVALID,
        m_axi_gmem_RREADY,
        m_axi_gmem_RDATA,
        m_axi_gmem_RLAST,
        m_axi_gmem_RID,
        m_axi_gmem_RUSER,
        m_axi_gmem_RRESP,
        m_axi_gmem_BVALID,
        m_axi_gmem_BREADY,
        m_axi_gmem_BRESP,
        m_axi_gmem_BID,
        m_axi_gmem_BUSER,
        photons_TDATA,
        photons_TVALID,
        photons_TREADY,
        s_axi_control_AWVALID,
        s_axi_control_AWREADY,
        s_axi_control_AWADDR,
        s_axi_control_WVALID,
        s_axi_control_WREADY,
        s_axi_control_WDATA,
        s_axi_control_WSTRB,
        s_axi_control_ARVALID,
        s_axi_control_ARREADY,
        s_axi_control_ARADDR,
        s_axi_control_RVALID,
        s_axi_control_RREADY,
        s_axi_control_RDATA,
        s_axi_control_RRESP,
        s_axi_control_BVALID,
        s_axi_control_BREADY,
        s_axi_control_BRESP,
        interrupt
);

parameter    ap_ST_fsm_state1 = 5'd1;
parameter    ap_ST_fsm_state2 = 5'd2;
parameter    ap_ST_fsm_state3 = 5'd4;
parameter    ap_ST_fsm_state4 = 5'd8;
parameter    ap_ST_fsm_state5 = 5'd16;
parameter    C_S_AXI_CONTROL_DATA_WIDTH = 32;
parameter    C_S_AXI_CONTROL_ADDR_WIDTH = 6;
parameter    C_S_AXI_DATA_WIDTH = 32;
parameter    C_M_AXI_GMEM_ID_WIDTH = 1;
parameter    C_M_AXI_GMEM_ADDR_WIDTH = 64;
parameter    C_M_AXI_GMEM_DATA_WIDTH = 64;
parameter    C_M_AXI_GMEM_AWUSER_WIDTH = 1;
parameter    C_M_AXI_GMEM_ARUSER_WIDTH = 1;
parameter    C_M_AXI_GMEM_WUSER_WIDTH = 1;
parameter    C_M_AXI_GMEM_RUSER_WIDTH = 1;
parameter    C_M_AXI_GMEM_BUSER_WIDTH = 1;
parameter    C_M_AXI_GMEM_USER_VALUE = 0;
parameter    C_M_AXI_GMEM_PROT_VALUE = 0;
parameter    C_M_AXI_GMEM_CACHE_VALUE = 3;
parameter    C_M_AXI_DATA_WIDTH = 32;

parameter C_S_AXI_CONTROL_WSTRB_WIDTH = (32 / 8);
parameter C_S_AXI_WSTRB_WIDTH = (32 / 8);
parameter C_M_AXI_GMEM_WSTRB_WIDTH = (64 / 8);
parameter C_M_AXI_WSTRB_WIDTH = (32 / 8);

input   ap_clk;
input   ap_rst_n;
output   m_axi_gmem_AWVALID;
input   m_axi_gmem_AWREADY;
output  [C_M_AXI_GMEM_ADDR_WIDTH - 1:0] m_axi_gmem_AWADDR;
output  [C_M_AXI_GMEM_ID_WIDTH - 1:0] m_axi_gmem_AWID;
output  [7:0] m_axi_gmem_AWLEN;
output  [2:0] m_axi_gmem_AWSIZE;
output  [1:0] m_axi_gmem_AWBURST;
output  [1:0] m_axi_gmem_AWLOCK;
output  [3:0] m_axi_gmem_AWCACHE;
output  [2:0] m_axi_gmem_AWPROT;
output  [3:0] m_axi_gmem_AWQOS;
output  [3:0] m_axi_gmem_AWREGION;
output  [C_M_AXI_GMEM_AWUSER_WIDTH - 1:0] m_axi_gmem_AWUSER;
output   m_axi_gmem_WVALID;
input   m_axi_gmem_WREADY;
output  [C_M_AXI_GMEM_DATA_WIDTH - 1:0] m_axi_gmem_WDATA;
output  [C_M_AXI_GMEM_WSTRB_WIDTH - 1:0] m_axi_gmem_WSTRB;
output   m_axi_gmem_WLAST;
output  [C_M_AXI_GMEM_ID_WIDTH - 1:0] m_axi_gmem_WID;
output  [C_M_AXI_GMEM_WUSER_WIDTH - 1:0] m_axi_gmem_WUSER;
output   m_axi_gmem_ARVALID;
input   m_axi_gmem_ARREADY;
output  [C_M_AXI_GMEM_ADDR_WIDTH - 1:0] m_axi_gmem_ARADDR;
output  [C_M_AXI_GMEM_ID_WIDTH - 1:0] m_axi_gmem_ARID;
output  [7:0] m_axi_gmem_ARLEN;
output  [2:0] m_axi_gmem_ARSIZE;
output  [1:0] m_axi_gmem_ARBURST;
output  [1:0] m_axi_gmem_ARLOCK;
output  [3:0] m_axi_gmem_ARCACHE;
output  [2:0] m_axi_gmem_ARPROT;
output  [3:0] m_axi_gmem_ARQOS;
output  [3:0] m_axi_gmem_ARREGION;
output  [C_M_AXI_GMEM_ARUSER_WIDTH - 1:0] m_axi_gmem_ARUSER;
input   m_axi_gmem_RVALID;
output   m_axi_gmem_RREADY;
input  [C_M_AXI_GMEM_DATA_WIDTH - 1:0] m_axi_gmem_RDATA;
input   m_axi_gmem_RLAST;
input  [C_M_AXI_GMEM_ID_WIDTH - 1:0] m_axi_gmem_RID;
input  [C_M_AXI_GMEM_RUSER_WIDTH - 1:0] m_axi_gmem_RUSER;
input  [1:0] m_axi_gmem_RRESP;
input   m_axi_gmem_BVALID;
output   m_axi_gmem_BREADY;
input  [1:0] m_axi_gmem_BRESP;
input  [C_M_AXI_GMEM_ID_WIDTH - 1:0] m_axi_gmem_BID;
input  [C_M_AXI_GMEM_BUSER_WIDTH - 1:0] m_axi_gmem_BUSER;
input  [47:0] photons_TDATA;
input   photons_TVALID;
output   photons_TREADY;
input   s_axi_control_AWVALID;
output   s_axi_control_AWREADY;
input  [C_S_AXI_CONTROL_ADDR_WIDTH - 1:0] s_axi_control_AWADDR;
input   s_axi_control_WVALID;
output   s_axi_control_WREADY;
input  [C_S_AXI_CONTROL_DATA_WIDTH - 1:0] s_axi_control_WDATA;
input  [C_S_AXI_CONTROL_WSTRB_WIDTH - 1:0] s_axi_control_WSTRB;
input   s_axi_control_ARVALID;
output   s_axi_control_ARREADY;
input  [C_S_AXI_CONTROL_ADDR_WIDTH - 1:0] s_axi_control_ARADDR;
output   s_axi_control_RVALID;
input   s_axi_control_RREADY;
output  [C_S_AXI_CONTROL_DATA_WIDTH - 1:0] s_axi_control_RDATA;
output  [1:0] s_axi_control_RRESP;
output   s_axi_control_BVALID;
input   s_axi_control_BREADY;
output  [1:0] s_axi_control_BRESP;
output   interrupt;

 reg    ap_rst_n_inv;
wire    ap_start;
reg    ap_done;
reg    ap_idle;
(* fsm_encoding = "none" *) reg   [4:0] ap_CS_fsm;
wire    ap_CS_fsm_state1;
reg    ap_ready;
wire   [63:0] photons_out;
reg   [0:0] n_photons_address0;
reg    n_photons_ce0;
reg    n_photons_we0;
reg   [12:0] n_photons_d0;
reg   [0:0] p_ab;
reg   [63:0] photons_out_read_reg_182;
wire   [2:0] trunc_ln132_fu_125_p1;
reg   [2:0] trunc_ln132_reg_187;
wire   [17:0] sub_ln132_fu_159_p2;
reg   [17:0] sub_ln132_reg_195;
wire    ap_CS_fsm_state2;
wire    ap_CS_fsm_state4;
wire    grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_ap_start;
wire    grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_ap_done;
wire    grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_ap_idle;
wire    grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_ap_ready;
wire    grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_AWVALID;
wire   [63:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_AWADDR;
wire   [0:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_AWID;
wire   [31:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_AWLEN;
wire   [2:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_AWSIZE;
wire   [1:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_AWBURST;
wire   [1:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_AWLOCK;
wire   [3:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_AWCACHE;
wire   [2:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_AWPROT;
wire   [3:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_AWQOS;
wire   [3:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_AWREGION;
wire   [0:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_AWUSER;
wire    grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_WVALID;
wire   [63:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_WDATA;
wire   [7:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_WSTRB;
wire    grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_WLAST;
wire   [0:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_WID;
wire   [0:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_WUSER;
wire    grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_ARVALID;
wire   [63:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_ARADDR;
wire   [0:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_ARID;
wire   [31:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_ARLEN;
wire   [2:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_ARSIZE;
wire   [1:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_ARBURST;
wire   [1:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_ARLOCK;
wire   [3:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_ARCACHE;
wire   [2:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_ARPROT;
wire   [3:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_ARQOS;
wire   [3:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_ARREGION;
wire   [0:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_ARUSER;
wire    grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_RREADY;
wire    grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_BREADY;
wire   [0:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_n_photons_address0;
wire    grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_n_photons_ce0;
wire    grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_n_photons_we0;
wire   [12:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_n_photons_d0;
wire   [7:0] grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_active_buffer;
wire    grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_active_buffer_ap_vld;
wire    grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_photons_TREADY;
reg    gmem_AWVALID;
wire    gmem_AWREADY;
reg    gmem_WVALID;
wire    gmem_WREADY;
wire    gmem_ARREADY;
wire    gmem_RVALID;
wire   [63:0] gmem_RDATA;
wire   [8:0] gmem_RFIFONUM;
wire    gmem_BVALID;
reg    gmem_BREADY;
reg    grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_ap_start_reg;
wire    ap_CS_fsm_state3;
wire   [63:0] zext_ln142_fu_177_p1;
wire   [0:0] xor_ln140_fu_166_p2;
wire    ap_CS_fsm_state5;
wire   [16:0] shl_ln132_3_fu_135_p3;
wire   [14:0] shl_ln132_4_fu_147_p3;
wire   [17:0] zext_ln132_fu_143_p1;
wire   [17:0] zext_ln132_1_fu_155_p1;
reg   [4:0] ap_NS_fsm;
reg    ap_ST_fsm_state1_blk;
wire    ap_ST_fsm_state2_blk;
reg    ap_ST_fsm_state3_blk;
wire    ap_ST_fsm_state4_blk;
wire    ap_ST_fsm_state5_blk;
wire    regslice_both_photons_U_apdone_blk;
wire   [47:0] photons_TDATA_int_regslice;
wire    photons_TVALID_int_regslice;
reg    photons_TREADY_int_regslice;
wire    regslice_both_photons_U_ack_in;
wire    ap_ce_reg;

// power-on initialization
initial begin
#0 ap_CS_fsm = 5'd1;
#0 p_ab = 1'd0;
#0 grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_ap_start_reg = 1'b0;
end

photons_maxi_id_photons_maxi_id_Pipeline_VITIS_LOOP_115_1 grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108(
    .ap_clk(ap_clk),
    .ap_rst(ap_rst_n_inv),
    .ap_start(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_ap_start),
    .ap_done(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_ap_done),
    .ap_idle(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_ap_idle),
    .ap_ready(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_ap_ready),
    .m_axi_gmem_AWVALID(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_AWVALID),
    .m_axi_gmem_AWREADY(gmem_AWREADY),
    .m_axi_gmem_AWADDR(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_AWADDR),
    .m_axi_gmem_AWID(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_AWID),
    .m_axi_gmem_AWLEN(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_AWLEN),
    .m_axi_gmem_AWSIZE(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_AWSIZE),
    .m_axi_gmem_AWBURST(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_AWBURST),
    .m_axi_gmem_AWLOCK(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_AWLOCK),
    .m_axi_gmem_AWCACHE(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_AWCACHE),
    .m_axi_gmem_AWPROT(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_AWPROT),
    .m_axi_gmem_AWQOS(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_AWQOS),
    .m_axi_gmem_AWREGION(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_AWREGION),
    .m_axi_gmem_AWUSER(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_AWUSER),
    .m_axi_gmem_WVALID(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_WVALID),
    .m_axi_gmem_WREADY(gmem_WREADY),
    .m_axi_gmem_WDATA(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_WDATA),
    .m_axi_gmem_WSTRB(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_WSTRB),
    .m_axi_gmem_WLAST(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_WLAST),
    .m_axi_gmem_WID(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_WID),
    .m_axi_gmem_WUSER(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_WUSER),
    .m_axi_gmem_ARVALID(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_ARVALID),
    .m_axi_gmem_ARREADY(1'b0),
    .m_axi_gmem_ARADDR(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_ARADDR),
    .m_axi_gmem_ARID(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_ARID),
    .m_axi_gmem_ARLEN(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_ARLEN),
    .m_axi_gmem_ARSIZE(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_ARSIZE),
    .m_axi_gmem_ARBURST(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_ARBURST),
    .m_axi_gmem_ARLOCK(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_ARLOCK),
    .m_axi_gmem_ARCACHE(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_ARCACHE),
    .m_axi_gmem_ARPROT(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_ARPROT),
    .m_axi_gmem_ARQOS(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_ARQOS),
    .m_axi_gmem_ARREGION(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_ARREGION),
    .m_axi_gmem_ARUSER(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_ARUSER),
    .m_axi_gmem_RVALID(1'b0),
    .m_axi_gmem_RREADY(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_RREADY),
    .m_axi_gmem_RDATA(64'd0),
    .m_axi_gmem_RLAST(1'b0),
    .m_axi_gmem_RID(1'd0),
    .m_axi_gmem_RFIFONUM(9'd0),
    .m_axi_gmem_RUSER(1'd0),
    .m_axi_gmem_RRESP(2'd0),
    .m_axi_gmem_BVALID(gmem_BVALID),
    .m_axi_gmem_BREADY(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_BREADY),
    .m_axi_gmem_BRESP(2'd0),
    .m_axi_gmem_BID(1'd0),
    .m_axi_gmem_BUSER(1'd0),
    .n_photons_address0(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_n_photons_address0),
    .n_photons_ce0(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_n_photons_ce0),
    .n_photons_we0(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_n_photons_we0),
    .n_photons_d0(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_n_photons_d0),
    .idxprom(p_ab),
    .p_ab_load_cast9(p_ab),
    .active_buffer(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_active_buffer),
    .active_buffer_ap_vld(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_active_buffer_ap_vld),
    .photons_TDATA(photons_TDATA_int_regslice),
    .photons_TVALID(photons_TVALID_int_regslice),
    .photons_TREADY(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_photons_TREADY),
    .photons_out(photons_out_read_reg_182),
    .sext_ln134(sub_ln132_reg_195),
    .trunc_ln132_3(trunc_ln132_reg_187)
);

photons_maxi_id_control_s_axi #(
    .C_S_AXI_ADDR_WIDTH( C_S_AXI_CONTROL_ADDR_WIDTH ),
    .C_S_AXI_DATA_WIDTH( C_S_AXI_CONTROL_DATA_WIDTH ))
control_s_axi_U(
    .AWVALID(s_axi_control_AWVALID),
    .AWREADY(s_axi_control_AWREADY),
    .AWADDR(s_axi_control_AWADDR),
    .WVALID(s_axi_control_WVALID),
    .WREADY(s_axi_control_WREADY),
    .WDATA(s_axi_control_WDATA),
    .WSTRB(s_axi_control_WSTRB),
    .ARVALID(s_axi_control_ARVALID),
    .ARREADY(s_axi_control_ARREADY),
    .ARADDR(s_axi_control_ARADDR),
    .RVALID(s_axi_control_RVALID),
    .RREADY(s_axi_control_RREADY),
    .RDATA(s_axi_control_RDATA),
    .RRESP(s_axi_control_RRESP),
    .BVALID(s_axi_control_BVALID),
    .BREADY(s_axi_control_BREADY),
    .BRESP(s_axi_control_BRESP),
    .ACLK(ap_clk),
    .ARESET(ap_rst_n_inv),
    .ACLK_EN(1'b1),
    .photons_out(photons_out),
    .n_photons_address0(n_photons_address0),
    .n_photons_ce0(n_photons_ce0),
    .n_photons_we0(n_photons_we0),
    .n_photons_d0(n_photons_d0),
    .active_buffer(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_active_buffer),
    .active_buffer_ap_vld(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_active_buffer_ap_vld),
    .ap_start(ap_start),
    .interrupt(interrupt),
    .ap_ready(ap_ready),
    .ap_done(ap_done),
    .ap_idle(ap_idle)
);

photons_maxi_id_gmem_m_axi #(
    .CONSERVATIVE( 1 ),
    .USER_MAXREQS( 5 ),
    .NUM_READ_OUTSTANDING( 16 ),
    .NUM_WRITE_OUTSTANDING( 16 ),
    .MAX_READ_BURST_LENGTH( 16 ),
    .MAX_WRITE_BURST_LENGTH( 256 ),
    .USER_RFIFONUM_WIDTH( 9 ),
    .C_M_AXI_ID_WIDTH( C_M_AXI_GMEM_ID_WIDTH ),
    .C_M_AXI_ADDR_WIDTH( C_M_AXI_GMEM_ADDR_WIDTH ),
    .C_M_AXI_DATA_WIDTH( C_M_AXI_GMEM_DATA_WIDTH ),
    .C_M_AXI_AWUSER_WIDTH( C_M_AXI_GMEM_AWUSER_WIDTH ),
    .C_M_AXI_ARUSER_WIDTH( C_M_AXI_GMEM_ARUSER_WIDTH ),
    .C_M_AXI_WUSER_WIDTH( C_M_AXI_GMEM_WUSER_WIDTH ),
    .C_M_AXI_RUSER_WIDTH( C_M_AXI_GMEM_RUSER_WIDTH ),
    .C_M_AXI_BUSER_WIDTH( C_M_AXI_GMEM_BUSER_WIDTH ),
    .C_USER_VALUE( C_M_AXI_GMEM_USER_VALUE ),
    .C_PROT_VALUE( C_M_AXI_GMEM_PROT_VALUE ),
    .C_CACHE_VALUE( C_M_AXI_GMEM_CACHE_VALUE ),
    .USER_DW( 64 ),
    .USER_AW( 64 ))
gmem_m_axi_U(
    .AWVALID(m_axi_gmem_AWVALID),
    .AWREADY(m_axi_gmem_AWREADY),
    .AWADDR(m_axi_gmem_AWADDR),
    .AWID(m_axi_gmem_AWID),
    .AWLEN(m_axi_gmem_AWLEN),
    .AWSIZE(m_axi_gmem_AWSIZE),
    .AWBURST(m_axi_gmem_AWBURST),
    .AWLOCK(m_axi_gmem_AWLOCK),
    .AWCACHE(m_axi_gmem_AWCACHE),
    .AWPROT(m_axi_gmem_AWPROT),
    .AWQOS(m_axi_gmem_AWQOS),
    .AWREGION(m_axi_gmem_AWREGION),
    .AWUSER(m_axi_gmem_AWUSER),
    .WVALID(m_axi_gmem_WVALID),
    .WREADY(m_axi_gmem_WREADY),
    .WDATA(m_axi_gmem_WDATA),
    .WSTRB(m_axi_gmem_WSTRB),
    .WLAST(m_axi_gmem_WLAST),
    .WID(m_axi_gmem_WID),
    .WUSER(m_axi_gmem_WUSER),
    .ARVALID(m_axi_gmem_ARVALID),
    .ARREADY(m_axi_gmem_ARREADY),
    .ARADDR(m_axi_gmem_ARADDR),
    .ARID(m_axi_gmem_ARID),
    .ARLEN(m_axi_gmem_ARLEN),
    .ARSIZE(m_axi_gmem_ARSIZE),
    .ARBURST(m_axi_gmem_ARBURST),
    .ARLOCK(m_axi_gmem_ARLOCK),
    .ARCACHE(m_axi_gmem_ARCACHE),
    .ARPROT(m_axi_gmem_ARPROT),
    .ARQOS(m_axi_gmem_ARQOS),
    .ARREGION(m_axi_gmem_ARREGION),
    .ARUSER(m_axi_gmem_ARUSER),
    .RVALID(m_axi_gmem_RVALID),
    .RREADY(m_axi_gmem_RREADY),
    .RDATA(m_axi_gmem_RDATA),
    .RLAST(m_axi_gmem_RLAST),
    .RID(m_axi_gmem_RID),
    .RUSER(m_axi_gmem_RUSER),
    .RRESP(m_axi_gmem_RRESP),
    .BVALID(m_axi_gmem_BVALID),
    .BREADY(m_axi_gmem_BREADY),
    .BRESP(m_axi_gmem_BRESP),
    .BID(m_axi_gmem_BID),
    .BUSER(m_axi_gmem_BUSER),
    .ACLK(ap_clk),
    .ARESET(ap_rst_n_inv),
    .ACLK_EN(1'b1),
    .I_ARVALID(1'b0),
    .I_ARREADY(gmem_ARREADY),
    .I_ARADDR(64'd0),
    .I_ARLEN(32'd0),
    .I_RVALID(gmem_RVALID),
    .I_RREADY(1'b0),
    .I_RDATA(gmem_RDATA),
    .I_RFIFONUM(gmem_RFIFONUM),
    .I_AWVALID(gmem_AWVALID),
    .I_AWREADY(gmem_AWREADY),
    .I_AWADDR(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_AWADDR),
    .I_AWLEN(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_AWLEN),
    .I_WVALID(gmem_WVALID),
    .I_WREADY(gmem_WREADY),
    .I_WDATA(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_WDATA),
    .I_WSTRB(grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_WSTRB),
    .I_BVALID(gmem_BVALID),
    .I_BREADY(gmem_BREADY)
);

photons_maxi_id_regslice_both #(
    .DataWidth( 48 ))
regslice_both_photons_U(
    .ap_clk(ap_clk),
    .ap_rst(ap_rst_n_inv),
    .data_in(photons_TDATA),
    .vld_in(photons_TVALID),
    .ack_in(regslice_both_photons_U_ack_in),
    .data_out(photons_TDATA_int_regslice),
    .vld_out(photons_TVALID_int_regslice),
    .ack_out(photons_TREADY_int_regslice),
    .apdone_blk(regslice_both_photons_U_apdone_blk)
);

always @ (posedge ap_clk) begin
    if (ap_rst_n_inv == 1'b1) begin
        ap_CS_fsm <= ap_ST_fsm_state1;
    end else begin
        ap_CS_fsm <= ap_NS_fsm;
    end
end

always @ (posedge ap_clk) begin
    if (ap_rst_n_inv == 1'b1) begin
        grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_ap_start_reg <= 1'b0;
    end else begin
        if ((1'b1 == ap_CS_fsm_state2)) begin
            grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_ap_start_reg <= 1'b1;
        end else if ((grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_ap_ready == 1'b1)) begin
            grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_ap_start_reg <= 1'b0;
        end
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_CS_fsm_state4)) begin
        p_ab <= xor_ln140_fu_166_p2;
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_CS_fsm_state1)) begin
        photons_out_read_reg_182 <= photons_out;
        trunc_ln132_reg_187 <= trunc_ln132_fu_125_p1;
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_CS_fsm_state2)) begin
        sub_ln132_reg_195[17 : 14] <= sub_ln132_fu_159_p2[17 : 14];
    end
end

always @ (*) begin
    if ((ap_start == 1'b0)) begin
        ap_ST_fsm_state1_blk = 1'b1;
    end else begin
        ap_ST_fsm_state1_blk = 1'b0;
    end
end

assign ap_ST_fsm_state2_blk = 1'b0;

always @ (*) begin
    if ((grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_ap_done == 1'b0)) begin
        ap_ST_fsm_state3_blk = 1'b1;
    end else begin
        ap_ST_fsm_state3_blk = 1'b0;
    end
end

assign ap_ST_fsm_state4_blk = 1'b0;

assign ap_ST_fsm_state5_blk = 1'b0;

always @ (*) begin
    if ((1'b1 == ap_CS_fsm_state5)) begin
        ap_done = 1'b1;
    end else begin
        ap_done = 1'b0;
    end
end

always @ (*) begin
    if (((1'b1 == ap_CS_fsm_state1) & (ap_start == 1'b0))) begin
        ap_idle = 1'b1;
    end else begin
        ap_idle = 1'b0;
    end
end

always @ (*) begin
    if ((1'b1 == ap_CS_fsm_state5)) begin
        ap_ready = 1'b1;
    end else begin
        ap_ready = 1'b0;
    end
end

always @ (*) begin
    if (((1'b1 == ap_CS_fsm_state3) | (1'b1 == ap_CS_fsm_state2))) begin
        gmem_AWVALID = grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_AWVALID;
    end else begin
        gmem_AWVALID = 1'b0;
    end
end

always @ (*) begin
    if (((1'b1 == ap_CS_fsm_state3) | (1'b1 == ap_CS_fsm_state2))) begin
        gmem_BREADY = grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_BREADY;
    end else begin
        gmem_BREADY = 1'b0;
    end
end

always @ (*) begin
    if (((1'b1 == ap_CS_fsm_state3) | (1'b1 == ap_CS_fsm_state2))) begin
        gmem_WVALID = grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_m_axi_gmem_WVALID;
    end else begin
        gmem_WVALID = 1'b0;
    end
end

always @ (*) begin
    if ((1'b1 == ap_CS_fsm_state4)) begin
        n_photons_address0 = zext_ln142_fu_177_p1;
    end else if ((1'b1 == ap_CS_fsm_state3)) begin
        n_photons_address0 = grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_n_photons_address0;
    end else begin
        n_photons_address0 = 'bx;
    end
end

always @ (*) begin
    if (((1'b1 == ap_CS_fsm_state5) | (1'b1 == ap_CS_fsm_state4))) begin
        n_photons_ce0 = 1'b1;
    end else if ((1'b1 == ap_CS_fsm_state3)) begin
        n_photons_ce0 = grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_n_photons_ce0;
    end else begin
        n_photons_ce0 = 1'b0;
    end
end

always @ (*) begin
    if ((1'b1 == ap_CS_fsm_state4)) begin
        n_photons_d0 = 13'd0;
    end else if ((1'b1 == ap_CS_fsm_state3)) begin
        n_photons_d0 = grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_n_photons_d0;
    end else begin
        n_photons_d0 = 'bx;
    end
end

always @ (*) begin
    if ((1'b1 == ap_CS_fsm_state4)) begin
        n_photons_we0 = 1'b1;
    end else if ((1'b1 == ap_CS_fsm_state3)) begin
        n_photons_we0 = grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_n_photons_we0;
    end else begin
        n_photons_we0 = 1'b0;
    end
end

always @ (*) begin
    if ((1'b1 == ap_CS_fsm_state3)) begin
        photons_TREADY_int_regslice = grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_photons_TREADY;
    end else begin
        photons_TREADY_int_regslice = 1'b0;
    end
end

always @ (*) begin
    case (ap_CS_fsm)
        ap_ST_fsm_state1 : begin
            if (((1'b1 == ap_CS_fsm_state1) & (ap_start == 1'b1))) begin
                ap_NS_fsm = ap_ST_fsm_state2;
            end else begin
                ap_NS_fsm = ap_ST_fsm_state1;
            end
        end
        ap_ST_fsm_state2 : begin
            ap_NS_fsm = ap_ST_fsm_state3;
        end
        ap_ST_fsm_state3 : begin
            if (((1'b1 == ap_CS_fsm_state3) & (grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_ap_done == 1'b1))) begin
                ap_NS_fsm = ap_ST_fsm_state4;
            end else begin
                ap_NS_fsm = ap_ST_fsm_state3;
            end
        end
        ap_ST_fsm_state4 : begin
            ap_NS_fsm = ap_ST_fsm_state5;
        end
        ap_ST_fsm_state5 : begin
            ap_NS_fsm = ap_ST_fsm_state1;
        end
        default : begin
            ap_NS_fsm = 'bx;
        end
    endcase
end

assign ap_CS_fsm_state1 = ap_CS_fsm[32'd0];

assign ap_CS_fsm_state2 = ap_CS_fsm[32'd1];

assign ap_CS_fsm_state3 = ap_CS_fsm[32'd2];

assign ap_CS_fsm_state4 = ap_CS_fsm[32'd3];

assign ap_CS_fsm_state5 = ap_CS_fsm[32'd4];

always @ (*) begin
    ap_rst_n_inv = ~ap_rst_n;
end

assign grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_ap_start = grp_photons_maxi_id_Pipeline_VITIS_LOOP_115_1_fu_108_ap_start_reg;

assign photons_TREADY = regslice_both_photons_U_ack_in;

assign shl_ln132_3_fu_135_p3 = {{p_ab}, {16'd0}};

assign shl_ln132_4_fu_147_p3 = {{p_ab}, {14'd0}};

assign sub_ln132_fu_159_p2 = (zext_ln132_fu_143_p1 - zext_ln132_1_fu_155_p1);

assign trunc_ln132_fu_125_p1 = photons_out[2:0];

assign xor_ln140_fu_166_p2 = (p_ab ^ 1'd1);

assign zext_ln132_1_fu_155_p1 = shl_ln132_4_fu_147_p3;

assign zext_ln132_fu_143_p1 = shl_ln132_3_fu_135_p3;

assign zext_ln142_fu_177_p1 = xor_ln140_fu_166_p2;

always @ (posedge ap_clk) begin
    sub_ln132_reg_195[13:0] <= 14'b00000000000000;
end


reg find_kernel_block = 0;
// synthesis translate_off
`include "photons_maxi_id_hls_deadlock_kernel_monitor_top.vh"
// synthesis translate_on

endmodule //photons_maxi_id
