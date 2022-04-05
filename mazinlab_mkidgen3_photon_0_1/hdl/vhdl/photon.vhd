-- ==============================================================
-- RTL generated by Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2021.1 (64-bit)
-- Version: 2021.1
-- Copyright (C) Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
-- 
-- ===========================================================

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity photon is
port (
    instream_TDATA : IN STD_LOGIC_VECTOR (63 downto 0);
    instream_TKEEP : IN STD_LOGIC_VECTOR (7 downto 0);
    instream_TSTRB : IN STD_LOGIC_VECTOR (7 downto 0);
    instream_TUSER : IN STD_LOGIC_VECTOR (12 downto 0);
    instream_TLAST : IN STD_LOGIC_VECTOR (0 downto 0);
    timestamps_V_TDATA : IN STD_LOGIC_VECTOR (15 downto 0);
    photons_V_TDATA : OUT STD_LOGIC_VECTOR (47 downto 0);
    ap_clk : IN STD_LOGIC;
    ap_rst_n : IN STD_LOGIC;
    instream_TVALID : IN STD_LOGIC;
    instream_TREADY : OUT STD_LOGIC;
    timestamps_V_TVALID : IN STD_LOGIC;
    timestamps_V_TREADY : OUT STD_LOGIC;
    photons_V_TVALID : OUT STD_LOGIC;
    photons_V_TREADY : IN STD_LOGIC );
end;


