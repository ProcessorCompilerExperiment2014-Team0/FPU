-------------------------------------------------------------------------------
-- Declaration
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.fcmp_p.all;

package fle_pipeline_p is

  component fle_pipeline is
    port (
      clk   : in  std_logic;
      xrst  : in  std_logic;
      stall : in  std_logic;
      a     : in  unsigned(31 downto 0);
      b     : in  unsigned(31 downto 0);
      s     : out unsigned(31 downto 0));
  end component fle_pipeline;

end package fle_pipeline_p;

-------------------------------------------------------------------------------
-- Definition
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.fcmp_p.all;
use work.fle_pipeline_p.all;

entity fle_pipeline is
    port (
      clk   : in  std_logic;
      xrst  : in  std_logic;
      stall : in  std_logic;
      a     : in  unsigned(31 downto 0);
      b     : in  unsigned(31 downto 0);
      s     : out unsigned(31 downto 0));
end entity fle_pipeline;

architecture behavior of fle_pipeline is

  type latch_t is record
    a : unsigned(31 downto 0);
    b : unsigned(31 downto 0);
    s : unsigned(31 downto 0);
  end record latch_t;

  constant latch_init : latch_t := (
    a => (others => '-'),
    b => (others => '-'),
    s => (others => '-'));

  signal r, rin : latch_t := latch_init;

begin

  comb: process (r, a, b, stall) is
    variable v: latch_t;
  begin
    v := r;

    if stall /= '1' then
      v.a := a;
      v.b := b;
      v.s := fcmp_eq(r.a, r.b);
    end if;

    s   <= r.s;
    rin <= v;
  end process comb;

  seq: process (clk, xrst) is
  begin
    if xrst = '0' then
      r <= latch_init;
    elsif rising_edge(clk) then
      r <= rin;
    end if;
  end process seq;

end architecture behavior;
