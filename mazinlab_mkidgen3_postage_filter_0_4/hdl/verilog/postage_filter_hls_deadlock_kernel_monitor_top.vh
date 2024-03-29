
wire kernel_monitor_reset;
wire kernel_monitor_clock;
assign kernel_monitor_reset = ~ap_rst_n;
assign kernel_monitor_clock = ap_clk;
wire [8:0] axis_block_sigs;
wire [0:0] inst_idle_sigs;
wire [0:0] inst_block_sigs;
wire kernel_block;

assign axis_block_sigs[0] = ~postage_stream_TDATA_blk_n;
assign axis_block_sigs[1] = ~iq_out_0_TDATA_blk_n;
assign axis_block_sigs[2] = ~iq_out_1_TDATA_blk_n;
assign axis_block_sigs[3] = ~iq_out_2_TDATA_blk_n;
assign axis_block_sigs[4] = ~iq_out_3_TDATA_blk_n;
assign axis_block_sigs[5] = ~iq_out_4_TDATA_blk_n;
assign axis_block_sigs[6] = ~iq_out_5_TDATA_blk_n;
assign axis_block_sigs[7] = ~iq_out_6_TDATA_blk_n;
assign axis_block_sigs[8] = ~iq_out_7_TDATA_blk_n;

assign inst_block_sigs[0] = 1'b0;

assign inst_idle_sigs[0] = 1'b0;

postage_filter_hls_deadlock_idx0_monitor postage_filter_hls_deadlock_idx0_monitor_U (
    .clock(kernel_monitor_clock),
    .reset(kernel_monitor_reset),
    .axis_block_sigs(axis_block_sigs),
    .inst_idle_sigs(inst_idle_sigs),
    .inst_block_sigs(inst_block_sigs),
    .block(kernel_block)
);

always @ (kernel_block or kernel_monitor_reset) begin
    if (kernel_block == 1'b1 && kernel_monitor_reset == 1'b0) begin
        find_kernel_block = 1'b1;
    end
    else begin
        find_kernel_block = 1'b0;
    end
end
