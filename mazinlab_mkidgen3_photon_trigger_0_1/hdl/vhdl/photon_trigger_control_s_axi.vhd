-- ==============================================================
-- Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2019.2.1 (64-bit)
-- Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.NUMERIC_STD.all;

entity photon_trigger_control_s_axi is
generic (
    C_S_AXI_ADDR_WIDTH    : INTEGER := 13;
    C_S_AXI_DATA_WIDTH    : INTEGER := 32);
port (
    ACLK                  :in   STD_LOGIC;
    ARESET                :in   STD_LOGIC;
    ACLK_EN               :in   STD_LOGIC;
    AWADDR                :in   STD_LOGIC_VECTOR(C_S_AXI_ADDR_WIDTH-1 downto 0);
    AWVALID               :in   STD_LOGIC;
    AWREADY               :out  STD_LOGIC;
    WDATA                 :in   STD_LOGIC_VECTOR(C_S_AXI_DATA_WIDTH-1 downto 0);
    WSTRB                 :in   STD_LOGIC_VECTOR(C_S_AXI_DATA_WIDTH/8-1 downto 0);
    WVALID                :in   STD_LOGIC;
    WREADY                :out  STD_LOGIC;
    BRESP                 :out  STD_LOGIC_VECTOR(1 downto 0);
    BVALID                :out  STD_LOGIC;
    BREADY                :in   STD_LOGIC;
    ARADDR                :in   STD_LOGIC_VECTOR(C_S_AXI_ADDR_WIDTH-1 downto 0);
    ARVALID               :in   STD_LOGIC;
    ARREADY               :out  STD_LOGIC;
    RDATA                 :out  STD_LOGIC_VECTOR(C_S_AXI_DATA_WIDTH-1 downto 0);
    RRESP                 :out  STD_LOGIC_VECTOR(1 downto 0);
    RVALID                :out  STD_LOGIC;
    RREADY                :in   STD_LOGIC;
    triggercfg_0_threshold_V_address0 :in   STD_LOGIC_VECTOR(8 downto 0);
    triggercfg_0_threshold_V_ce0 :in   STD_LOGIC;
    triggercfg_0_threshold_V_q0 :out  STD_LOGIC_VECTOR(15 downto 0);
    triggercfg_1_threshold_V_address0 :in   STD_LOGIC_VECTOR(8 downto 0);
    triggercfg_1_threshold_V_ce0 :in   STD_LOGIC;
    triggercfg_1_threshold_V_q0 :out  STD_LOGIC_VECTOR(15 downto 0);
    triggercfg_2_threshold_V_address0 :in   STD_LOGIC_VECTOR(8 downto 0);
    triggercfg_2_threshold_V_ce0 :in   STD_LOGIC;
    triggercfg_2_threshold_V_q0 :out  STD_LOGIC_VECTOR(15 downto 0);
    triggercfg_3_threshold_V_address0 :in   STD_LOGIC_VECTOR(8 downto 0);
    triggercfg_3_threshold_V_ce0 :in   STD_LOGIC;
    triggercfg_3_threshold_V_q0 :out  STD_LOGIC_VECTOR(15 downto 0);
    holdoff_V             :out  STD_LOGIC_VECTOR(9 downto 0);
    baseline_shrink_V     :out  STD_LOGIC_VECTOR(8 downto 0);
    baseline_grow_V       :out  STD_LOGIC_VECTOR(8 downto 0);
    dropped_i             :out  STD_LOGIC_VECTOR(31 downto 0);
    dropped_o             :in   STD_LOGIC_VECTOR(31 downto 0);
    dropped_o_ap_vld      :in   STD_LOGIC
);
end entity photon_trigger_control_s_axi;

