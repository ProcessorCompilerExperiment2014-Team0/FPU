library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.fpu_common_p.all;

package itof_p is

  function itof(a: fpu_data_t) return fpu_data_t;

end package;

package body itof_p is

  function round_even_26bit(n: fpu_data_t)
    return fpu_data_t is

    variable right4: fpu_data_t;
    variable num: fpu_data_t;
  begin

    num := n;
    right4 := num and x"0000000f";

    if (4 < right4 and right4 < 8) or 11 < right4 then
      num := shift_right(num, 3) + 1;
    else
      num := shift_right(num, 3);
    end if;

    return num;

  end function;


  function round_even_carry_26bit(num: fpu_data_t)
    return fpu_data_t is
  begin
    if x"3fffffc" <= num and num <= x"3ffffff" then
      return x"00000001";
    else
      return x"00000000";
    end if;
  end function;


  function or_nbit(a: fpu_data_t;
                   n: integer range 0 to 31)
    return fpu_data_t is
  begin

    if a(n-1 downto 0) > 0 then
      return x"00000001";
    else
      return x"00000000";
    end if;

  end function;

  function itof(a: fpu_data_t) return fpu_data_t is

    variable a_32bit, result: float_t;
    variable flag: unsigned(0 downto 0);
    variable i: integer range 0 to 30;
    variable frac: fpu_data_t;
    variable frac_grs: fpu_data_t;
    variable s_bit: fpu_data_t;
    variable temp: fpu_data_t;

  begin

    if is_metavalue(a) then
      return (others => 'X');
    end if;

    a_32bit := float(a);
    flag := a_32bit.sign;

    if a = 0 then
      result := float(x"00000000");
    elsif a = x"80000000" then
      result.sign := "1";
      result.expt := x"9e";
      result.frac := (others => '0');
    else
      if flag = 0 then
        temp := a;
      else
        temp := not ((a and x"7fffffff") - 1);
      end if;

      i := 30;
      while i > 0 and temp(i) = '0' loop
        i := i - 1;
      end loop;

      result.sign := flag;
      result.expt  := to_unsigned(127 + i, 8);

      if i < 24 then
        frac := temp and x"007FFFFF";
        frac := shift_left(frac, 23-i);
        result.frac := frac(22 downto 0);
      else
        if 23 < i and i < 27 then
          temp := temp((i-1) downto 0);
          frac_grs := shift_left(temp,26-i);
        else
          s_bit := or_nbit(temp, i-25);
          frac_grs := shift_left((shift_right(temp,i-25) and x"1ffffff"), 1);
          frac_grs := frac_grs or s_bit;
        end if;

        result.frac := round_even_26bit(frac_grs)(22 downto 0);
        if round_even_carry_26bit(frac_grs) = 1 then
          result.expt := result.expt + 1;
        end if;
      end if;
    end if;

    return fpu_data(result);
  end function;

end package body;
