-- ==============================================================
-- RTL generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and OpenCL
-- Version: 2019.2.1
-- Copyright (C) 1986-2019 Xilinx, Inc. All Rights Reserved.
-- 
-- ===========================================================

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity update_triggers is
port (
    ap_clk : IN STD_LOGIC;
    ap_rst : IN STD_LOGIC;
    data_0_phase_V_read : IN STD_LOGIC_VECTOR (15 downto 0);
    data_1_phase_V_read : IN STD_LOGIC_VECTOR (15 downto 0);
    data_2_phase_V_read : IN STD_LOGIC_VECTOR (15 downto 0);
    data_3_phase_V_read : IN STD_LOGIC_VECTOR (15 downto 0);
    data_0_baseline_V_r : IN STD_LOGIC_VECTOR (15 downto 0);
    data_1_baseline_V_r : IN STD_LOGIC_VECTOR (15 downto 0);
    data_2_baseline_V_r : IN STD_LOGIC_VECTOR (15 downto 0);
    data_3_baseline_V_r : IN STD_LOGIC_VECTOR (15 downto 0);
    trig_cfg_0_threshol : IN STD_LOGIC_VECTOR (15 downto 0);
    trig_cfg_1_threshol : IN STD_LOGIC_VECTOR (15 downto 0);
    trig_cfg_2_threshol : IN STD_LOGIC_VECTOR (15 downto 0);
    trig_cfg_3_threshol : IN STD_LOGIC_VECTOR (15 downto 0);
    ap_return_0 : OUT STD_LOGIC_VECTOR (15 downto 0);
    ap_return_1 : OUT STD_LOGIC_VECTOR (15 downto 0);
    ap_return_2 : OUT STD_LOGIC_VECTOR (15 downto 0);
    ap_return_3 : OUT STD_LOGIC_VECTOR (15 downto 0);
    ap_return_4 : OUT STD_LOGIC_VECTOR (15 downto 0);
    ap_return_5 : OUT STD_LOGIC_VECTOR (15 downto 0);
    ap_return_6 : OUT STD_LOGIC_VECTOR (15 downto 0);
    ap_return_7 : OUT STD_LOGIC_VECTOR (15 downto 0);
    ap_return_8 : OUT STD_LOGIC_VECTOR (0 downto 0);
    ap_return_9 : OUT STD_LOGIC_VECTOR (0 downto 0);
    ap_return_10 : OUT STD_LOGIC_VECTOR (0 downto 0);
    ap_return_11 : OUT STD_LOGIC_VECTOR (0 downto 0) );
end;


architecture behav of update_triggers is 
    constant ap_const_boolean_1 : BOOLEAN := true;
    constant ap_const_boolean_0 : BOOLEAN := false;
    constant ap_const_logic_1 : STD_LOGIC := '1';
    constant ap_const_logic_0 : STD_LOGIC := '0';

    signal trig_cfg_3_threshol_1_reg_212 : STD_LOGIC_VECTOR (15 downto 0);
    signal ap_block_state1_pp0_stage0_iter0 : BOOLEAN;
    signal ap_block_state2_pp0_stage0_iter1 : BOOLEAN;
    signal ap_block_pp0_stage0_11001 : BOOLEAN;
    signal trig_cfg_2_threshol_1_reg_217 : STD_LOGIC_VECTOR (15 downto 0);
    signal trig_cfg_1_threshol_1_reg_222 : STD_LOGIC_VECTOR (15 downto 0);
    signal trig_cfg_0_threshol_1_reg_227 : STD_LOGIC_VECTOR (15 downto 0);
    signal data_3_baseline_V_r_1_reg_232 : STD_LOGIC_VECTOR (15 downto 0);
    signal data_2_baseline_V_r_1_reg_237 : STD_LOGIC_VECTOR (15 downto 0);
    signal data_1_baseline_V_r_1_reg_242 : STD_LOGIC_VECTOR (15 downto 0);
    signal data_0_baseline_V_r_1_reg_247 : STD_LOGIC_VECTOR (15 downto 0);
    signal data_3_phase_V_read_1_reg_252 : STD_LOGIC_VECTOR (15 downto 0);
    signal data_2_phase_V_read_1_reg_257 : STD_LOGIC_VECTOR (15 downto 0);
    signal data_1_phase_V_read_1_reg_262 : STD_LOGIC_VECTOR (15 downto 0);
    signal data_0_phase_V_read_1_reg_267 : STD_LOGIC_VECTOR (15 downto 0);
    signal sub_ln703_fu_108_p2 : STD_LOGIC_VECTOR (15 downto 0);
    signal sub_ln703_reg_272 : STD_LOGIC_VECTOR (15 downto 0);
    signal sub_ln703_1_fu_114_p2 : STD_LOGIC_VECTOR (15 downto 0);
    signal sub_ln703_1_reg_277 : STD_LOGIC_VECTOR (15 downto 0);
    signal sub_ln703_2_fu_120_p2 : STD_LOGIC_VECTOR (15 downto 0);
    signal sub_ln703_2_reg_282 : STD_LOGIC_VECTOR (15 downto 0);
    signal sub_ln703_3_fu_126_p2 : STD_LOGIC_VECTOR (15 downto 0);
    signal sub_ln703_3_reg_287 : STD_LOGIC_VECTOR (15 downto 0);
    signal ap_block_pp0_stage0 : BOOLEAN;
    signal icmp_ln1495_fu_132_p2 : STD_LOGIC_VECTOR (0 downto 0);
    signal icmp_ln1495_1_fu_136_p2 : STD_LOGIC_VECTOR (0 downto 0);
    signal icmp_ln1495_2_fu_140_p2 : STD_LOGIC_VECTOR (0 downto 0);
    signal icmp_ln1495_3_fu_144_p2 : STD_LOGIC_VECTOR (0 downto 0);


