-- TestBench Template 

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_textio.all;

use std.textio.all;

library work;
use work.finv_p.all;

ENTITY finv_tb IS
END finv_tb;

ARCHITECTURE behavior OF finv_tb IS

  signal xrst, stall : std_logic;
  signal i1 :  unsigned(31 downto 0);
  signal o1 :  unsigned(31 downto 0);
  signal clk : std_logic;
  signal terminate : std_logic := '0';
  signal t_start : unsigned(31 downto 0) := x"00000000";
  signal t_end   : unsigned(31 downto 0) := x"00000000";

  file infile : text is in "./finv_test/testcase.txt";
  file outfile : text is out "./finv_test/result.txt";
  
BEGIN

  xrst  <= '1';
  stall <= '0';

  -- Component Instantiation
  uut : finv PORT MAP(
    clk   => clk,
    xrst  => xrst,
    stall => stall,
    a     => i1,
    s     => o1);

  --  Test Bench Statements
  tb : process (clk)
    variable my_line, out_line : LINE;
    variable a : std_logic_vector(31 downto 0);
    variable b : unsigned(31 downto 0);
    variable t : unsigned(31 downto 0);
  BEGIN

    if rising_edge(clk) then
      --wait for 100 ns; -- wait until global set/reset completes

      -- Add user defined stimulus here

      if not endfile(infile) then
        readline(infile, my_line);
        read(my_line, a);

        i1 <= unsigned(a);
      else
        t := t_end;
        if t >= 3 then
          terminate <= '1';
        end if;
        t_end <= t + 1;
      end if;
      --wait for 2 ns;

      b := o1;

      if t_start >= 4 then
        write(out_line, std_logic_vector(b));
        writeline(outfile, out_line);
      end if;
      t_start <= t_start + 1;
   end if;


  END PROCESS;

  --  End Test Bench

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
