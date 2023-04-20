-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
-- Tool Version Limit: 2022.04
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.NUMERIC_STD.all;

entity postage_filter_control_s_axi is
generic (
    C_S_AXI_ADDR_WIDTH    : INTEGER := 7;
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
    monitor_0             :out  STD_LOGIC_VECTOR(11 downto 0);
    monitor_1             :out  STD_LOGIC_VECTOR(11 downto 0);
    monitor_2             :out  STD_LOGIC_VECTOR(11 downto 0);
    monitor_3             :out  STD_LOGIC_VECTOR(11 downto 0);
    monitor_4             :out  STD_LOGIC_VECTOR(11 downto 0);
    monitor_5             :out  STD_LOGIC_VECTOR(11 downto 0);
    monitor_6             :out  STD_LOGIC_VECTOR(11 downto 0);
    monitor_7             :out  STD_LOGIC_VECTOR(11 downto 0)
);
end entity postage_filter_control_s_axi;

-- ------------------------Address Info-------------------
-- 0x00 : reserved
-- 0x04 : reserved
-- 0x08 : reserved
-- 0x0c : reserved
-- 0x10 : Data signal of monitor_0
--        bit 11~0 - monitor_0[11:0] (Read/Write)
--        others   - reserved
-- 0x14 : reserved
-- 0x18 : Data signal of monitor_1
--        bit 11~0 - monitor_1[11:0] (Read/Write)
--        others   - reserved
-- 0x1c : reserved
-- 0x20 : Data signal of monitor_2
--        bit 11~0 - monitor_2[11:0] (Read/Write)
--        others   - reserved
-- 0x24 : reserved
-- 0x28 : Data signal of monitor_3
--        bit 11~0 - monitor_3[11:0] (Read/Write)
--        others   - reserved
-- 0x2c : reserved
-- 0x30 : Data signal of monitor_4
--        bit 11~0 - monitor_4[11:0] (Read/Write)
--        others   - reserved
-- 0x34 : reserved
-- 0x38 : Data signal of monitor_5
--        bit 11~0 - monitor_5[11:0] (Read/Write)
--        others   - reserved
-- 0x3c : reserved
-- 0x40 : Data signal of monitor_6
--        bit 11~0 - monitor_6[11:0] (Read/Write)
--        others   - reserved
-- 0x44 : reserved
-- 0x48 : Data signal of monitor_7
--        bit 11~0 - monitor_7[11:0] (Read/Write)
--        others   - reserved
-- 0x4c : reserved
-- (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

architecture behave of postage_filter_control_s_axi is
    type states is (wridle, wrdata, wrresp, wrreset, rdidle, rddata, rdreset);  -- read and write fsm states
    signal wstate  : states := wrreset;
    signal rstate  : states := rdreset;
    signal wnext, rnext: states;
    constant ADDR_MONITOR_0_DATA_0 : INTEGER := 16#10#;
    constant ADDR_MONITOR_0_CTRL   : INTEGER := 16#14#;
    constant ADDR_MONITOR_1_DATA_0 : INTEGER := 16#18#;
    constant ADDR_MONITOR_1_CTRL   : INTEGER := 16#1c#;
    constant ADDR_MONITOR_2_DATA_0 : INTEGER := 16#20#;
    constant ADDR_MONITOR_2_CTRL   : INTEGER := 16#24#;
    constant ADDR_MONITOR_3_DATA_0 : INTEGER := 16#28#;
    constant ADDR_MONITOR_3_CTRL   : INTEGER := 16#2c#;
    constant ADDR_MONITOR_4_DATA_0 : INTEGER := 16#30#;
    constant ADDR_MONITOR_4_CTRL   : INTEGER := 16#34#;
    constant ADDR_MONITOR_5_DATA_0 : INTEGER := 16#38#;
    constant ADDR_MONITOR_5_CTRL   : INTEGER := 16#3c#;
    constant ADDR_MONITOR_6_DATA_0 : INTEGER := 16#40#;
    constant ADDR_MONITOR_6_CTRL   : INTEGER := 16#44#;
    constant ADDR_MONITOR_7_DATA_0 : INTEGER := 16#48#;
    constant ADDR_MONITOR_7_CTRL   : INTEGER := 16#4c#;
    constant ADDR_BITS         : INTEGER := 7;

    signal waddr               : UNSIGNED(ADDR_BITS-1 downto 0);
    signal wmask               : UNSIGNED(C_S_AXI_DATA_WIDTH-1 downto 0);
    signal aw_hs               : STD_LOGIC;
    signal w_hs                : STD_LOGIC;
    signal rdata_data          : UNSIGNED(C_S_AXI_DATA_WIDTH-1 downto 0);
    signal ar_hs               : STD_LOGIC;
    signal raddr               : UNSIGNED(ADDR_BITS-1 downto 0);
    signal AWREADY_t           : STD_LOGIC;
    signal WREADY_t            : STD_LOGIC;
    signal ARREADY_t           : STD_LOGIC;
    signal RVALID_t            : STD_LOGIC;
    -- internal registers
    signal int_monitor_0       : UNSIGNED(11 downto 0) := (others => '0');
    signal int_monitor_1       : UNSIGNED(11 downto 0) := (others => '0');
    signal int_monitor_2       : UNSIGNED(11 downto 0) := (others => '0');
    signal int_monitor_3       : UNSIGNED(11 downto 0) := (others => '0');
    signal int_monitor_4       : UNSIGNED(11 downto 0) := (others => '0');
    signal int_monitor_5       : UNSIGNED(11 downto 0) := (others => '0');
    signal int_monitor_6       : UNSIGNED(11 downto 0) := (others => '0');
    signal int_monitor_7       : UNSIGNED(11 downto 0) := (others => '0');


begin
-- ----------------------- Instantiation------------------


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
    RVALID_t  <= '1' when (rstate = rddata) else '0';
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
                    rdata_data <= (others => '0');
                    case (TO_INTEGER(raddr)) is
                    when ADDR_MONITOR_0_DATA_0 =>
                        rdata_data <= RESIZE(int_monitor_0(11 downto 0), 32);
                    when ADDR_MONITOR_1_DATA_0 =>
                        rdata_data <= RESIZE(int_monitor_1(11 downto 0), 32);
                    when ADDR_MONITOR_2_DATA_0 =>
                        rdata_data <= RESIZE(int_monitor_2(11 downto 0), 32);
                    when ADDR_MONITOR_3_DATA_0 =>
                        rdata_data <= RESIZE(int_monitor_3(11 downto 0), 32);
                    when ADDR_MONITOR_4_DATA_0 =>
                        rdata_data <= RESIZE(int_monitor_4(11 downto 0), 32);
                    when ADDR_MONITOR_5_DATA_0 =>
                        rdata_data <= RESIZE(int_monitor_5(11 downto 0), 32);
                    when ADDR_MONITOR_6_DATA_0 =>
                        rdata_data <= RESIZE(int_monitor_6(11 downto 0), 32);
                    when ADDR_MONITOR_7_DATA_0 =>
                        rdata_data <= RESIZE(int_monitor_7(11 downto 0), 32);
                    when others =>
                        NULL;
                    end case;
                end if;
            end if;
        end if;
    end process;

-- ----------------------- Register logic ----------------
    monitor_0            <= STD_LOGIC_VECTOR(int_monitor_0);
    monitor_1            <= STD_LOGIC_VECTOR(int_monitor_1);
    monitor_2            <= STD_LOGIC_VECTOR(int_monitor_2);
    monitor_3            <= STD_LOGIC_VECTOR(int_monitor_3);
    monitor_4            <= STD_LOGIC_VECTOR(int_monitor_4);
    monitor_5            <= STD_LOGIC_VECTOR(int_monitor_5);
    monitor_6            <= STD_LOGIC_VECTOR(int_monitor_6);
    monitor_7            <= STD_LOGIC_VECTOR(int_monitor_7);

    process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ACLK_EN = '1') then
                if (w_hs = '1' and waddr = ADDR_MONITOR_0_DATA_0) then
                    int_monitor_0(11 downto 0) <= (UNSIGNED(WDATA(11 downto 0)) and wmask(11 downto 0)) or ((not wmask(11 downto 0)) and int_monitor_0(11 downto 0));
                end if;
            end if;
        end if;
    end process;

    process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ACLK_EN = '1') then
                if (w_hs = '1' and waddr = ADDR_MONITOR_1_DATA_0) then
                    int_monitor_1(11 downto 0) <= (UNSIGNED(WDATA(11 downto 0)) and wmask(11 downto 0)) or ((not wmask(11 downto 0)) and int_monitor_1(11 downto 0));
                end if;
            end if;
        end if;
    end process;

    process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ACLK_EN = '1') then
                if (w_hs = '1' and waddr = ADDR_MONITOR_2_DATA_0) then
                    int_monitor_2(11 downto 0) <= (UNSIGNED(WDATA(11 downto 0)) and wmask(11 downto 0)) or ((not wmask(11 downto 0)) and int_monitor_2(11 downto 0));
                end if;
            end if;
        end if;
    end process;

    process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ACLK_EN = '1') then
                if (w_hs = '1' and waddr = ADDR_MONITOR_3_DATA_0) then
                    int_monitor_3(11 downto 0) <= (UNSIGNED(WDATA(11 downto 0)) and wmask(11 downto 0)) or ((not wmask(11 downto 0)) and int_monitor_3(11 downto 0));
                end if;
            end if;
        end if;
    end process;

    process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ACLK_EN = '1') then
                if (w_hs = '1' and waddr = ADDR_MONITOR_4_DATA_0) then
                    int_monitor_4(11 downto 0) <= (UNSIGNED(WDATA(11 downto 0)) and wmask(11 downto 0)) or ((not wmask(11 downto 0)) and int_monitor_4(11 downto 0));
                end if;
            end if;
        end if;
    end process;

    process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ACLK_EN = '1') then
                if (w_hs = '1' and waddr = ADDR_MONITOR_5_DATA_0) then
                    int_monitor_5(11 downto 0) <= (UNSIGNED(WDATA(11 downto 0)) and wmask(11 downto 0)) or ((not wmask(11 downto 0)) and int_monitor_5(11 downto 0));
                end if;
            end if;
        end if;
    end process;

    process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ACLK_EN = '1') then
                if (w_hs = '1' and waddr = ADDR_MONITOR_6_DATA_0) then
                    int_monitor_6(11 downto 0) <= (UNSIGNED(WDATA(11 downto 0)) and wmask(11 downto 0)) or ((not wmask(11 downto 0)) and int_monitor_6(11 downto 0));
                end if;
            end if;
        end if;
    end process;

    process (ACLK)
    begin
        if (ACLK'event and ACLK = '1') then
            if (ACLK_EN = '1') then
                if (w_hs = '1' and waddr = ADDR_MONITOR_7_DATA_0) then
                    int_monitor_7(11 downto 0) <= (UNSIGNED(WDATA(11 downto 0)) and wmask(11 downto 0)) or ((not wmask(11 downto 0)) and int_monitor_7(11 downto 0));
                end if;
            end if;
        end if;
    end process;


-- ----------------------- Memory logic ------------------

end architecture behave;