-- ------------------------Address Info-------------------
-- 0x0000 : reserved
-- 0x0004 : reserved
-- 0x0008 : reserved
-- 0x000c : reserved
-- 0x1400 : Data signal of holdoff_V
--          bit 9~0 - holdoff_V[9:0] (Read/Write)
--          others  - reserved
-- 0x1404 : reserved
-- 0x1408 : Data signal of baseline_shrink_V
--          bit 8~0 - baseline_shrink_V[8:0] (Read/Write)
--          others  - reserved
-- 0x140c : reserved
-- 0x1410 : Data signal of baseline_grow_V
--          bit 8~0 - baseline_grow_V[8:0] (Read/Write)
--          others  - reserved
-- 0x1414 : reserved
-- 0x1418 : Data signal of dropped_i
--          bit 31~0 - dropped_i[31:0] (Read/Write)
-- 0x141c : reserved
-- 0x1420 : Data signal of dropped_o
--          bit 31~0 - dropped_o[31:0] (Read)
-- 0x1424 : Control signal of dropped_o
--          bit 0  - dropped_o_ap_vld (Read/COR)
--          others - reserved
-- 0x0400 ~
-- 0x07ff : Memory 'triggercfg_0_threshold_V' (512 * 16b)
--          Word n : bit [15: 0] - triggercfg_0_threshold_V[2n]
--                   bit [31:16] - triggercfg_0_threshold_V[2n+1]
-- 0x0800 ~
-- 0x0bff : Memory 'triggercfg_1_threshold_V' (512 * 16b)
--          Word n : bit [15: 0] - triggercfg_1_threshold_V[2n]
--                   bit [31:16] - triggercfg_1_threshold_V[2n+1]
-- 0x0c00 ~
-- 0x0fff : Memory 'triggercfg_2_threshold_V' (512 * 16b)
--          Word n : bit [15: 0] - triggercfg_2_threshold_V[2n]
--                   bit [31:16] - triggercfg_2_threshold_V[2n+1]
-- 0x1000 ~
-- 0x13ff : Memory 'triggercfg_3_threshold_V' (512 * 16b)
--          Word n : bit [15: 0] - triggercfg_3_threshold_V[2n]
--                   bit [31:16] - triggercfg_3_threshold_V[2n+1]
-- (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

architecture behave of photon_trigger_control_s_axi is
    type states is (wridle, wrdata, wrresp, wrreset, rdidle, rddata, rdreset);  -- read and write fsm states
    signal wstate  : states := wrreset;
    signal rstate  : states := rdreset;
    signal wnext, rnext: states;
    constant ADDR_HOLDOFF_V_DATA_0              : INTEGER := 16#1400#;
    constant ADDR_HOLDOFF_V_CTRL                : INTEGER := 16#1404#;
    constant ADDR_BASELINE_SHRINK_V_DATA_0      : INTEGER := 16#1408#;
    constant ADDR_BASELINE_SHRINK_V_CTRL        : INTEGER := 16#140c#;
    constant ADDR_BASELINE_GROW_V_DATA_0        : INTEGER := 16#1410#;
    constant ADDR_BASELINE_GROW_V_CTRL          : INTEGER := 16#1414#;
    constant ADDR_DROPPED_I_DATA_0              : INTEGER := 16#1418#;
    constant ADDR_DROPPED_I_CTRL                : INTEGER := 16#141c#;
    constant ADDR_DROPPED_O_DATA_0              : INTEGER := 16#1420#;
    constant ADDR_DROPPED_O_CTRL                : INTEGER := 16#1424#;
    constant ADDR_TRIGGERCFG_0_THRESHOLD_V_BASE : INTEGER := 16#0400#;
    constant ADDR_TRIGGERCFG_0_THRESHOLD_V_HIGH : INTEGER := 16#07ff#;
    constant ADDR_TRIGGERCFG_1_THRESHOLD_V_BASE : INTEGER := 16#0800#;
    constant ADDR_TRIGGERCFG_1_THRESHOLD_V_HIGH : INTEGER := 16#0bff#;
    constant ADDR_TRIGGERCFG_2_THRESHOLD_V_BASE : INTEGER := 16#0c00#;
    constant ADDR_TRIGGERCFG_2_THRESHOLD_V_HIGH : INTEGER := 16#0fff#;
    constant ADDR_TRIGGERCFG_3_THRESHOLD_V_BASE : INTEGER := 16#1000#;
    constant ADDR_TRIGGERCFG_3_THRESHOLD_V_HIGH : INTEGER := 16#13ff#;
    constant ADDR_BITS         : INTEGER := 13;

    signal waddr               : UNSIGNED(ADDR_BITS-1 downto 0);
    signal wmask               : UNSIGNED(31 downto 0);
    signal aw_hs               : STD_LOGIC;
    signal w_hs                : STD_LOGIC;
    signal rdata_data          : UNSIGNED(31 downto 0);
    signal ar_hs               : STD_LOGIC;
    signal raddr               : UNSIGNED(ADDR_BITS-1 downto 0);
    signal AWREADY_t           : STD_LOGIC;
    signal WREADY_t            : STD_LOGIC;
    signal ARREADY_t           : STD_LOGIC;
    signal RVALID_t            : STD_LOGIC;
    -- internal registers
    signal int_holdoff_V       : UNSIGNED(9 downto 0) := (others => '0');
    signal int_baseline_shrink_V : UNSIGNED(8 downto 0) := (others => '0');
    signal int_baseline_grow_V : UNSIGNED(8 downto 0) := (others => '0');
    signal int_dropped_i       : UNSIGNED(31 downto 0) := (others => '0');
    signal int_dropped_o       : UNSIGNED(31 downto 0) := (others => '0');
    signal int_dropped_o_ap_vld : STD_LOGIC;
    -- memory signals
    signal int_triggercfg_0_threshold_V_address0 : UNSIGNED(7 downto 0);
    signal int_triggercfg_0_threshold_V_ce0 : STD_LOGIC;
    signal int_triggercfg_0_threshold_V_we0 : STD_LOGIC;
    signal int_triggercfg_0_threshold_V_be0 : UNSIGNED(3 downto 0);
    signal int_triggercfg_0_threshold_V_d0 : UNSIGNED(31 downto 0);
    signal int_triggercfg_0_threshold_V_q0 : UNSIGNED(31 downto 0);
    signal int_triggercfg_0_threshold_V_address1 : UNSIGNED(7 downto 0);
    signal int_triggercfg_0_threshold_V_ce1 : STD_LOGIC;
    signal int_triggercfg_0_threshold_V_we1 : STD_LOGIC;
    signal int_triggercfg_0_threshold_V_be1 : UNSIGNED(3 downto 0);
    signal int_triggercfg_0_threshold_V_d1 : UNSIGNED(31 downto 0);
    signal int_triggercfg_0_threshold_V_q1 : UNSIGNED(31 downto 0);
    signal int_triggercfg_0_threshold_V_read : STD_LOGIC;
    signal int_triggercfg_0_threshold_V_write : STD_LOGIC;
    signal int_triggercfg_0_threshold_V_shift : UNSIGNED(0 downto 0);
    signal int_triggercfg_1_threshold_V_address0 : UNSIGNED(7 downto 0);
    signal int_triggercfg_1_threshold_V_ce0 : STD_LOGIC;
    signal int_triggercfg_1_threshold_V_we0 : STD_LOGIC;
    signal int_triggercfg_1_threshold_V_be0 : UNSIGNED(3 downto 0);
    signal int_triggercfg_1_threshold_V_d0 : UNSIGNED(31 downto 0);
    signal int_triggercfg_1_threshold_V_q0 : UNSIGNED(31 downto 0);
    signal int_triggercfg_1_threshold_V_address1 : UNSIGNED(7 downto 0);
    signal int_triggercfg_1_threshold_V_ce1 : STD_LOGIC;
    signal int_triggercfg_1_threshold_V_we1 : STD_LOGIC;
    signal int_triggercfg_1_threshold_V_be1 : UNSIGNED(3 downto 0);
    signal int_triggercfg_1_threshold_V_d1 : UNSIGNED(31 downto 0);
    signal int_triggercfg_1_threshold_V_q1 : UNSIGNED(31 downto 0);
    signal int_triggercfg_1_threshold_V_read : STD_LOGIC;
    signal int_triggercfg_1_threshold_V_write : STD_LOGIC;
    signal int_triggercfg_1_threshold_V_shift : UNSIGNED(0 downto 0);
    signal int_triggercfg_2_threshold_V_address0 : UNSIGNED(7 downto 0);
    signal int_triggercfg_2_threshold_V_ce0 : STD_LOGIC;
    signal int_triggercfg_2_threshold_V_we0 : STD_LOGIC;
    signal int_triggercfg_2_threshold_V_be0 : UNSIGNED(3 downto 0);
    signal int_triggercfg_2_threshold_V_d0 : UNSIGNED(31 downto 0);
    signal int_triggercfg_2_threshold_V_q0 : UNSIGNED(31 downto 0);
    signal int_triggercfg_2_threshold_V_address1 : UNSIGNED(7 downto 0);
    signal int_triggercfg_2_threshold_V_ce1 : STD_LOGIC;
    signal int_triggercfg_2_threshold_V_we1 : STD_LOGIC;
    signal int_triggercfg_2_threshold_V_be1 : UNSIGNED(3 downto 0);
    signal int_triggercfg_2_threshold_V_d1 : UNSIGNED(31 downto 0);
    signal int_triggercfg_2_threshold_V_q1 : UNSIGNED(31 downto 0);
    signal int_triggercfg_2_threshold_V_read : STD_LOGIC;
    signal int_triggercfg_2_threshold_V_write : STD_LOGIC;
    signal int_triggercfg_2_threshold_V_shift : UNSIGNED(0 downto 0);
    signal int_triggercfg_3_threshold_V_address0 : UNSIGNED(7 downto 0);
    signal int_triggercfg_3_threshold_V_ce0 : STD_LOGIC;
    signal int_triggercfg_3_threshold_V_we0 : STD_LOGIC;
    signal int_triggercfg_3_threshold_V_be0 : UNSIGNED(3 downto 0);
    signal int_triggercfg_3_threshold_V_d0 : UNSIGNED(31 downto 0);
    signal int_triggercfg_3_threshold_V_q0 : UNSIGNED(31 downto 0);
    signal int_triggercfg_3_threshold_V_address1 : UNSIGNED(7 downto 0);
    signal int_triggercfg_3_threshold_V_ce1 : STD_LOGIC;
    signal int_triggercfg_3_threshold_V_we1 : STD_LOGIC;
    signal int_triggercfg_3_threshold_V_be1 : UNSIGNED(3 downto 0);
    signal int_triggercfg_3_threshold_V_d1 : UNSIGNED(31 downto 0);
    signal int_triggercfg_3_threshold_V_q1 : UNSIGNED(31 downto 0);
    signal int_triggercfg_3_threshold_V_read : STD_LOGIC;
    signal int_triggercfg_3_threshold_V_write : STD_LOGIC;
    signal int_triggercfg_3_threshold_V_shift : UNSIGNED(0 downto 0);

    component photon_trigger_control_s_axi_ram is
        generic (
            BYTES   : INTEGER :=4;
            DEPTH   : INTEGER :=256;
            AWIDTH  : INTEGER :=8);
        port (
            clk0    : in  STD_LOGIC;
            address0: in  UNSIGNED(AWIDTH-1 downto 0);
            ce0     : in  STD_LOGIC;
            we0     : in  STD_LOGIC;
            be0     : in  UNSIGNED(BYTES-1 downto 0);
            d0      : in  UNSIGNED(BYTES*8-1 downto 0);
            q0      : out UNSIGNED(BYTES*8-1 downto 0);
            clk1    : in  STD_LOGIC;
            address1: in  UNSIGNED(AWIDTH-1 downto 0);
            ce1     : in  STD_LOGIC;
            we1     : in  STD_LOGIC;
            be1     : in  UNSIGNED(BYTES-1 downto 0);
            d1      : in  UNSIGNED(BYTES*8-1 downto 0);
            q1      : out UNSIGNED(BYTES*8-1 downto 0));
    end component photon_trigger_control_s_axi_ram;

    function log2 (x : INTEGER) return INTEGER is
        variable n, m : INTEGER;
    begin
        n := 1;
        m := 2;
        while m < x loop
            n := n + 1;
            m := m * 2;
        end loop;
        return n;
    end function log2;

begin
-- ----------------------- Instantiation------------------
-- int_triggercfg_0_threshold_V
int_triggercfg_0_threshold_V : photon_trigger_control_s_axi_ram
generic map (
     BYTES    => 4,
     DEPTH    => 256,
     AWIDTH   => log2(256))
port map (
     clk0     => ACLK,
     address0 => int_triggercfg_0_threshold_V_address0,
     ce0      => int_triggercfg_0_threshold_V_ce0,
     we0      => int_triggercfg_0_threshold_V_we0,
     be0      => int_triggercfg_0_threshold_V_be0,
     d0       => int_triggercfg_0_threshold_V_d0,
     q0       => int_triggercfg_0_threshold_V_q0,
     clk1     => ACLK,
     address1 => int_triggercfg_0_threshold_V_address1,
     ce1      => int_triggercfg_0_threshold_V_ce1,
     we1      => int_triggercfg_0_threshold_V_we1,
     be1      => int_triggercfg_0_threshold_V_be1,
     d1       => int_triggercfg_0_threshold_V_d1,
     q1       => int_triggercfg_0_threshold_V_q1);
-- int_triggercfg_1_threshold_V
int_triggercfg_1_threshold_V : photon_trigger_control_s_axi_ram
generic map (
     BYTES    => 4,
     DEPTH    => 256,
     AWIDTH   => log2(256))
port map (
     clk0     => ACLK,
     address0 => int_triggercfg_1_threshold_V_address0,
     ce0      => int_triggercfg_1_threshold_V_ce0,
     we0      => int_triggercfg_1_threshold_V_we0,
     be0      => int_triggercfg_1_threshold_V_be0,
     d0       => int_triggercfg_1_threshold_V_d0,
     q0       => int_triggercfg_1_threshold_V_q0,
     clk1     => ACLK,
     address1 => int_triggercfg_1_threshold_V_address1,
     ce1      => int_triggercfg_1_threshold_V_ce1,
     we1      => int_triggercfg_1_threshold_V_we1,
     be1      => int_triggercfg_1_threshold_V_be1,
     d1       => int_triggercfg_1_threshold_V_d1,
     q1       => int_triggercfg_1_threshold_V_q1);
-- int_triggercfg_2_threshold_V
int_triggercfg_2_threshold_V : photon_trigger_control_s_axi_ram
generic map (
     BYTES    => 4,
     DEPTH    => 256,
     AWIDTH   => log2(256))
port map (
     clk0     => ACLK,
     address0 => int_triggercfg_2_threshold_V_address0,
     ce0      => int_triggercfg_2_threshold_V_ce0,
     we0      => int_triggercfg_2_threshold_V_we0,
     be0      => int_triggercfg_2_threshold_V_be0,
     d0       => int_triggercfg_2_threshold_V_d0,
     q0       => int_triggercfg_2_threshold_V_q0,
     clk1     => ACLK,
     address1 => int_triggercfg_2_threshold_V_address1,
     ce1      => int_triggercfg_2_threshold_V_ce1,
     we1      => int_triggercfg_2_threshold_V_we1,
     be1      => int_triggercfg_2_threshold_V_be1,
     d1       => int_triggercfg_2_threshold_V_d1,
     q1       => int_triggercfg_2_threshold_V_q1);
-- int_triggercfg_3_threshold_V
int_triggercfg_3_threshold_V : photon_trigger_control_s_axi_ram
generic map (
     BYTES    => 4,
     DEPTH    => 256,
     AWIDTH   => log2(256))
port map (
     clk0     => ACLK,
     address0 => int_triggercfg_3_threshold_V_address0,
     ce0      => int_triggercfg_3_threshold_V_ce0,
     we0      => int_triggercfg_3_threshold_V_we0,
     be0      => int_triggercfg_3_threshold_V_be0,
     d0       => int_triggercfg_3_threshold_V_d0,
     q0       => int_triggercfg_3_threshold_V_q0,
     clk1     => ACLK,
     address1 => int_triggercfg_3_threshold_V_address1,
     ce1      => int_triggercfg_3_threshold_V_ce1,
     we1      => int_triggercfg_3_threshold_V_we1,
     be1      => int_triggercfg_3_threshold_V_be1,
     d1       => int_triggercfg_3_threshold_V_d1,
     q1       => int_triggercfg_3_threshold_V_q1);

-- ----------------------- AXI WRITE ---------------------
    AWREADY_t <=  '1' when wstate = wridle else '0';
    AWREADY   <=  AWREADY_t;
    WREADY_t  <=  '1' when wstate = wrdata else '0';
    WREADY    <=  WREADY_t;
    BRESP     <=  "00";  -- OKAY
    BVALID    <=  '1' when wstate = wrresp else '0';
    wmask     <=  (31 downto 24 => WSTRB(3), 23 downto 16 => WSTRB(2), 15 downto 8 => WSTRB(1), 7 downto 0 => WSTRB(0));
    aw_hs     <=  AWVALID and AWREADY_t;
    w_hs      <=  WVALID and WREADY_t;

    -- write FSM
    process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ARESET = '1') then
                wstate <= wrreset;
            elsif (ACLK_EN = '1') then
                wstate <= wnext;
            end if;
        end if;
    end process;

    process (wstate, AWVALID, WVALID, BREADY)
    begin
        case (wstate) is
        when wridle =>
            if (AWVALID = '1') then
                wnext <= wrdata;
            else
                wnext <= wridle;
            end if;
        when wrdata =>
            if (WVALID = '1') then
                wnext <= wrresp;
            else
                wnext <= wrdata;
            end if;
        when wrresp =>
            if (BREADY = '1') then
                wnext <= wridle;
            else
                wnext <= wrresp;
            end if;
        when others =>
            wnext <= wridle;
        end case;
    end process;

    waddr_proc : process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ACLK_EN = '1') then
                if (aw_hs = '1') then
                    waddr <= UNSIGNED(AWADDR(ADDR_BITS-1 downto 0));
                end if;
            end if;
        end if;
    end process;

