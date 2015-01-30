library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity generic_register is
  generic (
    SIZE        : natural   := 4
  );
  port (
    clk         : in std_logic;
    rst         : in std_logic;
    write_en    : in std_logic;
    data_in     : in std_logic_vector(SIZE-1 downto 0);
    data_out    : out std_logic_vector(SIZE-1 downto 0)
  );
end generic_register;

architecture Behavioral of generic_register is

begin

  process (clk, rst)
  begin
    if rst = '1' then
      data_out <= (others => '0');
      
    elsif rising_edge(clk) then
      if write_en = '1' then
        data_out <= data_in;
      end if;
      
    end if;
  end process;
  
end Behavioral;
