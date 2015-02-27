-------------------------------------------------------------------------------
-- Declaration
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.fpu_common_p.all;

package fadd_p is

  function fsub (a : fpu_data_t; b : fpu_data_t) return fpu_data_t;
  function fadd (a : fpu_data_t; b : fpu_data_t) return fpu_data_t;

end package;

-------------------------------------------------------------------------------
-- Definition
-------------------------------------------------------------------------------

package body fadd_p is

  function fadd (
    a : fpu_data_t;
    b : fpu_data_t)
    return fpu_data_t is

    variable fa, fb, fc : float_t;
    variable fbig, fsmall : float_t;

    variable lzc                : integer range 0 to 31;
    variable expdiff            : integer range 0 to 255;
    variable bigfrac, smallfrac : unsigned(23 downto 0);
    variable rawfrac            : unsigned(24 downto 0);

  begin

    fa := float(a);
    fb := float(b);

    ---------------------------------------------------------------------------
    -- sign bit
    ---------------------------------------------------------------------------

    if (fa.sign = "1" and fb.sign = "1")
      or (fa.sign = "1" and (fa.expt & fa.frac) > (fb.expt & fb.frac))
      or (fb.sign = "1" and (fa.expt & fa.frac) < (fb.expt & fb.frac)) then
      fc.sign := "1";
    else
      fc.sign := "0";
    end if;

    ---------------------------------------------------------------------------

    if fa.expt > fb.expt then
      fbig   := fa;
      fsmall := fb;
    else
      fbig   := fb;
      fsmall := fa;
    end if;

    expdiff   := to_integer(fbig.expt - fsmall.expt);
    bigfrac   := "1" & fbig.frac;
    if expdiff < 24 then
      smallfrac := shift_right("1" & fsmall.frac, expdiff);
    else
      smallfrac := (others => '0');
    end if;

    if fa.sign /= fb.sign and smallfrac > bigfrac then
      rawfrac := resize(smallfrac, 25) - resize(bigfrac, 25);
    elsif fa.sign /= fb.sign and smallfrac <= bigfrac then
      rawfrac := resize(bigfrac, 25) - resize(smallfrac, 25);
    else
      rawfrac :=  resize(smallfrac, 25) + resize(bigfrac, 25);
    end if;

    lzc := leading_zero(rawfrac);

    if lzc = 0 then
      fc.expt := fbig.expt + 1;
    elsif lzc = 25 or fbig.expt < lzc then
      fc.expt := (others => '0');
    else
      fc.expt := fbig.expt - (lzc - 1);
    end if;

    if lzc = 0 then
      fc.frac := rawfrac(23 downto 1);
    elsif lzc = 25 or fbig.expt < lzc then
      fc.frac := (others => '0');
    else
      fc.frac := shift_left(rawfrac, lzc - 1)(22 downto 0);
    end if;

    ---------------------------------------------------------------------------

    if fa.expt = 0 then
      return b;
    elsif fb.expt = 0 then
      return a;
    else
      return fpu_data(fc);
    end if;

  end function;

  function fsub (
    a : fpu_data_t;
    b : fpu_data_t)
    return fpu_data_t is
    variable nb  : fpu_data_t;
  begin
    nb := (not b(31 downto 31)) & b(30 downto 0);
    return fadd(a, nb);
  end function;
  
end package body fadd_p;