-- ----------------------- AXI READ ----------------------
    ARREADY_t <= '1' when (rstate = rdidle) else '0';
    ARREADY <= ARREADY_t;
    RDATA   <= STD_LOGIC_VECTOR(rdata_data);
    RRESP   <= "00";  -- OKAY
    RVALID_t  <= '1' when (rstate = rddata) and (int_triggercfg_0_threshold_V_read = '0') and (int_triggercfg_1_threshold_V_read = '0') and (int_triggercfg_2_threshold_V_read = '0') and (int_triggercfg_3_threshold_V_read = '0') else '0';
    RVALID    <= RVALID_t;
    ar_hs   <= ARVALID and ARREADY_t;
    raddr   <= UNSIGNED(ARADDR(ADDR_BITS-1 downto 0));

    -- read FSM
    process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ARESET = '1') then
                rstate <= rdreset;
            elsif (ACLK_EN = '1') then
                rstate <= rnext;
            end if;
        end if;
    end process;

    process (rstate, ARVALID, RREADY, RVALID_t)
    begin
        case (rstate) is
        when rdidle =>
            if (ARVALID = '1') then
                rnext <= rddata;
            else
                rnext <= rdidle;
            end if;
        when rddata =>
            if (RREADY = '1' and RVALID_t = '1') then
                rnext <= rdidle;
            else
                rnext <= rddata;
            end if;
        when others =>
            rnext <= rdidle;
        end case;
    end process;

    rdata_proc : process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ACLK_EN = '1') then
                if (ar_hs = '1') then
                    case (TO_INTEGER(raddr)) is
                    when ADDR_HOLDOFF_V_DATA_0 =>
                        rdata_data <= RESIZE(int_holdoff_V(9 downto 0), 32);
                    when ADDR_BASELINE_SHRINK_V_DATA_0 =>
                        rdata_data <= RESIZE(int_baseline_shrink_V(8 downto 0), 32);
                    when ADDR_BASELINE_GROW_V_DATA_0 =>
                        rdata_data <= RESIZE(int_baseline_grow_V(8 downto 0), 32);
                    when ADDR_DROPPED_I_DATA_0 =>
                        rdata_data <= RESIZE(int_dropped_i(31 downto 0), 32);
                    when ADDR_DROPPED_O_DATA_0 =>
                        rdata_data <= RESIZE(int_dropped_o(31 downto 0), 32);
                    when ADDR_DROPPED_O_CTRL =>
                        rdata_data <= (0 => int_dropped_o_ap_vld, others => '0');
                    when others =>
                        rdata_data <= (others => '0');
                    end case;
                elsif (int_triggercfg_0_threshold_V_read = '1') then
                    rdata_data <= int_triggercfg_0_threshold_V_q1;
                elsif (int_triggercfg_1_threshold_V_read = '1') then
                    rdata_data <= int_triggercfg_1_threshold_V_q1;
                elsif (int_triggercfg_2_threshold_V_read = '1') then
                    rdata_data <= int_triggercfg_2_threshold_V_q1;
                elsif (int_triggercfg_3_threshold_V_read = '1') then
                    rdata_data <= int_triggercfg_3_threshold_V_q1;
                end if;
            end if;
        end if;
    end process;

