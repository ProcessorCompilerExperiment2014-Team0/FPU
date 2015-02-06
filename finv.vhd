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
      clk : in  std_logic;
      a   : in  std_logic_vector(31 downto 0);
      s   : out std_logic_vector(31 downto 0));
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

entity table_rom is
  port (
    clk  : in  std_logic;
    en   : in  std_logic;
    addr : in  unsigned(9 downto 0);
    data : out unsigned(35 downto 0));
end table_rom;

architecture behavior of table_rom is

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
    clk : in  std_logic;
    a   : in  std_logic_vector(31 downto 0);
    s   : out std_logic_vector(31 downto 0));
end finv;

architecture behavior of finv is

  component table_rom is
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
  signal state, state2       : state_t   := CORNER;
  signal bridge_data, bridge_data2 : fpu_data_t;

begin

  table : table_rom port map(
    clk  => clk,
    en   => rom_en,
    addr => rom_addr,
    data => rom_data);

  fetch : process(clk)
    variable next_state : state_t;
    variable f          : float_t;
    variable b          : fpu_data_t;
  begin
    if rising_edge(clk) then
      next_state := CORNER;
      if is_metavalue(a) then
        rom_en <= '0';
        b      := VAL_NAN;
      else
        f := float(unsigned(a));
        case float_type(f) is
          when NAN =>
            b := VAL_NAN;
          when INFORMAL =>
            if f.sign = "0" then
              b := VAL_PLUS_INF;
            else
              b := VAL_MINUS_INF;
            end if;
          when PLUS_INF   => b := VAL_PLUS_ZERO;
          when MINUS_INF  => b := VAL_MINUS_ZERO;
          when PLUS_ZERO  => b := VAL_PLUS_INF;
          when MINUS_ZERO => b := VAL_MINUS_INF;
          when others =>
            next_state := NORMAL;
            rom_en     <= '1';
            rom_addr   <= f.frac(22 downto 13);
            b          := unsigned(a);
        end case;
      end if;
      bridge_data <= b;
      bridge_data2 <= bridge_data;
      state       <= next_state;
      state2 <= state;
    end if;
  end process;

  calc : process(clk)
    variable f, g      : float_t;
    variable g_frac_25 : unsigned(24 downto 0);
    variable y         : unsigned(22 downto 0);
    variable d         : unsigned(12 downto 0);
    variable ans       : unsigned(31 downto 0);
    variable temp_frac : unsigned(25 downto 0);
  begin
    if rising_edge(clk) then
      case state2 is
        when CORNER =>
          ans := bridge_data2;
        when NORMAL =>
          f := float(bridge_data2);
          if is_metavalue(fpu_data(f)) then
            ans := VAL_NAN;
          elsif f.frac = 0 then
            g.sign := f.sign;
            g.expt := 254 - f.expt;
            g.frac := (others => '0');
            ans    := fpu_data(g);
          elsif f.expt = 254 then
            if f.sign = "1" then
              ans := VAL_MINUS_ZERO;
            else
              ans := VAL_PLUS_ZERO;
            end if;
          else
            y         := rom_data(35 downto 13);
            d         := rom_data(12 downto 0);
            g.sign    := f.sign;
            g.expt    := 253 - f.expt;
            if f.frac(12 downto 0) = 0 then
              temp_frac := d * 2;
            else
              temp_frac := shift_right((d * (8192 - f.frac(12 downto 0)) + 1), 12);
            end if;
            g.frac    := y + temp_frac(22 downto 0);
            ans       := fpu_data(g);
          end if;
      end case;
      s <= std_logic_vector(ans);
    end if;
  end process;

end behavior;