architecture behav of photon is 
    attribute CORE_GENERATION_INFO : STRING;
    attribute CORE_GENERATION_INFO of behav : architecture is
    "photon_photon,hls_ip_2021_1,{HLS_INPUT_TYPE=cxx,HLS_INPUT_FLOAT=0,HLS_INPUT_FIXED=0,HLS_INPUT_PART=xczu28dr-ffvg1517-2-e,HLS_INPUT_CLOCK=1.818000,HLS_INPUT_ARCH=dataflow,HLS_SYN_CLOCK=1.192570,HLS_SYN_LAT=-1,HLS_SYN_TPT=-1,HLS_SYN_MEM=0,HLS_SYN_DSP=0,HLS_SYN_FF=741,HLS_SYN_LUT=690,HLS_VERSION=2021_1}";
    constant ap_const_logic_1 : STD_LOGIC := '1';
    constant ap_const_logic_0 : STD_LOGIC := '0';

    signal ap_rst_n_inv : STD_LOGIC;
    signal photon_factory_U0_ap_start : STD_LOGIC;
    signal photon_factory_U0_ap_done : STD_LOGIC;
    signal photon_factory_U0_ap_continue : STD_LOGIC;
    signal photon_factory_U0_ap_idle : STD_LOGIC;
    signal photon_factory_U0_ap_ready : STD_LOGIC;
    signal photon_factory_U0_start_out : STD_LOGIC;
    signal photon_factory_U0_start_write : STD_LOGIC;
    signal photon_factory_U0_instream_TREADY : STD_LOGIC;
    signal photon_factory_U0_timestamps_V_TREADY : STD_LOGIC;
    signal photon_factory_U0_photon_fifo_din : STD_LOGIC_VECTOR (43 downto 0);
    signal photon_factory_U0_photon_fifo_write : STD_LOGIC;
    signal photon_factory_U0_photon_fifo1_din : STD_LOGIC_VECTOR (43 downto 0);
    signal photon_factory_U0_photon_fifo1_write : STD_LOGIC;
    signal photon_factory_U0_photon_fifo2_din : STD_LOGIC_VECTOR (43 downto 0);
    signal photon_factory_U0_photon_fifo2_write : STD_LOGIC;
    signal photon_factory_U0_photon_fifo3_din : STD_LOGIC_VECTOR (43 downto 0);
    signal photon_factory_U0_photon_fifo3_write : STD_LOGIC;
    signal read_distribute_U0_ap_start : STD_LOGIC;
    signal read_distribute_U0_ap_done : STD_LOGIC;
    signal read_distribute_U0_ap_continue : STD_LOGIC;
    signal read_distribute_U0_ap_idle : STD_LOGIC;
    signal read_distribute_U0_ap_ready : STD_LOGIC;
    signal read_distribute_U0_istrms_read : STD_LOGIC;
    signal read_distribute_U0_istrms1_read : STD_LOGIC;
    signal read_distribute_U0_istrms2_read : STD_LOGIC;
    signal read_distribute_U0_istrms3_read : STD_LOGIC;
    signal read_distribute_U0_photons_V_TDATA : STD_LOGIC_VECTOR (47 downto 0);
    signal read_distribute_U0_photons_V_TVALID : STD_LOGIC;
    signal photon_fifos_0_full_n : STD_LOGIC;
    signal photon_fifos_0_dout : STD_LOGIC_VECTOR (43 downto 0);
    signal photon_fifos_0_empty_n : STD_LOGIC;
    signal photon_fifos_1_full_n : STD_LOGIC;
    signal photon_fifos_1_dout : STD_LOGIC_VECTOR (43 downto 0);
    signal photon_fifos_1_empty_n : STD_LOGIC;
    signal photon_fifos_2_full_n : STD_LOGIC;
    signal photon_fifos_2_dout : STD_LOGIC_VECTOR (43 downto 0);
    signal photon_fifos_2_empty_n : STD_LOGIC;
    signal photon_fifos_3_full_n : STD_LOGIC;
    signal photon_fifos_3_dout : STD_LOGIC_VECTOR (43 downto 0);
    signal photon_fifos_3_empty_n : STD_LOGIC;
    signal start_for_read_distribute_U0_din : STD_LOGIC_VECTOR (0 downto 0);
    signal start_for_read_distribute_U0_full_n : STD_LOGIC;
    signal start_for_read_distribute_U0_dout : STD_LOGIC_VECTOR (0 downto 0);
    signal start_for_read_distribute_U0_empty_n : STD_LOGIC;

    component photon_photon_factory IS
    port (
        ap_clk : IN STD_LOGIC;
        ap_rst : IN STD_LOGIC;
        ap_start : IN STD_LOGIC;
        start_full_n : IN STD_LOGIC;
        ap_done : OUT STD_LOGIC;
        ap_continue : IN STD_LOGIC;
        ap_idle : OUT STD_LOGIC;
        ap_ready : OUT STD_LOGIC;
        start_out : OUT STD_LOGIC;
        start_write : OUT STD_LOGIC;
        instream_TDATA : IN STD_LOGIC_VECTOR (63 downto 0);
        instream_TVALID : IN STD_LOGIC;
        instream_TREADY : OUT STD_LOGIC;
        instream_TKEEP : IN STD_LOGIC_VECTOR (7 downto 0);
        instream_TSTRB : IN STD_LOGIC_VECTOR (7 downto 0);
        instream_TUSER : IN STD_LOGIC_VECTOR (12 downto 0);
        instream_TLAST : IN STD_LOGIC_VECTOR (0 downto 0);
        timestamps_V_TDATA : IN STD_LOGIC_VECTOR (15 downto 0);
        timestamps_V_TVALID : IN STD_LOGIC;
        timestamps_V_TREADY : OUT STD_LOGIC;
        photon_fifo_din : OUT STD_LOGIC_VECTOR (43 downto 0);
        photon_fifo_full_n : IN STD_LOGIC;
        photon_fifo_write : OUT STD_LOGIC;
        photon_fifo1_din : OUT STD_LOGIC_VECTOR (43 downto 0);
        photon_fifo1_full_n : IN STD_LOGIC;
        photon_fifo1_write : OUT STD_LOGIC;
        photon_fifo2_din : OUT STD_LOGIC_VECTOR (43 downto 0);
        photon_fifo2_full_n : IN STD_LOGIC;
        photon_fifo2_write : OUT STD_LOGIC;
        photon_fifo3_din : OUT STD_LOGIC_VECTOR (43 downto 0);
        photon_fifo3_full_n : IN STD_LOGIC;
        photon_fifo3_write : OUT STD_LOGIC );
    end component;


    component photon_read_distribute IS
    port (
        ap_clk : IN STD_LOGIC;
        ap_rst : IN STD_LOGIC;
        ap_start : IN STD_LOGIC;
        ap_done : OUT STD_LOGIC;
        ap_continue : IN STD_LOGIC;
        ap_idle : OUT STD_LOGIC;
        ap_ready : OUT STD_LOGIC;
        istrms_dout : IN STD_LOGIC_VECTOR (43 downto 0);
        istrms_empty_n : IN STD_LOGIC;
        istrms_read : OUT STD_LOGIC;
        istrms1_dout : IN STD_LOGIC_VECTOR (43 downto 0);
        istrms1_empty_n : IN STD_LOGIC;
        istrms1_read : OUT STD_LOGIC;
        istrms2_dout : IN STD_LOGIC_VECTOR (43 downto 0);
        istrms2_empty_n : IN STD_LOGIC;
        istrms2_read : OUT STD_LOGIC;
        istrms3_dout : IN STD_LOGIC_VECTOR (43 downto 0);
        istrms3_empty_n : IN STD_LOGIC;
        istrms3_read : OUT STD_LOGIC;
        photons_V_TDATA : OUT STD_LOGIC_VECTOR (47 downto 0);
        photons_V_TVALID : OUT STD_LOGIC;
        photons_V_TREADY : IN STD_LOGIC );
    end component;


    component photon_fifo_w44_d4_S IS
    port (
        clk : IN STD_LOGIC;
        reset : IN STD_LOGIC;
        if_read_ce : IN STD_LOGIC;
        if_write_ce : IN STD_LOGIC;
        if_din : IN STD_LOGIC_VECTOR (43 downto 0);
        if_full_n : OUT STD_LOGIC;
        if_write : IN STD_LOGIC;
        if_dout : OUT STD_LOGIC_VECTOR (43 downto 0);
        if_empty_n : OUT STD_LOGIC;
        if_read : IN STD_LOGIC );
    end component;


    component photon_start_for_read_distribute_U0 IS
    port (
        clk : IN STD_LOGIC;
        reset : IN STD_LOGIC;
        if_read_ce : IN STD_LOGIC;
        if_write_ce : IN STD_LOGIC;
        if_din : IN STD_LOGIC_VECTOR (0 downto 0);
        if_full_n : OUT STD_LOGIC;
        if_write : IN STD_LOGIC;
        if_dout : OUT STD_LOGIC_VECTOR (0 downto 0);
        if_empty_n : OUT STD_LOGIC;
        if_read : IN STD_LOGIC );
    end component;



