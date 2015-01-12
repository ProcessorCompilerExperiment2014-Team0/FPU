-- TestBench Template 

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;
use IEEE.std_logic_textio.all;

use std.textio.all;

use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

ENTITY testbench IS
END testbench;

ARCHITECTURE behavior OF testbench IS

  -- Component Declaration
  COMPONENT finv
    PORT(clk : in std_logic;
         a : IN std_logic_vector(31 downto 0);
         s : OUT std_logic_vector(31 downto 0));
  END COMPONENT;

  SIGNAL i1 :  std_logic_vector(31 downto 0);
  SIGNAL o1 :  std_logic_vector(31 downto 0);
  signal clk : std_logic;
  signal terminate : std_logic := '0';

  file infile : text is in "/home/kazuki/CPU/team0/FPU/finv_test/testcase.txt";
  file outfile : text is out "/home/kazuki/CPU/team0/FPU/finv_test/result.txt";
  
BEGIN

  -- Component Instantiation
  uut: finv PORT MAP(
    clk => clk,
    a => i1,
    s => o1);

  --  Test Bench Statements
  tb : process (clk)
    variable my_line, out_line : LINE;
    variable a, b : std_logic_vector(31 downto 0);
  BEGIN

    if rising_edge(clk) then
      --wait for 100 ns; -- wait until global set/reset completes

      -- Add user defined stimulus here

      if not endfile(infile) then
        readline(infile, my_line);
        read(my_line, a);

        i1 <= a;
      else
        terminate <= '1';
      end if;
      --wait for 2 ns;

      b := o1;

      write(out_line, b);
      writeline(outfile, out_line);
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
      clk <= '0';
      wait for 5 ns;
      clk <= '1';
      wait for 5 ns;
      clk <= '0';
      wait for 5 ns;
      clk <= '1';
      wait;
    end if;
  end process;
  
end behavior;
