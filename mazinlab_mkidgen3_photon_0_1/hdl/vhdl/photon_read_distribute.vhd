-- ==============================================================
-- RTL generated by Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2021.1 (64-bit)
-- Version: 2021.1
-- Copyright (C) Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
-- 
-- ===========================================================

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity photon_read_distribute is
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
end;


architecture behav of photon_read_distribute is 
    constant ap_const_logic_1 : STD_LOGIC := '1';
    constant ap_const_logic_0 : STD_LOGIC := '0';
    constant ap_ST_fsm_state1 : STD_LOGIC_VECTOR (4 downto 0) := "00001";
    constant ap_ST_fsm_pp0_stage0 : STD_LOGIC_VECTOR (4 downto 0) := "00010";
    constant ap_ST_fsm_pp0_stage1 : STD_LOGIC_VECTOR (4 downto 0) := "00100";
    constant ap_ST_fsm_pp0_stage2 : STD_LOGIC_VECTOR (4 downto 0) := "01000";
    constant ap_ST_fsm_pp0_stage3 : STD_LOGIC_VECTOR (4 downto 0) := "10000";
    constant ap_const_lv32_0 : STD_LOGIC_VECTOR (31 downto 0) := "00000000000000000000000000000000";
    constant ap_const_boolean_1 : BOOLEAN := true;
    constant ap_const_lv32_2 : STD_LOGIC_VECTOR (31 downto 0) := "00000000000000000000000000000010";
    constant ap_const_boolean_0 : BOOLEAN := false;
    constant ap_const_lv1_1 : STD_LOGIC_VECTOR (0 downto 0) := "1";
    constant ap_const_lv32_3 : STD_LOGIC_VECTOR (31 downto 0) := "00000000000000000000000000000011";
    constant ap_const_lv32_4 : STD_LOGIC_VECTOR (31 downto 0) := "00000000000000000000000000000100";
    constant ap_const_lv32_1 : STD_LOGIC_VECTOR (31 downto 0) := "00000000000000000000000000000001";

