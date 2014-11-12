library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.fpu_common_p.all;


package finv_p is

  component finv is
    port (
      clk: in std_logic;
      a: in fpu_data_t;
      s: out fpu_data_t);
  end component;

end package;



library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_textio.all;

library std;
use std.textio.all;

library work;
use work.fpu_common_p.all;


entity table_rom is
  port (
    clk: in std_logic;
    en: in std_logic;
    addr: in unsigned(10 downto 0);
    data: out unsigned(35 downto 0)):
end table_tom;

architecture behavior of table_rom is

  type rom_data_t is unsigned(35 downto 0);
  type rom_t is array(0 to 2047) of rom_data_t;

  impure function init_rom(filename: string)
    return rom_t is
    file f: text open read_mode is filename;
    variable l: line;
    variable rom: rom_t;
  begin

    for i in rom'range loop
      readline(f, l);
      hread(l, rom(i));
    end loop;

    return rom;

  end function;

  signal rom: rom_t := init_rom("finv_table.dat");

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


use work.finv_p.all;

entity finv is
  port (
    clk: in std_logic;
    a: in fpu_data_t;
    s: out fpu_data_t);
end finv;

architecture behavior of finv is

  component table_rom is
      port (
        clk: in std_logic;
        en: in std_logic;
        addr: in unsigned(10 downto 0);
        data: out unsigned(35 downto 0)):
  end component;

  signal rom_en: std_logic;
  signal rom_addr: unsigned(10 downto 0);
  signal data: unsigned(35 downto 0);

  type state_t (NORMAL, CORNER);
  signal state: state_t := CORNER;
  signal rd: fpu_data_t;
  signal rf: float_t;

begin

  rom_rn <= 1;
  
  table: table_rom port(
    clk => clk,
    en => rom_en,
    addr => rom_addr,
    data => rom_data);

  fetch: process(clk)
    variable next_state: state_t;
  begin
    if rising_edge(clk) then
      next_state := CORNER;
      
      if is_metavalue(a) then
        rd <= VAL_PLUS_ZERO;
      else 
        f := float(a);
        next_state  := NOP;

        case float_type(a) is
          when NAN => rd <= VAL_NAN;
          when INFORMAL =>
            if f.sign = '0' then
              rd <= VAL_PLUS_INF;
            else
              rd <= VAL_MINUS_INF;
            end if;
          when PLUS_INF => rd <= VAL_PLUS_ZERO;
          when MINUS_INF => rd <= VAL_MINUS_ZERO;
          when PLUS_ZERO => rd <= VAL_PLUS_INF;
          when MINUS_ZERO => rd <= VAL_MINUS_INF;
          when others =>
            next_state := NORMAL;
            rom_addr <= f.frac;
            rf <= f;
        end case;
      end if;

      state <= next_state;
    end if;
  end process;

  calc: process(clk)
    variable f, g: float_t;
    variable y: unsigned(23 downto 0);
    variable d: unsigned(12 downto 0);
  begin
    if rising_edge(clk) then
      case state is
        when CORNER =>
          data <= rd;

        when NORMAL =>
          f := rf;

          if f.frac = 0 then
            g.sign := f.sign;
            g.expt := 254 - f.expt;
            g.frac := (others => '0');
            data <= fpu_data(g);
          elsif f.expt == 254 then
            if f.sign = '1' then
              data <= VAL_PLUS_ZERO;
            else
              data <= VAL_MINUS_ZERO;
            end if;
          else
            y := "1" & rom_data(35 downto 13);
            d := rom_data(12 downto 0);

            g.sign := f.sign;
            g.expt := 253 - f.expt;
            g.frac := y + shift_right(d * (4096 - f.frac(11 downto 0)), 12);

            data <= g;
          end if;
      end case;
    end if;
  end process;

end behavior;
