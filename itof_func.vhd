
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.fpu_common_p.all;

package itof_p is

  function itof(a: fpu_data_t) return fpu_data_t;

end package;

package body itof_p is

  function round_even_26bit(n: unsigned(25 downto 0))
    return unsigned is

    variable right4: unsigned(3 downto 0);
  begin

    right4 := n(3 downto 0);

    if (4 < right4 and right4 < 8) or 11 < right4 then
      return n(25 downto 3) + 1;
    else
      return n(25 downto 3);
    end if;

  end function;

  function round_even_carry_26bit(num: unsigned(25 downto 0))
    return fpu_data_t is
  begin
    if x"3fffffc" <= num and num <= x"3ffffff" then
      return x"00000001";
    else
      return x"00000000";
    end if;
  end function;

  function itof(a: fpu_data_t) return fpu_data_t is

    variable a_32bit, result : float_t;
    variable flag            : unsigned(0 downto 0);
    variable i               : integer range 0 to 30;
    variable frac            : unsigned(22 downto 0);
    variable frac_grs        : unsigned(25 downto 0);
    variable temp            : unsigned(30 downto 0);

  begin

    if is_metavalue(a) then
      return (others => 'X');
    end if;

    a_32bit := float(a);
    flag    := a_32bit.sign;

    if a = 0 then
      result := float(x"00000000");
    elsif a = x"80000000" then
      result.sign := "1";
      result.expt := x"9e";
      result.frac := (others => '0');
    else
      if flag = 0 then
        temp := a(30 downto 0);
      else
        temp := unsigned(- signed(a(30 downto 0)));
      end if;

      i := leading_zero_negative(temp);

      result.sign := flag;
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

    return fpu_data(result);
  end function;

end package body;
