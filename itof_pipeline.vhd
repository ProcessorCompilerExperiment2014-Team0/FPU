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

  type latch_t is record
    a : unsigned(31 downto 0);
    s : unsigned(31 downto 0);
  end record latch_t;

  constant latch_init : latch_t := (
    a => (others => '-'),
    s => (others => '-'));

  signal r, rin : latch_t := latch_init;

begin

  comb: process (r, a, stall) is
    variable v: latch_t;

    variable fa, result : float_t;
    variable i               : integer range 0 to 30;
    variable frac_grs        : unsigned(25 downto 0);
    variable temp            : unsigned(30 downto 0);
  begin
    v := r;

    if stall /= '1' then
      -------------------------------------------------------------------------
      -- Stage 1
      -------------------------------------------------------------------------
      v.a    := a;

      -------------------------------------------------------------------------
      -- Stage 2
      -------------------------------------------------------------------------
      fa := float(r.a);

      if is_metavalue(r.a) or r.a = 0 then
        result := float(x"00000000");
      elsif r.a = x"80000000" then
        result.sign := "1";
        result.expt := x"9e";
        result.frac := (others => '0');
      else
        if fa.sign = 0 then
          temp := fa.expt & fa.frac;
        else
          temp := unsigned(- signed(fa.expt & fa.frac));
        end if;

        i := leading_zero_negative(temp);

        result.sign := fa.sign;
        result.expt := to_unsigned(127 + i, 8);

        if i < 24 then
          result.frac := shift_left(temp(22 downto 0), 23-i);
        else
          if i = 24 then
            frac_grs := temp(23 downto 0) & "00";
          elsif i = 25 then
            frac_grs := temp(24 downto 0) & "0";
          elsif i = 26 then
            frac_grs := temp(25 downto 0);
          else
            frac_grs := resize(shift_right(temp, i-25), 25) & to_unsigned(or_nbit_31(temp, i-25), 1);
          end if;

          result.frac := round_even_26bit(frac_grs);
          if round_even_carry_26bit(frac_grs) = 1 then
            result.expt := result.expt + 1;
          end if;
        end if;
      end if;

      v.s    := fpu_data(result);
    end if;

    ---------------------------------------------------------------------------
    -- Stage 3
    ---------------------------------------------------------------------------
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
