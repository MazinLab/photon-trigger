// ==============================================================
// RTL generated by Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
// Version: 2022.1
// Copyright (C) Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// 
// ===========================================================

`timescale 1 ns / 1 ps 

module postage_maxi_postage_maxi_Pipeline_VITIS_LOOP_120_1_VITIS_LOOP_121_2 (
        ap_clk,
        ap_rst,
        ap_start,
        ap_done,
        ap_idle,
        ap_ready,
        buf_r_address0,
        buf_r_ce0,
        buf_r_we0,
        buf_r_d0,
        sample_count_address0,
        sample_count_ce0,
        sample_count_we0,
        sample_count_d0,
        p_event_count_address0,
        p_event_count_ce0,
        p_event_count_we0,
        p_event_count_d0
);

parameter    ap_ST_fsm_pp0_stage0 = 1'd1;

input   ap_clk;
input   ap_rst;
input   ap_start;
output   ap_done;
output   ap_idle;
output   ap_ready;
output  [9:0] buf_r_address0;
output   buf_r_ce0;
output   buf_r_we0;
output  [31:0] buf_r_d0;
output  [2:0] sample_count_address0;
output   sample_count_ce0;
output   sample_count_we0;
output  [7:0] sample_count_d0;
output  [2:0] p_event_count_address0;
output   p_event_count_ce0;
output   p_event_count_we0;
output  [15:0] p_event_count_d0;

reg ap_idle;
reg buf_r_ce0;
reg buf_r_we0;
reg sample_count_ce0;
reg sample_count_we0;
reg p_event_count_ce0;
reg p_event_count_we0;

(* fsm_encoding = "none" *) reg   [0:0] ap_CS_fsm;
wire    ap_CS_fsm_pp0_stage0;
wire    ap_enable_reg_pp0_iter0;
reg    ap_enable_reg_pp0_iter1;
reg    ap_enable_reg_pp0_iter2;
reg    ap_enable_reg_pp0_iter3;
reg    ap_idle_pp0;
wire    ap_block_state1_pp0_stage0_iter0;
wire    ap_block_state2_pp0_stage0_iter1;
wire    ap_block_state3_pp0_stage0_iter2;
wire    ap_block_state4_pp0_stage0_iter3;
wire    ap_block_pp0_stage0_subdone;
wire   [0:0] icmp_ln120_fu_122_p2;
reg    ap_condition_exit_pp0_iter0_stage0;
wire    ap_loop_exit_ready;
reg    ap_ready_int;
wire    ap_block_pp0_stage0_11001;
wire   [6:0] select_ln120_fu_152_p3;
reg   [6:0] select_ln120_reg_245;
reg   [6:0] select_ln120_reg_245_pp0_iter1_reg;
wire   [3:0] select_ln120_1_fu_160_p3;
reg   [3:0] select_ln120_1_reg_250;
wire   [0:0] ifzero_fu_178_p2;
reg   [0:0] ifzero_reg_260;
wire   [63:0] zext_ln120_fu_199_p1;
wire    ap_block_pp0_stage0;
wire   [63:0] zext_ln121_1_fu_207_p1;
reg   [6:0] j_fu_50;
wire   [6:0] add_ln121_fu_172_p2;
wire    ap_loop_init;
reg   [6:0] ap_sig_allocacmp_j_load;
reg   [3:0] i_fu_54;
reg   [3:0] ap_sig_allocacmp_i_load;
reg   [9:0] indvar_flatten_fu_58;
wire   [9:0] add_ln120_1_fu_128_p2;
reg   [9:0] ap_sig_allocacmp_indvar_flatten_load;
wire   [0:0] icmp_ln121_fu_146_p2;
wire   [3:0] add_ln120_fu_140_p2;
wire   [9:0] grp_fu_211_p3;
wire   [3:0] grp_fu_211_p0;
wire   [6:0] grp_fu_211_p1;
wire   [6:0] grp_fu_211_p2;
reg    ap_done_reg;
wire    ap_continue_int;
reg    ap_done_int;
reg    ap_loop_exit_ready_pp0_iter1_reg;
reg    ap_loop_exit_ready_pp0_iter2_reg;
reg   [0:0] ap_NS_fsm;
wire    ap_enable_pp0;
wire    ap_start_int;
wire   [9:0] grp_fu_211_p00;
wire   [9:0] grp_fu_211_p20;
wire    ap_ce_reg;

// power-on initialization
initial begin
#0 ap_CS_fsm = 1'd1;
#0 ap_enable_reg_pp0_iter1 = 1'b0;
#0 ap_enable_reg_pp0_iter2 = 1'b0;
#0 ap_enable_reg_pp0_iter3 = 1'b0;
#0 ap_done_reg = 1'b0;
end

postage_maxi_mac_muladd_4ns_7ns_7ns_10_4_1 #(
    .ID( 1 ),
    .NUM_STAGE( 4 ),
    .din0_WIDTH( 4 ),
    .din1_WIDTH( 7 ),
    .din2_WIDTH( 7 ),
    .dout_WIDTH( 10 ))
mac_muladd_4ns_7ns_7ns_10_4_1_U1(
    .clk(ap_clk),
    .reset(ap_rst),
    .din0(grp_fu_211_p0),
    .din1(grp_fu_211_p1),
    .din2(grp_fu_211_p2),
    .ce(1'b1),
    .dout(grp_fu_211_p3)
);

postage_maxi_flow_control_loop_pipe_sequential_init flow_control_loop_pipe_sequential_init_U(
    .ap_clk(ap_clk),
    .ap_rst(ap_rst),
    .ap_start(ap_start),
    .ap_ready(ap_ready),
    .ap_done(ap_done),
    .ap_start_int(ap_start_int),
    .ap_loop_init(ap_loop_init),
    .ap_ready_int(ap_ready_int),
    .ap_loop_exit_ready(ap_condition_exit_pp0_iter0_stage0),
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
        end else if (((1'b0 == ap_block_pp0_stage0_subdone) & (ap_loop_exit_ready_pp0_iter2_reg == 1'b1))) begin
            ap_done_reg <= 1'b1;
        end
    end
end

always @ (posedge ap_clk) begin
    if (ap_rst == 1'b1) begin
        ap_enable_reg_pp0_iter1 <= 1'b0;
    end else begin
        if ((1'b1 == ap_condition_exit_pp0_iter0_stage0)) begin
            ap_enable_reg_pp0_iter1 <= 1'b0;
        end else if (((1'b0 == ap_block_pp0_stage0_subdone) & (1'b1 == ap_CS_fsm_pp0_stage0))) begin
            ap_enable_reg_pp0_iter1 <= ap_start_int;
        end
    end
end

always @ (posedge ap_clk) begin
    if (ap_rst == 1'b1) begin
        ap_enable_reg_pp0_iter2 <= 1'b0;
    end else begin
        if ((1'b0 == ap_block_pp0_stage0_subdone)) begin
            ap_enable_reg_pp0_iter2 <= ap_enable_reg_pp0_iter1;
        end
    end
end

always @ (posedge ap_clk) begin
    if (ap_rst == 1'b1) begin
        ap_enable_reg_pp0_iter3 <= 1'b0;
    end else begin
        if ((1'b0 == ap_block_pp0_stage0_subdone)) begin
            ap_enable_reg_pp0_iter3 <= ap_enable_reg_pp0_iter2;
        end
    end
end

always @ (posedge ap_clk) begin
    if (((1'b0 == ap_block_pp0_stage0_11001) & (1'b1 == ap_CS_fsm_pp0_stage0))) begin
        if (((icmp_ln120_fu_122_p2 == 1'd0) & (ap_enable_reg_pp0_iter0 == 1'b1))) begin
            i_fu_54 <= select_ln120_1_fu_160_p3;
        end else if ((ap_loop_init == 1'b1)) begin
            i_fu_54 <= 4'd0;
        end
    end
end

always @ (posedge ap_clk) begin
    if (((1'b0 == ap_block_pp0_stage0_11001) & (1'b1 == ap_CS_fsm_pp0_stage0))) begin
        if (((icmp_ln120_fu_122_p2 == 1'd0) & (ap_enable_reg_pp0_iter0 == 1'b1))) begin
            indvar_flatten_fu_58 <= add_ln120_1_fu_128_p2;
        end else if ((ap_loop_init == 1'b1)) begin
            indvar_flatten_fu_58 <= 10'd0;
        end
    end
end

always @ (posedge ap_clk) begin
    if (((1'b0 == ap_block_pp0_stage0_11001) & (1'b1 == ap_CS_fsm_pp0_stage0))) begin
        if (((icmp_ln120_fu_122_p2 == 1'd0) & (ap_enable_reg_pp0_iter0 == 1'b1))) begin
            j_fu_50 <= add_ln121_fu_172_p2;
        end else if ((ap_loop_init == 1'b1)) begin
            j_fu_50 <= 7'd0;
        end
    end
end

always @ (posedge ap_clk) begin
    if (((1'b0 == ap_block_pp0_stage0_11001) & (1'b1 == ap_CS_fsm_pp0_stage0))) begin
        ap_loop_exit_ready_pp0_iter1_reg <= ap_loop_exit_ready;
        ap_loop_exit_ready_pp0_iter2_reg <= ap_loop_exit_ready_pp0_iter1_reg;
        select_ln120_reg_245_pp0_iter1_reg <= select_ln120_reg_245;
    end
end

always @ (posedge ap_clk) begin
    if (((icmp_ln120_fu_122_p2 == 1'd0) & (1'b0 == ap_block_pp0_stage0_11001) & (1'b1 == ap_CS_fsm_pp0_stage0))) begin
        ifzero_reg_260 <= ifzero_fu_178_p2;
        select_ln120_1_reg_250 <= select_ln120_1_fu_160_p3;
        select_ln120_reg_245 <= select_ln120_fu_152_p3;
    end
end

always @ (*) begin
    if (((icmp_ln120_fu_122_p2 == 1'd1) & (1'b0 == ap_block_pp0_stage0_subdone) & (ap_enable_reg_pp0_iter0 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0))) begin
        ap_condition_exit_pp0_iter0_stage0 = 1'b1;
    end else begin
        ap_condition_exit_pp0_iter0_stage0 = 1'b0;
    end
end

always @ (*) begin
    if (((1'b0 == ap_block_pp0_stage0_subdone) & (ap_loop_exit_ready_pp0_iter2_reg == 1'b1))) begin
        ap_done_int = 1'b1;
    end else begin
        ap_done_int = ap_done_reg;
    end
end

always @ (*) begin
    if (((ap_idle_pp0 == 1'b1) & (ap_start_int == 1'b0) & (1'b1 == ap_CS_fsm_pp0_stage0))) begin
        ap_idle = 1'b1;
    end else begin
        ap_idle = 1'b0;
    end
end

always @ (*) begin
    if (((ap_enable_reg_pp0_iter3 == 1'b0) & (ap_enable_reg_pp0_iter2 == 1'b0) & (ap_enable_reg_pp0_iter1 == 1'b0) & (ap_enable_reg_pp0_iter0 == 1'b0))) begin
        ap_idle_pp0 = 1'b1;
    end else begin
        ap_idle_pp0 = 1'b0;
    end
end

always @ (*) begin
    if (((1'b0 == ap_block_pp0_stage0_subdone) & (ap_enable_reg_pp0_iter0 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0))) begin
        ap_ready_int = 1'b1;
    end else begin
        ap_ready_int = 1'b0;
    end
end

always @ (*) begin
    if (((1'b0 == ap_block_pp0_stage0) & (1'b1 == ap_CS_fsm_pp0_stage0) & (ap_loop_init == 1'b1))) begin
        ap_sig_allocacmp_i_load = 4'd0;
    end else begin
        ap_sig_allocacmp_i_load = i_fu_54;
    end
end

always @ (*) begin
    if (((1'b0 == ap_block_pp0_stage0) & (1'b1 == ap_CS_fsm_pp0_stage0) & (ap_loop_init == 1'b1))) begin
        ap_sig_allocacmp_indvar_flatten_load = 10'd0;
    end else begin
        ap_sig_allocacmp_indvar_flatten_load = indvar_flatten_fu_58;
    end
end

always @ (*) begin
    if (((1'b0 == ap_block_pp0_stage0) & (1'b1 == ap_CS_fsm_pp0_stage0) & (ap_loop_init == 1'b1))) begin
        ap_sig_allocacmp_j_load = 7'd0;
    end else begin
        ap_sig_allocacmp_j_load = j_fu_50;
    end
end

always @ (*) begin
    if (((1'b0 == ap_block_pp0_stage0_11001) & (ap_enable_reg_pp0_iter3 == 1'b1))) begin
        buf_r_ce0 = 1'b1;
    end else begin
        buf_r_ce0 = 1'b0;
    end
end

always @ (*) begin
    if (((1'b0 == ap_block_pp0_stage0_11001) & (ap_enable_reg_pp0_iter3 == 1'b1))) begin
        buf_r_we0 = 1'b1;
    end else begin
        buf_r_we0 = 1'b0;
    end
end

always @ (*) begin
    if (((1'b0 == ap_block_pp0_stage0_11001) & (ap_enable_reg_pp0_iter1 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0))) begin
        p_event_count_ce0 = 1'b1;
    end else begin
        p_event_count_ce0 = 1'b0;
    end
end

always @ (*) begin
    if (((ifzero_reg_260 == 1'd1) & (1'b0 == ap_block_pp0_stage0_11001) & (ap_enable_reg_pp0_iter1 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0))) begin
        p_event_count_we0 = 1'b1;
    end else begin
        p_event_count_we0 = 1'b0;
    end
end

always @ (*) begin
    if (((1'b0 == ap_block_pp0_stage0_11001) & (ap_enable_reg_pp0_iter1 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0))) begin
        sample_count_ce0 = 1'b1;
    end else begin
        sample_count_ce0 = 1'b0;
    end
end

always @ (*) begin
    if (((ifzero_reg_260 == 1'd1) & (1'b0 == ap_block_pp0_stage0_11001) & (ap_enable_reg_pp0_iter1 == 1'b1) & (1'b1 == ap_CS_fsm_pp0_stage0))) begin
        sample_count_we0 = 1'b1;
    end else begin
        sample_count_we0 = 1'b0;
    end
end

always @ (*) begin
    case (ap_CS_fsm)
        ap_ST_fsm_pp0_stage0 : begin
            ap_NS_fsm = ap_ST_fsm_pp0_stage0;
        end
        default : begin
            ap_NS_fsm = 'bx;
        end
    endcase
end

assign add_ln120_1_fu_128_p2 = (ap_sig_allocacmp_indvar_flatten_load + 10'd1);

assign add_ln120_fu_140_p2 = (ap_sig_allocacmp_i_load + 4'd1);

assign add_ln121_fu_172_p2 = (select_ln120_fu_152_p3 + 7'd1);

assign ap_CS_fsm_pp0_stage0 = ap_CS_fsm[32'd0];

assign ap_block_pp0_stage0 = ~(1'b1 == 1'b1);

assign ap_block_pp0_stage0_11001 = ~(1'b1 == 1'b1);

assign ap_block_pp0_stage0_subdone = ~(1'b1 == 1'b1);

assign ap_block_state1_pp0_stage0_iter0 = ~(1'b1 == 1'b1);

assign ap_block_state2_pp0_stage0_iter1 = ~(1'b1 == 1'b1);

assign ap_block_state3_pp0_stage0_iter2 = ~(1'b1 == 1'b1);

assign ap_block_state4_pp0_stage0_iter3 = ~(1'b1 == 1'b1);

assign ap_enable_pp0 = (ap_idle_pp0 ^ 1'b1);

assign ap_enable_reg_pp0_iter0 = ap_start_int;

assign ap_loop_exit_ready = ap_condition_exit_pp0_iter0_stage0;

assign buf_r_address0 = zext_ln121_1_fu_207_p1;

assign buf_r_d0 = 32'd0;

assign grp_fu_211_p0 = grp_fu_211_p00;

assign grp_fu_211_p00 = select_ln120_1_fu_160_p3;

assign grp_fu_211_p1 = 10'd90;

assign grp_fu_211_p2 = grp_fu_211_p20;

assign grp_fu_211_p20 = select_ln120_reg_245_pp0_iter1_reg;

assign icmp_ln120_fu_122_p2 = ((ap_sig_allocacmp_indvar_flatten_load == 10'd720) ? 1'b1 : 1'b0);

assign icmp_ln121_fu_146_p2 = ((ap_sig_allocacmp_j_load == 7'd90) ? 1'b1 : 1'b0);

assign ifzero_fu_178_p2 = ((add_ln121_fu_172_p2 == 7'd90) ? 1'b1 : 1'b0);

assign p_event_count_address0 = zext_ln120_fu_199_p1;

assign p_event_count_d0 = 16'd0;

assign sample_count_address0 = zext_ln120_fu_199_p1;

assign sample_count_d0 = 8'd0;

assign select_ln120_1_fu_160_p3 = ((icmp_ln121_fu_146_p2[0:0] == 1'b1) ? add_ln120_fu_140_p2 : ap_sig_allocacmp_i_load);

assign select_ln120_fu_152_p3 = ((icmp_ln121_fu_146_p2[0:0] == 1'b1) ? 7'd0 : ap_sig_allocacmp_j_load);

assign zext_ln120_fu_199_p1 = select_ln120_1_reg_250;

assign zext_ln121_1_fu_207_p1 = grp_fu_211_p3;

endmodule //postage_maxi_postage_maxi_Pipeline_VITIS_LOOP_120_1_VITIS_LOOP_121_2