attribute shreg_extract : string;
    signal ap_done_reg : STD_LOGIC := '0';
    signal ap_CS_fsm : STD_LOGIC_VECTOR (4 downto 0) := "00001";
    attribute fsm_encoding : string;
    attribute fsm_encoding of ap_CS_fsm : signal is "none";
    signal ap_CS_fsm_state1 : STD_LOGIC;
    attribute fsm_encoding of ap_CS_fsm_state1 : signal is "none";
    signal photons_V_TDATA_blk_n : STD_LOGIC;
    signal ap_CS_fsm_pp0_stage1 : STD_LOGIC;
    attribute fsm_encoding of ap_CS_fsm_pp0_stage1 : signal is "none";
    signal ap_enable_reg_pp0_iter0 : STD_LOGIC := '0';
    signal ap_block_pp0_stage1 : BOOLEAN;
    signal read_reg_172 : STD_LOGIC_VECTOR (0 downto 0);
    signal ap_CS_fsm_pp0_stage2 : STD_LOGIC;
    attribute fsm_encoding of ap_CS_fsm_pp0_stage2 : signal is "none";
    signal ap_block_pp0_stage2 : BOOLEAN;
    signal read_1_reg_187 : STD_LOGIC_VECTOR (0 downto 0);
    signal ap_CS_fsm_pp0_stage3 : STD_LOGIC;
    attribute fsm_encoding of ap_CS_fsm_pp0_stage3 : signal is "none";
    signal ap_block_pp0_stage3 : BOOLEAN;
    signal read_2_reg_197 : STD_LOGIC_VECTOR (0 downto 0);
    signal ap_CS_fsm_pp0_stage0 : STD_LOGIC;
    attribute fsm_encoding of ap_CS_fsm_pp0_stage0 : signal is "none";
    signal ap_enable_reg_pp0_iter1 : STD_LOGIC := '0';
    signal ap_block_pp0_stage0 : BOOLEAN;
    signal read_3_reg_207 : STD_LOGIC_VECTOR (0 downto 0);
    signal ap_block_state2_pp0_stage0_iter0 : BOOLEAN;
    signal ap_block_state6_pp0_stage0_iter1 : BOOLEAN;
    signal ap_block_state6_io : BOOLEAN;
    signal ap_block_pp0_stage0_11001 : BOOLEAN;
    signal tmp_1_reg_177 : STD_LOGIC_VECTOR (43 downto 0);
    signal tmp_fu_89_p1 : STD_LOGIC_VECTOR (47 downto 0);
    signal ap_block_state3_pp0_stage1_iter0 : BOOLEAN;
    signal ap_block_state3_io : BOOLEAN;
    signal ap_block_state7_pp0_stage1_iter1 : BOOLEAN;
    signal ap_block_state7_io : BOOLEAN;
    signal ap_block_pp0_stage1_11001 : BOOLEAN;
    signal photon_2_fu_115_p3 : STD_LOGIC_VECTOR (47 downto 0);
    signal photon_2_reg_191 : STD_LOGIC_VECTOR (47 downto 0);
    signal ap_block_state4_pp0_stage2_iter0 : BOOLEAN;
    signal ap_block_state4_io : BOOLEAN;
    signal ap_block_pp0_stage2_11001 : BOOLEAN;
    signal photon_4_fu_135_p3 : STD_LOGIC_VECTOR (47 downto 0);
    signal photon_4_reg_201 : STD_LOGIC_VECTOR (47 downto 0);
    signal ap_block_state5_pp0_stage3_iter0 : BOOLEAN;
    signal ap_block_state5_io : BOOLEAN;
    signal ap_block_pp0_stage3_11001 : BOOLEAN;
    signal photon_6_fu_154_p3 : STD_LOGIC_VECTOR (47 downto 0);
    signal photon_6_reg_211 : STD_LOGIC_VECTOR (47 downto 0);
    signal ap_block_state1 : BOOLEAN;
    signal ap_block_pp0_stage3_subdone : BOOLEAN;
    signal photon_1_fu_46 : STD_LOGIC_VECTOR (47 downto 0);
    signal istrms_read_nbread_fu_50_p2_0 : STD_LOGIC_VECTOR (0 downto 0);
    signal ap_block_pp0_stage1_01001 : BOOLEAN;
    signal ap_block_pp0_stage2_01001 : BOOLEAN;
    signal ap_block_pp0_stage3_01001 : BOOLEAN;
    signal ap_block_pp0_stage0_01001 : BOOLEAN;
    signal istrms1_read_nbread_fu_63_p2_0 : STD_LOGIC_VECTOR (0 downto 0);
    signal istrms2_read_nbread_fu_69_p2_0 : STD_LOGIC_VECTOR (0 downto 0);
    signal istrms3_read_nbread_fu_75_p2_0 : STD_LOGIC_VECTOR (0 downto 0);
    signal tmp_8_fu_104_p1 : STD_LOGIC_VECTOR (47 downto 0);
    signal select_ln164_fu_108_p3 : STD_LOGIC_VECTOR (47 downto 0);
    signal tmp_9_fu_131_p1 : STD_LOGIC_VECTOR (47 downto 0);
    signal tmp_10_fu_150_p1 : STD_LOGIC_VECTOR (47 downto 0);
    signal ap_NS_fsm : STD_LOGIC_VECTOR (4 downto 0);
    signal ap_ST_fsm_state1_blk : STD_LOGIC;
    signal ap_block_pp0_stage0_subdone : BOOLEAN;
    signal ap_block_pp0_stage1_subdone : BOOLEAN;
    signal ap_block_pp0_stage2_subdone : BOOLEAN;
    signal ap_idle_pp0 : STD_LOGIC;
    signal ap_enable_pp0 : STD_LOGIC;
    signal regslice_both_photons_V_U_apdone_blk : STD_LOGIC;
    signal photons_V_TDATA_int_regslice : STD_LOGIC_VECTOR (47 downto 0);
    signal photons_V_TVALID_int_regslice : STD_LOGIC;
    signal photons_V_TREADY_int_regslice : STD_LOGIC;
    signal regslice_both_photons_V_U_vld_out : STD_LOGIC;
    signal ap_ce_reg : STD_LOGIC;

    component photon_regslice_both IS
    generic (
        DataWidth : INTEGER );
    port (
        ap_clk : IN STD_LOGIC;
        ap_rst : IN STD_LOGIC;
        data_in : IN STD_LOGIC_VECTOR (DataWidth-1 downto 0);
        vld_in : IN STD_LOGIC;
        ack_in : OUT STD_LOGIC;
        data_out : OUT STD_LOGIC_VECTOR (DataWidth-1 downto 0);
        vld_out : OUT STD_LOGIC;
        ack_out : IN STD_LOGIC;
        apdone_blk : OUT STD_LOGIC );
    end component;