-- ----------------------- Register logic ----------------
    holdoff_V            <= STD_LOGIC_VECTOR(int_holdoff_V);
    baseline_shrink_V    <= STD_LOGIC_VECTOR(int_baseline_shrink_V);
    baseline_grow_V      <= STD_LOGIC_VECTOR(int_baseline_grow_V);
    dropped_i            <= STD_LOGIC_VECTOR(int_dropped_i);

    process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ACLK_EN = '1') then
                if (w_hs = '1' and waddr = ADDR_HOLDOFF_V_DATA_0) then
                    int_holdoff_V(9 downto 0) <= (UNSIGNED(WDATA(9 downto 0)) and wmask(9 downto 0)) or ((not wmask(9 downto 0)) and int_holdoff_V(9 downto 0));
                end if;
            end if;
        end if;
    end process;

    process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ACLK_EN = '1') then
                if (w_hs = '1' and waddr = ADDR_BASELINE_SHRINK_V_DATA_0) then
                    int_baseline_shrink_V(8 downto 0) <= (UNSIGNED(WDATA(8 downto 0)) and wmask(8 downto 0)) or ((not wmask(8 downto 0)) and int_baseline_shrink_V(8 downto 0));
                end if;
            end if;
        end if;
    end process;

    process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ACLK_EN = '1') then
                if (w_hs = '1' and waddr = ADDR_BASELINE_GROW_V_DATA_0) then
                    int_baseline_grow_V(8 downto 0) <= (UNSIGNED(WDATA(8 downto 0)) and wmask(8 downto 0)) or ((not wmask(8 downto 0)) and int_baseline_grow_V(8 downto 0));
                end if;
            end if;
        end if;
    end process;

    process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ACLK_EN = '1') then
                if (w_hs = '1' and waddr = ADDR_DROPPED_I_DATA_0) then
                    int_dropped_i(31 downto 0) <= (UNSIGNED(WDATA(31 downto 0)) and wmask(31 downto 0)) or ((not wmask(31 downto 0)) and int_dropped_i(31 downto 0));
                end if;
            end if;
        end if;
    end process;

    process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ARESET = '1') then
                int_dropped_o <= (others => '0');
            elsif (ACLK_EN = '1') then
                if (dropped_o_ap_vld = '1') then
                    int_dropped_o <= UNSIGNED(dropped_o); -- clear on read
                end if;
            end if;
        end if;
    end process;

    process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ARESET = '1') then
                int_dropped_o_ap_vld <= '0';
            elsif (ACLK_EN = '1') then
                if (dropped_o_ap_vld = '1') then
                    int_dropped_o_ap_vld <= '1';
                elsif (ar_hs = '1' and raddr = ADDR_DROPPED_O_CTRL) then
                    int_dropped_o_ap_vld <= '0'; -- clear on read
                end if;
            end if;
        end if;
    end process;


