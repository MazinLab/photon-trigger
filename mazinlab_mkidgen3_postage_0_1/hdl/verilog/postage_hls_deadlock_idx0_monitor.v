`timescale 1 ns / 1 ps

module postage_hls_deadlock_idx0_monitor ( // for module postage_postage_inst
    input wire clock,
    input wire reset,
    input wire [9:0] axis_block_sigs,
    input wire [0:0] inst_idle_sigs,
    input wire [0:0] inst_block_sigs,
    output wire [99:0] axis_block_info,
    output wire block
);

// signal declare
reg [99:0] monitor_axis_block_info;
reg monitor_find_block;
wire sub_parallel_block;
wire all_sub_parallel_has_block;
wire all_sub_single_has_block;
wire cur_axis_has_block;
wire seq_is_axis_block;

assign axis_block_info = (monitor_find_block == 1'b1) ? monitor_axis_block_info : 100'h0;
assign block = monitor_find_block;
assign all_sub_parallel_has_block = 1'b0;
assign all_sub_single_has_block = 1'b0;
assign cur_axis_has_block = 1'b0 | axis_block_sigs[0] | axis_block_sigs[1] | axis_block_sigs[2] | axis_block_sigs[3] | axis_block_sigs[4] | axis_block_sigs[5] | axis_block_sigs[6] | axis_block_sigs[7] | axis_block_sigs[8] | axis_block_sigs[9];
assign seq_is_axis_block = all_sub_parallel_has_block | all_sub_single_has_block | cur_axis_has_block;

always @(posedge clock) begin
    if (reset == 1'b1)
        monitor_find_block <= 1'b0;
    else if (seq_is_axis_block == 1'b1)
        monitor_find_block <= 1'b1;
    else
        monitor_find_block <= 1'b0;
end

always @(posedge clock) begin
    if (reset == 1'b1)
        monitor_axis_block_info[9:0] <= 10'h0;
    else if (axis_block_sigs[0])
        monitor_axis_block_info[9:0] <= ~(10'h1 << 0);
    else
        monitor_axis_block_info[9:0] <= 10'h0;
end
always @(posedge clock) begin
    if (reset == 1'b1)
        monitor_axis_block_info[19:10] <= 10'h0;
    else if (axis_block_sigs[1])
        monitor_axis_block_info[19:10] <= ~(10'h1 << 1);
    else
        monitor_axis_block_info[19:10] <= 10'h0;
end
always @(posedge clock) begin
    if (reset == 1'b1)
        monitor_axis_block_info[29:20] <= 10'h0;
    else if (axis_block_sigs[2])
        monitor_axis_block_info[29:20] <= ~(10'h1 << 2);
    else
        monitor_axis_block_info[29:20] <= 10'h0;
end
always @(posedge clock) begin
    if (reset == 1'b1)
        monitor_axis_block_info[39:30] <= 10'h0;
    else if (axis_block_sigs[3])
        monitor_axis_block_info[39:30] <= ~(10'h1 << 3);
    else
        monitor_axis_block_info[39:30] <= 10'h0;
end
always @(posedge clock) begin
    if (reset == 1'b1)
        monitor_axis_block_info[49:40] <= 10'h0;
    else if (axis_block_sigs[4])
        monitor_axis_block_info[49:40] <= ~(10'h1 << 4);
    else
        monitor_axis_block_info[49:40] <= 10'h0;
end
always @(posedge clock) begin
    if (reset == 1'b1)
        monitor_axis_block_info[59:50] <= 10'h0;
    else if (axis_block_sigs[5])
        monitor_axis_block_info[59:50] <= ~(10'h1 << 5);
    else
        monitor_axis_block_info[59:50] <= 10'h0;
end
always @(posedge clock) begin
    if (reset == 1'b1)
        monitor_axis_block_info[69:60] <= 10'h0;
    else if (axis_block_sigs[6])
        monitor_axis_block_info[69:60] <= ~(10'h1 << 6);
    else
        monitor_axis_block_info[69:60] <= 10'h0;
end
always @(posedge clock) begin
    if (reset == 1'b1)
        monitor_axis_block_info[79:70] <= 10'h0;
    else if (axis_block_sigs[7])
        monitor_axis_block_info[79:70] <= ~(10'h1 << 7);
    else
        monitor_axis_block_info[79:70] <= 10'h0;
end
always @(posedge clock) begin
    if (reset == 1'b1)
        monitor_axis_block_info[89:80] <= 10'h0;
    else if (axis_block_sigs[8])
        monitor_axis_block_info[89:80] <= ~(10'h1 << 8);
    else
        monitor_axis_block_info[89:80] <= 10'h0;
end
always @(posedge clock) begin
    if (reset == 1'b1)
        monitor_axis_block_info[99:90] <= 10'h0;
    else if (axis_block_sigs[9])
        monitor_axis_block_info[99:90] <= ~(10'h1 << 9);
    else
        monitor_axis_block_info[99:90] <= 10'h0;
end

// instant sub module
endmodule
