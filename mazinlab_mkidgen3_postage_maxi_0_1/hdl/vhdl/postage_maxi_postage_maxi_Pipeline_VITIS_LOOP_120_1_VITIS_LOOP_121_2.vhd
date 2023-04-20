-- ==============================================================
-- RTL generated by Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
-- Version: 2022.1
-- Copyright (C) Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- 
-- ===========================================================

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity postage_maxi_postage_maxi_Pipeline_VITIS_LOOP_120_1_VITIS_LOOP_121_2 is
port (
    ap_clk : IN STD_LOGIC;
    ap_rst : IN STD_LOGIC;
    ap_start : IN STD_LOGIC;
    ap_done : OUT STD_LOGIC;
    ap_idle : OUT STD_LOGIC;
    ap_ready : OUT STD_LOGIC;
    buf_r_address0 : OUT STD_LOGIC_VECTOR (9 downto 0);
    buf_r_ce0 : OUT STD_LOGIC;
    buf_r_we0 : OUT STD_LOGIC;
    buf_r_d0 : OUT STD_LOGIC_VECTOR (31 downto 0);
    sample_count_address0 : OUT STD_LOGIC_VECTOR (2 downto 0);
    sample_count_ce0 : OUT STD_LOGIC;
    sample_count_we0 : OUT STD_LOGIC;
    sample_count_d0 : OUT STD_LOGIC_VECTOR (7 downto 0);
    p_event_count_address0 : OUT STD_LOGIC_VECTOR (2 downto 0);
    p_event_count_ce0 : OUT STD_LOGIC;
    p_event_count_we0 : OUT STD_LOGIC;
    p_event_count_d0 : OUT STD_LOGIC_VECTOR (15 downto 0) );
end;


architecture behav of postage_maxi_postage_maxi_Pipeline_VITIS_LOOP_120_1_VITIS_LOOP_121_2 is 
    constant ap_const_logic_1 : STD_LOGIC := '1';
    constant ap_const_logic_0 : STD_LOGIC := '0';
    constant ap_ST_fsm_pp0_stage0 : STD_LOGIC_VECTOR (0 downto 0) := "1";
    constant ap_const_lv32_0 : STD_LOGIC_VECTOR (31 downto 0) := "00000000000000000000000000000000";
    constant ap_const_boolean_1 : BOOLEAN := true;
    constant ap_const_boolean_0 : BOOLEAN := false;
    constant ap_const_lv1_1 : STD_LOGIC_VECTOR (0 downto 0) := "1";
    constant ap_const_lv1_0 : STD_LOGIC_VECTOR (0 downto 0) := "0";
    constant ap_const_lv7_0 : STD_LOGIC_VECTOR (6 downto 0) := "0000000";
    constant ap_const_lv4_0 : STD_LOGIC_VECTOR (3 downto 0) := "0000";
    constant ap_const_lv10_0 : STD_LOGIC_VECTOR (9 downto 0) := "0000000000";
    constant ap_const_lv8_0 : STD_LOGIC_VECTOR (7 downto 0) := "00000000";
    constant ap_const_lv16_0 : STD_LOGIC_VECTOR (15 downto 0) := "0000000000000000";
    constant ap_const_lv10_2D0 : STD_LOGIC_VECTOR (9 downto 0) := "1011010000";
    constant ap_const_lv10_1 : STD_LOGIC_VECTOR (9 downto 0) := "0000000001";
    constant ap_const_lv4_1 : STD_LOGIC_VECTOR (3 downto 0) := "0001";
    constant ap_const_lv7_5A : STD_LOGIC_VECTOR (6 downto 0) := "1011010";
    constant ap_const_lv7_1 : STD_LOGIC_VECTOR (6 downto 0) := "0000001";
    constant ap_const_lv10_5A : STD_LOGIC_VECTOR (9 downto 0) := "0001011010";