-- ----------------------- Memory logic ------------------
    -- triggercfg_0_threshold_V
    int_triggercfg_0_threshold_V_address0 <= SHIFT_RIGHT(UNSIGNED(triggercfg_0_threshold_V_address0), 1)(7 downto 0);
    int_triggercfg_0_threshold_V_ce0 <= triggercfg_0_threshold_V_ce0;
    int_triggercfg_0_threshold_V_we0 <= '0';
    int_triggercfg_0_threshold_V_be0 <= (others => '0');
    int_triggercfg_0_threshold_V_d0 <= (others => '0');
    triggercfg_0_threshold_V_q0 <= STD_LOGIC_VECTOR(SHIFT_RIGHT(int_triggercfg_0_threshold_V_q0, TO_INTEGER(int_triggercfg_0_threshold_V_shift) * 16)(15 downto 0));
    int_triggercfg_0_threshold_V_address1 <= raddr(9 downto 2) when ar_hs = '1' else waddr(9 downto 2);
    int_triggercfg_0_threshold_V_ce1 <= '1' when ar_hs = '1' or (int_triggercfg_0_threshold_V_write = '1' and WVALID  = '1') else '0';
    int_triggercfg_0_threshold_V_we1 <= '1' when int_triggercfg_0_threshold_V_write = '1' and WVALID = '1' else '0';
    int_triggercfg_0_threshold_V_be1 <= UNSIGNED(WSTRB);
    int_triggercfg_0_threshold_V_d1 <= UNSIGNED(WDATA);
    -- triggercfg_1_threshold_V
    int_triggercfg_1_threshold_V_address0 <= SHIFT_RIGHT(UNSIGNED(triggercfg_1_threshold_V_address0), 1)(7 downto 0);
    int_triggercfg_1_threshold_V_ce0 <= triggercfg_1_threshold_V_ce0;
    int_triggercfg_1_threshold_V_we0 <= '0';
    int_triggercfg_1_threshold_V_be0 <= (others => '0');
    int_triggercfg_1_threshold_V_d0 <= (others => '0');
    triggercfg_1_threshold_V_q0 <= STD_LOGIC_VECTOR(SHIFT_RIGHT(int_triggercfg_1_threshold_V_q0, TO_INTEGER(int_triggercfg_1_threshold_V_shift) * 16)(15 downto 0));
    int_triggercfg_1_threshold_V_address1 <= raddr(9 downto 2) when ar_hs = '1' else waddr(9 downto 2);
    int_triggercfg_1_threshold_V_ce1 <= '1' when ar_hs = '1' or (int_triggercfg_1_threshold_V_write = '1' and WVALID  = '1') else '0';
    int_triggercfg_1_threshold_V_we1 <= '1' when int_triggercfg_1_threshold_V_write = '1' and WVALID = '1' else '0';
    int_triggercfg_1_threshold_V_be1 <= UNSIGNED(WSTRB);
    int_triggercfg_1_threshold_V_d1 <= UNSIGNED(WDATA);
    -- triggercfg_2_threshold_V
    int_triggercfg_2_threshold_V_address0 <= SHIFT_RIGHT(UNSIGNED(triggercfg_2_threshold_V_address0), 1)(7 downto 0);
    int_triggercfg_2_threshold_V_ce0 <= triggercfg_2_threshold_V_ce0;
    int_triggercfg_2_threshold_V_we0 <= '0';
    int_triggercfg_2_threshold_V_be0 <= (others => '0');
    int_triggercfg_2_threshold_V_d0 <= (others => '0');
    triggercfg_2_threshold_V_q0 <= STD_LOGIC_VECTOR(SHIFT_RIGHT(int_triggercfg_2_threshold_V_q0, TO_INTEGER(int_triggercfg_2_threshold_V_shift) * 16)(15 downto 0));
    int_triggercfg_2_threshold_V_address1 <= raddr(9 downto 2) when ar_hs = '1' else waddr(9 downto 2);
    int_triggercfg_2_threshold_V_ce1 <= '1' when ar_hs = '1' or (int_triggercfg_2_threshold_V_write = '1' and WVALID  = '1') else '0';
    int_triggercfg_2_threshold_V_we1 <= '1' when int_triggercfg_2_threshold_V_write = '1' and WVALID = '1' else '0';
    int_triggercfg_2_threshold_V_be1 <= UNSIGNED(WSTRB);
    int_triggercfg_2_threshold_V_d1 <= UNSIGNED(WDATA);
    -- triggercfg_3_threshold_V
    int_triggercfg_3_threshold_V_address0 <= SHIFT_RIGHT(UNSIGNED(triggercfg_3_threshold_V_address0), 1)(7 downto 0);
    int_triggercfg_3_threshold_V_ce0 <= triggercfg_3_threshold_V_ce0;
    int_triggercfg_3_threshold_V_we0 <= '0';
    int_triggercfg_3_threshold_V_be0 <= (others => '0');
    int_triggercfg_3_threshold_V_d0 <= (others => '0');
    triggercfg_3_threshold_V_q0 <= STD_LOGIC_VECTOR(SHIFT_RIGHT(int_triggercfg_3_threshold_V_q0, TO_INTEGER(int_triggercfg_3_threshold_V_shift) * 16)(15 downto 0));
    int_triggercfg_3_threshold_V_address1 <= raddr(9 downto 2) when ar_hs = '1' else waddr(9 downto 2);
    int_triggercfg_3_threshold_V_ce1 <= '1' when ar_hs = '1' or (int_triggercfg_3_threshold_V_write = '1' and WVALID  = '1') else '0';
    int_triggercfg_3_threshold_V_we1 <= '1' when int_triggercfg_3_threshold_V_write = '1' and WVALID = '1' else '0';
    int_triggercfg_3_threshold_V_be1 <= UNSIGNED(WSTRB);
    int_triggercfg_3_threshold_V_d1 <= UNSIGNED(WDATA);

    process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ARESET = '1') then
                int_triggercfg_0_threshold_V_read <= '0';
            elsif (ACLK_EN = '1') then
                if (ar_hs = '1' and raddr >= ADDR_TRIGGERCFG_0_THRESHOLD_V_BASE and raddr <= ADDR_TRIGGERCFG_0_THRESHOLD_V_HIGH) then
                    int_triggercfg_0_threshold_V_read <= '1';
                else
                    int_triggercfg_0_threshold_V_read <= '0';
                end if;
            end if;
        end if;
    end process;

    process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ARESET = '1') then
                int_triggercfg_0_threshold_V_write <= '0';
            elsif (ACLK_EN = '1') then
                if (aw_hs = '1' and UNSIGNED(AWADDR(ADDR_BITS-1 downto 0)) >= ADDR_TRIGGERCFG_0_THRESHOLD_V_BASE and UNSIGNED(AWADDR(ADDR_BITS-1 downto 0)) <= ADDR_TRIGGERCFG_0_THRESHOLD_V_HIGH) then
                    int_triggercfg_0_threshold_V_write <= '1';
                elsif (WVALID = '1') then
                    int_triggercfg_0_threshold_V_write <= '0';
                end if;
            end if;
        end if;
    end process;

    process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ACLK_EN = '1') then
                if (triggercfg_0_threshold_V_ce0 = '1') then
                    int_triggercfg_0_threshold_V_shift(0) <= triggercfg_0_threshold_V_address0(0);
                end if;
            end if;
        end if;
    end process;

    process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ARESET = '1') then
                int_triggercfg_1_threshold_V_read <= '0';
            elsif (ACLK_EN = '1') then
                if (ar_hs = '1' and raddr >= ADDR_TRIGGERCFG_1_THRESHOLD_V_BASE and raddr <= ADDR_TRIGGERCFG_1_THRESHOLD_V_HIGH) then
                    int_triggercfg_1_threshold_V_read <= '1';
                else
                    int_triggercfg_1_threshold_V_read <= '0';
                end if;
            end if;
        end if;
    end process;

    process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ARESET = '1') then
                int_triggercfg_1_threshold_V_write <= '0';
            elsif (ACLK_EN = '1') then
                if (aw_hs = '1' and UNSIGNED(AWADDR(ADDR_BITS-1 downto 0)) >= ADDR_TRIGGERCFG_1_THRESHOLD_V_BASE and UNSIGNED(AWADDR(ADDR_BITS-1 downto 0)) <= ADDR_TRIGGERCFG_1_THRESHOLD_V_HIGH) then
                    int_triggercfg_1_threshold_V_write <= '1';
                elsif (WVALID = '1') then
                    int_triggercfg_1_threshold_V_write <= '0';
                end if;
            end if;
        end if;
    end process;

    process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ACLK_EN = '1') then
                if (triggercfg_1_threshold_V_ce0 = '1') then
                    int_triggercfg_1_threshold_V_shift(0) <= triggercfg_1_threshold_V_address0(0);
                end if;
            end if;
        end if;
    end process;

    process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ARESET = '1') then
                int_triggercfg_2_threshold_V_read <= '0';
            elsif (ACLK_EN = '1') then
                if (ar_hs = '1' and raddr >= ADDR_TRIGGERCFG_2_THRESHOLD_V_BASE and raddr <= ADDR_TRIGGERCFG_2_THRESHOLD_V_HIGH) then
                    int_triggercfg_2_threshold_V_read <= '1';
                else
                    int_triggercfg_2_threshold_V_read <= '0';
                end if;
            end if;
        end if;
    end process;

    process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ARESET = '1') then
                int_triggercfg_2_threshold_V_write <= '0';
            elsif (ACLK_EN = '1') then
                if (aw_hs = '1' and UNSIGNED(AWADDR(ADDR_BITS-1 downto 0)) >= ADDR_TRIGGERCFG_2_THRESHOLD_V_BASE and UNSIGNED(AWADDR(ADDR_BITS-1 downto 0)) <= ADDR_TRIGGERCFG_2_THRESHOLD_V_HIGH) then
                    int_triggercfg_2_threshold_V_write <= '1';
                elsif (WVALID = '1') then
                    int_triggercfg_2_threshold_V_write <= '0';
                end if;
            end if;
        end if;
    end process;

    process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ACLK_EN = '1') then
                if (triggercfg_2_threshold_V_ce0 = '1') then
                    int_triggercfg_2_threshold_V_shift(0) <= triggercfg_2_threshold_V_address0(0);
                end if;
            end if;
        end if;
    end process;

    process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ARESET = '1') then
                int_triggercfg_3_threshold_V_read <= '0';
            elsif (ACLK_EN = '1') then
                if (ar_hs = '1' and raddr >= ADDR_TRIGGERCFG_3_THRESHOLD_V_BASE and raddr <= ADDR_TRIGGERCFG_3_THRESHOLD_V_HIGH) then
                    int_triggercfg_3_threshold_V_read <= '1';
                else
                    int_triggercfg_3_threshold_V_read <= '0';
                end if;
            end if;
        end if;
    end process;

    process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ARESET = '1') then
                int_triggercfg_3_threshold_V_write <= '0';
            elsif (ACLK_EN = '1') then
                if (aw_hs = '1' and UNSIGNED(AWADDR(ADDR_BITS-1 downto 0)) >= ADDR_TRIGGERCFG_3_THRESHOLD_V_BASE and UNSIGNED(AWADDR(ADDR_BITS-1 downto 0)) <= ADDR_TRIGGERCFG_3_THRESHOLD_V_HIGH) then
                    int_triggercfg_3_threshold_V_write <= '1';
                elsif (WVALID = '1') then
                    int_triggercfg_3_threshold_V_write <= '0';
                end if;
            end if;
        end if;
    end process;

    process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ACLK_EN = '1') then
                if (triggercfg_3_threshold_V_ce0 = '1') then
                    int_triggercfg_3_threshold_V_shift(0) <= triggercfg_3_threshold_V_address0(0);
                end if;
            end if;
        end if;
    end process;


