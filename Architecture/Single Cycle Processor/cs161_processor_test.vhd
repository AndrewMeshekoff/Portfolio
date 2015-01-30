-- TestBench Template 

  LIBRARY ieee;
  USE ieee.std_logic_1164.ALL;
  USE ieee.numeric_std.ALL;
	use IEEE.STD_LOGIC_UNSIGNED.ALL;
	--use work.cpu_component_library.all;

  ENTITY testbench IS
  END testbench;

  ARCHITECTURE behavior OF testbench IS 

  -- Component Declaration
          COMPONENT cs161_processor
          PORT(    clk            : in std_logic;
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
          END COMPONENT;

				
--inputs
    SIGNAL clk :  std_logic;
    SIGNAL rst :  std_logic;
			 
			 
--outputs
	 SIGNAL prog_count     :  std_logic_vector(31 downto 0);
	 SIGNAL instr_opcode   :  std_logic_vector(5 downto 0);
    SIGNAL reg1_addr      :  std_logic_vector(4 downto 0);
    SIGNAL reg1_data      :  std_logic_vector(31 downto 0);
    SIGNAL reg2_addr      :  std_logic_vector(4 downto 0);
    SIGNAL reg2_data      :  std_logic_vector(31 downto 0);
    SIGNAL write_reg_addr :  std_logic_vector(4 downto 0);
    SIGNAL write_reg_data :  std_logic_vector(31 downto 0);
         
constant clk_period : time := 10 ns;			

  BEGIN

  -- Component Instantiation
          uut: cs161_processor PORT MAP(
                  clk => clk,
						rst => rst,
						
						 prog_count => prog_count,
						 instr_opcode => instr_opcode,
						 reg1_addr => reg1_addr,
						 reg1_data => reg1_data,
						 reg2_addr => reg2_addr,
						 reg2_data => reg2_data,
						 write_reg_addr => write_reg_addr,
						 write_reg_data => write_reg_data
          );


   -- Clock process definitions
   clk_process :process
   begin
		clk <= '0';
		wait for clk_period/2;
		clk <= '1';
		wait for clk_period/2;
   end process;
	

  --  Test Bench Statements
     tb : PROCESS
     BEGIN

        wait for 120 ns; -- wait until global set/reset completes
		  rst <= '1';
		  
		  wait for 10 ns;
		  
		  rst <= '0';

        -- Add user defined stimulus here

        wait; -- will wait forever
     END PROCESS tb;
  --  End Test Bench 

  END;
