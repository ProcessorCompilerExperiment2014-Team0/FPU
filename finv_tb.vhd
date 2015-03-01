library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_textio.all;

library std;
use std.textio.all;

library work;
use work.finv_p.all;

entity finv_tb is
end finv_tb;

architecture behavior of finv_tb is

  signal xrst, stall : std_logic;
  signal i1 :  unsigned(31 downto 0);
  signal o1 :  unsigned(31 downto 0);
  signal clk : std_logic;
  signal terminate : std_logic := '0';
  signal t_start : unsigned(31 downto 0) := x"00000000";
  signal t_end   : unsigned(31 downto 0) := x"00000000";

  file infile : text is in "finv_test/testcase.txt";
  file outfile : text is out "finv_test/result.txt";
  
begin

  xrst  <= '1';
  stall <= '0';

  uut : finv port map(
    clk   => clk,
    xrst  => xrst,
    stall => stall,
    a     => i1,
    s     => o1);

  tb : process (clk)
    variable my_line, out_line : line;
    variable a : std_logic_vector(31 downto 0);
    variable t : unsigned(31 downto 0);
  begin

    if rising_edge(clk) then
      if not endfile(infile) then
        readline(infile, my_line);
        read(my_line, a);

        i1 <= unsigned(a);
      else
        t := t_end;
        if t >= 2 then
          terminate <= '1';
        end if;
        t_end <= t + 1;
      end if;

      if t_start >= 3 then
        write(out_line, std_logic_vector(o1));
        writeline(outfile, out_line);
      end if;
      t_start <= t_start + 1;
   end if;
  end process;

  clockgen: process  
  begin
    if terminate = '0' then
      clk <= '0';
      wait for 5 ns;
      clk <= '1';
      wait for 5 ns;
    else
      wait;
    end if;
  end process;

end behavior;
