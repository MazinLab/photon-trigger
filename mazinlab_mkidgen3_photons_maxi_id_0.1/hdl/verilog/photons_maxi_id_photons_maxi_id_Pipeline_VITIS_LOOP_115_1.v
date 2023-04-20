// ==============================================================
// RTL generated by Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
// Version: 2022.1
// Copyright (C) Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// 
// ===========================================================

`timescale 1 ns / 1 ps 

module photons_maxi_id_photons_maxi_id_Pipeline_VITIS_LOOP_115_1 (
        ap_clk,
        ap_rst,
        ap_start,
        ap_done,
        ap_idle,
        ap_ready,
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
        m_axi_gmem_RFIFONUM,
        m_axi_gmem_RUSER,
        m_axi_gmem_RRESP,
        m_axi_gmem_BVALID,
        m_axi_gmem_BREADY,
        m_axi_gmem_BRESP,
        m_axi_gmem_BID,
        m_axi_gmem_BUSER,
        n_photons_address0,
        n_photons_ce0,
        n_photons_we0,
        n_photons_d0,
        idxprom,
        p_ab_load_cast9,
        active_buffer,
        active_buffer_ap_vld,
        photons_TDATA,
        photons_TVALID,
        photons_TREADY,
        photons_out,
        sext_ln134,
        trunc_ln132_3
);

parameter    ap_ST_fsm_pp0_stage0 = 3'd1;
parameter    ap_ST_fsm_pp0_stage1 = 3'd2;
parameter    ap_ST_fsm_pp0_stage2 = 3'd4;

input   ap_clk;
input   ap_rst;
input   ap_start;
output   ap_done;
output   ap_idle;
output   ap_ready;
output   m_axi_gmem_AWVALID;
input   m_axi_gmem_AWREADY;
output  [63:0] m_axi_gmem_AWADDR;
output  [0:0] m_axi_gmem_AWID;
output  [31:0] m_axi_gmem_AWLEN;
output  [2:0] m_axi_gmem_AWSIZE;
output  [1:0] m_axi_gmem_AWBURST;
output  [1:0] m_axi_gmem_AWLOCK;
output  [3:0] m_axi_gmem_AWCACHE;
output  [2:0] m_axi_gmem_AWPROT;
output  [3:0] m_axi_gmem_AWQOS;
output  [3:0] m_axi_gmem_AWREGION;
output  [0:0] m_axi_gmem_AWUSER;
output   m_axi_gmem_WVALID;
input   m_axi_gmem_WREADY;
output  [63:0] m_axi_gmem_WDATA;
output  [7:0] m_axi_gmem_WSTRB;
output   m_axi_gmem_WLAST;
output  [0:0] m_axi_gmem_WID;
output  [0:0] m_axi_gmem_WUSER;
output   m_axi_gmem_ARVALID;
input   m_axi_gmem_ARREADY;
output  [63:0] m_axi_gmem_ARADDR;
output  [0:0] m_axi_gmem_ARID;
output  [31:0] m_axi_gmem_ARLEN;
output  [2:0] m_axi_gmem_ARSIZE;
output  [1:0] m_axi_gmem_ARBURST;
output  [1:0] m_axi_gmem_ARLOCK;
output  [3:0] m_axi_gmem_ARCACHE;
output  [2:0] m_axi_gmem_ARPROT;
output  [3:0] m_axi_gmem_ARQOS;
output  [3:0] m_axi_gmem_ARREGION;
output  [0:0] m_axi_gmem_ARUSER;
input   m_axi_gmem_RVALID;
output   m_axi_gmem_RREADY;
input  [63:0] m_axi_gmem_RDATA;
input   m_axi_gmem_RLAST;
input  [0:0] m_axi_gmem_RID;
input  [8:0] m_axi_gmem_RFIFONUM;
input  [0:0] m_axi_gmem_RUSER;
input  [1:0] m_axi_gmem_RRESP;
input   m_axi_gmem_BVALID;
output   m_axi_gmem_BREADY;
input  [1:0] m_axi_gmem_BRESP;
input  [0:0] m_axi_gmem_BID;
input  [0:0] m_axi_gmem_BUSER;
output  [0:0] n_photons_address0;
output   n_photons_ce0;
output   n_photons_we0;
output  [12:0] n_photons_d0;
input  [0:0] idxprom;
input  [0:0] p_ab_load_cast9;
output  [7:0] active_buffer;
output   active_buffer_ap_vld;
input  [47:0] photons_TDATA;
input   photons_TVALID;
output   photons_TREADY;
input  [63:0] photons_out;
input  [17:0] sext_ln134;
input  [2:0] trunc_ln132_3;

reg ap_idle;
reg m_axi_gmem_AWVALID;
reg m_axi_gmem_WVALID;
reg[63:0] m_axi_gmem_WDATA;
reg[7:0] m_axi_gmem_WSTRB;
reg m_axi_gmem_BREADY;
reg n_photons_ce0;
reg n_photons_we0;
reg active_buffer_ap_vld;
reg photons_TREADY;

(* fsm_encoding = "none" *) reg   [2:0] ap_CS_fsm;
wire    ap_CS_fsm_pp0_stage0;
reg    ap_enable_reg_pp0_iter0;
reg    ap_enable_reg_pp0_iter1;
reg    ap_enable_reg_pp0_iter2;
reg    ap_idle_pp0;
wire    ap_CS_fsm_pp0_stage2;
wire    ap_block_state3_pp0_stage2_iter0;
reg   [0:0] and_ln115_reg_534;
reg    ap_block_state3_io;
wire    ap_block_state6_pp0_stage2_iter1;
reg   [0:0] and_ln115_reg_534_pp0_iter2_reg;
reg    ap_block_state9_pp0_stage2_iter2;
reg    ap_block_pp0_stage2_subdone;
reg    ap_condition_exit_pp0_iter0_stage2;
wire    ap_loop_exit_ready;
reg    ap_ready_int;
reg    gmem_blk_n_B;
wire    ap_block_pp0_stage2;
reg    gmem_blk_n_AW;
wire    ap_CS_fsm_pp0_stage1;
wire    ap_block_pp0_stage1;
reg    gmem_blk_n_W;
wire    ap_block_pp0_stage0;
reg   [0:0] icmp_ln132_reg_543;
reg    photons_TDATA_blk_n;
reg   [0:0] started_reg_215;
reg   [15:0] p_elapsed_V_5_reg_529;
wire    ap_block_state1_pp0_stage0_iter0;
wire    ap_block_state4_pp0_stage0_iter1;
reg    ap_block_state4_io;
wire    ap_block_state7_pp0_stage0_iter2;
reg    ap_block_pp0_stage0_11001;
wire   [0:0] and_ln115_fu_265_p2;
reg   [0:0] and_ln115_reg_534_pp0_iter1_reg;
wire   [2:0] add_ln132_2_fu_321_p2;
reg   [2:0] add_ln132_2_reg_538;
wire   [0:0] icmp_ln132_fu_337_p2;
reg   [60:0] trunc_ln1_reg_547;
wire   [31:0] select_ln132_fu_353_p3;
reg   [31:0] select_ln132_reg_552;
wire   [7:0] trunc_ln132_1_fu_361_p1;
reg   [7:0] trunc_ln132_1_reg_557;
reg   [4:0] tmp_1_reg_562;
reg    ap_block_state2_pp0_stage1_iter0;
reg    ap_block_state2_io;
wire    ap_block_state5_pp0_stage1_iter1;
wire    ap_block_state8_pp0_stage1_iter2;
reg    ap_block_pp0_stage1_11001;
wire   [63:0] trunc_ln132_2_fu_478_p1;
reg   [63:0] trunc_ln132_2_reg_574;
reg   [39:0] tmp_2_reg_579;
reg    ap_enable_reg_pp0_iter0_reg;
reg   [0:0] ap_phi_mux_started_phi_fu_220_p4;
wire    ap_loop_init;
reg    ap_block_pp0_stage0_subdone;
wire   [63:0] idxprom_cast_fu_237_p1;
wire  signed [63:0] sext_ln132_fu_468_p1;
reg    ap_block_pp0_stage2_11001;
reg    ap_block_pp0_stage2_01001;
wire   [63:0] zext_ln132_6_fu_506_p1;
wire    ap_block_pp0_stage0_01001;
wire   [7:0] zext_ln132_5_fu_502_p1;
reg   [15:0] p_start_V_fu_126;
wire   [15:0] photon_time_V_2_fu_439_p3;
reg   [15:0] p_elapsed_V_fu_130;
wire   [15:0] p_elapsed_V_3_fu_432_p3;
reg   [15:0] ap_sig_allocacmp_p_elapsed_V_5;
reg   [12:0] p_n_photons_V_fu_134;
wire   [12:0] p_n_photons_V_3_fu_375_p2;
wire   [0:0] icmp_ln1073_fu_253_p2;
wire   [0:0] icmp_ln1073_1_fu_259_p2;
wire   [15:0] shl_ln132_5_fu_271_p3;
wire   [13:0] shl_ln132_6_fu_283_p3;
wire   [16:0] zext_ln132_fu_279_p1;
wire   [16:0] zext_ln132_1_fu_291_p1;
wire  signed [16:0] sub_ln132_fu_295_p2;
wire  signed [63:0] sext_ln132_1_fu_301_p1;
wire   [63:0] add_ln132_fu_309_p2;
wire  signed [63:0] sext_ln134_cast_fu_228_p1;
wire   [2:0] trunc_ln132_fu_305_p1;
wire   [12:0] zext_ln132_3_fu_327_p1;
wire   [63:0] add_ln132_1_fu_315_p2;
wire   [12:0] shl_ln132_fu_331_p2;
wire   [15:0] photon_time_V_fu_390_p1;
wire   [15:0] xor_ln232_fu_400_p2;
wire   [0:0] icmp_ln1081_fu_394_p2;
wire   [0:0] and_ln1081_fu_418_p2;
wire   [15:0] p_elapsed_V_6_fu_406_p2;
wire   [15:0] p_elapsed_V_7_fu_412_p2;
wire   [15:0] p_elapsed_V_8_fu_424_p3;
wire   [5:0] shl_ln132_1_fu_451_p3;
wire   [103:0] zext_ln132_2_fu_447_p1;
wire   [103:0] zext_ln132_4_fu_458_p1;
wire   [103:0] shl_ln132_2_fu_462_p2;
reg    ap_done_reg;
wire    ap_continue_int;
reg    ap_done_int;
reg    ap_loop_exit_ready_pp0_iter1_reg;
reg   [2:0] ap_NS_fsm;
reg    ap_idle_pp0_1to2;
reg    ap_done_pending_pp0;
reg    ap_block_pp0_stage1_subdone;
wire    ap_enable_pp0;
wire    ap_start_int;
wire    ap_ce_reg;

// power-on initialization
initial begin
#0 ap_CS_fsm = 3'd1;
#0 ap_enable_reg_pp0_iter1 = 1'b0;
#0 ap_enable_reg_pp0_iter2 = 1'b0;
#0 ap_enable_reg_pp0_iter0_reg = 1'b0;
#0 ap_done_reg = 1'b0;
end

photons_maxi_id_flow_control_loop_pipe_sequential_init flow_control_loop_pipe_sequential_init_U(
    .ap_clk(ap_clk),
    .ap_rst(ap_rst),
    .ap_start(ap_start),
    .ap_ready(ap_ready),
    .ap_done(ap_done),
    .ap_start_int(ap_start_int),
    .ap_loop_init(ap_loop_init),
    .ap_ready_int(ap_ready_int),
    .ap_loop_exit_ready(ap_condition_exit_pp0_iter0_stage2),
    .ap_loop_exit_done(ap_done_int),
    .ap_continue_int(ap_continue_int),
    .ap_done_int(ap_done_int)
);

always @ (posedge ap_clk) begin
    if (ap_rst == 1'b1) begin
        ap_CS_fsm <= ap_ST_fsm_pp0_stage0;
    end else begin
        ap_CS_fsm <= ap_NS_fsm;
    end
end

always @ (posedge ap_clk) begin
    if (ap_rst == 1'b1) begin
        ap_done_reg <= 1'b0;
    end else begin
        if ((ap_continue_int == 1'b1)) begin
            ap_done_reg <= 1'b0;
        end else if (((ap_loop_exit_ready_pp0_iter1_reg == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage2) & (1'b0 == ap_block_pp0_stage2_subdone))) begin
            ap_done_reg <= 1'b1;
        end
    end
end

always @ (posedge ap_clk) begin
    if (ap_rst == 1'b1) begin
        ap_enable_reg_pp0_iter0_reg <= 1'b0;
    end else begin
        if ((1'b1 == ap_CS_fsm_pp0_stage0)) begin
            ap_enable_reg_pp0_iter0_reg <= ap_start_int;
        end
    end
end

always @ (posedge ap_clk) begin
    if (ap_rst == 1'b1) begin
        ap_enable_reg_pp0_iter1 <= 1'b0;
    end else begin
        if ((1'b1 == ap_condition_exit_pp0_iter0_stage2)) begin
            ap_enable_reg_pp0_iter1 <= 1'b0;
        end else if (((1'b1 == ap_CS_fsm_pp0_stage2) & (1'b0 == ap_block_pp0_stage2_subdone))) begin
            ap_enable_reg_pp0_iter1 <= ap_enable_reg_pp0_iter0;
        end
    end
end

always @ (posedge ap_clk) begin
    if (ap_rst == 1'b1) begin
        ap_enable_reg_pp0_iter2 <= 1'b0;
    end else begin
        if (((1'b1 == ap_CS_fsm_pp0_stage2) & (1'b0 == ap_block_pp0_stage2_subdone))) begin
            ap_enable_reg_pp0_iter2 <= ap_enable_reg_pp0_iter1;
        end
    end
end

always @ (posedge ap_clk) begin
    if (((ap_loop_init == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0) & (1'b0 == ap_block_pp0_stage0_11001))) begin
        p_elapsed_V_fu_130 <= 16'd0;
    end else if (((1'd1 == and_ln115_reg_534) & (ap_enable_reg_pp0_iter0 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage1) & (1'b0 == ap_block_pp0_stage1_11001))) begin
        p_elapsed_V_fu_130 <= p_elapsed_V_3_fu_432_p3;
    end
end

always @ (posedge ap_clk) begin
    if (((1'd1 == and_ln115_reg_534_pp0_iter1_reg) & (ap_enable_reg_pp0_iter1 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage1) & (1'b0 == ap_block_pp0_stage1_11001))) begin
        started_reg_215 <= 1'd1;
    end else if (((ap_loop_init == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0) & (1'b0 == ap_block_pp0_stage0_subdone))) begin
        started_reg_215 <= 1'd0;
    end
end

always @ (posedge ap_clk) begin
    if (((1'd1 == and_ln115_fu_265_p2) & (1'b1 == ap_CS_fsm_pp0_stage0) & (1'b0 == ap_block_pp0_stage0_11001))) begin
        add_ln132_2_reg_538 <= add_ln132_2_fu_321_p2;
        icmp_ln132_reg_543 <= icmp_ln132_fu_337_p2;
        select_ln132_reg_552[1 : 0] <= select_ln132_fu_353_p3[1 : 0];
        tmp_1_reg_562 <= {{shl_ln132_fu_331_p2[12:8]}};
        trunc_ln132_1_reg_557 <= trunc_ln132_1_fu_361_p1;
        trunc_ln1_reg_547 <= {{add_ln132_1_fu_315_p2[63:3]}};
    end
end

always @ (posedge ap_clk) begin
    if (((1'b1 == ap_CS_fsm_pp0_stage0) & (1'b0 == ap_block_pp0_stage0_11001))) begin
        and_ln115_reg_534 <= and_ln115_fu_265_p2;
        and_ln115_reg_534_pp0_iter1_reg <= and_ln115_reg_534;
        and_ln115_reg_534_pp0_iter2_reg <= and_ln115_reg_534_pp0_iter1_reg;
        p_elapsed_V_5_reg_529 <= ap_sig_allocacmp_p_elapsed_V_5;
    end
end

always @ (posedge ap_clk) begin
    if (((1'b1 == ap_CS_fsm_pp0_stage2) & (1'b0 == ap_block_pp0_stage2_11001))) begin
        ap_loop_exit_ready_pp0_iter1_reg <= ap_loop_exit_ready;
    end
end

always @ (posedge ap_clk) begin
    if (((1'd1 == and_ln115_fu_265_p2) & (ap_enable_reg_pp0_iter0 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0) & (1'b0 == ap_block_pp0_stage0_11001))) begin
        p_n_photons_V_fu_134 <= p_n_photons_V_3_fu_375_p2;
    end
end

always @ (posedge ap_clk) begin
    if (((1'd1 == and_ln115_reg_534) & (ap_enable_reg_pp0_iter0 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage1) & (1'b0 == ap_block_pp0_stage1_11001))) begin
        p_start_V_fu_126 <= photon_time_V_2_fu_439_p3;
    end
end

always @ (posedge ap_clk) begin
    if (((1'd1 == and_ln115_reg_534) & (1'b1 == ap_CS_fsm_pp0_stage1) & (1'b0 == ap_block_pp0_stage1_11001))) begin
        tmp_2_reg_579 <= {{shl_ln132_2_fu_462_p2[103:64]}};
        trunc_ln132_2_reg_574 <= trunc_ln132_2_fu_478_p1;
    end
end

always @ (*) begin
    if (((1'd1 == and_ln115_fu_265_p2) & (ap_enable_reg_pp0_iter0 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0) & (1'b0 == ap_block_pp0_stage0_11001))) begin
        active_buffer_ap_vld = 1'b1;
    end else begin
        active_buffer_ap_vld = 1'b0;
    end
end

always @ (*) begin
    if (((ap_enable_reg_pp0_iter0 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage2) & (1'd0 == and_ln115_reg_534) & (1'b0 == ap_block_pp0_stage2_subdone))) begin
        ap_condition_exit_pp0_iter0_stage2 = 1'b1;
    end else begin
        ap_condition_exit_pp0_iter0_stage2 = 1'b0;
    end
end

always @ (*) begin
    if (((ap_loop_exit_ready_pp0_iter1_reg == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage2) & (1'b0 == ap_block_pp0_stage2_subdone))) begin
        ap_done_int = 1'b1;
    end else begin
        ap_done_int = ap_done_reg;
    end
end

always @ (*) begin
    if (~((ap_loop_exit_ready_pp0_iter1_reg == 1'b0) & (ap_loop_exit_ready == 1'b0))) begin
        ap_done_pending_pp0 = 1'b1;
    end else begin
        ap_done_pending_pp0 = 1'b0;
    end
end

always @ (*) begin
    if ((1'b1 == ap_CS_fsm_pp0_stage0)) begin
        ap_enable_reg_pp0_iter0 = ap_start_int;
    end else begin
        ap_enable_reg_pp0_iter0 = ap_enable_reg_pp0_iter0_reg;
    end
end

always @ (*) begin
    if (((ap_start_int == 1'b0) & (ap_idle_pp0 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0))) begin
        ap_idle = 1'b1;
    end else begin
        ap_idle = 1'b0;
    end
end

always @ (*) begin
    if (((ap_enable_reg_pp0_iter2 == 1'b0) & (ap_enable_reg_pp0_iter1 == 1'b0) & (ap_enable_reg_pp0_iter0 == 1'b0))) begin
        ap_idle_pp0 = 1'b1;
    end else begin
        ap_idle_pp0 = 1'b0;
    end
end

always @ (*) begin
    if (((ap_enable_reg_pp0_iter2 == 1'b0) & (ap_enable_reg_pp0_iter1 == 1'b0))) begin
        ap_idle_pp0_1to2 = 1'b1;
    end else begin
        ap_idle_pp0_1to2 = 1'b0;
    end
end

always @ (*) begin
    if (((1'd1 == and_ln115_reg_534_pp0_iter1_reg) & (ap_enable_reg_pp0_iter1 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage1) & (1'b0 == ap_block_pp0_stage1))) begin
        ap_phi_mux_started_phi_fu_220_p4 = 1'd1;
    end else begin
        ap_phi_mux_started_phi_fu_220_p4 = started_reg_215;
    end
end

always @ (*) begin
    if (((ap_enable_reg_pp0_iter0 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage2) & (1'b0 == ap_block_pp0_stage2_subdone))) begin
        ap_ready_int = 1'b1;
    end else begin
        ap_ready_int = 1'b0;
    end
end

always @ (*) begin
    if (((ap_loop_init == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0) & (1'b0 == ap_block_pp0_stage0))) begin
        ap_sig_allocacmp_p_elapsed_V_5 = 16'd0;
    end else begin
        ap_sig_allocacmp_p_elapsed_V_5 = p_elapsed_V_fu_130;
    end
end

always @ (*) begin
    if (((1'd1 == and_ln115_reg_534) & (ap_enable_reg_pp0_iter0 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage1) & (1'b0 == ap_block_pp0_stage1))) begin
        gmem_blk_n_AW = m_axi_gmem_AWREADY;
    end else begin
        gmem_blk_n_AW = 1'b1;
    end
end

always @ (*) begin
    if (((1'd1 == and_ln115_reg_534_pp0_iter2_reg) & (ap_enable_reg_pp0_iter2 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage2) & (1'b0 == ap_block_pp0_stage2))) begin
        gmem_blk_n_B = m_axi_gmem_BVALID;
    end else begin
        gmem_blk_n_B = 1'b1;
    end
end

always @ (*) begin
    if ((((1'd1 == and_ln115_reg_534) & (ap_enable_reg_pp0_iter0 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage2) & (1'b0 == ap_block_pp0_stage2)) | ((ap_enable_reg_pp0_iter1 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0) & (icmp_ln132_reg_543 == 1'd1) & (1'b0 == ap_block_pp0_stage0)))) begin
        gmem_blk_n_W = m_axi_gmem_WREADY;
    end else begin
        gmem_blk_n_W = 1'b1;
    end
end

always @ (*) begin
    if (((1'd1 == and_ln115_reg_534) & (ap_enable_reg_pp0_iter0 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage1) & (1'b0 == ap_block_pp0_stage1_11001))) begin
        m_axi_gmem_AWVALID = 1'b1;
    end else begin
        m_axi_gmem_AWVALID = 1'b0;
    end
end

always @ (*) begin
    if (((1'd1 == and_ln115_reg_534_pp0_iter2_reg) & (ap_enable_reg_pp0_iter2 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage2) & (1'b0 == ap_block_pp0_stage2_11001))) begin
        m_axi_gmem_BREADY = 1'b1;
    end else begin
        m_axi_gmem_BREADY = 1'b0;
    end
end

always @ (*) begin
    if (((ap_enable_reg_pp0_iter1 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0) & (icmp_ln132_reg_543 == 1'd1) & (1'b0 == ap_block_pp0_stage0_01001))) begin
        m_axi_gmem_WDATA = zext_ln132_6_fu_506_p1;
    end else if (((1'd1 == and_ln115_reg_534) & (ap_enable_reg_pp0_iter0 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage2) & (1'b0 == ap_block_pp0_stage2_01001))) begin
        m_axi_gmem_WDATA = trunc_ln132_2_reg_574;
    end else begin
        m_axi_gmem_WDATA = 'bx;
    end
end

always @ (*) begin
    if (((ap_enable_reg_pp0_iter1 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0) & (icmp_ln132_reg_543 == 1'd1) & (1'b0 == ap_block_pp0_stage0_01001))) begin
        m_axi_gmem_WSTRB = zext_ln132_5_fu_502_p1;
    end else if (((1'd1 == and_ln115_reg_534) & (ap_enable_reg_pp0_iter0 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage2) & (1'b0 == ap_block_pp0_stage2_01001))) begin
        m_axi_gmem_WSTRB = trunc_ln132_1_reg_557;
    end else begin
        m_axi_gmem_WSTRB = 'bx;
    end
end

always @ (*) begin
    if ((((1'd1 == and_ln115_reg_534) & (ap_enable_reg_pp0_iter0 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage2) & (1'b0 == ap_block_pp0_stage2_11001)) | ((ap_enable_reg_pp0_iter1 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0) & (icmp_ln132_reg_543 == 1'd1) & (1'b0 == ap_block_pp0_stage0_11001)))) begin
        m_axi_gmem_WVALID = 1'b1;
    end else begin
        m_axi_gmem_WVALID = 1'b0;
    end
end

always @ (*) begin
    if (((ap_enable_reg_pp0_iter0 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0) & (1'b0 == ap_block_pp0_stage0_11001))) begin
        n_photons_ce0 = 1'b1;
    end else begin
        n_photons_ce0 = 1'b0;
    end
end

always @ (*) begin
    if (((1'd1 == and_ln115_fu_265_p2) & (ap_enable_reg_pp0_iter0 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0) & (1'b0 == ap_block_pp0_stage0_11001))) begin
        n_photons_we0 = 1'b1;
    end else begin
        n_photons_we0 = 1'b0;
    end
end

always @ (*) begin
    if (((1'd1 == and_ln115_reg_534) & (ap_enable_reg_pp0_iter0 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage1) & (1'b0 == ap_block_pp0_stage1))) begin
        photons_TDATA_blk_n = photons_TVALID;
    end else begin
        photons_TDATA_blk_n = 1'b1;
    end
end

always @ (*) begin
    if (((1'd1 == and_ln115_reg_534) & (ap_enable_reg_pp0_iter0 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage1) & (1'b0 == ap_block_pp0_stage1_11001))) begin
        photons_TREADY = 1'b1;
    end else begin
        photons_TREADY = 1'b0;
    end
end

always @ (*) begin
    case (ap_CS_fsm)
        ap_ST_fsm_pp0_stage0 : begin
            if ((~((ap_start_int == 1'b0) & (ap_done_pending_pp0 == 1'b0) & (ap_idle_pp0_1to2 == 1'b1)) & (1'b0 == ap_block_pp0_stage0_subdone))) begin
                ap_NS_fsm = ap_ST_fsm_pp0_stage1;
            end else begin
                ap_NS_fsm = ap_ST_fsm_pp0_stage0;
            end
        end
        ap_ST_fsm_pp0_stage1 : begin
            if ((1'b0 == ap_block_pp0_stage1_subdone)) begin
                ap_NS_fsm = ap_ST_fsm_pp0_stage2;
            end else begin
                ap_NS_fsm = ap_ST_fsm_pp0_stage1;
            end
        end
        ap_ST_fsm_pp0_stage2 : begin
            if ((1'b0 == ap_block_pp0_stage2_subdone)) begin
                ap_NS_fsm = ap_ST_fsm_pp0_stage0;
            end else begin
                ap_NS_fsm = ap_ST_fsm_pp0_stage2;
            end
        end
        default : begin
            ap_NS_fsm = 'bx;
        end
    endcase
end

assign active_buffer = p_ab_load_cast9;

assign add_ln132_1_fu_315_p2 = ($signed(add_ln132_fu_309_p2) + $signed(sext_ln134_cast_fu_228_p1));

assign add_ln132_2_fu_321_p2 = (trunc_ln132_3 + trunc_ln132_fu_305_p1);

assign add_ln132_fu_309_p2 = ($signed(sext_ln132_1_fu_301_p1) + $signed(photons_out));

assign and_ln1081_fu_418_p2 = (icmp_ln1081_fu_394_p2 & ap_phi_mux_started_phi_fu_220_p4);

assign and_ln115_fu_265_p2 = (icmp_ln1073_fu_253_p2 & icmp_ln1073_1_fu_259_p2);

assign ap_CS_fsm_pp0_stage0 = ap_CS_fsm[32'd0];

assign ap_CS_fsm_pp0_stage1 = ap_CS_fsm[32'd1];

assign ap_CS_fsm_pp0_stage2 = ap_CS_fsm[32'd2];

assign ap_block_pp0_stage0 = ~(1'b1 == 1'b1);

assign ap_block_pp0_stage0_01001 = ~(1'b1 == 1'b1);

always @ (*) begin
    ap_block_pp0_stage0_11001 = ((ap_enable_reg_pp0_iter1 == 1'b1) & (1'b1 == ap_block_state4_io));
end

always @ (*) begin
    ap_block_pp0_stage0_subdone = ((ap_enable_reg_pp0_iter1 == 1'b1) & (1'b1 == ap_block_state4_io));
end

assign ap_block_pp0_stage1 = ~(1'b1 == 1'b1);

always @ (*) begin
    ap_block_pp0_stage1_11001 = ((ap_enable_reg_pp0_iter0 == 1'b1) & ((1'b1 == ap_block_state2_io) | ((1'd1 == and_ln115_reg_534) & (photons_TVALID == 1'b0))));
end

always @ (*) begin
    ap_block_pp0_stage1_subdone = ((ap_enable_reg_pp0_iter0 == 1'b1) & ((1'b1 == ap_block_state2_io) | ((1'd1 == and_ln115_reg_534) & (photons_TVALID == 1'b0))));
end

assign ap_block_pp0_stage2 = ~(1'b1 == 1'b1);

always @ (*) begin
    ap_block_pp0_stage2_01001 = ((1'd1 == and_ln115_reg_534_pp0_iter2_reg) & (m_axi_gmem_BVALID == 1'b0) & (ap_enable_reg_pp0_iter2 == 1'b1));
end

always @ (*) begin
    ap_block_pp0_stage2_11001 = (((ap_enable_reg_pp0_iter0 == 1'b1) & (1'b1 == ap_block_state3_io)) | ((1'd1 == and_ln115_reg_534_pp0_iter2_reg) & (m_axi_gmem_BVALID == 1'b0) & (ap_enable_reg_pp0_iter2 == 1'b1)));
end

always @ (*) begin
    ap_block_pp0_stage2_subdone = (((ap_enable_reg_pp0_iter0 == 1'b1) & (1'b1 == ap_block_state3_io)) | ((1'd1 == and_ln115_reg_534_pp0_iter2_reg) & (m_axi_gmem_BVALID == 1'b0) & (ap_enable_reg_pp0_iter2 == 1'b1)));
end

assign ap_block_state1_pp0_stage0_iter0 = ~(1'b1 == 1'b1);

always @ (*) begin
    ap_block_state2_io = ((1'd1 == and_ln115_reg_534) & (m_axi_gmem_AWREADY == 1'b0));
end

always @ (*) begin
    ap_block_state2_pp0_stage1_iter0 = ((1'd1 == and_ln115_reg_534) & (photons_TVALID == 1'b0));
end

always @ (*) begin
    ap_block_state3_io = ((1'd1 == and_ln115_reg_534) & (m_axi_gmem_WREADY == 1'b0));
end

assign ap_block_state3_pp0_stage2_iter0 = ~(1'b1 == 1'b1);

always @ (*) begin
    ap_block_state4_io = ((m_axi_gmem_WREADY == 1'b0) & (icmp_ln132_reg_543 == 1'd1));
end

assign ap_block_state4_pp0_stage0_iter1 = ~(1'b1 == 1'b1);

assign ap_block_state5_pp0_stage1_iter1 = ~(1'b1 == 1'b1);

assign ap_block_state6_pp0_stage2_iter1 = ~(1'b1 == 1'b1);

assign ap_block_state7_pp0_stage0_iter2 = ~(1'b1 == 1'b1);

assign ap_block_state8_pp0_stage1_iter2 = ~(1'b1 == 1'b1);

always @ (*) begin
    ap_block_state9_pp0_stage2_iter2 = ((1'd1 == and_ln115_reg_534_pp0_iter2_reg) & (m_axi_gmem_BVALID == 1'b0));
end

assign ap_enable_pp0 = (ap_idle_pp0 ^ 1'b1);

assign ap_loop_exit_ready = ap_condition_exit_pp0_iter0_stage2;

assign icmp_ln1073_1_fu_259_p2 = ((ap_sig_allocacmp_p_elapsed_V_5 < 16'd500) ? 1'b1 : 1'b0);

assign icmp_ln1073_fu_253_p2 = ((p_n_photons_V_fu_134 != 13'd8191) ? 1'b1 : 1'b0);

assign icmp_ln1081_fu_394_p2 = ((p_start_V_fu_126 > photon_time_V_fu_390_p1) ? 1'b1 : 1'b0);

assign icmp_ln132_fu_337_p2 = ((add_ln132_2_fu_321_p2 > 3'd2) ? 1'b1 : 1'b0);

assign idxprom_cast_fu_237_p1 = idxprom;

assign m_axi_gmem_ARADDR = 64'd0;

assign m_axi_gmem_ARBURST = 2'd0;

assign m_axi_gmem_ARCACHE = 4'd0;

assign m_axi_gmem_ARID = 1'd0;

assign m_axi_gmem_ARLEN = 32'd0;

assign m_axi_gmem_ARLOCK = 2'd0;

assign m_axi_gmem_ARPROT = 3'd0;

assign m_axi_gmem_ARQOS = 4'd0;

assign m_axi_gmem_ARREGION = 4'd0;

assign m_axi_gmem_ARSIZE = 3'd0;

assign m_axi_gmem_ARUSER = 1'd0;

assign m_axi_gmem_ARVALID = 1'b0;

assign m_axi_gmem_AWADDR = sext_ln132_fu_468_p1;

assign m_axi_gmem_AWBURST = 2'd0;

assign m_axi_gmem_AWCACHE = 4'd0;

assign m_axi_gmem_AWID = 1'd0;

assign m_axi_gmem_AWLEN = select_ln132_reg_552;

assign m_axi_gmem_AWLOCK = 2'd0;

assign m_axi_gmem_AWPROT = 3'd0;

assign m_axi_gmem_AWQOS = 4'd0;

assign m_axi_gmem_AWREGION = 4'd0;

assign m_axi_gmem_AWSIZE = 3'd0;

assign m_axi_gmem_AWUSER = 1'd0;

assign m_axi_gmem_RREADY = 1'b0;

assign m_axi_gmem_WID = 1'd0;

assign m_axi_gmem_WLAST = 1'b0;

assign m_axi_gmem_WUSER = 1'd0;

assign n_photons_address0 = idxprom_cast_fu_237_p1;

assign n_photons_d0 = (p_n_photons_V_fu_134 + 13'd1);

assign p_elapsed_V_3_fu_432_p3 = ((ap_phi_mux_started_phi_fu_220_p4[0:0] == 1'b1) ? p_elapsed_V_8_fu_424_p3 : p_elapsed_V_5_reg_529);

assign p_elapsed_V_6_fu_406_p2 = (photon_time_V_fu_390_p1 + xor_ln232_fu_400_p2);

assign p_elapsed_V_7_fu_412_p2 = (photon_time_V_fu_390_p1 - p_start_V_fu_126);

assign p_elapsed_V_8_fu_424_p3 = ((and_ln1081_fu_418_p2[0:0] == 1'b1) ? p_elapsed_V_6_fu_406_p2 : p_elapsed_V_7_fu_412_p2);

assign p_n_photons_V_3_fu_375_p2 = (p_n_photons_V_fu_134 + 13'd1);

assign photon_time_V_2_fu_439_p3 = ((ap_phi_mux_started_phi_fu_220_p4[0:0] == 1'b1) ? p_start_V_fu_126 : photon_time_V_fu_390_p1);

assign photon_time_V_fu_390_p1 = photons_TDATA[15:0];

assign select_ln132_fu_353_p3 = ((icmp_ln132_fu_337_p2[0:0] == 1'b1) ? 32'd2 : 32'd1);

assign sext_ln132_1_fu_301_p1 = sub_ln132_fu_295_p2;

assign sext_ln132_fu_468_p1 = $signed(trunc_ln1_reg_547);

assign sext_ln134_cast_fu_228_p1 = $signed(sext_ln134);

assign shl_ln132_1_fu_451_p3 = {{add_ln132_2_reg_538}, {3'd0}};

assign shl_ln132_2_fu_462_p2 = zext_ln132_2_fu_447_p1 << zext_ln132_4_fu_458_p1;

assign shl_ln132_5_fu_271_p3 = {{p_n_photons_V_fu_134}, {3'd0}};

assign shl_ln132_6_fu_283_p3 = {{p_n_photons_V_fu_134}, {1'd0}};

assign shl_ln132_fu_331_p2 = 13'd63 << zext_ln132_3_fu_327_p1;

assign sub_ln132_fu_295_p2 = (zext_ln132_fu_279_p1 - zext_ln132_1_fu_291_p1);

assign trunc_ln132_1_fu_361_p1 = shl_ln132_fu_331_p2[7:0];

assign trunc_ln132_2_fu_478_p1 = shl_ln132_2_fu_462_p2[63:0];

assign trunc_ln132_fu_305_p1 = sub_ln132_fu_295_p2[2:0];

assign xor_ln232_fu_400_p2 = (p_start_V_fu_126 ^ 16'd65535);

assign zext_ln132_1_fu_291_p1 = shl_ln132_6_fu_283_p3;

assign zext_ln132_2_fu_447_p1 = photons_TDATA;

assign zext_ln132_3_fu_327_p1 = add_ln132_2_fu_321_p2;

assign zext_ln132_4_fu_458_p1 = shl_ln132_1_fu_451_p3;

assign zext_ln132_5_fu_502_p1 = tmp_1_reg_562;

assign zext_ln132_6_fu_506_p1 = tmp_2_reg_579;

assign zext_ln132_fu_279_p1 = shl_ln132_5_fu_271_p3;

always @ (posedge ap_clk) begin
    select_ln132_reg_552[31:2] <= 30'b000000000000000000000000000000;
end

endmodule //photons_maxi_id_photons_maxi_id_Pipeline_VITIS_LOOP_115_1