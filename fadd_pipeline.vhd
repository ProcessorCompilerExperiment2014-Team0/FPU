-------------------------------------------------------------------------------
-- Declaration
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

package fadd_pipeline_p is

  component fadd_pipeline is
    generic (
      negate_b : boolean);
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
use work.fpu_common_p.all;
use work.fadd_pipeline_p.all;

entity fadd_pipeline is
  generic (
    negate_b : boolean := false);
  port (
    clk   : in  std_logic;
    xrst  : in  std_logic;
    stall : in  std_logic;
    a     : in  unsigned(31 downto 0);
    b     : in  unsigned(31 downto 0);
    s     : out unsigned(31 downto 0));
end entity fadd_pipeline;

architecture behavior of fadd_pipeline is

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

  comb : process (r, a, b, stall) is
    variable v : latch_t;

    variable fa, fb, fc : float_t;
    variable fbig, fsmall : float_t;

    variable lzc                : integer range 0 to 31;
    variable expdiff            : integer range 0 to 255;
    variable bigfrac, smallfrac : unsigned(24 downto 0);
    variable rawfrac            : unsigned(25 downto 0);

  begin
    v := r;

    if stall /= '1' then

      -------------------------------------------------------------------------
      -- Stage 1
      -------------------------------------------------------------------------

      v.a := a;
      if negate_b then
        v.b := not b(31) & b(30 downto 0);
      else
        v.b := b;
      end if;

      -------------------------------------------------------------------------
      -- Stage 2
      -------------------------------------------------------------------------

      fa := float(r.a);
      fb := float(r.b);

      -- sign bit

      if (fa.sign = "1" and fb.sign = "1")
        or (fa.sign = "1" and (fa.expt & fa.frac) > (fb.expt & fb.frac))
        or (fb.sign = "1" and (fa.expt & fa.frac) < (fb.expt & fb.frac)) then
        fc.sign := "1";
      else
        fc.sign := "0";
      end if;

      -- exp and frac

      if fa.expt > fb.expt or (fa.expt = fb.expt and fa.frac > fb.frac) then
        fbig   := fa;
        fsmall := fb;
      else
        fbig   := fb;
        fsmall := fa;
      end if;

      expdiff   := to_integer(fbig.expt - fsmall.expt);
      bigfrac   := "1" & fbig.frac & "0";
      if expdiff <= 24 then
        smallfrac := shift_right("1" & fsmall.frac & "0", expdiff)
                     + or_nbit("1" & fsmall.frac & "0", expdiff);
      else
        smallfrac := (others => '0');
      end if;

      if fa.sign /= fb.sign then
        rawfrac := resize(bigfrac, 26) - resize(smallfrac, 26);
      else
        rawfrac :=  resize(smallfrac, 26) + resize(bigfrac, 26);
      end if;

      lzc := leading_zero(rawfrac);

      if lzc = 0 and fbig.expt = 254 then
        fc.expt := (others => '1');
      elsif lzc = 0 then
        fc.expt := fbig.expt + 1;
      elsif lzc = 26 or fbig.expt < lzc then
        fc.expt := (others => '0');
      else
        fc.expt := fbig.expt - (lzc - 1);
      end if;

      if lzc = 0 and fbig.expt = 254 then
        fc.frac := (others => '0');
      elsif lzc = 0 then
        fc.frac := rawfrac(24 downto 2);
      elsif lzc = 26 or fbig.expt < lzc then
        fc.frac := (others => '0');
      else
        fc.frac := shift_left(rawfrac, lzc - 1)(23 downto 1);
      end if;

      ---------------------------------------------------------------------------

      if fa.expt = 0 then
        v.s := b;
      elsif fb.expt = 0 then
        v.s := a;
      elsif fa.expt = 255  and fa.frac /= 0 then
        v.s := VAL_NAN;
      elsif fb.expt = 255 and fb.frac /= 0 then
        v.s := VAL_NAN;
      elsif fa.expt = 255 then
        if fb.expt = 255 and fa.sign /= fb.sign then
          v.s := VAL_NAN;
        else
          v.s := a;
        end if;
      elsif fb.expt = 255 then
        v.s :=  b;
      else
        v.s :=  fpu_data(fc);
      end if;

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
