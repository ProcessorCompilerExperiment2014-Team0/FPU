-------------------------------------------------------------------------------
-- Declaration
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

package fmul_pipeline_p is

  component fmul_pipeline is
    port (
      clk   : in  std_logic;
      xrst  : in  std_logic;
      stall : in  std_logic;
      a     : in  unsigned(31 downto 0);
      b     : in  unsigned(31 downto 0);
      s     : out unsigned(31 downto 0));
  end component fmul_pipeline;

end package fmul_pipeline_p;

-------------------------------------------------------------------------------
-- Definition
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.fpu_common_p.all;
use work.fmul_pipeline_p.all;

entity fmul_pipeline is
  port (
    clk   : in  std_logic;
    xrst  : in  std_logic;
    stall : in  std_logic;
    a     : in  unsigned(31 downto 0);
    b     : in  unsigned(31 downto 0);
    s     : out unsigned(31 downto 0));
end entity fmul_pipeline;

architecture behavior of fmul_pipeline is

  type state_t is (CORNER, NORMAL);

  type latch_t is record
    -- stage 1
    state0 : state_t;
    data0  : fpu_data_t;
    sign0  : unsigned(31 downto 31);
    exp0   : unsigned(8 downto 0);
    ah_bh  : unsigned(27 downto 0);
    ah_bl  : unsigned(17 downto 0);
    al_bh  : unsigned(17 downto 0);
    -- stage 2
    state1 : state_t;
    data1  : fpu_data_t;
    sign1  : unsigned(31 downto 31);
    exp1   : unsigned(8 downto 0);
    frac   : unsigned(22 downto 0);
  end record latch_t;

  constant latch_init : latch_t := (
    state0 => CORNER,
    data0  => (others => '0'),
    sign0  => (others => '0'),
    exp0   => (others => '0'),
    ah_bh  => (others => '0'),
    ah_bl  => (others => '0'),
    al_bh  => (others => '0'),
    state1 => CORNER,
    data1  => (others => '0'),
    sign1  => (others => '0'),
    exp1   => (others => '0'),
    frac   => (others => '0'));

  signal r, rin : latch_t := latch_init;

begin

  comb: process (r, a, b, stall) is

    variable v: latch_t;

    -- stage 1
    variable fa      : float_t;
    variable fb      : float_t;
    variable a_hmant : unsigned(13 downto 0);
    variable b_hmant : unsigned(13 downto 0);
    variable a_lmant : unsigned(13 downto 0);  -- 14bit。0詰め必要。
    variable b_lmant : unsigned(13 downto 0);
    -- stage 2
    variable product : unsigned(27 downto 0);
    variable mant    : unsigned(25 downto 0);
    -- stage 3
    variable exp     : unsigned(8 downto 0);
    variable result  : float_t;


  begin
    v      := r;
    result := float(x"00000000");

    if stall /= '1' then
      -- stage 1
      if is_metavalue(a) or is_metavalue(b) then
        fa := float(x"00000000");
        fb := float(x"00000000");
      else
        fa := float(a);
        fb := float(b);
      end if;

      v.state0 := CORNER;
      v.data0  := (others => '-');

      if (fa.expt = 255 and fa.frac /= 0) or
        (fb.expt = 255 and fb.frac /= 0) then
        v.data0 := VAL_NAN;
      elsif (fa.expt = 255 and fb.expt = 0) or
        (fa.expt = 0 and fb.expt = 255) then
        v.data0 := VAL_NAN;
      elsif fa.expt = 255 or fb.expt = 255 then
        if fa.sign = fb.sign then
          v.data0 := VAL_PLUS_INF;
        else
          v.data0 := VAL_MINUS_INF;
        end if;
      elsif fa.expt = 0 or fb.expt = 0 then
        if fa.sign = fb.sign then
          v.data0 := VAL_PLUS_ZERO;
        else
          v.data0 := VAL_MINUS_ZERO;
        end if;
      else
        v.state0 := NORMAL;
      end if;

      a_hmant := '1' & fa.frac(22 downto 10);
      b_hmant := '1' & fb.frac(22 downto 10);
      a_lmant := "0000" & fa.frac(9 downto 0);
      b_lmant := "0000" & fb.frac(9 downto 0);

      v.sign0 := fa.sign xor fb.sign;
      v.exp0  := resize(fa.expt, 9) + resize(fb.expt, 9);  -- -127が必要だが0未満になると困るので後で。
      v.ah_bh := a_hmant * b_hmant;  -- 14bit * 14bit = 28bit
      v.ah_bl := resize(shift_right(a_hmant * b_lmant, 10), 18);
      v.al_bh := resize(shift_right(a_lmant * b_hmant, 10), 18);

      -- stage 2
      v.state1 := r.state0;
      v.data1  := r.data0;
      v.sign1  := r.sign0;

      product := r.ah_bh + r.ah_bl + r.al_bh;

      v.exp1 := r.exp0 + product(27 downto 27);

      if product(27) = '1' then         -- 繰り上がりありの場合
        mant   := product(26 downto 2) & (product(1) or product(0));
        v.exp1 := v.exp1 + round_even_carry_26bit(mant);
        v.frac := round_even_26bit(mant);
      else
        mant   := product(25 downto 0);
        v.exp1 := v.exp1 + round_even_carry_26bit(mant);
        v.frac := round_even_26bit(mant);
      end if;

      -- stage 3
      result.sign := r.sign1;

      -- exp-127 が指数部に実際に使う値
      if r.exp1 > 127 then -- 指数部が正の場合
        if r.exp1 > 381 then
          result.expt := to_unsigned(255, 8);
          result.frac := to_unsigned(0, 23);
        else
          exp := r.exp1 - 127;
          result.expt := exp(7 downto 0);
          result.frac := r.frac;
        end if;
      else  -- 指数部が0以下になってしまう場合
        result.expt := to_unsigned(0, 8);
        result.frac := to_unsigned(0, 23);
      end if;
    end if;

    case r.state1 is
      when CORNER => s <= r.data1;
      when NORMAL => s <= fpu_data(result);
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
