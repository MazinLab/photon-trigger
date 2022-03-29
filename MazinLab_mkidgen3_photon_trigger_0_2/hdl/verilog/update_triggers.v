// ==============================================================
// RTL generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and OpenCL
// Version: 2019.2.1
// Copyright (C) 1986-2019 Xilinx, Inc. All Rights Reserved.
// 
// ===========================================================

`timescale 1 ns / 1 ps 

module update_triggers (
        ap_clk,
        ap_rst,
        data_0_phase_V_read,
        data_1_phase_V_read,
        data_2_phase_V_read,
        data_3_phase_V_read,
        data_0_baseline_V_r,
        data_1_baseline_V_r,
        data_2_baseline_V_r,
        data_3_baseline_V_r,
        trig_cfg_0_threshol,
        trig_cfg_1_threshol,
        trig_cfg_2_threshol,
        trig_cfg_3_threshol,
        ap_return_0,
        ap_return_1,
        ap_return_2,
        ap_return_3,
        ap_return_4,
        ap_return_5,
        ap_return_6,
        ap_return_7,
        ap_return_8,
        ap_return_9,
        ap_return_10,
        ap_return_11
);


input   ap_clk;
input   ap_rst;
input  [15:0] data_0_phase_V_read;
input  [15:0] data_1_phase_V_read;
input  [15:0] data_2_phase_V_read;
input  [15:0] data_3_phase_V_read;
input  [15:0] data_0_baseline_V_r;
input  [15:0] data_1_baseline_V_r;
input  [15:0] data_2_baseline_V_r;
input  [15:0] data_3_baseline_V_r;
input  [15:0] trig_cfg_0_threshol;
input  [15:0] trig_cfg_1_threshol;
input  [15:0] trig_cfg_2_threshol;
input  [15:0] trig_cfg_3_threshol;
output  [15:0] ap_return_0;
output  [15:0] ap_return_1;
output  [15:0] ap_return_2;
output  [15:0] ap_return_3;
output  [15:0] ap_return_4;
output  [15:0] ap_return_5;
output  [15:0] ap_return_6;
output  [15:0] ap_return_7;
output  [0:0] ap_return_8;
output  [0:0] ap_return_9;
output  [0:0] ap_return_10;
output  [0:0] ap_return_11;

reg   [15:0] trig_cfg_3_threshol_1_reg_212;
wire    ap_block_state1_pp0_stage0_iter0;
wire    ap_block_state2_pp0_stage0_iter1;
wire    ap_block_pp0_stage0_11001;
reg   [15:0] trig_cfg_2_threshol_1_reg_217;
reg   [15:0] trig_cfg_1_threshol_1_reg_222;
reg   [15:0] trig_cfg_0_threshol_1_reg_227;
reg   [15:0] data_3_baseline_V_r_1_reg_232;
reg   [15:0] data_2_baseline_V_r_1_reg_237;
reg   [15:0] data_1_baseline_V_r_1_reg_242;
reg   [15:0] data_0_baseline_V_r_1_reg_247;
reg   [15:0] data_3_phase_V_read_1_reg_252;
reg   [15:0] data_2_phase_V_read_1_reg_257;
reg   [15:0] data_1_phase_V_read_1_reg_262;
reg   [15:0] data_0_phase_V_read_1_reg_267;
wire   [15:0] sub_ln703_fu_108_p2;
reg   [15:0] sub_ln703_reg_272;
wire   [15:0] sub_ln703_1_fu_114_p2;
reg   [15:0] sub_ln703_1_reg_277;
wire   [15:0] sub_ln703_2_fu_120_p2;
reg   [15:0] sub_ln703_2_reg_282;
wire   [15:0] sub_ln703_3_fu_126_p2;
reg   [15:0] sub_ln703_3_reg_287;
wire    ap_block_pp0_stage0;
wire   [0:0] icmp_ln1495_fu_132_p2;
wire   [0:0] icmp_ln1495_1_fu_136_p2;
wire   [0:0] icmp_ln1495_2_fu_140_p2;
wire   [0:0] icmp_ln1495_3_fu_144_p2;

always @ (posedge ap_clk) begin
    if ((1'b0 == ap_block_pp0_stage0_11001)) begin
        data_0_baseline_V_r_1_reg_247 <= data_0_baseline_V_r;
        data_0_phase_V_read_1_reg_267 <= data_0_phase_V_read;
        data_1_baseline_V_r_1_reg_242 <= data_1_baseline_V_r;
        data_1_phase_V_read_1_reg_262 <= data_1_phase_V_read;
        data_2_baseline_V_r_1_reg_237 <= data_2_baseline_V_r;
        data_2_phase_V_read_1_reg_257 <= data_2_phase_V_read;
        data_3_baseline_V_r_1_reg_232 <= data_3_baseline_V_r;
        data_3_phase_V_read_1_reg_252 <= data_3_phase_V_read;
        sub_ln703_1_reg_277 <= sub_ln703_1_fu_114_p2;
        sub_ln703_2_reg_282 <= sub_ln703_2_fu_120_p2;
        sub_ln703_3_reg_287 <= sub_ln703_3_fu_126_p2;
        sub_ln703_reg_272 <= sub_ln703_fu_108_p2;
        trig_cfg_0_threshol_1_reg_227 <= trig_cfg_0_threshol;
        trig_cfg_1_threshol_1_reg_222 <= trig_cfg_1_threshol;
        trig_cfg_2_threshol_1_reg_217 <= trig_cfg_2_threshol;
        trig_cfg_3_threshol_1_reg_212 <= trig_cfg_3_threshol;
    end
end

assign ap_block_pp0_stage0 = ~(1'b1 == 1'b1);

assign ap_block_pp0_stage0_11001 = ~(1'b1 == 1'b1);

assign ap_block_state1_pp0_stage0_iter0 = ~(1'b1 == 1'b1);

assign ap_block_state2_pp0_stage0_iter1 = ~(1'b1 == 1'b1);

assign ap_return_0 = data_0_phase_V_read_1_reg_267;

assign ap_return_1 = data_1_phase_V_read_1_reg_262;

assign ap_return_10 = icmp_ln1495_2_fu_140_p2;

assign ap_return_11 = icmp_ln1495_3_fu_144_p2;

assign ap_return_2 = data_2_phase_V_read_1_reg_257;

assign ap_return_3 = data_3_phase_V_read_1_reg_252;

assign ap_return_4 = data_0_baseline_V_r_1_reg_247;

assign ap_return_5 = data_1_baseline_V_r_1_reg_242;

assign ap_return_6 = data_2_baseline_V_r_1_reg_237;

assign ap_return_7 = data_3_baseline_V_r_1_reg_232;

assign ap_return_8 = icmp_ln1495_fu_132_p2;

assign ap_return_9 = icmp_ln1495_1_fu_136_p2;

assign icmp_ln1495_1_fu_136_p2 = (($signed(sub_ln703_1_reg_277) < $signed(trig_cfg_1_threshol_1_reg_222)) ? 1'b1 : 1'b0);

assign icmp_ln1495_2_fu_140_p2 = (($signed(sub_ln703_2_reg_282) < $signed(trig_cfg_2_threshol_1_reg_217)) ? 1'b1 : 1'b0);

assign icmp_ln1495_3_fu_144_p2 = (($signed(sub_ln703_3_reg_287) < $signed(trig_cfg_3_threshol_1_reg_212)) ? 1'b1 : 1'b0);

assign icmp_ln1495_fu_132_p2 = (($signed(sub_ln703_reg_272) < $signed(trig_cfg_0_threshol_1_reg_227)) ? 1'b1 : 1'b0);

assign sub_ln703_1_fu_114_p2 = (data_1_phase_V_read - data_1_baseline_V_r);

assign sub_ln703_2_fu_120_p2 = (data_2_phase_V_read - data_2_baseline_V_r);

assign sub_ln703_3_fu_126_p2 = (data_3_phase_V_read - data_3_baseline_V_r);

assign sub_ln703_fu_108_p2 = (data_0_phase_V_read - data_0_baseline_V_r);

endmodule //update_triggers