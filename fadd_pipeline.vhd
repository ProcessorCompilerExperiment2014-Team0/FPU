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
use ieee.std_logic_textio.all;

library std;
use std.textio.all;

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

  type state_t is (CORNER, NORMAL);

  type latch_t is record
    -- stage 1
    fa        : float_t;
    fb        : float_t;
    state1    : state_t;
    fcorner1  : fpu_data_t;
    bigexpt1  : unsigned(7 downto 0);
    bigfrac   : unsigned(22 downto 0);
    smallfrac : unsigned(22 downto 0);
    expdiff   : integer range 0 to 255;
    -- stage 2
    state2    : state_t;
    fcorner2  : fpu_data_t;
    fcsign    : unsigned(0 downto 0);
    bigexpt2  : unsigned(7 downto 0);
    rawfrac   : unsigned(25 downto 0);
  end record latch_t;

  constant latch_init : latch_t := (
    -- stage 1
    fa        => float(x"00000000"),
    fb        => float(x"00000000"),
    state1    => CORNER,
    fcorner1  => (others => '0'),
    bigexpt1  => (others => '0'),
    bigfrac   => (others => '0'),
    smallfrac => (others => '0'),
    expdiff   => 0,
    -- stage 2
    state2    => CORNER,
    fcorner2  => (others => '0'),
    fcsign    => (others => '0'),
    bigexpt2  => (others => '0'),
    rawfrac   => (others => '0'));

  signal r, rin : latch_t := latch_init;

begin

  comb : process (r, a, b, stall) is
    variable v : latch_t;

    -- stage 1
    variable fa, fb, fc : float_t;
    variable fbig, fsmall : float_t;
    -- stage 2
    variable smallfrac : unsigned(24 downto 0);
    variable bigfrac   : unsigned(24 downto 0);
    -- stage 3
    variable lzc : integer range 0 to 31;

    variable l : line;

  begin
    v  := r;
    fc := float(x"00000000");

    if stall /= '1' then

      -------------------------------------------------------------------------
      -- Stage 1
      -------------------------------------------------------------------------

      if is_metavalue(a) or is_metavalue(b) then
        fa := float(x"00000000");
        fb := float(x"00000000");
      else
        fa := float(a);
        if negate_b then
          fb := float(not b(31) & b(30 downto 0));
        else
          fb := float(b);
        end if;
      end if;

      v.fa := fa;
      v.fb := fb;

      if fa.expt = 0 then
        v.state1   := CORNER;
        v.fcorner1 := fpu_data(fb);
      elsif fb.expt = 0 then
        v.state1   := CORNER;
        v.fcorner1 := fpu_data(fa);
      elsif fa.expt = 255 and fa.frac /= 0 then
        v.state1   := CORNER;
        v.fcorner1 := VAL_NAN;
      elsif fb.expt = 255 and fb.frac /= 0 then
        v.state1   := CORNER;
        v.fcorner1 := VAL_NAN;
      elsif fa.expt = 255 then
        if fb.expt = 255 and fa.sign /= fb.sign then
        v.state1   := CORNER;
        v.fcorner1 := VAL_NAN;
        else
          v.state1   := CORNER;
          v.fcorner1 := fpu_data(fa);
        end if;
      elsif fb.expt = 255 then
          v.state1   := CORNER;
          v.fcorner1 := fpu_data(fb);
      else
        v.state1 := NORMAL;
        v.fcorner1 := (others => '-');
      end if;

      if fa.expt > fb.expt or (fa.expt = fb.expt and fa.frac > fb.frac) then
        fbig   := fa;
        fsmall := fb;
      else
        fbig   := fb;
        fsmall := fa;
      end if;

      v.bigexpt1  := fbig.expt;
      v.bigfrac   := fbig.frac;
      v.smallfrac := fsmall.frac;
      v.expdiff   := to_integer(resize(unsigned(abs(signed(resize(fa.expt, 9) - resize(fb.expt, 9)))), 8));

      -------------------------------------------------------------------------
      -- Stage 2
      -------------------------------------------------------------------------

      v.state2   := r.state1;
      v.fcorner2 := r.fcorner1;
      v.bigexpt2 := r.bigexpt1;

      -- sign bit

      if (r.fa.sign = "1" and r.fb.sign = "1")
        or (r.fa.sign = "1" and (r.fa.expt & r.fa.frac) > (r.fb.expt & r.fb.frac))
        or (r.fb.sign = "1" and (r.fa.expt & r.fa.frac) < (r.fb.expt & r.fb.frac)) then
        v.fcsign := "1";
      else
        v.fcsign := "0";
      end if;

      -- exp and frac
      bigfrac := "1" & r.bigfrac & "0";
      if r.expdiff <= 25 then
        smallfrac := shift_right("1" & r.smallfrac & "0", r.expdiff)
                     + or_nbit("1" & r.smallfrac & "0", r.expdiff);
      else
        smallfrac := (others => '0');
      end if;

      if r.fa.sign /= r.fb.sign then
        v.rawfrac := resize(bigfrac, 26) - resize(smallfrac, 26);
      else
        v.rawfrac := resize(bigfrac, 26) + resize(smallfrac, 26);
      end if;

      -------------------------------------------------------------------------
      -- Stage 3
      -------------------------------------------------------------------------

      fc.sign := r.fcsign;
      lzc     := leading_zero(r.rawfrac);

      if lzc = 0 and r.bigexpt2 = 254 then
        fc.expt := (others => '1');
      elsif lzc = 0 then
        fc.expt := r.bigexpt2 + 1;
      elsif lzc = 26 or r.bigexpt2 < lzc then
        fc.expt := (others => '0');
      else
        fc.expt := r.bigexpt2 - (lzc - 1);
      end if;

      if lzc = 0 and r.bigexpt2 = 254 then
        fc.frac := (others => '0');
      elsif lzc = 0 then
        fc.frac := r.rawfrac(24 downto 2);
      elsif lzc = 26 or r.bigexpt2 < lzc then
        fc.frac := (others => '0');
      else
        fc.frac := shift_left(r.rawfrac, lzc - 1)(23 downto 1);
      end if;

    end if;

    case r.state2 is
      when CORNER => s <= r.fcorner2;
      when NORMAL => s <= fpu_data(fc);
    end case;

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
