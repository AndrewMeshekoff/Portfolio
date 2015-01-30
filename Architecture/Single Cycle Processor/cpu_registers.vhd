library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.std_logic_unsigned.all;

entity cpu_registers is
  port (
	 clk					 : in std_logic;
    rst               : in std_logic;
    reg_write         : in std_logic;
    read_register_1   : in std_logic_vector(4 downto 0);
    read_register_2   : in std_logic_vector(4 downto 0);
    write_register    : in std_logic_vector(4 downto 0);
    write_data        : in std_logic_vector(31 downto 0);
    read_data_1       : out std_logic_vector(31 downto 0);
    read_data_2       : out std_logic_vector(31 downto 0)
    );
end cpu_registers;

architecture Behavioral of cpu_registers is
  type REG_BUFF is array(0 to 31) of std_logic_vector(31 downto 0);
  signal registers   : REG_BUFF;

begin

  process (rst, read_register_1, read_register_2)
  begin
    if rst = '1' then
      read_data_1 <= (others => '0');
      read_data_2 <= (others => '0');
    else
      read_data_1 <= registers(conv_integer(read_register_1));
      read_data_2 <= registers(conv_integer(read_register_2));
    end if;
    
  end process;
  
  process (rst, clk)
  begin
    if rst = '1' then
      for i in 31 downto 0 loop
        registers(i) <= (others => '0');
      end loop;
    elsif rising_edge(clk) then
      if reg_write = '1' then
        registers(conv_integer(write_register)) <= write_data;
      end if;
    end if;
    
  end process;

end Behavioral;