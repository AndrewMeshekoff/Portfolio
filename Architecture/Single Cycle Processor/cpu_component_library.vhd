library IEEE;
use IEEE.STD_LOGIC_1164.all;

package cpu_component_library is

  component    generic_register
    generic (
      SIZE      : natural   := 4
      );
    port (
      clk       : in std_logic;
      rst       : in std_logic;
      write_en  : in std_logic;
      data_in   : in std_logic_vector(SIZE-1 downto 0);
      data_out  : out std_logic_vector(SIZE-1 downto 0)
      );
  end component;

  component mux_2_1 is
    generic(
      SIZE        : natural   := 4
      );
    port (
      select_in   : in std_logic;
      data_0_in   : in std_logic_vector(SIZE-1 downto 0);
      data_1_in   : in std_logic_vector(SIZE-1 downto 0);
      data_out    : out std_logic_vector(SIZE-1 downto 0)
      );
  end component;
  
  component cpu_registers is
    port (
	   clk               : in std_logic;
      rst               : in std_logic;
      reg_write         : in std_logic;
      read_register_1   : in std_logic_vector(4 downto 0);
      read_register_2   : in std_logic_vector(4 downto 0);
      write_register    : in std_logic_vector(4 downto 0);
      write_data        : in std_logic_vector(31 downto 0);
      read_data_1       : out std_logic_vector(31 downto 0);
      read_data_2       : out std_logic_vector(31 downto 0)
      );
  end component;
  
  component alu is
    port (
      alu_control_in   : in std_logic_vector(3 downto 0);
      channel_a_in     : in std_logic_vector(31 downto 0);
      channel_b_in     : in std_logic_vector(31 downto 0);
      zero_out         : out std_logic;
      alu_result_out   : out std_logic_vector(31 downto 0)
      );
  end component;
  
 component control_unit is
  port (
    instr_op           : in std_logic_vector(5 downto 0);
    reg_dst           : out std_logic;
    branch            : out std_logic;
    mem_read          : out std_logic;
    mem_to_reg        : out std_logic;
    alu_op            : out std_logic_vector(1 downto 0);
    mem_write         : out std_logic;
    alu_src           : out std_logic;
    reg_write         : out std_logic
    );
 end component;
 
  component alu_control is
  port (
    alu_op            : in std_logic_vector(1 downto 0);
    instruction_5_0   : in std_logic_vector(5 downto 0);
    alu_out           : out std_logic_vector(3 downto 0)
    );
  end component;
  
  -- Instruction/Data memory Unit
  component memory is
    generic (
      COE_FILE_NAME         : string   := "init.coe"
      );
    port (
	   clk                   : in std_logic;
      rst                   : in std_logic;
      instr_read_address    : in std_logic_vector(7 downto 0);
      instr_instruction     : out std_logic_vector(31 downto 0);
      data_mem_write        : in std_logic;
      data_address          : in std_logic_vector(7 downto 0);
      data_write_data       : in std_logic_vector(31 downto 0);
      data_read_data        : out std_logic_vector(31 downto 0)
      );
  end component;

end cpu_component_library;

package body cpu_component_library is 
end cpu_component_library;