end architecture behave;

library IEEE;
USE IEEE.std_logic_1164.all;
USE IEEE.numeric_std.all;

entity photon_trigger_control_s_axi_ram is
    generic (
        BYTES   : INTEGER :=4;
        DEPTH   : INTEGER :=256;
        AWIDTH  : INTEGER :=8);
    port (
        clk0    : in  STD_LOGIC;
        address0: in  UNSIGNED(AWIDTH-1 downto 0);
        ce0     : in  STD_LOGIC;
        we0     : in  STD_LOGIC;
        be0     : in  UNSIGNED(BYTES-1 downto 0);
        d0      : in  UNSIGNED(BYTES*8-1 downto 0);
        q0      : out UNSIGNED(BYTES*8-1 downto 0);
        clk1    : in  STD_LOGIC;
        address1: in  UNSIGNED(AWIDTH-1 downto 0);
        ce1     : in  STD_LOGIC;
        we1     : in  STD_LOGIC;
        be1     : in  UNSIGNED(BYTES-1 downto 0);
        d1      : in  UNSIGNED(BYTES*8-1 downto 0);
        q1      : out UNSIGNED(BYTES*8-1 downto 0));

end entity photon_trigger_control_s_axi_ram;

architecture behave of photon_trigger_control_s_axi_ram is
    signal address0_tmp : UNSIGNED(AWIDTH-1 downto 0);
    signal address1_tmp : UNSIGNED(AWIDTH-1 downto 0);
    type RAM_T is array (0 to DEPTH - 1) of UNSIGNED(BYTES*8 - 1 downto 0);
    shared variable mem : RAM_T := (others => (others => '0'));
