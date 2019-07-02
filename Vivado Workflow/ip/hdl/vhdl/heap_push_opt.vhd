-- ==============================================================
-- RTL generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
-- Version: 2017.2
-- Copyright (C) 1986-2017 Xilinx, Inc. All Rights Reserved.
-- 
-- ===========================================================

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity heap_push_opt is
port (
    ap_clk : IN STD_LOGIC;
    ap_rst : IN STD_LOGIC;
    ap_start : IN STD_LOGIC;
    ap_done : OUT STD_LOGIC;
    ap_idle : OUT STD_LOGIC;
    ap_ready : OUT STD_LOGIC;
    current_graph_index_address0 : OUT STD_LOGIC_VECTOR (9 downto 0);
    current_graph_index_ce0 : OUT STD_LOGIC;
    current_graph_index_we0 : OUT STD_LOGIC;
    current_graph_index_d0 : OUT STD_LOGIC_VECTOR (31 downto 0);
    current_graph_index_q0 : IN STD_LOGIC_VECTOR (31 downto 0);
    current_graph_index_address1 : OUT STD_LOGIC_VECTOR (9 downto 0);
    current_graph_index_ce1 : OUT STD_LOGIC;
    current_graph_index_we1 : OUT STD_LOGIC;
    current_graph_index_d1 : OUT STD_LOGIC_VECTOR (31 downto 0);
    current_graph_index_q1 : IN STD_LOGIC_VECTOR (31 downto 0);
    current_graph_dist_address0 : OUT STD_LOGIC_VECTOR (9 downto 0);
    current_graph_dist_ce0 : OUT STD_LOGIC;
    current_graph_dist_we0 : OUT STD_LOGIC;
    current_graph_dist_d0 : OUT STD_LOGIC_VECTOR (31 downto 0);
    current_graph_dist_q0 : IN STD_LOGIC_VECTOR (31 downto 0);
    current_graph_dist_address1 : OUT STD_LOGIC_VECTOR (9 downto 0);
    current_graph_dist_ce1 : OUT STD_LOGIC;
    current_graph_dist_we1 : OUT STD_LOGIC;
    current_graph_dist_d1 : OUT STD_LOGIC_VECTOR (31 downto 0);
    current_graph_dist_q1 : IN STD_LOGIC_VECTOR (31 downto 0);
    current_graph_flag_address0 : OUT STD_LOGIC_VECTOR (9 downto 0);
    current_graph_flag_ce0 : OUT STD_LOGIC;
    current_graph_flag_we0 : OUT STD_LOGIC;
    current_graph_flag_d0 : OUT STD_LOGIC_VECTOR (31 downto 0);
    current_graph_flag_q0 : IN STD_LOGIC_VECTOR (31 downto 0);
    current_graph_flag_address1 : OUT STD_LOGIC_VECTOR (9 downto 0);
    current_graph_flag_ce1 : OUT STD_LOGIC;
    current_graph_flag_we1 : OUT STD_LOGIC;
    current_graph_flag_d1 : OUT STD_LOGIC_VECTOR (31 downto 0);
    current_graph_flag_q1 : IN STD_LOGIC_VECTOR (31 downto 0);
    row : IN STD_LOGIC_VECTOR (31 downto 0) );
end;


