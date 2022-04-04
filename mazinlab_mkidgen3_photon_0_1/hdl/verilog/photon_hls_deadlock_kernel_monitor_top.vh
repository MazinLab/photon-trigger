
wire kernel_monitor_reset;
wire kernel_monitor_clock;
assign kernel_monitor_reset = ~ap_rst_n;
assign kernel_monitor_clock = ap_clk;
wire [2:0] axis_block_sigs;
wire [4:0] inst_idle_sigs;
wire [1:0] inst_block_sigs;
wire kernel_block;

assign axis_block_sigs[0] = ~photon_factory_U0.instream_TDATA_blk_n;
assign axis_block_sigs[1] = ~photon_factory_U0.timestamps_V_TDATA_blk_n;
assign axis_block_sigs[2] = ~read_distribute_U0.photons_V_TDATA_blk_n;

assign inst_idle_sigs[0] = photon_factory_U0.ap_idle;
assign inst_block_sigs[0] = (photon_factory_U0.ap_done & ~photon_factory_U0.ap_continue) | ~photon_factory_U0.photon_fifo_blk_n | ~photon_factory_U0.photon_fifo1_blk_n | ~photon_factory_U0.photon_fifo2_blk_n | ~photon_factory_U0.photon_fifo3_blk_n | ~photon_factory_U0.done3_blk_n;
assign inst_idle_sigs[1] = read_distribute_U0.ap_idle;
assign inst_block_sigs[1] = (read_distribute_U0.ap_done & ~read_distribute_U0.ap_continue);

assign inst_idle_sigs[2] = 1'b0;
assign inst_idle_sigs[3] = photon_factory_U0.ap_idle;
assign inst_idle_sigs[4] = read_distribute_U0.ap_idle;

photon_hls_deadlock_idx0_monitor photon_hls_deadlock_idx0_monitor_U (
    .clock(kernel_monitor_clock),
    .reset(kernel_monitor_reset),
    .axis_block_sigs(axis_block_sigs),
    .inst_idle_sigs(inst_idle_sigs),
    .inst_block_sigs(inst_block_sigs),
    .block(kernel_block)
);