begin
    regslice_both_photons_V_U : component photon_regslice_both
    generic map (
        DataWidth => 48)
    port map (
        ap_clk => ap_clk,
        ap_rst => ap_rst,
        data_in => photons_V_TDATA_int_regslice,
        vld_in => photons_V_TVALID_int_regslice,
        ack_in => photons_V_TREADY_int_regslice,
        data_out => photons_V_TDATA,
        vld_out => regslice_both_photons_V_U_vld_out,
        ack_out => photons_V_TREADY,
        apdone_blk => regslice_both_photons_V_U_apdone_blk);





    ap_CS_fsm_assign_proc : process(ap_clk)
    begin
        if (ap_clk'event and ap_clk =  '1') then
            if (ap_rst = '1') then
                ap_CS_fsm <= ap_ST_fsm_state1;
            else
                ap_CS_fsm <= ap_NS_fsm;
            end if;
        end if;
    end process;


    ap_done_reg_assign_proc : process(ap_clk)
    begin
        if (ap_clk'event and ap_clk =  '1') then
            if (ap_rst = '1') then
                
            else
            end if;
        end if;
    end process;


    ap_enable_reg_pp0_iter0_assign_proc : process(ap_clk)
    begin
        if (ap_clk'event and ap_clk =  '1') then
            if (ap_rst = '1') then
                ap_enable_reg_pp0_iter0 <= ap_const_logic_0;
            else
                if ((not(((ap_done_reg = ap_const_logic_1) or (ap_start = ap_const_logic_0))) and (ap_const_logic_1 = ap_CS_fsm_state1))) then 
                    ap_enable_reg_pp0_iter0 <= ap_const_logic_1;
                end if; 
            end if;
        end if;
    end process;


    ap_enable_reg_pp0_iter1_assign_proc : process(ap_clk)
    begin
        if (ap_clk'event and ap_clk =  '1') then
            if (ap_rst = '1') then
                ap_enable_reg_pp0_iter1 <= ap_const_logic_0;
            else
                if (((ap_const_boolean_0 = ap_block_pp0_stage3_subdone) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage3))) then 
                    ap_enable_reg_pp0_iter1 <= ap_enable_reg_pp0_iter0;
                elsif ((not(((ap_done_reg = ap_const_logic_1) or (ap_start = ap_const_logic_0))) and (ap_const_logic_1 = ap_CS_fsm_state1))) then 
                    ap_enable_reg_pp0_iter1 <= ap_const_logic_0;
                end if; 
            end if;
        end if;
    end process;

    process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if (((ap_const_boolean_0 = ap_block_pp0_stage3_11001) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage3))) then
                photon_1_fu_46 <= photon_6_fu_154_p3;
            end if;
        end if;
    end process;
    process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if (((ap_const_boolean_0 = ap_block_pp0_stage1_11001) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage1))) then
                photon_2_reg_191 <= photon_2_fu_115_p3;
                read_1_reg_187 <= istrms1_read_nbread_fu_63_p2_0;
            end if;
        end if;
    end process;
    process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if (((ap_const_boolean_0 = ap_block_pp0_stage2_11001) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage2))) then
                photon_4_reg_201 <= photon_4_fu_135_p3;
                read_2_reg_197 <= istrms2_read_nbread_fu_69_p2_0;
            end if;
        end if;
    end process;
    process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if (((ap_const_boolean_0 = ap_block_pp0_stage3_11001) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage3))) then
                photon_6_reg_211 <= photon_6_fu_154_p3;
                read_3_reg_207 <= istrms3_read_nbread_fu_75_p2_0;
            end if;
        end if;
    end process;
    process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if (((ap_const_boolean_0 = ap_block_pp0_stage0_11001) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0))) then
                read_reg_172 <= istrms_read_nbread_fu_50_p2_0;
                tmp_1_reg_177 <= istrms_dout;
            end if;
        end if;
    end process;
    ap_done_reg <= '0';

    ap_NS_fsm_assign_proc : process (ap_start, ap_done_reg, ap_CS_fsm, ap_CS_fsm_state1, ap_block_pp0_stage3_subdone, ap_block_pp0_stage0_subdone, ap_block_pp0_stage1_subdone, ap_block_pp0_stage2_subdone)
    begin
        case ap_CS_fsm is
            when ap_ST_fsm_state1 => 
                if ((not(((ap_done_reg = ap_const_logic_1) or (ap_start = ap_const_logic_0))) and (ap_const_logic_1 = ap_CS_fsm_state1))) then
                    ap_NS_fsm <= ap_ST_fsm_pp0_stage0;
                else
                    ap_NS_fsm <= ap_ST_fsm_state1;
                end if;
            when ap_ST_fsm_pp0_stage0 => 
                if ((ap_const_boolean_0 = ap_block_pp0_stage0_subdone)) then
                    ap_NS_fsm <= ap_ST_fsm_pp0_stage1;
                else
                    ap_NS_fsm <= ap_ST_fsm_pp0_stage0;
                end if;
            when ap_ST_fsm_pp0_stage1 => 
                if ((ap_const_boolean_0 = ap_block_pp0_stage1_subdone)) then
                    ap_NS_fsm <= ap_ST_fsm_pp0_stage2;
                else
                    ap_NS_fsm <= ap_ST_fsm_pp0_stage1;
                end if;
            when ap_ST_fsm_pp0_stage2 => 
                if ((ap_const_boolean_0 = ap_block_pp0_stage2_subdone)) then
                    ap_NS_fsm <= ap_ST_fsm_pp0_stage3;
                else
                    ap_NS_fsm <= ap_ST_fsm_pp0_stage2;
                end if;
            when ap_ST_fsm_pp0_stage3 => 
                if ((ap_const_boolean_0 = ap_block_pp0_stage3_subdone)) then
                    ap_NS_fsm <= ap_ST_fsm_pp0_stage0;
                else
                    ap_NS_fsm <= ap_ST_fsm_pp0_stage3;
                end if;
            when others =>  
                ap_NS_fsm <= "XXXXX";
        end case;
    end process;
    ap_CS_fsm_pp0_stage0 <= ap_CS_fsm(1);
    ap_CS_fsm_pp0_stage1 <= ap_CS_fsm(2);
    ap_CS_fsm_pp0_stage2 <= ap_CS_fsm(3);
    ap_CS_fsm_pp0_stage3 <= ap_CS_fsm(4);
    ap_CS_fsm_state1 <= ap_CS_fsm(0);

    ap_ST_fsm_state1_blk_assign_proc : process(ap_start, ap_done_reg)
    begin
        if (((ap_done_reg = ap_const_logic_1) or (ap_start = ap_const_logic_0))) then 
            ap_ST_fsm_state1_blk <= ap_const_logic_1;
        else 
            ap_ST_fsm_state1_blk <= ap_const_logic_0;
        end if; 
    end process;

        ap_block_pp0_stage0 <= not((ap_const_boolean_1 = ap_const_boolean_1));

    ap_block_pp0_stage0_01001_assign_proc : process(read_2_reg_197, ap_enable_reg_pp0_iter1, read_3_reg_207, photons_V_TREADY_int_regslice)
    begin
                ap_block_pp0_stage0_01001 <= ((ap_enable_reg_pp0_iter1 = ap_const_logic_1) and (((read_2_reg_197 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0)) or ((photons_V_TREADY_int_regslice = ap_const_logic_0) and (read_3_reg_207 = ap_const_lv1_1))));
    end process;


    ap_block_pp0_stage0_11001_assign_proc : process(read_2_reg_197, ap_enable_reg_pp0_iter1, read_3_reg_207, ap_block_state6_io, photons_V_TREADY_int_regslice)
    begin
                ap_block_pp0_stage0_11001 <= ((ap_enable_reg_pp0_iter1 = ap_const_logic_1) and ((ap_const_boolean_1 = ap_block_state6_io) or ((read_2_reg_197 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0)) or ((photons_V_TREADY_int_regslice = ap_const_logic_0) and (read_3_reg_207 = ap_const_lv1_1))));
    end process;


    ap_block_pp0_stage0_subdone_assign_proc : process(read_2_reg_197, ap_enable_reg_pp0_iter1, read_3_reg_207, ap_block_state6_io, photons_V_TREADY_int_regslice)
    begin
                ap_block_pp0_stage0_subdone <= ((ap_enable_reg_pp0_iter1 = ap_const_logic_1) and ((ap_const_boolean_1 = ap_block_state6_io) or ((read_2_reg_197 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0)) or ((photons_V_TREADY_int_regslice = ap_const_logic_0) and (read_3_reg_207 = ap_const_lv1_1))));
    end process;

        ap_block_pp0_stage1 <= not((ap_const_boolean_1 = ap_const_boolean_1));

    ap_block_pp0_stage1_01001_assign_proc : process(ap_enable_reg_pp0_iter0, read_reg_172, ap_enable_reg_pp0_iter1, read_3_reg_207, photons_V_TREADY_int_regslice)
    begin
                ap_block_pp0_stage1_01001 <= (((read_reg_172 = ap_const_lv1_1) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0)) or ((photons_V_TREADY_int_regslice = ap_const_logic_0) and (read_3_reg_207 = ap_const_lv1_1) and (ap_enable_reg_pp0_iter1 = ap_const_logic_1)));
    end process;


    ap_block_pp0_stage1_11001_assign_proc : process(ap_enable_reg_pp0_iter0, read_reg_172, ap_enable_reg_pp0_iter1, read_3_reg_207, ap_block_state3_io, ap_block_state7_io, photons_V_TREADY_int_regslice)
    begin
                ap_block_pp0_stage1_11001 <= (((ap_enable_reg_pp0_iter0 = ap_const_logic_1) and ((ap_const_boolean_1 = ap_block_state3_io) or ((read_reg_172 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0)))) or ((ap_enable_reg_pp0_iter1 = ap_const_logic_1) and ((ap_const_boolean_1 = ap_block_state7_io) or ((photons_V_TREADY_int_regslice = ap_const_logic_0) and (read_3_reg_207 = ap_const_lv1_1)))));
    end process;


    ap_block_pp0_stage1_subdone_assign_proc : process(ap_enable_reg_pp0_iter0, read_reg_172, ap_enable_reg_pp0_iter1, read_3_reg_207, ap_block_state3_io, ap_block_state7_io, photons_V_TREADY_int_regslice)
    begin
                ap_block_pp0_stage1_subdone <= (((ap_enable_reg_pp0_iter0 = ap_const_logic_1) and ((ap_const_boolean_1 = ap_block_state3_io) or ((read_reg_172 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0)))) or ((ap_enable_reg_pp0_iter1 = ap_const_logic_1) and ((ap_const_boolean_1 = ap_block_state7_io) or ((photons_V_TREADY_int_regslice = ap_const_logic_0) and (read_3_reg_207 = ap_const_lv1_1)))));
    end process;

        ap_block_pp0_stage2 <= not((ap_const_boolean_1 = ap_const_boolean_1));

    ap_block_pp0_stage2_01001_assign_proc : process(ap_enable_reg_pp0_iter0, read_reg_172, read_1_reg_187, photons_V_TREADY_int_regslice)
    begin
                ap_block_pp0_stage2_01001 <= ((ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (((read_1_reg_187 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0)) or ((read_reg_172 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0))));
    end process;


    ap_block_pp0_stage2_11001_assign_proc : process(ap_enable_reg_pp0_iter0, read_reg_172, read_1_reg_187, ap_block_state4_io, photons_V_TREADY_int_regslice)
    begin
                ap_block_pp0_stage2_11001 <= ((ap_enable_reg_pp0_iter0 = ap_const_logic_1) and ((ap_const_boolean_1 = ap_block_state4_io) or ((read_1_reg_187 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0)) or ((read_reg_172 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0))));
    end process;


    ap_block_pp0_stage2_subdone_assign_proc : process(ap_enable_reg_pp0_iter0, read_reg_172, read_1_reg_187, ap_block_state4_io, photons_V_TREADY_int_regslice)
    begin
                ap_block_pp0_stage2_subdone <= ((ap_enable_reg_pp0_iter0 = ap_const_logic_1) and ((ap_const_boolean_1 = ap_block_state4_io) or ((read_1_reg_187 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0)) or ((read_reg_172 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0))));
    end process;

        ap_block_pp0_stage3 <= not((ap_const_boolean_1 = ap_const_boolean_1));

    ap_block_pp0_stage3_01001_assign_proc : process(ap_enable_reg_pp0_iter0, read_1_reg_187, read_2_reg_197, photons_V_TREADY_int_regslice)
    begin
                ap_block_pp0_stage3_01001 <= ((ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (((read_2_reg_197 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0)) or ((read_1_reg_187 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0))));
    end process;


    ap_block_pp0_stage3_11001_assign_proc : process(ap_enable_reg_pp0_iter0, read_1_reg_187, read_2_reg_197, ap_block_state5_io, photons_V_TREADY_int_regslice)
    begin
                ap_block_pp0_stage3_11001 <= ((ap_enable_reg_pp0_iter0 = ap_const_logic_1) and ((ap_const_boolean_1 = ap_block_state5_io) or ((read_2_reg_197 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0)) or ((read_1_reg_187 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0))));
    end process;


    ap_block_pp0_stage3_subdone_assign_proc : process(ap_enable_reg_pp0_iter0, read_1_reg_187, read_2_reg_197, ap_block_state5_io, photons_V_TREADY_int_regslice)
    begin
                ap_block_pp0_stage3_subdone <= ((ap_enable_reg_pp0_iter0 = ap_const_logic_1) and ((ap_const_boolean_1 = ap_block_state5_io) or ((read_2_reg_197 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0)) or ((read_1_reg_187 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0))));
    end process;


    ap_block_state1_assign_proc : process(ap_start, ap_done_reg)
    begin
                ap_block_state1 <= ((ap_done_reg = ap_const_logic_1) or (ap_start = ap_const_logic_0));
    end process;

        ap_block_state2_pp0_stage0_iter0 <= not((ap_const_boolean_1 = ap_const_boolean_1));

    ap_block_state3_io_assign_proc : process(read_reg_172, photons_V_TREADY_int_regslice)
    begin
                ap_block_state3_io <= ((read_reg_172 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0));
    end process;


    ap_block_state3_pp0_stage1_iter0_assign_proc : process(read_reg_172, photons_V_TREADY_int_regslice)
    begin
                ap_block_state3_pp0_stage1_iter0 <= ((read_reg_172 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0));
    end process;


    ap_block_state4_io_assign_proc : process(read_reg_172, read_1_reg_187, photons_V_TREADY_int_regslice)
    begin
                ap_block_state4_io <= (((read_1_reg_187 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0)) or ((read_reg_172 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0)));
    end process;


    ap_block_state4_pp0_stage2_iter0_assign_proc : process(read_reg_172, read_1_reg_187, photons_V_TREADY_int_regslice)
    begin
                ap_block_state4_pp0_stage2_iter0 <= (((read_1_reg_187 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0)) or ((read_reg_172 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0)));
    end process;


    ap_block_state5_io_assign_proc : process(read_1_reg_187, read_2_reg_197, photons_V_TREADY_int_regslice)
    begin
                ap_block_state5_io <= (((read_2_reg_197 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0)) or ((read_1_reg_187 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0)));
    end process;


    ap_block_state5_pp0_stage3_iter0_assign_proc : process(read_1_reg_187, read_2_reg_197, photons_V_TREADY_int_regslice)
    begin
                ap_block_state5_pp0_stage3_iter0 <= (((read_2_reg_197 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0)) or ((read_1_reg_187 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0)));
    end process;


    ap_block_state6_io_assign_proc : process(read_2_reg_197, read_3_reg_207, photons_V_TREADY_int_regslice)
    begin
                ap_block_state6_io <= (((read_2_reg_197 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0)) or ((photons_V_TREADY_int_regslice = ap_const_logic_0) and (read_3_reg_207 = ap_const_lv1_1)));
    end process;


    ap_block_state6_pp0_stage0_iter1_assign_proc : process(read_2_reg_197, read_3_reg_207, photons_V_TREADY_int_regslice)
    begin
                ap_block_state6_pp0_stage0_iter1 <= (((read_2_reg_197 = ap_const_lv1_1) and (photons_V_TREADY_int_regslice = ap_const_logic_0)) or ((photons_V_TREADY_int_regslice = ap_const_logic_0) and (read_3_reg_207 = ap_const_lv1_1)));
    end process;


    ap_block_state7_io_assign_proc : process(read_3_reg_207, photons_V_TREADY_int_regslice)
    begin
                ap_block_state7_io <= ((photons_V_TREADY_int_regslice = ap_const_logic_0) and (read_3_reg_207 = ap_const_lv1_1));
    end process;


    ap_block_state7_pp0_stage1_iter1_assign_proc : process(read_3_reg_207, photons_V_TREADY_int_regslice)
    begin
                ap_block_state7_pp0_stage1_iter1 <= ((photons_V_TREADY_int_regslice = ap_const_logic_0) and (read_3_reg_207 = ap_const_lv1_1));
    end process;

    ap_done <= ap_done_reg;
    ap_enable_pp0 <= (ap_idle_pp0 xor ap_const_logic_1);

    ap_idle_assign_proc : process(ap_start, ap_CS_fsm_state1)
    begin
        if (((ap_const_logic_1 = ap_CS_fsm_state1) and (ap_start = ap_const_logic_0))) then 
            ap_idle <= ap_const_logic_1;
        else 
            ap_idle <= ap_const_logic_0;
        end if; 
    end process;


    ap_idle_pp0_assign_proc : process(ap_enable_reg_pp0_iter0, ap_enable_reg_pp0_iter1)
    begin
        if (((ap_enable_reg_pp0_iter0 = ap_const_logic_0) and (ap_enable_reg_pp0_iter1 = ap_const_logic_0))) then 
            ap_idle_pp0 <= ap_const_logic_1;
        else 
            ap_idle_pp0 <= ap_const_logic_0;
        end if; 
    end process;

    ap_ready <= ap_const_logic_0;

    istrms1_read_assign_proc : process(istrms1_empty_n, ap_CS_fsm_pp0_stage1, ap_enable_reg_pp0_iter0, ap_block_pp0_stage1_11001)
    begin
        if (((ap_const_boolean_0 = ap_block_pp0_stage1_11001) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (istrms1_empty_n = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage1))) then 
            istrms1_read <= ap_const_logic_1;
        else 
            istrms1_read <= ap_const_logic_0;
        end if; 
    end process;

    istrms1_read_nbread_fu_63_p2_0 <= (0=>(istrms1_empty_n), others=>'-');

    istrms2_read_assign_proc : process(istrms2_empty_n, ap_enable_reg_pp0_iter0, ap_CS_fsm_pp0_stage2, ap_block_pp0_stage2_11001)
    begin
        if (((ap_const_boolean_0 = ap_block_pp0_stage2_11001) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (istrms2_empty_n = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage2))) then 
            istrms2_read <= ap_const_logic_1;
        else 
            istrms2_read <= ap_const_logic_0;
        end if; 
    end process;

    istrms2_read_nbread_fu_69_p2_0 <= (0=>(istrms2_empty_n), others=>'-');

    istrms3_read_assign_proc : process(istrms3_empty_n, ap_enable_reg_pp0_iter0, ap_CS_fsm_pp0_stage3, ap_block_pp0_stage3_11001)
    begin
        if (((ap_const_boolean_0 = ap_block_pp0_stage3_11001) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (istrms3_empty_n = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage3))) then 
            istrms3_read <= ap_const_logic_1;
        else 
            istrms3_read <= ap_const_logic_0;
        end if; 
    end process;

    istrms3_read_nbread_fu_75_p2_0 <= (0=>(istrms3_empty_n), others=>'-');

    istrms_read_assign_proc : process(istrms_empty_n, ap_enable_reg_pp0_iter0, ap_CS_fsm_pp0_stage0, ap_block_pp0_stage0_11001)
    begin
        if (((ap_const_boolean_0 = ap_block_pp0_stage0_11001) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (istrms_empty_n = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0))) then 
            istrms_read <= ap_const_logic_1;
        else 
            istrms_read <= ap_const_logic_0;
        end if; 
    end process;

    istrms_read_nbread_fu_50_p2_0 <= (0=>(istrms_empty_n), others=>'-');
    photon_2_fu_115_p3 <= 
        tmp_8_fu_104_p1 when (istrms1_read_nbread_fu_63_p2_0(0) = '1') else 
        select_ln164_fu_108_p3;
    photon_4_fu_135_p3 <= 
        tmp_9_fu_131_p1 when (istrms2_read_nbread_fu_69_p2_0(0) = '1') else 
        photon_2_reg_191;
    photon_6_fu_154_p3 <= 
        tmp_10_fu_150_p1 when (istrms3_read_nbread_fu_75_p2_0(0) = '1') else 
        photon_4_reg_201;

    photons_V_TDATA_blk_n_assign_proc : process(ap_CS_fsm_pp0_stage1, ap_enable_reg_pp0_iter0, ap_block_pp0_stage1, read_reg_172, ap_CS_fsm_pp0_stage2, ap_block_pp0_stage2, read_1_reg_187, ap_CS_fsm_pp0_stage3, ap_block_pp0_stage3, read_2_reg_197, ap_CS_fsm_pp0_stage0, ap_enable_reg_pp0_iter1, ap_block_pp0_stage0, read_3_reg_207, photons_V_TREADY_int_regslice)
    begin
        if ((((read_2_reg_197 = ap_const_lv1_1) and (ap_const_boolean_0 = ap_block_pp0_stage3) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage3)) or ((read_2_reg_197 = ap_const_lv1_1) and (ap_const_boolean_0 = ap_block_pp0_stage0) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0) and (ap_enable_reg_pp0_iter1 = ap_const_logic_1)) or ((read_1_reg_187 = ap_const_lv1_1) and (ap_const_boolean_0 = ap_block_pp0_stage3) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage3)) or ((read_1_reg_187 = ap_const_lv1_1) and (ap_const_boolean_0 = ap_block_pp0_stage2) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage2)) or ((read_reg_172 = ap_const_lv1_1) and (ap_const_boolean_0 = ap_block_pp0_stage2) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage2)) or ((read_reg_172 = ap_const_lv1_1) and (ap_const_boolean_0 = ap_block_pp0_stage1) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage1)) or ((ap_const_boolean_0 = ap_block_pp0_stage1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage1) and (read_3_reg_207 = ap_const_lv1_1) and (ap_enable_reg_pp0_iter1 = ap_const_logic_1)) or ((ap_const_boolean_0 = ap_block_pp0_stage0) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0) and (read_3_reg_207 = ap_const_lv1_1) and (ap_enable_reg_pp0_iter1 = ap_const_logic_1)))) then 
            photons_V_TDATA_blk_n <= photons_V_TREADY_int_regslice;
        else 
            photons_V_TDATA_blk_n <= ap_const_logic_1;
        end if; 
    end process;


    photons_V_TDATA_int_regslice_assign_proc : process(ap_CS_fsm_pp0_stage1, ap_enable_reg_pp0_iter0, read_reg_172, ap_CS_fsm_pp0_stage2, read_1_reg_187, ap_CS_fsm_pp0_stage3, read_2_reg_197, ap_CS_fsm_pp0_stage0, ap_enable_reg_pp0_iter1, read_3_reg_207, tmp_fu_89_p1, photon_2_reg_191, photon_4_reg_201, photon_6_reg_211, ap_block_pp0_stage1_01001, ap_block_pp0_stage2_01001, ap_block_pp0_stage3_01001, ap_block_pp0_stage0_01001)
    begin
        if (((ap_const_boolean_0 = ap_block_pp0_stage0_01001) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0) and (read_3_reg_207 = ap_const_lv1_1) and (ap_enable_reg_pp0_iter1 = ap_const_logic_1))) then 
            photons_V_TDATA_int_regslice <= photon_6_reg_211;
        elsif (((read_2_reg_197 = ap_const_lv1_1) and (ap_const_boolean_0 = ap_block_pp0_stage3_01001) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage3))) then 
            photons_V_TDATA_int_regslice <= photon_4_reg_201;
        elsif (((read_1_reg_187 = ap_const_lv1_1) and (ap_const_boolean_0 = ap_block_pp0_stage2_01001) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage2))) then 
            photons_V_TDATA_int_regslice <= photon_2_reg_191;
        elsif (((read_reg_172 = ap_const_lv1_1) and (ap_const_boolean_0 = ap_block_pp0_stage1_01001) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage1))) then 
            photons_V_TDATA_int_regslice <= tmp_fu_89_p1;
        else 
            photons_V_TDATA_int_regslice <= "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
        end if; 
    end process;

    photons_V_TVALID <= regslice_both_photons_V_U_vld_out;

    photons_V_TVALID_int_regslice_assign_proc : process(ap_CS_fsm_pp0_stage1, ap_enable_reg_pp0_iter0, read_reg_172, ap_CS_fsm_pp0_stage2, read_1_reg_187, ap_CS_fsm_pp0_stage3, read_2_reg_197, ap_CS_fsm_pp0_stage0, ap_enable_reg_pp0_iter1, read_3_reg_207, ap_block_pp0_stage0_11001, ap_block_pp0_stage1_11001, ap_block_pp0_stage2_11001, ap_block_pp0_stage3_11001)
    begin
        if ((((read_2_reg_197 = ap_const_lv1_1) and (ap_const_boolean_0 = ap_block_pp0_stage3_11001) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage3)) or ((read_1_reg_187 = ap_const_lv1_1) and (ap_const_boolean_0 = ap_block_pp0_stage2_11001) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage2)) or ((read_reg_172 = ap_const_lv1_1) and (ap_const_boolean_0 = ap_block_pp0_stage1_11001) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage1)) or ((ap_const_boolean_0 = ap_block_pp0_stage0_11001) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0) and (read_3_reg_207 = ap_const_lv1_1) and (ap_enable_reg_pp0_iter1 = ap_const_logic_1)))) then 
            photons_V_TVALID_int_regslice <= ap_const_logic_1;
        else 
            photons_V_TVALID_int_regslice <= ap_const_logic_0;
        end if; 
    end process;

    select_ln164_fu_108_p3 <= 
        tmp_fu_89_p1 when (read_reg_172(0) = '1') else 
        photon_1_fu_46;
    tmp_10_fu_150_p1 <= std_logic_vector(IEEE.numeric_std.resize(unsigned(istrms3_dout),48));
    tmp_8_fu_104_p1 <= std_logic_vector(IEEE.numeric_std.resize(unsigned(istrms1_dout),48));
    tmp_9_fu_131_p1 <= std_logic_vector(IEEE.numeric_std.resize(unsigned(istrms2_dout),48));
    tmp_fu_89_p1 <= std_logic_vector(IEEE.numeric_std.resize(unsigned(tmp_1_reg_177),48));
end behav;