-- TestBench Template 

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;
use IEEE.std_logic_textio.all;

use std.textio.all;

use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

ENTITY fadd_tb IS
END fadd_tb;

ARCHITECTURE behavior OF fadd_tb IS

  -- Component Declaration
  COMPONENT fadd
    PORT(A : IN std_logic_vector(31 downto 0);
         B : IN std_logic_vector(31 downto 0);
         S : OUT std_logic_vector(31 downto 0));
  END COMPONENT;

  SIGNAL i1 :  std_logic_vector(31 downto 0);
  SIGNAL i2 :  std_logic_vector(31 downto 0);
  SIGNAL o1 :  std_logic_vector(31 downto 0);

BEGIN

  -- Component Instantiation
  uut: fadd PORT MAP(
    A => i1,
    B => i2,
    S => o1);

  --  Test Bench Statements
  tb : PROCESS
    file infile : text is in "./fadd_test/testcase.txt";
    file outfile : text is out "./fadd_test/result.txt";
    variable my_line, out_line : LINE;
    variable a, b, c : std_logic_vector(31 downto 0);
  BEGIN
    
    wait for 100 ns; -- wait until global set/reset completes

    -- Add user defined stimulus here
    
    while not endfile(infile) loop
      readline(infile, my_line);
      read(my_line, a);
      readline(infile, my_line);
      read(my_line, b);

      i1 <= a;
      i2 <= b;

      wait for 2 ns;

      c := o1;

      write(out_line, c);
      writeline(outfile, out_line);
    end loop;

  END PROCESS;

  --  End Test Bench

end behavior;
