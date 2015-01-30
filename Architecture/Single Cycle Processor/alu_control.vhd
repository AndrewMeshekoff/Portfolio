library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use work.cpu_constant_library.all;

entity alu_control is
  port (
    alu_op            : in std_logic_vector(1 downto 0);
    instruction_5_0   : in std_logic_vector(5 downto 0);
    alu_out           : out std_logic_vector(3 downto 0)
    );
end alu_control;

architecture Behavioral of alu_control is

begin

  process (alu_op, instruction_5_0) 
  begin
    if alu_op = "00" then -- LW or SW
      alu_out <= ALU_ADD;
      
    elsif alu_op = "01" then -- branch
      alu_out <= ALU_SUBTRACT;
      
    else -- R_Type
      case instruction_5_0 is
        when FUNCT_AND       =>      alu_out <= ALU_AND;
        when FUNCT_OR        =>      alu_out <= ALU_OR;
        when FUNCT_ADD       =>      alu_out <= ALU_ADD;
        when FUNCT_SUBTRACT  =>      alu_out <= ALU_SUBTRACT;
        when FUNCT_LESS_THAN =>      alu_out <= ALU_LESS_THAN;
        when FUNCT_NOR       =>      alu_out <= ALU_NOR;
        when others          =>      alu_out <= (others => '0');
      end case;
    end if;
  end process;

end Behavioral;
