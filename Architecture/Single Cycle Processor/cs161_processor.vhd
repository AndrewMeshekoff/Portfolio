--amesh001, Andrew Meshekoff
library ieee;
use ieee.std_logic_1164.all;
use IEEE.NUMERIC_STD.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use work.cpu_component_library.all;

entity cs161_processor is
  port (
    clk            : in std_logic;
    rst            : in std_logic;
    
	 
    -- Debug Signals
    prog_count     : out std_logic_vector(31 downto 0);
    instr_opcode   : out std_logic_vector(5 downto 0);
    reg1_addr      : out std_logic_vector(4 downto 0);
    reg1_data      : out std_logic_vector(31 downto 0);
    reg2_addr      : out std_logic_vector(4 downto 0);
    reg2_data      : out std_logic_vector(31 downto 0);
    write_reg_addr : out std_logic_vector(4 downto 0);
    write_reg_data : out std_logic_vector(31 downto 0)
    );
end cs161_processor;

architecture Behavioral of cs161_processor is
	signal PC_out	: std_logic_vector(31 downto 0) := (others => '0');
	signal PC_mux_out	: std_logic_vector(31 downto 0) := (others => '0');
	signal PC_mux_sel	: std_logic := '0';
	signal inc_out	: std_logic_vector(31 downto 0) := (others => '0');
	signal IMEM_out: std_logic_vector(31 downto 0) := (others => '0');
	signal DMEM_out: std_logic_vector(31 downto 0) := (others => '0');
	signal add_out	: std_logic_vector(31 downto 0) := (others => '0');
	signal S_Ext	: std_logic_vector(31 downto 0) := (others => '0');
	signal L_shift	: std_logic_vector(31 downto 0) := (others => '0');
	signal Read_data_1	: std_logic_vector(31 downto 0) := (others => '0');
	signal Read_data_2	: std_logic_vector(31 downto 0) := (others => '0');
	signal Reg_addr_mux_out	: std_logic_vector(4 downto 0) := (others => '0');
	signal Reg_data_mux_out	: std_logic_vector(31 downto 0) := (others => '0');
	signal ALU_mux_out	: std_logic_vector(31 downto 0) := (others => '0');
	signal ALU_out	: std_logic_vector(31 downto 0) := (others => '0');
	signal AlU_zero: std_logic := '0';
	signal RegDst	: std_logic := '0';
	signal Branch	: std_logic := '0';
	signal MemRead	: std_logic := '0';
	signal MemtoReg: std_logic := '0';
	signal ALUOp	: std_logic_vector(1 downto 0) := (others => '0');
	signal MemWrite: std_logic := '0';
	signal ALUSrc	: std_logic := '0';
	signal RegWrite: std_logic := '0';
	signal ALU_control_out	: std_logic_vector(3 downto 0) := (others => '0');

begin
	PC : generic_register generic map (size => 32)
									port map (clk => clk,
												rst => rst,
												write_en => '1',
												data_out => PC_out,
												data_in => PC_mux_out);
												
	IMEM	: memory port map (clk => clk,
									 rst => rst,
									 instr_read_address => PC_out(9 downto 2),
									 instr_instruction => IMEM_out,
									 data_mem_write => MemWrite,
									 data_address => ALU_out(7 downto 0),
									 data_write_data => Read_data_2,
									 data_read_data => DMEM_out);
												
	control_unit_comp	:	control_unit port map  (instr_op => IMEM_out(31 downto 26),
																reg_dst => RegDst,
																branch => Branch,
																mem_read	=> MemRead,
																mem_to_reg => MemtoReg,
																alu_op => ALUOp,
																mem_write => MemWrite,
																alu_src => ALUSrc,
																reg_write => RegWrite);
																
	alu_control_comp	: alu_control port map (alu_op => ALUOp,
															instruction_5_0 => IMEM_out(5 downto 0),
															alu_out => ALU_control_out);

	PC_mux: mux_2_1 generic map (size => 32)
								port map   (data_0_in => inc_out,
												data_1_in => add_out,
												select_in => PC_mux_sel,
												data_out => PC_mux_out);
	PC_mux_sel <= Branch and ALU_zero;
												
	Reg_addr_mux: mux_2_1 generic map (size => 5)
								port map   (data_0_in => IMEM_out(20 downto 16),
												data_1_in => IMEM_out(15 downto 11),
												select_in => RegDst,
												data_out => Reg_addr_mux_out);
												
	cpu_registers_comp	: cpu_registers port map 	(clk => clk,
																	 rst => rst,
																	 reg_write => RegWrite,
																	 read_register_1 => IMEM_out(25 downto 21),
																	 read_register_2 => IMEM_out(20 downto 16),
																	 write_register => Reg_addr_mux_out,
																	 write_data => Reg_data_mux_out,
																	 read_data_1 => Read_data_1,
																	 read_data_2 => Read_data_2);
												
	ALU_mux: mux_2_1 generic map (size => 32)
								port map   (data_0_in => Read_data_2,
												data_1_in => S_Ext,
												select_in => ALUSrc,
												data_out => ALU_mux_out);
												
	ALU_comp: alu port map	(alu_control_in => ALU_control_out,
									 channel_a_in => Read_data_1,
									 channel_b_in => ALU_mux_out,
									 zero_out => ALU_zero,
									 alu_result_out => ALU_out);
									 
	Reg_data_mux: mux_2_1 generic map (size => 32)
								port map   (data_0_in => ALU_out,
												data_1_in => DMEM_out,
												select_in => MemtoReg,
												data_out => Reg_data_mux_out);
												
	inc_out <= std_logic_vector(unsigned(PC_out) + to_unsigned(4,32));
	
	S_Ext <= std_logic_vector(resize(signed(IMEM_out(15 downto 0)), 32));
	
	L_shift <= std_logic_vector(S_Ext(29 downto 0)&"00");
	
	add_out <= std_logic_vector(unsigned(inc_out) + unsigned(L_shift));

	prog_count <= PC_out;
	
    instr_opcode <= IMEM_out(31 downto 26);
    reg1_addr <= IMEM_out(25 downto 21);
    reg1_data <= Read_data_1;
    reg2_addr <= IMEM_out(20 downto 16);
    reg2_data <= Read_data_2;
    write_reg_addr <= Reg_addr_mux_out;
    write_reg_data <= Reg_data_mux_out;
	 
end Behavioral;
