-------------------------------------------------------------------------------
-- Declaration
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.fadd_p.all;

package fadd_pipeline_p is

  component fadd_pipeline is
    port (
      clk   : in  std_logic;
      xrst  : in  std_logic;
      stall : in  std_logic;
      a     : in  unsigned(31 downto 0);
      b     : in  unsigned(31 downto 0);
      s     : out unsigned(31 downto 0));
  end component fadd_pipeline;

end package fadd_pipeline_p;

-------------------------------------------------------------------------------
-- Definition
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.fadd_p.all;
use work.fadd_pipeline_p.all;

entity fadd_pipeline is
    port (
      clk   : in  std_logic;
      xrst  : in  std_logic;
      stall : in  std_logic;
      a     : in  unsigned(31 downto 0);
      b     : in  unsigned(31 downto 0);
      s     : out unsigned(31 downto 0));
end entity fadd_pipeline;

architecture behavior of fadd_pipeline is
  signal a_fadd, b_fadd, s_fadd : std_logic_vector(31 downto 0);

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

  fadd_comb : fadd port map (
    a => a_fadd,
    b => b_fadd,
    s => s_fadd);

  comb: process (r, a, b, stall, s_fadd) is
    variable v: latch_t;
  begin
    v := r;

    if stall /= '1' then
      v.a    := a;
      v.b    := b;
      v.s    := unsigned(s_fadd);
      a_fadd <= std_logic_vector(r.a);
      b_fadd <= std_logic_vector(r.b);
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
