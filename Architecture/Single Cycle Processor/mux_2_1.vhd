library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity mux_2_1 is
  generic(
    SIZE           : natural   := 4
  );
  port (
    select_in      : in std_logic;
    data_0_in      : in std_logic_vector(SIZE-1 downto 0);
    data_1_in      : in std_logic_vector(SIZE-1 downto 0);
    data_out       : out std_logic_vector(SIZE-1 downto 0)
  );
end mux_2_1;

architecture Behavioral of mux_2_1 is

begin

  process (select_in, data_0_in, data_1_in)
  begin
    case select_in is
      when '0'    =>    data_out <= data_0_in;
      when '1'    =>    data_out <= data_1_in;
      when others =>    data_out <= (others => '0');
    end case;
  end process;

end Behavioral;