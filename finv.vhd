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
-- Table
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.fpu_common_p.all;
use work.table_p.all;

entity finv_table_rom is
  port (
    clk  : in  std_logic;
    en   : in  std_logic;
    addr : in  unsigned(9 downto 0);
    data : out unsigned(35 downto 0));
end finv_table_rom;

architecture behavior of finv_table_rom is

  signal rom: finv_table_t := finv_table;

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
use work.finv_p.all;

entity finv is
  port (
    clk   : in  std_logic;
    xrst  : in  std_logic;
    stall : in  std_logic;
    a     : in  unsigned(31 downto 0);
    s     : out unsigned(31 downto 0));
end finv;

architecture behavior of finv is

  component finv_table_rom is
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
    state0, state1   : state_t;
    bridge0, bridge1 : fpu_data_t;
    data             : unsigned(35 downto 0);
  end record latch_t;

  constant latch_init : latch_t := (
    state0  => CORNER,
    state1  => CORNER,
    bridge0 => (others => '-'),
    bridge1 => (others => '-'),
    data    => (others => '-'));

  signal r, rin : latch_t;

begin

  table : finv_table_rom port map(
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
    -- variables for 3rd stage
    variable g, h      : float_t;
    variable g_frac_25 : unsigned(24 downto 0);
    variable y         : unsigned(22 downto 0);
    variable d         : unsigned(12 downto 0);
    variable ans       : unsigned(31 downto 0);
    variable temp_frac : unsigned(26 downto 0);
    variable q         : unsigned(13 downto 0);

  begin
    v        := r;
    en       := '0';
    addr     := (others => '-');

    if stall = '1' then
      rom_en   <= en;
      rom_addr <= addr;
    else
      -- 1st stage
      v.state0 := CORNER;

      if is_metavalue(a) then
        v.bridge0 := VAL_NAN;
      else
        f := float(a);
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
            en        := '1';
            addr      := f.frac(22 downto 13);
            v.state0  := NORMAL;
            v.bridge0 := unsigned(a);
        end case;
      end if;

      rom_en   <= en;
      rom_addr <= addr;

      -- 2nd stage
      v.bridge1 := r.bridge0;
      v.state1  := r.state0;
      v.data    := rom_data;

      -- 3rd stage
      case r.state1 is
        when CORNER =>
          ans := r.bridge1;
        when NORMAL =>
          h := float(r.bridge1);
          if is_metavalue(fpu_data(h)) then
            ans := VAL_NAN;
          elsif h.frac = 0 then
            g.sign := h.sign;
            g.expt := 254 - h.expt;
            g.frac := (others => '0');
            ans    := fpu_data(g);
          elsif h.expt = 254 then
            if h.sign = "1" then
              ans := VAL_MINUS_ZERO;
            else
              ans := VAL_PLUS_ZERO;
            end if;
          else
            y         := r.data(35 downto 13);
            d         := r.data(12 downto 0);
            g.sign    := h.sign;
            g.expt    := 253 - h.expt;
            q         := '0' & h.frac(12 downto 0);
            temp_frac := shift_right((d * (8192 - q) + 1), 12);
            g.frac    := y + temp_frac(22 downto 0);
            ans       := fpu_data(g);
          end if;
      end case;

      s   <= ans;
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