attribute shreg_extract : string;
    signal ap_CS_fsm : STD_LOGIC_VECTOR (0 downto 0) := "1";
    attribute fsm_encoding : string;
    attribute fsm_encoding of ap_CS_fsm : signal is "none";
    signal ap_CS_fsm_pp0_stage0 : STD_LOGIC;
    attribute fsm_encoding of ap_CS_fsm_pp0_stage0 : signal is "none";
    signal ap_enable_reg_pp0_iter0 : STD_LOGIC;
    signal ap_enable_reg_pp0_iter1 : STD_LOGIC := '0';
    signal ap_enable_reg_pp0_iter2 : STD_LOGIC := '0';
    signal ap_enable_reg_pp0_iter3 : STD_LOGIC := '0';
    signal ap_idle_pp0 : STD_LOGIC;
    signal ap_block_state1_pp0_stage0_iter0 : BOOLEAN;
    signal ap_block_state2_pp0_stage0_iter1 : BOOLEAN;
    signal ap_block_state3_pp0_stage0_iter2 : BOOLEAN;
    signal ap_block_state4_pp0_stage0_iter3 : BOOLEAN;
    signal ap_block_pp0_stage0_subdone : BOOLEAN;
    signal icmp_ln120_fu_122_p2 : STD_LOGIC_VECTOR (0 downto 0);
    signal ap_condition_exit_pp0_iter0_stage0 : STD_LOGIC;
    signal ap_loop_exit_ready : STD_LOGIC;
    signal ap_ready_int : STD_LOGIC;
    signal ap_block_pp0_stage0_11001 : BOOLEAN;
    signal select_ln120_fu_152_p3 : STD_LOGIC_VECTOR (6 downto 0);
    signal select_ln120_reg_245 : STD_LOGIC_VECTOR (6 downto 0);
    signal select_ln120_reg_245_pp0_iter1_reg : STD_LOGIC_VECTOR (6 downto 0);
    signal select_ln120_1_fu_160_p3 : STD_LOGIC_VECTOR (3 downto 0);
    signal select_ln120_1_reg_250 : STD_LOGIC_VECTOR (3 downto 0);
    signal ifzero_fu_178_p2 : STD_LOGIC_VECTOR (0 downto 0);
    signal ifzero_reg_260 : STD_LOGIC_VECTOR (0 downto 0);
    signal zext_ln120_fu_199_p1 : STD_LOGIC_VECTOR (63 downto 0);
    signal ap_block_pp0_stage0 : BOOLEAN;
    signal zext_ln121_1_fu_207_p1 : STD_LOGIC_VECTOR (63 downto 0);
    signal j_fu_50 : STD_LOGIC_VECTOR (6 downto 0);
    signal add_ln121_fu_172_p2 : STD_LOGIC_VECTOR (6 downto 0);
    signal ap_loop_init : STD_LOGIC;
    signal ap_sig_allocacmp_j_load : STD_LOGIC_VECTOR (6 downto 0);
    signal i_fu_54 : STD_LOGIC_VECTOR (3 downto 0);
    signal ap_sig_allocacmp_i_load : STD_LOGIC_VECTOR (3 downto 0);
    signal indvar_flatten_fu_58 : STD_LOGIC_VECTOR (9 downto 0);
    signal add_ln120_1_fu_128_p2 : STD_LOGIC_VECTOR (9 downto 0);
    signal ap_sig_allocacmp_indvar_flatten_load : STD_LOGIC_VECTOR (9 downto 0);
    signal icmp_ln121_fu_146_p2 : STD_LOGIC_VECTOR (0 downto 0);
    signal add_ln120_fu_140_p2 : STD_LOGIC_VECTOR (3 downto 0);
    signal grp_fu_211_p3 : STD_LOGIC_VECTOR (9 downto 0);
    signal grp_fu_211_p0 : STD_LOGIC_VECTOR (3 downto 0);
    signal grp_fu_211_p1 : STD_LOGIC_VECTOR (6 downto 0);
    signal grp_fu_211_p2 : STD_LOGIC_VECTOR (6 downto 0);
    signal ap_done_reg : STD_LOGIC := '0';
    signal ap_continue_int : STD_LOGIC;
    signal ap_done_int : STD_LOGIC;
    signal ap_loop_exit_ready_pp0_iter1_reg : STD_LOGIC;
    signal ap_loop_exit_ready_pp0_iter2_reg : STD_LOGIC;
    signal ap_NS_fsm : STD_LOGIC_VECTOR (0 downto 0);
    signal ap_enable_pp0 : STD_LOGIC;
    signal ap_start_int : STD_LOGIC;
    signal grp_fu_211_p00 : STD_LOGIC_VECTOR (9 downto 0);
    signal grp_fu_211_p20 : STD_LOGIC_VECTOR (9 downto 0);
    signal ap_ce_reg : STD_LOGIC;

    component postage_maxi_mac_muladd_4ns_7ns_7ns_10_4_1 IS
    generic (
        ID : INTEGER;
        NUM_STAGE : INTEGER;
        din0_WIDTH : INTEGER;
        din1_WIDTH : INTEGER;
        din2_WIDTH : INTEGER;
        dout_WIDTH : INTEGER );
    port (
        clk : IN STD_LOGIC;
        reset : IN STD_LOGIC;
        din0 : IN STD_LOGIC_VECTOR (3 downto 0);
        din1 : IN STD_LOGIC_VECTOR (6 downto 0);
        din2 : IN STD_LOGIC_VECTOR (6 downto 0);
        ce : IN STD_LOGIC;
        dout : OUT STD_LOGIC_VECTOR (9 downto 0) );
    end component;


    component postage_maxi_flow_control_loop_pipe_sequential_init IS
    port (
        ap_clk : IN STD_LOGIC;
        ap_rst : IN STD_LOGIC;
        ap_start : IN STD_LOGIC;
        ap_ready : OUT STD_LOGIC;
        ap_done : OUT STD_LOGIC;
        ap_start_int : OUT STD_LOGIC;
        ap_loop_init : OUT STD_LOGIC;
        ap_ready_int : IN STD_LOGIC;
        ap_loop_exit_ready : IN STD_LOGIC;
        ap_loop_exit_done : IN STD_LOGIC;
        ap_continue_int : OUT STD_LOGIC;
        ap_done_int : IN STD_LOGIC );
    end component;



