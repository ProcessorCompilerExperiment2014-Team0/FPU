-------------------------------------------------------------------------------
-- Declaration
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.ftoi_p.all;

package ftoi_pipeline_p is

  component core is
    port (
      clk   : in  std_logic;
      xrst  : in  std_logic;
      stall : in  std_logic;
      a     : in  unsigned(31 downto 0);
      s     : out unsigned(31 downto 0));
  end component core;

end package core_p;

-------------------------------------------------------------------------------
-- Definition
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.ftoi_p.all;
use work.ftoi_pipeline_p.all;

entity ftoi_pipeline is
    port (
      clk   : in std_logic;
      xrst  : in std_logic;
      stall : in std_logic;
      a     : in unsigned(31 downto 0);
      s     : out unsigned(31 downto 0));
end entity ftoi_pipeline;

architecture behavior of ftoi_pipeline is

  type latch_t is record
    a : unsigned(31 downto 0);
    s : unsigned(31 downto 0);
  end record latch_t;

  constant latch_init : latch_t (
    a => (others => '-');
    s => (others => '-'));

  signal r, rin : latch_t := latch_init;

begin

  comb: process (r, a, b, stall) is
    v: latch_t;
  begin
    v := r;

    if stall /= '1' then
      v.a    := a;
      v.s    := ftoi(r.a);
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
