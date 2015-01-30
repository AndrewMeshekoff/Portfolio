library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.std_logic_unsigned.all;
--use work.cpu_constant_library.all;

entity alu is
  port (
    alu_control_in : in std_logic_vector(3 downto 0);
    channel_a_in   : in std_logic_vector(31 downto 0);
    channel_b_in   : in std_logic_vector(31 downto 0);
    zero_out       : out std_logic;
    alu_result_out : out std_logic_vector(31 downto 0)
    );
end alu;

architecture Behavioral of alu is
  signal result_s   : std_logic_vector(31 downto 0);

begin

  process (alu_control_in, channel_a_in, channel_b_in)
  begin
    case alu_control_in is
      --AND
      when "0000"          => result_s <= channel_a_in and channel_b_in;
      --OR
      when "0001"          => result_s <= channel_a_in or channel_b_in;
      --ADD
      when "0010"          => result_s <= channel_a_in + channel_b_in;
      --SUB
      when "0110"          => result_s <= channel_a_in - channel_b_in;
      --SLT
      when "0111"          => if channel_a_in < channel_b_in then
                                 result_s <= (others => '1');
                              else
                                 result_s <= (others => '0');
                              end if;
      --NOR
      when "1100"          => result_s <= channel_a_in nor channel_b_in;
      when others          => result_s <= (others => '0');
    end case;
  end process;

  alu_result_out <= result_s;
  zero_out <= '1' when result_s = 0 else '0';

end Behavioral;