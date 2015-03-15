-------------------------------------------------------------------------------
-- Declaration
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.fpu_common_p.all;

package finv_p is

  component finv is
    port (
      clk   : in  std_logic;
      xrst  : in  std_logic;
      stall : in  std_logic;
      a     : in  unsigned(31 downto 0);
      s     : out unsigned(31 downto 0));
  end component;

end package;

-------------------------------------------------------------------------------
-- Definition
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.fpu_common_p.all;
use work.finv_p.all;
use work.table_p.all;

entity finv is
  port (
    clk   : in  std_logic;
    xrst  : in  std_logic;
    stall : in  std_logic;
    a     : in  unsigned(31 downto 0);
    s     : out unsigned(31 downto 0));
end finv;

architecture behavior of finv is

  signal finv_table_distram : finv_table_t := finv_table;
  attribute rom_style : string;
  attribute rom_style of finv_table_distram : signal is "distributed";

  type state_t is (NORMAL, CORNER);

  type latch_t is record
    state0, state1   : state_t;
    bridge0, bridge1 : fpu_data_t;
    rom_data         : unsigned(35 downto 0);
    q                : unsigned(13 downto 0);
    y                : unsigned(22 downto 0);
    temp_frac        : unsigned(13 downto 0);
  end record latch_t;

  constant latch_init : latch_t := (
    state0    => CORNER,
    state1    => CORNER,
    bridge0   => (others => '0'),
    bridge1   => (others => '0'),
    rom_data  => (others => '0'),
    q         => (others => '0'),
    y         => (others => '0'),
    temp_frac => (others => '0'));

  signal r, rin : latch_t;

begin

  comb : process (r, a, stall) is

    variable v    : latch_t;
    -- variables for 1st stage
    variable addr : unsigned(9 downto 0);
    variable f    : float_t;
    -- variables for 2nd stage
    variable h    : float_t;
    variable d    : unsigned(12 downto 0);
    variable diff : unsigned(26 downto 0);
    -- variables for 3rd stage
    variable g, k : float_t;

  begin
    v    := r;
    addr := (others => '0');
    g    := float(x"00000000");

    if stall = '0' then
      -- 1st stage
      v.state0 := CORNER;

      if is_metavalue(a) then
        v.bridge0 := VAL_NAN;
      else
        f   := float(a);
        v.q := to_unsigned(8192, 14) - f.frac(12 downto 0);

        case float_type(f) is
          when NAN =>
            v.bridge0 := VAL_NAN;
          when INFORMAL =>
            if f.sign = "0" then
              v.bridge0 := VAL_PLUS_INF;
            else
              v.bridge0 := VAL_MINUS_INF;
            end if;
          when PLUS_INF   => v.bridge0 := VAL_PLUS_ZERO;
          when MINUS_INF  => v.bridge0 := VAL_MINUS_ZERO;
          when PLUS_ZERO  => v.bridge0 := VAL_PLUS_INF;
          when MINUS_ZERO => v.bridge0 := VAL_MINUS_INF;
          when others =>
            addr      := f.frac(22 downto 13);
            v.state0  := NORMAL;
            v.bridge0 := a;
        end case;
      end if;

      v.rom_data := finv_table_distram(to_integer(addr));

      -- 2nd stage
      v.bridge1 := r.bridge0;
      v.state1  := r.state0;

      d           := r.rom_data(12 downto 0);
      diff        := d * r.q;
      v.temp_frac := resize(shift_right(diff + 1, 12), 14);
      v.y         := r.rom_data(35 downto 13);

      -- 3rd stage
      k      := float(r.bridge1);
      g.sign := k.sign;

      if k.frac = 0 then
        g.expt := 254 - k.expt;
        g.frac := (others => '0');
      elsif k.expt = 254 then
        g.expt := (others => '0');
        g.frac := (others => '0');
      else
        g.expt := 253 - k.expt;
        g.frac := r.y + r.temp_frac;
      end if;
    end if;

    case r.state1 is
      when CORNER => s <= r.bridge1;
      when NORMAL => s <= fpu_data(g);
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
