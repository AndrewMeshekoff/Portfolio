library IEEE;
use IEEE.STD_LOGIC_1164.all;

package cpu_constant_library is

  -- opcodes
  constant OPCODE_R_TYPE       : std_logic_vector(5 downto 0)   := "000000"; 
  constant OPCODE_LOAD_WORD    : std_logic_vector(5 downto 0)   := "100011"; 
  constant OPCODE_STORE_WORD   : std_logic_vector(5 downto 0)   := "101011"; 
  constant OPCODE_BRANCH_EQ    : std_logic_vector(5 downto 0)   := "000100";
  constant OPCODE_ADDI		    : std_logic_vector(5 downto 0)   := "001000"; 
  
  -- funct100
  constant FUNCT_AND           : std_logic_vector(5 downto 0)   := "100100"; 
  constant FUNCT_OR            : std_logic_vector(5 downto 0)   := "100101";
  constant FUNCT_ADD           : std_logic_vector(5 downto 0)   := "100000";
  constant FUNCT_SUBTRACT      : std_logic_vector(5 downto 0)   := "100010";
  constant FUNCT_LESS_THAN     : std_logic_vector(5 downto 0)   := "101010";
  constant FUNCT_NOR		       : std_logic_vector(5 downto 0)   := "100111";  

  -- ALU signals
  constant ALU_AND             : std_logic_vector(3 downto 0)   := "0000"; 
  constant ALU_OR              : std_logic_vector(3 downto 0)   := "0001";
  constant ALU_ADD             : std_logic_vector(3 downto 0)   := "0010";
  constant ALU_SUBTRACT        : std_logic_vector(3 downto 0)   := "0110";
  constant ALU_LESS_THAN       : std_logic_vector(3 downto 0)   := "0111";
  constant ALU_NOR             : std_logic_vector(3 downto 0)   := "1100";

end cpu_constant_library;

package body cpu_constant_library is 
end cpu_constant_library;