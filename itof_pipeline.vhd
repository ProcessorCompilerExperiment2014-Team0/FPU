-------------------------------------------------------------------------------
-- Declaration
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

package itof_pipeline_p is

  component itof_pipeline is
    port (
      clk   : in  std_logic;
      xrst  : in  std_logic;
      stall : in  std_logic;
      a     : in  unsigned(31 downto 0);
      s     : out unsigned(31 downto 0));
  end component itof_pipeline;

end package itof_pipeline_p;

-------------------------------------------------------------------------------
-- Definition
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.fpu_common_p.all;
use work.itof_pipeline_p.all;

entity itof_pipeline is
    port (
      clk   : in std_logic;
      xrst  : in std_logic;
      stall : in std_logic;
      a     : in unsigned(31 downto 0);
      s     : out unsigned(31 downto 0));
end entity itof_pipeline;

architecture behavior of itof_pipeline is

  function or_nbit_31 (
    a : unsigned(30 downto 0);
    n : integer range 2 to 6)
    return unsigned is
    variable cond : boolean;
  begin

    case n is
      when 2  => cond := a(1 downto 0) = 0;
      when 3  => cond := a(2 downto 0) = 0;
      when 4  => cond := a(3 downto 0) = 0;
      when 5  => cond := a(4 downto 0) = 0;
      when 6  => cond := a(5 downto 0) = 0;
    end case;

    if cond then return "0"; else return "1"; end if;

  end function or_nbit_31;

  type state_t is (CORNER, NORMAL);

  type latch_t is record
    -- stage 1
    state0    : state_t;
    data0     : fpu_data_t;
    sign0     : unsigned(0 downto 0);
    -- stage 2
    state1    : state_t;
    data1     : fpu_data_t;
    sign1     : unsigned(0 downto 0);
    expt1     : unsigned(7 downto 0);
    frac_grs1 : unsigned(25 downto 0);
  end record latch_t;

  constant latch_init : latch_t := (
    state0    => CORNER,
    data0     => (others => '0'),
    sign0     => (others => '0'),
    state1    => CORNER,
    data1     => (others => '0'),
    sign1     => (others => '0'),
    expt1     => (others => '0'),
    frac_grs1 => (others => '-'));

  signal r, rin : latch_t := latch_init;

begin

  comb: process (r, a, stall) is
    variable v: latch_t;

    variable result : float_t;
    variable i      : integer range 0 to 30;

  begin
    v := r;

    if stall /= '1' then
      -------------------------------------------------------------------------
      -- Stage 1
      -------------------------------------------------------------------------

      v.sign0 := a(31 downto 31);

      if is_metavalue(a) then
        v.state0 := CORNER;
        v.data0  := x"00000000";
      elsif a = 0 then
        v.state0 := CORNER;
        v.data0  := x"00000000";
      elsif a = x"80000000" then
        v.state0 := CORNER;
        v.data0  := x"cf000000";  -- -1 * 2 ^ 32
      else
        v.state0 := NORMAL;

        if a(31 downto 31) = 0 then
          v.data0 := a;
        else
          v.data0 := unsigned(- signed(a));
        end if;
      end if;

      -------------------------------------------------------------------------
      -- Stage 2
      -------------------------------------------------------------------------

      i := leading_zero_negative(r.data0(30 downto 0));

      v.state1 := r.state0;
      v.data1  := r.data0;
      v.sign1  := r.sign0;
      v.expt1  := to_unsigned(127 + i, 8);

      if i < 24 then
        v.frac_grs1 := shift_left(r.data0(22 downto 0), 23 - i) & "000";
      elsif i = 24 then
        v.frac_grs1 := r.data0(23 downto 0) & "00";
      elsif i = 25 then
        v.frac_grs1 := r.data0(24 downto 0) & "0";
      elsif i = 26 then
        v.frac_grs1 := r.data0(25 downto 0);
      else
        v.frac_grs1 := resize(shift_right(r.data0, i-25), 25) & or_nbit_31(r.data0(30 downto 0), i-25);
      end if;

      -------------------------------------------------------------------------
      -- Stage 3
      -------------------------------------------------------------------------
      result.sign := r.sign1;
      result.expt := r.expt1 + round_even_carry_26bit(r.frac_grs1);
      result.frac := round_even_26bit(r.frac_grs1);

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
