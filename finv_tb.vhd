-- TestBench Template 

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;
use IEEE.std_logic_textio.all;

use std.textio.all;

use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

ENTITY finv_tb IS
END finv_tb;

ARCHITECTURE behavior OF finv_tb IS

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
  signal t_start : std_logic_vector(31 downto 0) := x"00000000";
  signal t_end   : std_logic_vector(31 downto 0) := x"00000000";

  file infile : text is in "./finv_test/testcase.txt";
  file outfile : text is out "./finv_test/result.txt";
  
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
    variable t : std_logic_vector(31 downto 0);
  BEGIN

    if rising_edge(clk) then
      --wait for 100 ns; -- wait until global set/reset completes

      -- Add user defined stimulus here

      if not endfile(infile) then
        readline(infile, my_line);
        read(my_line, a);

        i1 <= a;
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
        write(out_line, b);
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