begin

    process (address0)
    begin
    address0_tmp <= address0;
    --synthesis translate_off
          if (address0 > DEPTH-1) then
              address0_tmp <= (others => '0');
          else
              address0_tmp <= address0;
          end if;
    --synthesis translate_on
    end process;

    process (address1)
    begin
    address1_tmp <= address1;
    --synthesis translate_off
          if (address1 > DEPTH-1) then
              address1_tmp <= (others => '0');
          else
              address1_tmp <= address1;
          end if;
    --synthesis translate_on
    end process;

    --read port 0
    process (clk0) begin
        if (clk0'event and clk0 = '1') then
            if (ce0 = '1') then
                q0 <= mem(to_integer(address0_tmp));
            end if;
        end if;
    end process;

    --read port 1
    process (clk1) begin
        if (clk1'event and clk1 = '1') then
            if (ce1 = '1') then
                q1 <= mem(to_integer(address1_tmp));
            end if;
        end if;
    end process;

    gen_write : for i in 0 to BYTES - 1 generate
    begin
        --write port 0
        process (clk0)
        begin
            if (clk0'event and clk0 = '1') then
                if (ce0 = '1' and we0 = '1' and be0(i) = '1') then
                    mem(to_integer(address0_tmp))(8*i+7 downto 8*i) := d0(8*i+7 downto 8*i);
                end if;
            end if;
        end process;

        --write port 1
        process (clk1)
        begin
            if (clk1'event and clk1 = '1') then
                if (ce1 = '1' and we1 = '1' and be1(i) = '1') then
                    mem(to_integer(address1_tmp))(8*i+7 downto 8*i) := d1(8*i+7 downto 8*i);
                end if;
            end if;
        end process;

    end generate;

end architecture behave;


