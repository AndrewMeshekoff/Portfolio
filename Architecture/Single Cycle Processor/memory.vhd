library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.std_logic_unsigned.all;

library std;
use std.textio.all;

entity memory is
  generic (
    COE_FILE_NAME        : string   := "init.coe"
    );
  port (
	 clk						 : in std_logic;
    rst                  : in std_logic;
    instr_read_address   : in std_logic_vector(7 downto 0);
    instr_instruction    : out std_logic_vector(31 downto 0);
    data_mem_write       : in std_logic;
    data_address         : in std_logic_vector(7 downto 0);
    data_write_data      : in std_logic_vector(31 downto 0);
    data_read_data       : out std_logic_vector(31 downto 0)
    );
end memory;

architecture Behavioral of memory is
  type MEMORY_BUFFER is array(255 downto 0) of std_logic_vector(31 downto 0);
  signal buff      : MEMORY_BUFFER;
  
begin

  process (rst, clk, instr_read_address, data_mem_write, data_address, data_write_data, buff)
    file     coe_file  :   text;
    variable coe_line  :   line;
    variable coe_str   :   bit_vector(31 downto 0);
    variable coe_status:   file_open_status;
  begin
    if rst = '1' then
      
      file_open(coe_status, coe_file, COE_FILE_NAME, read_mode);
      
      if coe_status = OPEN_OK then
        for i in 0 to 255 loop
          if not endfile(coe_file) then
            readline(coe_file, coe_line);
            read(coe_line, coe_str);
            buff(i) <= to_StdLogicVector(coe_str);
          else
            buff(i) <= (others => '0');
          end if;
        end loop;
        file_close(coe_file);
      else
        report "Could not open COE file" severity warning;
        for i in 0 to 255 loop
          buff(i) <= (others => '0');
        end loop;
        
      end if;
      
      instr_instruction <= (others => '0');
      data_read_data      <= (others => '0');
    else
		if rising_edge(clk) and data_mem_write = '1' then
				buff(conv_integer(data_address)) <= data_write_data;
		end if;
		
      instr_instruction <= buff(conv_integer(instr_read_address));
      data_read_data <= buff(conv_integer(data_address));
    end if;
  end process;


end Behavioral;