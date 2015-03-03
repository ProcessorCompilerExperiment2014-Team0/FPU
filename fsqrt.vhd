-------------------------------------------------------------------------------
-- Declaration
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.fpu_common_p.all;

package fsqrt_p is

  component fsqrt is
    port (
      clk   : in  std_logic;
      xrst  : in  std_logic;
      stall : in  std_logic;
      a     : in  unsigned(31 downto 0);
      s     : out unsigned(31 downto 0));
  end component;

end package;

-------------------------------------------------------------------------------
-- Table
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.fpu_common_p.all;
use work.table_p.all;

entity fsqrt_table_rom is
  port (
    clk  : in  std_logic;
    en   : in  std_logic;
    addr : in  unsigned(9 downto 0);
    data : out unsigned(35 downto 0));
end fsqrt_table_rom;

architecture behavior of fsqrt_table_rom is

  signal rom: fsqrt_table_t := fsqrt_table;

begin
  process(clk)
  begin
    if rising_edge(clk) then
      if en = '1' then
        data <= ROM(to_integer(addr));
      end if;
    end if;
  end process;
end behavior;

-------------------------------------------------------------------------------
-- Definition
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.fpu_common_p.all;
use work.fsqrt_p.all;

entity fsqrt is
  port (
    clk   : in  std_logic;
    xrst  : in  std_logic;
    stall : in  std_logic;
    a     : in  unsigned(31 downto 0);
    s     : out unsigned(31 downto 0));
end fsqrt;

architecture behavior of fsqrt is

  component fsqrt_table_rom is
    port (
      clk  : in  std_logic;
      en   : in  std_logic;
      addr : in  unsigned(9 downto 0);
      data : out unsigned(35 downto 0));
  end component;

  signal rom_en      : std_logic := '0';
  signal rom_addr    : unsigned(9 downto 0);
  signal rom_data    : unsigned(35 downto 0);

  type state_t is (NORMAL, CORNER);

  type latch_t is record
    state0, state1 : state_t;
    bridge0        : fpu_data_t;
    bridge1        : fpu_data_t;
    y              : unsigned(22 downto 0);
    temp_expt      : unsigned(7 downto 0);
    temp_frac      : unsigned(13 downto 0);
  end record latch_t;

  constant latch_init : latch_t := (
    state0    => CORNER,
    state1    => CORNER,
    bridge0   => (others => '-'),
    bridge1   => (others => '-'),
    y         => (others => '-'),
    temp_expt => (others => '-'),
    temp_frac => (others => '-'));

  signal r, rin : latch_t;

begin

  table : fsqrt_table_rom port map(
    clk  => clk,
    en   => rom_en,
    addr => rom_addr,
    data => rom_data);

  comb : process (r, a, stall, rom_data) is

    variable v    : latch_t;
    -- variables for 1st stage
    variable en   : std_logic;
    variable addr : unsigned(9 downto 0);
    variable f    : float_t;
    -- variables for 2nd stage
    variable h : float_t;
    variable d, n: unsigned(13 downto 0);
    -- variables for 3rd stage
    variable g: float_t;
    variable g_frac_25: unsigned(24 downto 0);
    variable ans: unsigned(31 downto 0);

  begin
    v        := r;
    en       := '0';
    addr     := (others => '-');

    if stall = '1' then
      rom_en   <= en;
      rom_addr <= addr;
      s        <= (others => '-');
    else
      -- 1st stage
      v.state0 := CORNER;

      if is_metavalue(a) then
        v.bridge0 := VAL_NAN;
      else
        f := float(a);
        case float_type(f) is
          when NAN =>
            if f.sign = "0" then
              v.bridge0 := VAL_NAN;
            else
              v.bridge0 := VAL_MINUS_NAN;
            end if;
          when INFORMAL =>
            if f.sign = "0" then
              v.bridge0 := VAL_PLUS_ZERO;
            else
              v.bridge0 := VAL_MINUS_ZERO;
            end if;
          when PLUS_INF   => v.bridge0 := VAL_PLUS_INF;
          when MINUS_INF  => v.bridge0 := VAL_MINUS_NAN;  -- 新たにVAL_MINUS_NANを追加
          when PLUS_ZERO  => v.bridge0 := VAL_PLUS_ZERO;
          when MINUS_ZERO => v.bridge0 := VAL_MINUS_ZERO;
          when others =>
            if f.sign = "1" then
              v.bridge0 := VAL_MINUS_NAN;
            else
              v.state0  := NORMAL;
              v.bridge0 := unsigned(a);
              en        := '1';
              addr      := (not f.expt(0)) & f.frac(22 downto 14);
            end if;
        end case;
      end if;

      rom_en   <= en;
      rom_addr <= addr;

      -- 2nd stage
      v.state1  := r.state0;
      v.bridge1 := r.bridge0;

      h           := float(r.bridge0);
      d           := (not h.expt(0)) & rom_data(12 downto 0);
      n           := h.frac(13 downto 0);
      v.y         := rom_data(35 downto 13);
      v.temp_frac := resize(shift_right(d * n, 14), 14);

      if h.expt >= 127 then
        v.temp_expt := 127 + shift_right(h.expt - 127, 1);
      else
        v.temp_expt := 127 - shift_right(128 - h.expt, 1);
      end if;

      -- 3rd stage
      g.sign := "0";
      g.expt := r.temp_expt;
      g.frac := r.y + r.temp_frac;

      case r.state1 is
        when CORNER =>
          ans := r.bridge1;
        when NORMAL =>
          ans := fpu_data(g);
      end case;

      s <= ans;
    end if;

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