begin



    process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if ((ap_const_boolean_0 = ap_block_pp0_stage0_11001)) then
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
            end if;
        end if;
    end process;
        ap_block_pp0_stage0 <= not((ap_const_boolean_1 = ap_const_boolean_1));
        ap_block_pp0_stage0_11001 <= not((ap_const_boolean_1 = ap_const_boolean_1));
        ap_block_state1_pp0_stage0_iter0 <= not((ap_const_boolean_1 = ap_const_boolean_1));
        ap_block_state2_pp0_stage0_iter1 <= not((ap_const_boolean_1 = ap_const_boolean_1));
    ap_return_0 <= data_0_phase_V_read_1_reg_267;
    ap_return_1 <= data_1_phase_V_read_1_reg_262;
    ap_return_10 <= icmp_ln1495_2_fu_140_p2;
    ap_return_11 <= icmp_ln1495_3_fu_144_p2;
    ap_return_2 <= data_2_phase_V_read_1_reg_257;
    ap_return_3 <= data_3_phase_V_read_1_reg_252;
    ap_return_4 <= data_0_baseline_V_r_1_reg_247;
    ap_return_5 <= data_1_baseline_V_r_1_reg_242;
    ap_return_6 <= data_2_baseline_V_r_1_reg_237;
    ap_return_7 <= data_3_baseline_V_r_1_reg_232;
    ap_return_8 <= icmp_ln1495_fu_132_p2;
    ap_return_9 <= icmp_ln1495_1_fu_136_p2;
    icmp_ln1495_1_fu_136_p2 <= "1" when (signed(sub_ln703_1_reg_277) < signed(trig_cfg_1_threshol_1_reg_222)) else "0";
    icmp_ln1495_2_fu_140_p2 <= "1" when (signed(sub_ln703_2_reg_282) < signed(trig_cfg_2_threshol_1_reg_217)) else "0";
    icmp_ln1495_3_fu_144_p2 <= "1" when (signed(sub_ln703_3_reg_287) < signed(trig_cfg_3_threshol_1_reg_212)) else "0";
    icmp_ln1495_fu_132_p2 <= "1" when (signed(sub_ln703_reg_272) < signed(trig_cfg_0_threshol_1_reg_227)) else "0";
    sub_ln703_1_fu_114_p2 <= std_logic_vector(unsigned(data_1_phase_V_read) - unsigned(data_1_baseline_V_r));
    sub_ln703_2_fu_120_p2 <= std_logic_vector(unsigned(data_2_phase_V_read) - unsigned(data_2_baseline_V_r));
    sub_ln703_3_fu_126_p2 <= std_logic_vector(unsigned(data_3_phase_V_read) - unsigned(data_3_baseline_V_r));
    sub_ln703_fu_108_p2 <= std_logic_vector(unsigned(data_0_phase_V_read) - unsigned(data_0_baseline_V_r));
end behav;