architecture behav of heap_push_opt is 
    constant ap_const_logic_1 : STD_LOGIC := '1';
    constant ap_const_logic_0 : STD_LOGIC := '0';
    constant ap_ST_fsm_state1 : STD_LOGIC_VECTOR (6 downto 0) := "0000001";
    constant ap_ST_fsm_state2 : STD_LOGIC_VECTOR (6 downto 0) := "0000010";
    constant ap_ST_fsm_state3 : STD_LOGIC_VECTOR (6 downto 0) := "0000100";
    constant ap_ST_fsm_state4 : STD_LOGIC_VECTOR (6 downto 0) := "0001000";
    constant ap_ST_fsm_state5 : STD_LOGIC_VECTOR (6 downto 0) := "0010000";
    constant ap_ST_fsm_state6 : STD_LOGIC_VECTOR (6 downto 0) := "0100000";
    constant ap_ST_fsm_state7 : STD_LOGIC_VECTOR (6 downto 0) := "1000000";
    constant ap_const_lv32_0 : STD_LOGIC_VECTOR (31 downto 0) := "00000000000000000000000000000000";
    constant ap_const_lv32_2 : STD_LOGIC_VECTOR (31 downto 0) := "00000000000000000000000000000010";
    constant ap_const_lv32_3 : STD_LOGIC_VECTOR (31 downto 0) := "00000000000000000000000000000011";
    constant ap_const_lv32_5 : STD_LOGIC_VECTOR (31 downto 0) := "00000000000000000000000000000101";
    constant ap_const_lv32_1 : STD_LOGIC_VECTOR (31 downto 0) := "00000000000000000000000000000001";
    constant ap_const_lv32_4 : STD_LOGIC_VECTOR (31 downto 0) := "00000000000000000000000000000100";
    constant ap_const_lv32_6 : STD_LOGIC_VECTOR (31 downto 0) := "00000000000000000000000000000110";
    constant ap_const_lv2_0 : STD_LOGIC_VECTOR (1 downto 0) := "00";
    constant ap_const_lv2_3 : STD_LOGIC_VECTOR (1 downto 0) := "11";
    constant ap_const_lv2_1 : STD_LOGIC_VECTOR (1 downto 0) := "01";
    constant ap_const_lv32_A : STD_LOGIC_VECTOR (31 downto 0) := "00000000000000000000000000001010";
    constant ap_const_lv1_1 : STD_LOGIC_VECTOR (0 downto 0) := "1";
    constant ap_const_boolean_1 : BOOLEAN := true;

    signal ap_CS_fsm : STD_LOGIC_VECTOR (6 downto 0) := "0000001";
    attribute fsm_encoding : string;
    attribute fsm_encoding of ap_CS_fsm : signal is "none";
    signal ap_CS_fsm_state1 : STD_LOGIC;
    attribute fsm_encoding of ap_CS_fsm_state1 : signal is "none";
    signal reg_172 : STD_LOGIC_VECTOR (31 downto 0);
    signal ap_CS_fsm_state3 : STD_LOGIC;
    attribute fsm_encoding of ap_CS_fsm_state3 : signal is "none";
    signal ap_CS_fsm_state4 : STD_LOGIC;
    attribute fsm_encoding of ap_CS_fsm_state4 : signal is "none";
    signal ap_CS_fsm_state6 : STD_LOGIC;
    attribute fsm_encoding of ap_CS_fsm_state6 : signal is "none";
    signal tmp_fu_191_p2 : STD_LOGIC_VECTOR (31 downto 0);
    signal tmp_reg_358 : STD_LOGIC_VECTOR (31 downto 0);
    signal i_4_fu_203_p2 : STD_LOGIC_VECTOR (1 downto 0);
    signal i_4_reg_369 : STD_LOGIC_VECTOR (1 downto 0);
    signal ap_CS_fsm_state2 : STD_LOGIC;
    attribute fsm_encoding of ap_CS_fsm_state2 : signal is "none";
    signal exitcond_fu_197_p2 : STD_LOGIC_VECTOR (0 downto 0);
    signal current_graph_dist_a_2_reg_384 : STD_LOGIC_VECTOR (9 downto 0);
    signal current_graph_index_1_reg_389 : STD_LOGIC_VECTOR (9 downto 0);
    signal current_graph_flag_a_1_reg_394 : STD_LOGIC_VECTOR (9 downto 0);
    signal tmp_46_fu_259_p2 : STD_LOGIC_VECTOR (0 downto 0);
    signal tmp_46_reg_399 : STD_LOGIC_VECTOR (0 downto 0);
    signal tmp_47_fu_265_p2 : STD_LOGIC_VECTOR (0 downto 0);
    signal tmp_47_reg_404 : STD_LOGIC_VECTOR (0 downto 0);
    signal sel_tmp1_fu_277_p2 : STD_LOGIC_VECTOR (0 downto 0);
    signal sel_tmp1_reg_409 : STD_LOGIC_VECTOR (0 downto 0);
    signal idx1_4_fu_305_p3 : STD_LOGIC_VECTOR (31 downto 0);
    signal idx1_4_reg_416 : STD_LOGIC_VECTOR (31 downto 0);
    signal ap_CS_fsm_state5 : STD_LOGIC;
    attribute fsm_encoding of ap_CS_fsm_state5 : signal is "none";
    signal current_son_fu_334_p2 : STD_LOGIC_VECTOR (31 downto 0);
    signal current_son_reg_421 : STD_LOGIC_VECTOR (31 downto 0);
    signal current_daughter_fu_340_p2 : STD_LOGIC_VECTOR (31 downto 0);
    signal current_daughter_reg_426 : STD_LOGIC_VECTOR (31 downto 0);
    signal current_graph_index_s_reg_431 : STD_LOGIC_VECTOR (9 downto 0);
    signal current_graph_dist_a_3_reg_436 : STD_LOGIC_VECTOR (9 downto 0);
    signal current_graph_flag_a_reg_442 : STD_LOGIC_VECTOR (9 downto 0);
    signal temp_reg_447 : STD_LOGIC_VECTOR (31 downto 0);
    signal temp_2_reg_452 : STD_LOGIC_VECTOR (31 downto 0);
    signal idx2_reg_125 : STD_LOGIC_VECTOR (31 downto 0);
    signal ap_CS_fsm_state7 : STD_LOGIC;
    attribute fsm_encoding of ap_CS_fsm_state7 : signal is "none";
    signal current_element_reg_137 : STD_LOGIC_VECTOR (31 downto 0);
    signal current_element_1_reg_149 : STD_LOGIC_VECTOR (31 downto 0);
    signal i_reg_161 : STD_LOGIC_VECTOR (1 downto 0);
    signal tmp_39_fu_214_p1 : STD_LOGIC_VECTOR (63 downto 0);
    signal tmp_41_fu_224_p1 : STD_LOGIC_VECTOR (63 downto 0);
    signal tmp_44_fu_234_p1 : STD_LOGIC_VECTOR (63 downto 0);
    signal tmp_49_fu_351_p1 : STD_LOGIC_VECTOR (63 downto 0);
    signal tmp_18_fu_179_p2 : STD_LOGIC_VECTOR (31 downto 0);
    signal tmp_19_fu_185_p2 : STD_LOGIC_VECTOR (31 downto 0);
    signal tmp_38_fu_209_p2 : STD_LOGIC_VECTOR (31 downto 0);
    signal tmp_40_fu_219_p2 : STD_LOGIC_VECTOR (31 downto 0);
    signal tmp_43_fu_229_p2 : STD_LOGIC_VECTOR (31 downto 0);
    signal tmp_42_fu_247_p2 : STD_LOGIC_VECTOR (0 downto 0);
    signal tmp_45_fu_253_p2 : STD_LOGIC_VECTOR (0 downto 0);
    signal tmp1_fu_271_p2 : STD_LOGIC_VECTOR (0 downto 0);
    signal tmp_s_fu_241_p2 : STD_LOGIC_VECTOR (0 downto 0);
    signal sel_tmp8_fu_290_p2 : STD_LOGIC_VECTOR (0 downto 0);
    signal tmp2_fu_295_p2 : STD_LOGIC_VECTOR (0 downto 0);
    signal sel_tmp9_fu_300_p2 : STD_LOGIC_VECTOR (0 downto 0);
    signal idx1_fu_283_p3 : STD_LOGIC_VECTOR (31 downto 0);
    signal idx1_5_fu_313_p3 : STD_LOGIC_VECTOR (31 downto 0);
    signal tmp_20_fu_328_p2 : STD_LOGIC_VECTOR (31 downto 0);
    signal idx1_2_fu_320_p3 : STD_LOGIC_VECTOR (31 downto 0);
    signal tmp_48_fu_346_p2 : STD_LOGIC_VECTOR (31 downto 0);
    signal ap_NS_fsm : STD_LOGIC_VECTOR (6 downto 0);


