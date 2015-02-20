-------------------------------------------------------------------------------
-- Declaration
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.fsub_p.all;

package fsub_pipeline_p is

  component core is
    port (
      clk   : in  std_logic;
      xrst  : in  std_logic;
      stall : in  std_logic;
      a     : in  unsigned(31 downto 0);
      b     : in  unsigned(31 downto 0);
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
use work.fsub_p.all;
use work.fsub_pipeline_p.all;

entity fsub_pipeline is
    port (
      clk   : in  std_logic;
      xrst  : in  std_logic;
      stall : in  std_logic;
      a     : in  unsigned(31 downto 0);
      b     : in  unsigned(31 downto 0);
      s     : out unsigned(31 downto 0));
end entity fsub_pipeline;

architecture behavior of fsub_pipeline is
  signal a_fsub, b_fsub, s_fsub : std_logic_vector(31 dnwoto 0);

  type latch_t is record
    a : unsigned(31 downto 0);
    b : unsigned(31 downto 0);
    s : unsigned(31 downto 0);
  end record latch_t;

  constant latch_init : latch_t (
    a => (others => '-');
    b => (others => '-');
    s => (others => '-'));

  signal r, rin : latch_t := latch_init;

begin

  fsub_comb : fsub port map (
    a => a_fsub,
    b => b_fsub,
    s => s_fsub);

  comb: process (r, a, b, stall, s_fsub) is
    v: latch_t;
  begin
    v := r;

    if stall /= '1' then
      v.a    := a;
      v.b    := b;
      v.s    := unsigned(s_fsub);
      a_fsub <= std_logic_vector(r.a);
      b_fsub <= std_logic_vector(r.b);
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