begin
    mac_muladd_4ns_7ns_7ns_10_4_1_U1 : component postage_maxi_mac_muladd_4ns_7ns_7ns_10_4_1
    generic map (
        ID => 1,
        NUM_STAGE => 4,
        din0_WIDTH => 4,
        din1_WIDTH => 7,
        din2_WIDTH => 7,
        dout_WIDTH => 10)
    port map (
        clk => ap_clk,
        reset => ap_rst,
        din0 => grp_fu_211_p0,
        din1 => grp_fu_211_p1,
        din2 => grp_fu_211_p2,
        ce => ap_const_logic_1,
        dout => grp_fu_211_p3);

    flow_control_loop_pipe_sequential_init_U : component postage_maxi_flow_control_loop_pipe_sequential_init
    port map (
        ap_clk => ap_clk,
        ap_rst => ap_rst,
        ap_start => ap_start,
        ap_ready => ap_ready,
        ap_done => ap_done,
        ap_start_int => ap_start_int,
        ap_loop_init => ap_loop_init,
        ap_ready_int => ap_ready_int,
        ap_loop_exit_ready => ap_condition_exit_pp0_iter0_stage0,
        ap_loop_exit_done => ap_done_int,
        ap_continue_int => ap_continue_int,
        ap_done_int => ap_done_int);





    ap_CS_fsm_assign_proc : process(ap_clk)
    begin
        if (ap_clk'event and ap_clk =  '1') then
            if (ap_rst = '1') then
                ap_CS_fsm <= ap_ST_fsm_pp0_stage0;
            else
                ap_CS_fsm <= ap_NS_fsm;
            end if;
        end if;
    end process;


    ap_done_reg_assign_proc : process(ap_clk)
    begin
        if (ap_clk'event and ap_clk =  '1') then
            if (ap_rst = '1') then
                ap_done_reg <= ap_const_logic_0;
            else
                if ((ap_continue_int = ap_const_logic_1)) then 
                    ap_done_reg <= ap_const_logic_0;
                elsif (((ap_const_boolean_0 = ap_block_pp0_stage0_subdone) and (ap_loop_exit_ready_pp0_iter2_reg = ap_const_logic_1))) then 
                    ap_done_reg <= ap_const_logic_1;
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
                if ((ap_const_logic_1 = ap_condition_exit_pp0_iter0_stage0)) then 
                    ap_enable_reg_pp0_iter1 <= ap_const_logic_0;
                elsif (((ap_const_boolean_0 = ap_block_pp0_stage0_subdone) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0))) then 
                    ap_enable_reg_pp0_iter1 <= ap_start_int;
                end if; 
            end if;
        end if;
    end process;


    ap_enable_reg_pp0_iter2_assign_proc : process(ap_clk)
    begin
        if (ap_clk'event and ap_clk =  '1') then
            if (ap_rst = '1') then
                ap_enable_reg_pp0_iter2 <= ap_const_logic_0;
            else
                if ((ap_const_boolean_0 = ap_block_pp0_stage0_subdone)) then 
                    ap_enable_reg_pp0_iter2 <= ap_enable_reg_pp0_iter1;
                end if; 
            end if;
        end if;
    end process;


    ap_enable_reg_pp0_iter3_assign_proc : process(ap_clk)
    begin
        if (ap_clk'event and ap_clk =  '1') then
            if (ap_rst = '1') then
                ap_enable_reg_pp0_iter3 <= ap_const_logic_0;
            else
                if ((ap_const_boolean_0 = ap_block_pp0_stage0_subdone)) then 
                    ap_enable_reg_pp0_iter3 <= ap_enable_reg_pp0_iter2;
                end if; 
            end if;
        end if;
    end process;


    i_fu_54_assign_proc : process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if (((ap_const_boolean_0 = ap_block_pp0_stage0_11001) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0))) then
                if (((icmp_ln120_fu_122_p2 = ap_const_lv1_0) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1))) then 
                    i_fu_54 <= select_ln120_1_fu_160_p3;
                elsif ((ap_loop_init = ap_const_logic_1)) then 
                    i_fu_54 <= ap_const_lv4_0;
                end if;
            end if; 
        end if;
    end process;

    indvar_flatten_fu_58_assign_proc : process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if (((ap_const_boolean_0 = ap_block_pp0_stage0_11001) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0))) then
                if (((icmp_ln120_fu_122_p2 = ap_const_lv1_0) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1))) then 
                    indvar_flatten_fu_58 <= add_ln120_1_fu_128_p2;
                elsif ((ap_loop_init = ap_const_logic_1)) then 
                    indvar_flatten_fu_58 <= ap_const_lv10_0;
                end if;
            end if; 
        end if;
    end process;

    j_fu_50_assign_proc : process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if (((ap_const_boolean_0 = ap_block_pp0_stage0_11001) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0))) then
                if (((icmp_ln120_fu_122_p2 = ap_const_lv1_0) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1))) then 
                    j_fu_50 <= add_ln121_fu_172_p2;
                elsif ((ap_loop_init = ap_const_logic_1)) then 
                    j_fu_50 <= ap_const_lv7_0;
                end if;
            end if; 
        end if;
    end process;
    process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if (((ap_const_boolean_0 = ap_block_pp0_stage0_11001) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0))) then
                ap_loop_exit_ready_pp0_iter1_reg <= ap_loop_exit_ready;
                ap_loop_exit_ready_pp0_iter2_reg <= ap_loop_exit_ready_pp0_iter1_reg;
                select_ln120_reg_245_pp0_iter1_reg <= select_ln120_reg_245;
            end if;
        end if;
    end process;
    process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if (((icmp_ln120_fu_122_p2 = ap_const_lv1_0) and (ap_const_boolean_0 = ap_block_pp0_stage0_11001) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0))) then
                ifzero_reg_260 <= ifzero_fu_178_p2;
                select_ln120_1_reg_250 <= select_ln120_1_fu_160_p3;
                select_ln120_reg_245 <= select_ln120_fu_152_p3;
            end if;
        end if;
    end process;

    ap_NS_fsm_assign_proc : process (ap_CS_fsm)
    begin
        case ap_CS_fsm is
            when ap_ST_fsm_pp0_stage0 => 
                ap_NS_fsm <= ap_ST_fsm_pp0_stage0;
            when others =>  
                ap_NS_fsm <= "X";
        end case;
    end process;
    add_ln120_1_fu_128_p2 <= std_logic_vector(unsigned(ap_sig_allocacmp_indvar_flatten_load) + unsigned(ap_const_lv10_1));
    add_ln120_fu_140_p2 <= std_logic_vector(unsigned(ap_sig_allocacmp_i_load) + unsigned(ap_const_lv4_1));
    add_ln121_fu_172_p2 <= std_logic_vector(unsigned(select_ln120_fu_152_p3) + unsigned(ap_const_lv7_1));
    ap_CS_fsm_pp0_stage0 <= ap_CS_fsm(0);
        ap_block_pp0_stage0 <= not((ap_const_boolean_1 = ap_const_boolean_1));
        ap_block_pp0_stage0_11001 <= not((ap_const_boolean_1 = ap_const_boolean_1));
        ap_block_pp0_stage0_subdone <= not((ap_const_boolean_1 = ap_const_boolean_1));
        ap_block_state1_pp0_stage0_iter0 <= not((ap_const_boolean_1 = ap_const_boolean_1));
        ap_block_state2_pp0_stage0_iter1 <= not((ap_const_boolean_1 = ap_const_boolean_1));
        ap_block_state3_pp0_stage0_iter2 <= not((ap_const_boolean_1 = ap_const_boolean_1));
        ap_block_state4_pp0_stage0_iter3 <= not((ap_const_boolean_1 = ap_const_boolean_1));

    ap_condition_exit_pp0_iter0_stage0_assign_proc : process(ap_CS_fsm_pp0_stage0, ap_enable_reg_pp0_iter0, ap_block_pp0_stage0_subdone, icmp_ln120_fu_122_p2)
    begin
        if (((icmp_ln120_fu_122_p2 = ap_const_lv1_1) and (ap_const_boolean_0 = ap_block_pp0_stage0_subdone) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0))) then 
            ap_condition_exit_pp0_iter0_stage0 <= ap_const_logic_1;
        else 
            ap_condition_exit_pp0_iter0_stage0 <= ap_const_logic_0;
        end if; 
    end process;


    ap_done_int_assign_proc : process(ap_block_pp0_stage0_subdone, ap_done_reg, ap_loop_exit_ready_pp0_iter2_reg)
    begin
        if (((ap_const_boolean_0 = ap_block_pp0_stage0_subdone) and (ap_loop_exit_ready_pp0_iter2_reg = ap_const_logic_1))) then 
            ap_done_int <= ap_const_logic_1;
        else 
            ap_done_int <= ap_done_reg;
        end if; 
    end process;

    ap_enable_pp0 <= (ap_idle_pp0 xor ap_const_logic_1);
    ap_enable_reg_pp0_iter0 <= ap_start_int;

    ap_idle_assign_proc : process(ap_CS_fsm_pp0_stage0, ap_idle_pp0, ap_start_int)
    begin
        if (((ap_idle_pp0 = ap_const_logic_1) and (ap_start_int = ap_const_logic_0) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0))) then 
            ap_idle <= ap_const_logic_1;
        else 
            ap_idle <= ap_const_logic_0;
        end if; 
    end process;


    ap_idle_pp0_assign_proc : process(ap_enable_reg_pp0_iter0, ap_enable_reg_pp0_iter1, ap_enable_reg_pp0_iter2, ap_enable_reg_pp0_iter3)
    begin
        if (((ap_enable_reg_pp0_iter3 = ap_const_logic_0) and (ap_enable_reg_pp0_iter2 = ap_const_logic_0) and (ap_enable_reg_pp0_iter1 = ap_const_logic_0) and (ap_enable_reg_pp0_iter0 = ap_const_logic_0))) then 
            ap_idle_pp0 <= ap_const_logic_1;
        else 
            ap_idle_pp0 <= ap_const_logic_0;
        end if; 
    end process;

    ap_loop_exit_ready <= ap_condition_exit_pp0_iter0_stage0;

    ap_ready_int_assign_proc : process(ap_CS_fsm_pp0_stage0, ap_enable_reg_pp0_iter0, ap_block_pp0_stage0_subdone)
    begin
        if (((ap_const_boolean_0 = ap_block_pp0_stage0_subdone) and (ap_enable_reg_pp0_iter0 = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0))) then 
            ap_ready_int <= ap_const_logic_1;
        else 
            ap_ready_int <= ap_const_logic_0;
        end if; 
    end process;


    ap_sig_allocacmp_i_load_assign_proc : process(ap_CS_fsm_pp0_stage0, ap_block_pp0_stage0, ap_loop_init, i_fu_54)
    begin
        if (((ap_const_boolean_0 = ap_block_pp0_stage0) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0) and (ap_loop_init = ap_const_logic_1))) then 
            ap_sig_allocacmp_i_load <= ap_const_lv4_0;
        else 
            ap_sig_allocacmp_i_load <= i_fu_54;
        end if; 
    end process;


    ap_sig_allocacmp_indvar_flatten_load_assign_proc : process(ap_CS_fsm_pp0_stage0, ap_block_pp0_stage0, ap_loop_init, indvar_flatten_fu_58)
    begin
        if (((ap_const_boolean_0 = ap_block_pp0_stage0) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0) and (ap_loop_init = ap_const_logic_1))) then 
            ap_sig_allocacmp_indvar_flatten_load <= ap_const_lv10_0;
        else 
            ap_sig_allocacmp_indvar_flatten_load <= indvar_flatten_fu_58;
        end if; 
    end process;


    ap_sig_allocacmp_j_load_assign_proc : process(ap_CS_fsm_pp0_stage0, ap_block_pp0_stage0, j_fu_50, ap_loop_init)
    begin
        if (((ap_const_boolean_0 = ap_block_pp0_stage0) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0) and (ap_loop_init = ap_const_logic_1))) then 
            ap_sig_allocacmp_j_load <= ap_const_lv7_0;
        else 
            ap_sig_allocacmp_j_load <= j_fu_50;
        end if; 
    end process;

    buf_r_address0 <= zext_ln121_1_fu_207_p1(10 - 1 downto 0);

    buf_r_ce0_assign_proc : process(ap_enable_reg_pp0_iter3, ap_block_pp0_stage0_11001)
    begin
        if (((ap_const_boolean_0 = ap_block_pp0_stage0_11001) and (ap_enable_reg_pp0_iter3 = ap_const_logic_1))) then 
            buf_r_ce0 <= ap_const_logic_1;
        else 
            buf_r_ce0 <= ap_const_logic_0;
        end if; 
    end process;

    buf_r_d0 <= ap_const_lv32_0;

    buf_r_we0_assign_proc : process(ap_enable_reg_pp0_iter3, ap_block_pp0_stage0_11001)
    begin
        if (((ap_const_boolean_0 = ap_block_pp0_stage0_11001) and (ap_enable_reg_pp0_iter3 = ap_const_logic_1))) then 
            buf_r_we0 <= ap_const_logic_1;
        else 
            buf_r_we0 <= ap_const_logic_0;
        end if; 
    end process;

    grp_fu_211_p0 <= grp_fu_211_p00(4 - 1 downto 0);
    grp_fu_211_p00 <= std_logic_vector(IEEE.numeric_std.resize(unsigned(select_ln120_1_fu_160_p3),10));
    grp_fu_211_p1 <= ap_const_lv10_5A(7 - 1 downto 0);
    grp_fu_211_p2 <= grp_fu_211_p20(7 - 1 downto 0);
    grp_fu_211_p20 <= std_logic_vector(IEEE.numeric_std.resize(unsigned(select_ln120_reg_245_pp0_iter1_reg),10));
    icmp_ln120_fu_122_p2 <= "1" when (ap_sig_allocacmp_indvar_flatten_load = ap_const_lv10_2D0) else "0";
    icmp_ln121_fu_146_p2 <= "1" when (ap_sig_allocacmp_j_load = ap_const_lv7_5A) else "0";
    ifzero_fu_178_p2 <= "1" when (add_ln121_fu_172_p2 = ap_const_lv7_5A) else "0";
    p_event_count_address0 <= zext_ln120_fu_199_p1(3 - 1 downto 0);

    p_event_count_ce0_assign_proc : process(ap_CS_fsm_pp0_stage0, ap_enable_reg_pp0_iter1, ap_block_pp0_stage0_11001)
    begin
        if (((ap_const_boolean_0 = ap_block_pp0_stage0_11001) and (ap_enable_reg_pp0_iter1 = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0))) then 
            p_event_count_ce0 <= ap_const_logic_1;
        else 
            p_event_count_ce0 <= ap_const_logic_0;
        end if; 
    end process;

    p_event_count_d0 <= ap_const_lv16_0;

    p_event_count_we0_assign_proc : process(ap_CS_fsm_pp0_stage0, ap_enable_reg_pp0_iter1, ap_block_pp0_stage0_11001, ifzero_reg_260)
    begin
        if (((ifzero_reg_260 = ap_const_lv1_1) and (ap_const_boolean_0 = ap_block_pp0_stage0_11001) and (ap_enable_reg_pp0_iter1 = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0))) then 
            p_event_count_we0 <= ap_const_logic_1;
        else 
            p_event_count_we0 <= ap_const_logic_0;
        end if; 
    end process;

    sample_count_address0 <= zext_ln120_fu_199_p1(3 - 1 downto 0);

    sample_count_ce0_assign_proc : process(ap_CS_fsm_pp0_stage0, ap_enable_reg_pp0_iter1, ap_block_pp0_stage0_11001)
    begin
        if (((ap_const_boolean_0 = ap_block_pp0_stage0_11001) and (ap_enable_reg_pp0_iter1 = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0))) then 
            sample_count_ce0 <= ap_const_logic_1;
        else 
            sample_count_ce0 <= ap_const_logic_0;
        end if; 
    end process;

    sample_count_d0 <= ap_const_lv8_0;

    sample_count_we0_assign_proc : process(ap_CS_fsm_pp0_stage0, ap_enable_reg_pp0_iter1, ap_block_pp0_stage0_11001, ifzero_reg_260)
    begin
        if (((ifzero_reg_260 = ap_const_lv1_1) and (ap_const_boolean_0 = ap_block_pp0_stage0_11001) and (ap_enable_reg_pp0_iter1 = ap_const_logic_1) and (ap_const_logic_1 = ap_CS_fsm_pp0_stage0))) then 
            sample_count_we0 <= ap_const_logic_1;
        else 
            sample_count_we0 <= ap_const_logic_0;
        end if; 
    end process;

    select_ln120_1_fu_160_p3 <= 
        add_ln120_fu_140_p2 when (icmp_ln121_fu_146_p2(0) = '1') else 
        ap_sig_allocacmp_i_load;
    select_ln120_fu_152_p3 <= 
        ap_const_lv7_0 when (icmp_ln121_fu_146_p2(0) = '1') else 
        ap_sig_allocacmp_j_load;
    zext_ln120_fu_199_p1 <= std_logic_vector(IEEE.numeric_std.resize(unsigned(select_ln120_1_reg_250),64));
    zext_ln121_1_fu_207_p1 <= std_logic_vector(IEEE.numeric_std.resize(unsigned(grp_fu_211_p3),64));
end behav;