begin
    photon_factory_U0 : component photon_photon_factory
    port map (
        ap_clk => ap_clk,
        ap_rst => ap_rst_n_inv,
        ap_start => photon_factory_U0_ap_start,
        start_full_n => start_for_read_distribute_U0_full_n,
        ap_done => photon_factory_U0_ap_done,
        ap_continue => photon_factory_U0_ap_continue,
        ap_idle => photon_factory_U0_ap_idle,
        ap_ready => photon_factory_U0_ap_ready,
        start_out => photon_factory_U0_start_out,
        start_write => photon_factory_U0_start_write,
        instream_TDATA => instream_TDATA,
        instream_TVALID => instream_TVALID,
        instream_TREADY => photon_factory_U0_instream_TREADY,
        instream_TKEEP => instream_TKEEP,
        instream_TSTRB => instream_TSTRB,
        instream_TUSER => instream_TUSER,
        instream_TLAST => instream_TLAST,
        timestamps_V_TDATA => timestamps_V_TDATA,
        timestamps_V_TVALID => timestamps_V_TVALID,
        timestamps_V_TREADY => photon_factory_U0_timestamps_V_TREADY,
        photon_fifo_din => photon_factory_U0_photon_fifo_din,
        photon_fifo_full_n => photon_fifos_0_full_n,
        photon_fifo_write => photon_factory_U0_photon_fifo_write,
        photon_fifo1_din => photon_factory_U0_photon_fifo1_din,
        photon_fifo1_full_n => photon_fifos_1_full_n,
        photon_fifo1_write => photon_factory_U0_photon_fifo1_write,
        photon_fifo2_din => photon_factory_U0_photon_fifo2_din,
        photon_fifo2_full_n => photon_fifos_2_full_n,
        photon_fifo2_write => photon_factory_U0_photon_fifo2_write,
        photon_fifo3_din => photon_factory_U0_photon_fifo3_din,
        photon_fifo3_full_n => photon_fifos_3_full_n,
        photon_fifo3_write => photon_factory_U0_photon_fifo3_write);

    read_distribute_U0 : component photon_read_distribute
    port map (
        ap_clk => ap_clk,
        ap_rst => ap_rst_n_inv,
        ap_start => read_distribute_U0_ap_start,
        ap_done => read_distribute_U0_ap_done,
        ap_continue => read_distribute_U0_ap_continue,
        ap_idle => read_distribute_U0_ap_idle,
        ap_ready => read_distribute_U0_ap_ready,
        istrms_dout => photon_fifos_0_dout,
        istrms_empty_n => photon_fifos_0_empty_n,
        istrms_read => read_distribute_U0_istrms_read,
        istrms1_dout => photon_fifos_1_dout,
        istrms1_empty_n => photon_fifos_1_empty_n,
        istrms1_read => read_distribute_U0_istrms1_read,
        istrms2_dout => photon_fifos_2_dout,
        istrms2_empty_n => photon_fifos_2_empty_n,
        istrms2_read => read_distribute_U0_istrms2_read,
        istrms3_dout => photon_fifos_3_dout,
        istrms3_empty_n => photon_fifos_3_empty_n,
        istrms3_read => read_distribute_U0_istrms3_read,
        photons_V_TDATA => read_distribute_U0_photons_V_TDATA,
        photons_V_TVALID => read_distribute_U0_photons_V_TVALID,
        photons_V_TREADY => photons_V_TREADY);

    photon_fifos_0_U : component photon_fifo_w44_d4_S
    port map (
        clk => ap_clk,
        reset => ap_rst_n_inv,
        if_read_ce => ap_const_logic_1,
        if_write_ce => ap_const_logic_1,
        if_din => photon_factory_U0_photon_fifo_din,
        if_full_n => photon_fifos_0_full_n,
        if_write => photon_factory_U0_photon_fifo_write,
        if_dout => photon_fifos_0_dout,
        if_empty_n => photon_fifos_0_empty_n,
        if_read => read_distribute_U0_istrms_read);

    photon_fifos_1_U : component photon_fifo_w44_d4_S
    port map (
        clk => ap_clk,
        reset => ap_rst_n_inv,
        if_read_ce => ap_const_logic_1,
        if_write_ce => ap_const_logic_1,
        if_din => photon_factory_U0_photon_fifo1_din,
        if_full_n => photon_fifos_1_full_n,
        if_write => photon_factory_U0_photon_fifo1_write,
        if_dout => photon_fifos_1_dout,
        if_empty_n => photon_fifos_1_empty_n,
        if_read => read_distribute_U0_istrms1_read);

    photon_fifos_2_U : component photon_fifo_w44_d4_S
    port map (
        clk => ap_clk,
        reset => ap_rst_n_inv,
        if_read_ce => ap_const_logic_1,
        if_write_ce => ap_const_logic_1,
        if_din => photon_factory_U0_photon_fifo2_din,
        if_full_n => photon_fifos_2_full_n,
        if_write => photon_factory_U0_photon_fifo2_write,
        if_dout => photon_fifos_2_dout,
        if_empty_n => photon_fifos_2_empty_n,
        if_read => read_distribute_U0_istrms2_read);

    photon_fifos_3_U : component photon_fifo_w44_d4_S
    port map (
        clk => ap_clk,
        reset => ap_rst_n_inv,
        if_read_ce => ap_const_logic_1,
        if_write_ce => ap_const_logic_1,
        if_din => photon_factory_U0_photon_fifo3_din,
        if_full_n => photon_fifos_3_full_n,
        if_write => photon_factory_U0_photon_fifo3_write,
        if_dout => photon_fifos_3_dout,
        if_empty_n => photon_fifos_3_empty_n,
        if_read => read_distribute_U0_istrms3_read);

    start_for_read_distribute_U0_U : component photon_start_for_read_distribute_U0
    port map (
        clk => ap_clk,
        reset => ap_rst_n_inv,
        if_read_ce => ap_const_logic_1,
        if_write_ce => ap_const_logic_1,
        if_din => start_for_read_distribute_U0_din,
        if_full_n => start_for_read_distribute_U0_full_n,
        if_write => photon_factory_U0_start_write,
        if_dout => start_for_read_distribute_U0_dout,
        if_empty_n => start_for_read_distribute_U0_empty_n,
        if_read => read_distribute_U0_ap_ready);





    ap_rst_n_inv_assign_proc : process(ap_rst_n)
    begin
                ap_rst_n_inv <= not(ap_rst_n);
    end process;

    instream_TREADY <= photon_factory_U0_instream_TREADY;
    photon_factory_U0_ap_continue <= ap_const_logic_1;
    photon_factory_U0_ap_start <= ap_const_logic_1;
    photons_V_TDATA <= read_distribute_U0_photons_V_TDATA;
    photons_V_TVALID <= read_distribute_U0_photons_V_TVALID;
    read_distribute_U0_ap_continue <= ap_const_logic_1;
    read_distribute_U0_ap_start <= start_for_read_distribute_U0_empty_n;
    start_for_read_distribute_U0_din <= (0=>ap_const_logic_1, others=>'-');
    timestamps_V_TREADY <= photon_factory_U0_timestamps_V_TREADY;
end behav;