begin




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


    current_element_1_reg_149_assign_proc : process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if (((ap_const_logic_1 = ap_CS_fsm_state1) and (ap_start = ap_const_logic_1))) then 
                current_element_1_reg_149(1) <= '1';
                current_element_1_reg_149(2) <= '0';
                current_element_1_reg_149(3) <= '0';
                current_element_1_reg_149(4) <= '0';
                current_element_1_reg_149(5) <= '0';
                current_element_1_reg_149(6) <= '0';
                current_element_1_reg_149(7) <= '0';
                current_element_1_reg_149(8) <= '0';
                current_element_1_reg_149(9) <= '0';
                current_element_1_reg_149(10) <= '0';
                current_element_1_reg_149(11) <= '0';
                current_element_1_reg_149(12) <= '0';
                current_element_1_reg_149(13) <= '0';
                current_element_1_reg_149(14) <= '0';
                current_element_1_reg_149(15) <= '0';
                current_element_1_reg_149(16) <= '0';
                current_element_1_reg_149(17) <= '0';
                current_element_1_reg_149(18) <= '0';
                current_element_1_reg_149(19) <= '0';
                current_element_1_reg_149(20) <= '0';
                current_element_1_reg_149(21) <= '0';
                current_element_1_reg_149(22) <= '0';
                current_element_1_reg_149(23) <= '0';
                current_element_1_reg_149(24) <= '0';
                current_element_1_reg_149(25) <= '0';
                current_element_1_reg_149(26) <= '0';
                current_element_1_reg_149(27) <= '0';
                current_element_1_reg_149(28) <= '0';
                current_element_1_reg_149(29) <= '0';
                current_element_1_reg_149(30) <= '0';
                current_element_1_reg_149(31) <= '0';
            elsif ((ap_const_logic_1 = ap_CS_fsm_state7)) then 
                                current_element_1_reg_149(31 downto 1) <= current_daughter_reg_426(31 downto 1);
            end if; 
        end if;
    end process;

    current_element_reg_137_assign_proc : process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if (((ap_const_logic_1 = ap_CS_fsm_state1) and (ap_start = ap_const_logic_1))) then 
                current_element_reg_137(1) <= '0';
                current_element_reg_137(2) <= '0';
                current_element_reg_137(3) <= '0';
                current_element_reg_137(4) <= '0';
                current_element_reg_137(5) <= '0';
                current_element_reg_137(6) <= '0';
                current_element_reg_137(7) <= '0';
                current_element_reg_137(8) <= '0';
                current_element_reg_137(9) <= '0';
                current_element_reg_137(10) <= '0';
                current_element_reg_137(11) <= '0';
                current_element_reg_137(12) <= '0';
                current_element_reg_137(13) <= '0';
                current_element_reg_137(14) <= '0';
                current_element_reg_137(15) <= '0';
                current_element_reg_137(16) <= '0';
                current_element_reg_137(17) <= '0';
                current_element_reg_137(18) <= '0';
                current_element_reg_137(19) <= '0';
                current_element_reg_137(20) <= '0';
                current_element_reg_137(21) <= '0';
                current_element_reg_137(22) <= '0';
                current_element_reg_137(23) <= '0';
                current_element_reg_137(24) <= '0';
                current_element_reg_137(25) <= '0';
                current_element_reg_137(26) <= '0';
                current_element_reg_137(27) <= '0';
                current_element_reg_137(28) <= '0';
                current_element_reg_137(29) <= '0';
                current_element_reg_137(30) <= '0';
                current_element_reg_137(31) <= '0';
            elsif ((ap_const_logic_1 = ap_CS_fsm_state7)) then 
                                current_element_reg_137(31 downto 1) <= current_son_reg_421(31 downto 1);
            end if; 
        end if;
    end process;

    i_reg_161_assign_proc : process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if (((ap_const_logic_1 = ap_CS_fsm_state1) and (ap_start = ap_const_logic_1))) then 
                i_reg_161 <= ap_const_lv2_0;
            elsif ((ap_const_logic_1 = ap_CS_fsm_state7)) then 
                i_reg_161 <= i_4_reg_369;
            end if; 
        end if;
    end process;

    idx2_reg_125_assign_proc : process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if (((ap_const_logic_1 = ap_CS_fsm_state1) and (ap_start = ap_const_logic_1))) then 
                idx2_reg_125 <= ap_const_lv32_0;
            elsif ((ap_const_logic_1 = ap_CS_fsm_state7)) then 
                idx2_reg_125 <= idx1_4_reg_416;
            end if; 
        end if;
    end process;

    reg_172_assign_proc : process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if (((ap_const_logic_1 = ap_CS_fsm_state4) or (ap_const_logic_1 = ap_CS_fsm_state6))) then 
                reg_172 <= current_graph_dist_q1;
            elsif ((ap_const_logic_1 = ap_CS_fsm_state3)) then 
                reg_172 <= current_graph_dist_q0;
            end if; 
        end if;
    end process;
    process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if ((ap_const_logic_1 = ap_CS_fsm_state5)) then
                    current_daughter_reg_426(31 downto 1) <= current_daughter_fu_340_p2(31 downto 1);
                current_graph_dist_a_3_reg_436 <= tmp_49_fu_351_p1(10 - 1 downto 0);
                current_graph_flag_a_reg_442 <= tmp_49_fu_351_p1(10 - 1 downto 0);
                current_graph_index_s_reg_431 <= tmp_49_fu_351_p1(10 - 1 downto 0);
                    current_son_reg_421(31 downto 1) <= current_son_fu_334_p2(31 downto 1);
                idx1_4_reg_416 <= idx1_4_fu_305_p3;
            end if;
        end if;
    end process;
    process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if ((ap_const_logic_1 = ap_CS_fsm_state3)) then
                current_graph_dist_a_2_reg_384 <= tmp_44_fu_234_p1(10 - 1 downto 0);
                current_graph_flag_a_1_reg_394 <= tmp_44_fu_234_p1(10 - 1 downto 0);
                current_graph_index_1_reg_389 <= tmp_44_fu_234_p1(10 - 1 downto 0);
            end if;
        end if;
    end process;
    process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if ((ap_const_logic_1 = ap_CS_fsm_state2)) then
                i_4_reg_369 <= i_4_fu_203_p2;
            end if;
        end if;
    end process;
    process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if ((ap_const_logic_1 = ap_CS_fsm_state4)) then
                sel_tmp1_reg_409 <= sel_tmp1_fu_277_p2;
                tmp_46_reg_399 <= tmp_46_fu_259_p2;
                tmp_47_reg_404 <= tmp_47_fu_265_p2;
            end if;
        end if;
    end process;
    process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if ((ap_const_logic_1 = ap_CS_fsm_state6)) then
                temp_2_reg_452 <= current_graph_flag_q0;
                temp_reg_447 <= current_graph_index_q0;
            end if;
        end if;
    end process;
    process (ap_clk)
    begin
        if (ap_clk'event and ap_clk = '1') then
            if (((ap_const_logic_1 = ap_CS_fsm_state1) and (ap_start = ap_const_logic_1))) then
                    tmp_reg_358(31 downto 1) <= tmp_fu_191_p2(31 downto 1);
            end if;
        end if;
    end process;
    tmp_reg_358(0) <= '0';
    current_son_reg_421(0) <= '1';
    current_daughter_reg_426(0) <= '0';
    current_element_reg_137(0) <= '1';
    current_element_1_reg_149(0) <= '0';

    ap_NS_fsm_assign_proc : process (ap_start, ap_CS_fsm, ap_CS_fsm_state1, ap_CS_fsm_state2, exitcond_fu_197_p2)
    begin
        case ap_CS_fsm is
            when ap_ST_fsm_state1 => 
                if (((ap_const_logic_1 = ap_CS_fsm_state1) and (ap_start = ap_const_logic_1))) then
                    ap_NS_fsm <= ap_ST_fsm_state2;
                else
                    ap_NS_fsm <= ap_ST_fsm_state1;
                end if;
            when ap_ST_fsm_state2 => 
                if (((ap_const_logic_1 = ap_CS_fsm_state2) and (exitcond_fu_197_p2 = ap_const_lv1_1))) then
                    ap_NS_fsm <= ap_ST_fsm_state1;
                else
                    ap_NS_fsm <= ap_ST_fsm_state3;
                end if;
            when ap_ST_fsm_state3 => 
                ap_NS_fsm <= ap_ST_fsm_state4;
            when ap_ST_fsm_state4 => 
                ap_NS_fsm <= ap_ST_fsm_state5;
            when ap_ST_fsm_state5 => 
                ap_NS_fsm <= ap_ST_fsm_state6;
            when ap_ST_fsm_state6 => 
                ap_NS_fsm <= ap_ST_fsm_state7;
            when ap_ST_fsm_state7 => 
                ap_NS_fsm <= ap_ST_fsm_state2;
            when others =>  
                ap_NS_fsm <= "XXXXXXX";
        end case;
    end process;
    ap_CS_fsm_state1 <= ap_CS_fsm(0);
    ap_CS_fsm_state2 <= ap_CS_fsm(1);
    ap_CS_fsm_state3 <= ap_CS_fsm(2);
    ap_CS_fsm_state4 <= ap_CS_fsm(3);
    ap_CS_fsm_state5 <= ap_CS_fsm(4);
    ap_CS_fsm_state6 <= ap_CS_fsm(5);
    ap_CS_fsm_state7 <= ap_CS_fsm(6);

    ap_done_assign_proc : process(ap_start, ap_CS_fsm_state1, ap_CS_fsm_state2, exitcond_fu_197_p2)
    begin
        if ((((ap_const_logic_0 = ap_start) and (ap_const_logic_1 = ap_CS_fsm_state1)) or ((ap_const_logic_1 = ap_CS_fsm_state2) and (exitcond_fu_197_p2 = ap_const_lv1_1)))) then 
            ap_done <= ap_const_logic_1;
        else 
            ap_done <= ap_const_logic_0;
        end if; 
    end process;


    ap_idle_assign_proc : process(ap_start, ap_CS_fsm_state1)
    begin
        if (((ap_const_logic_0 = ap_start) and (ap_const_logic_1 = ap_CS_fsm_state1))) then 
            ap_idle <= ap_const_logic_1;
        else 
            ap_idle <= ap_const_logic_0;
        end if; 
    end process;


    ap_ready_assign_proc : process(ap_CS_fsm_state2, exitcond_fu_197_p2)
    begin
        if (((ap_const_logic_1 = ap_CS_fsm_state2) and (exitcond_fu_197_p2 = ap_const_lv1_1))) then 
            ap_ready <= ap_const_logic_1;
        else 
            ap_ready <= ap_const_logic_0;
        end if; 
    end process;

    current_daughter_fu_340_p2 <= std_logic_vector(unsigned(ap_const_lv32_1) + unsigned(current_son_fu_334_p2));

    current_graph_dist_address0_assign_proc : process(ap_CS_fsm_state3, ap_CS_fsm_state6, ap_CS_fsm_state2, current_graph_dist_a_3_reg_436, tmp_39_fu_214_p1, tmp_41_fu_224_p1)
    begin
        if ((ap_const_logic_1 = ap_CS_fsm_state6)) then 
            current_graph_dist_address0 <= current_graph_dist_a_3_reg_436;
        elsif ((ap_const_logic_1 = ap_CS_fsm_state3)) then 
            current_graph_dist_address0 <= tmp_41_fu_224_p1(10 - 1 downto 0);
        elsif ((ap_const_logic_1 = ap_CS_fsm_state2)) then 
            current_graph_dist_address0 <= tmp_39_fu_214_p1(10 - 1 downto 0);
        else 
            current_graph_dist_address0 <= "XXXXXXXXXX";
        end if; 
    end process;


    current_graph_dist_address1_assign_proc : process(ap_CS_fsm_state3, current_graph_dist_a_2_reg_384, ap_CS_fsm_state5, ap_CS_fsm_state7, tmp_44_fu_234_p1, tmp_49_fu_351_p1)
    begin
        if ((ap_const_logic_1 = ap_CS_fsm_state7)) then 
            current_graph_dist_address1 <= current_graph_dist_a_2_reg_384;
        elsif ((ap_const_logic_1 = ap_CS_fsm_state5)) then 
            current_graph_dist_address1 <= tmp_49_fu_351_p1(10 - 1 downto 0);
        elsif ((ap_const_logic_1 = ap_CS_fsm_state3)) then 
            current_graph_dist_address1 <= tmp_44_fu_234_p1(10 - 1 downto 0);
        else 
            current_graph_dist_address1 <= "XXXXXXXXXX";
        end if; 
    end process;


    current_graph_dist_ce0_assign_proc : process(ap_CS_fsm_state3, ap_CS_fsm_state6, ap_CS_fsm_state2)
    begin
        if (((ap_const_logic_1 = ap_CS_fsm_state3) or (ap_const_logic_1 = ap_CS_fsm_state6) or (ap_const_logic_1 = ap_CS_fsm_state2))) then 
            current_graph_dist_ce0 <= ap_const_logic_1;
        else 
            current_graph_dist_ce0 <= ap_const_logic_0;
        end if; 
    end process;


    current_graph_dist_ce1_assign_proc : process(ap_CS_fsm_state3, ap_CS_fsm_state5, ap_CS_fsm_state7)
    begin
        if (((ap_const_logic_1 = ap_CS_fsm_state3) or (ap_const_logic_1 = ap_CS_fsm_state5) or (ap_const_logic_1 = ap_CS_fsm_state7))) then 
            current_graph_dist_ce1 <= ap_const_logic_1;
        else 
            current_graph_dist_ce1 <= ap_const_logic_0;
        end if; 
    end process;

    current_graph_dist_d0 <= reg_172;
    current_graph_dist_d1 <= reg_172;

    current_graph_dist_we0_assign_proc : process(ap_CS_fsm_state6)
    begin
        if ((ap_const_logic_1 = ap_CS_fsm_state6)) then 
            current_graph_dist_we0 <= ap_const_logic_1;
        else 
            current_graph_dist_we0 <= ap_const_logic_0;
        end if; 
    end process;


    current_graph_dist_we1_assign_proc : process(ap_CS_fsm_state7)
    begin
        if ((ap_const_logic_1 = ap_CS_fsm_state7)) then 
            current_graph_dist_we1 <= ap_const_logic_1;
        else 
            current_graph_dist_we1 <= ap_const_logic_0;
        end if; 
    end process;


    current_graph_flag_address0_assign_proc : process(ap_CS_fsm_state6, ap_CS_fsm_state5, current_graph_flag_a_reg_442, tmp_49_fu_351_p1)
    begin
        if ((ap_const_logic_1 = ap_CS_fsm_state6)) then 
            current_graph_flag_address0 <= current_graph_flag_a_reg_442;
        elsif ((ap_const_logic_1 = ap_CS_fsm_state5)) then 
            current_graph_flag_address0 <= tmp_49_fu_351_p1(10 - 1 downto 0);
        else 
            current_graph_flag_address0 <= "XXXXXXXXXX";
        end if; 
    end process;

    current_graph_flag_address1 <= current_graph_flag_a_1_reg_394;

    current_graph_flag_ce0_assign_proc : process(ap_CS_fsm_state6, ap_CS_fsm_state5)
    begin
        if (((ap_const_logic_1 = ap_CS_fsm_state6) or (ap_const_logic_1 = ap_CS_fsm_state5))) then 
            current_graph_flag_ce0 <= ap_const_logic_1;
        else 
            current_graph_flag_ce0 <= ap_const_logic_0;
        end if; 
    end process;


    current_graph_flag_ce1_assign_proc : process(ap_CS_fsm_state5, ap_CS_fsm_state7)
    begin
        if (((ap_const_logic_1 = ap_CS_fsm_state5) or (ap_const_logic_1 = ap_CS_fsm_state7))) then 
            current_graph_flag_ce1 <= ap_const_logic_1;
        else 
            current_graph_flag_ce1 <= ap_const_logic_0;
        end if; 
    end process;

    current_graph_flag_d0 <= current_graph_flag_q1;
    current_graph_flag_d1 <= temp_2_reg_452;

    current_graph_flag_we0_assign_proc : process(ap_CS_fsm_state6)
    begin
        if ((ap_const_logic_1 = ap_CS_fsm_state6)) then 
            current_graph_flag_we0 <= ap_const_logic_1;
        else 
            current_graph_flag_we0 <= ap_const_logic_0;
        end if; 
    end process;


    current_graph_flag_we1_assign_proc : process(ap_CS_fsm_state7)
    begin
        if ((ap_const_logic_1 = ap_CS_fsm_state7)) then 
            current_graph_flag_we1 <= ap_const_logic_1;
        else 
            current_graph_flag_we1 <= ap_const_logic_0;
        end if; 
    end process;


    current_graph_index_address0_assign_proc : process(ap_CS_fsm_state6, ap_CS_fsm_state5, current_graph_index_s_reg_431, tmp_49_fu_351_p1)
    begin
        if ((ap_const_logic_1 = ap_CS_fsm_state6)) then 
            current_graph_index_address0 <= current_graph_index_s_reg_431;
        elsif ((ap_const_logic_1 = ap_CS_fsm_state5)) then 
            current_graph_index_address0 <= tmp_49_fu_351_p1(10 - 1 downto 0);
        else 
            current_graph_index_address0 <= "XXXXXXXXXX";
        end if; 
    end process;

    current_graph_index_address1 <= current_graph_index_1_reg_389;

    current_graph_index_ce0_assign_proc : process(ap_CS_fsm_state6, ap_CS_fsm_state5)
    begin
        if (((ap_const_logic_1 = ap_CS_fsm_state6) or (ap_const_logic_1 = ap_CS_fsm_state5))) then 
            current_graph_index_ce0 <= ap_const_logic_1;
        else 
            current_graph_index_ce0 <= ap_const_logic_0;
        end if; 
    end process;


    current_graph_index_ce1_assign_proc : process(ap_CS_fsm_state5, ap_CS_fsm_state7)
    begin
        if (((ap_const_logic_1 = ap_CS_fsm_state5) or (ap_const_logic_1 = ap_CS_fsm_state7))) then 
            current_graph_index_ce1 <= ap_const_logic_1;
        else 
            current_graph_index_ce1 <= ap_const_logic_0;
        end if; 
    end process;

    current_graph_index_d0 <= current_graph_index_q1;
    current_graph_index_d1 <= temp_reg_447;

    current_graph_index_we0_assign_proc : process(ap_CS_fsm_state6)
    begin
        if ((ap_const_logic_1 = ap_CS_fsm_state6)) then 
            current_graph_index_we0 <= ap_const_logic_1;
        else 
            current_graph_index_we0 <= ap_const_logic_0;
        end if; 
    end process;


    current_graph_index_we1_assign_proc : process(ap_CS_fsm_state7)
    begin
        if ((ap_const_logic_1 = ap_CS_fsm_state7)) then 
            current_graph_index_we1 <= ap_const_logic_1;
        else 
            current_graph_index_we1 <= ap_const_logic_0;
        end if; 
    end process;

    current_son_fu_334_p2 <= (tmp_20_fu_328_p2 or ap_const_lv32_1);
    exitcond_fu_197_p2 <= "1" when (i_reg_161 = ap_const_lv2_3) else "0";
    i_4_fu_203_p2 <= std_logic_vector(unsigned(i_reg_161) + unsigned(ap_const_lv2_1));
    idx1_2_fu_320_p3 <= 
        current_element_reg_137 when (sel_tmp9_fu_300_p2(0) = '1') else 
        idx1_5_fu_313_p3;
    idx1_4_fu_305_p3 <= 
        current_element_reg_137 when (sel_tmp9_fu_300_p2(0) = '1') else 
        idx1_fu_283_p3;
    idx1_5_fu_313_p3 <= 
        current_element_1_reg_149 when (sel_tmp1_reg_409(0) = '1') else 
        idx2_reg_125;
    idx1_fu_283_p3 <= 
        current_element_1_reg_149 when (sel_tmp1_reg_409(0) = '1') else 
        current_element_reg_137;
    sel_tmp1_fu_277_p2 <= (tmp1_fu_271_p2 and tmp_s_fu_241_p2);
    sel_tmp8_fu_290_p2 <= (sel_tmp1_reg_409 xor ap_const_lv1_1);
    sel_tmp9_fu_300_p2 <= (tmp2_fu_295_p2 and tmp_46_reg_399);
    tmp1_fu_271_p2 <= (tmp_42_fu_247_p2 and tmp_45_fu_253_p2);
    tmp2_fu_295_p2 <= (tmp_47_reg_404 and sel_tmp8_fu_290_p2);
    tmp_18_fu_179_p2 <= std_logic_vector(shift_left(unsigned(row),to_integer(unsigned('0' & ap_const_lv32_3(31-1 downto 0)))));
    tmp_19_fu_185_p2 <= std_logic_vector(shift_left(unsigned(row),to_integer(unsigned('0' & ap_const_lv32_1(31-1 downto 0)))));
    tmp_20_fu_328_p2 <= std_logic_vector(shift_left(unsigned(idx1_4_fu_305_p3),to_integer(unsigned('0' & ap_const_lv32_1(31-1 downto 0)))));
    tmp_38_fu_209_p2 <= std_logic_vector(unsigned(current_element_1_reg_149) + unsigned(tmp_reg_358));
        tmp_39_fu_214_p1 <= std_logic_vector(IEEE.numeric_std.resize(signed(tmp_38_fu_209_p2),64));

    tmp_40_fu_219_p2 <= std_logic_vector(unsigned(current_element_reg_137) + unsigned(tmp_reg_358));
        tmp_41_fu_224_p1 <= std_logic_vector(IEEE.numeric_std.resize(signed(tmp_40_fu_219_p2),64));

    tmp_42_fu_247_p2 <= "1" when (signed(reg_172) > signed(current_graph_dist_q0)) else "0";
    tmp_43_fu_229_p2 <= std_logic_vector(unsigned(idx2_reg_125) + unsigned(tmp_reg_358));
        tmp_44_fu_234_p1 <= std_logic_vector(IEEE.numeric_std.resize(signed(tmp_43_fu_229_p2),64));

    tmp_45_fu_253_p2 <= "1" when (signed(reg_172) > signed(current_graph_dist_q1)) else "0";
    tmp_46_fu_259_p2 <= "1" when (signed(current_element_reg_137) < signed(ap_const_lv32_A)) else "0";
    tmp_47_fu_265_p2 <= "1" when (signed(current_graph_dist_q0) > signed(current_graph_dist_q1)) else "0";
    tmp_48_fu_346_p2 <= std_logic_vector(unsigned(idx1_2_fu_320_p3) + unsigned(tmp_reg_358));
        tmp_49_fu_351_p1 <= std_logic_vector(IEEE.numeric_std.resize(signed(tmp_48_fu_346_p2),64));

    tmp_fu_191_p2 <= std_logic_vector(unsigned(tmp_18_fu_179_p2) + unsigned(tmp_19_fu_185_p2));
    tmp_s_fu_241_p2 <= "1" when (signed(current_element_1_reg_149) < signed(ap_const_lv32_A)) else "0";
end behav;
