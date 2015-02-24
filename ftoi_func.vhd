library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.fpu_common_p.all;

package ftoi_p is

  function ftoi(a: fpu_data_t) return fpu_data_t;

end package;

package body ftoi_p is

  function ftoi(a: fpu_data_t)
    return fpu_data_t is

    variable a_32bit : float_t;
    variable result  : fpu_data_t;
    variable flag    : fpu_data_t;
    variable diff    : integer range 0 to 128;
    variable nbit    : fpu_data_t;
    variable n       : integer range 0 to 23;

  begin

    if is_metavalue(a) then
      return (others => 'X');
    end if;

    a_32bit := float(a);
    flag := resize(a_32bit.sign, 32);

    if a_32bit.expt < 127 then
      result := x"00000000";
    else
      diff := to_integer(a_32bit.expt) - 127;

      if diff > 30 then
        if flag = 0 then
          result := x"7fffffff";
        else
          result := x"80000000";
        end if;
      else
        if diff < 23 then
          n := diff;
          nbit := resize(shift_right(a_32bit.frac, 23-n), 32);
        else
          n := 23;
          nbit := resize(a_32bit.frac, 32);
        end if;
        result := shift_left(x"00000001", diff) +
                  shift_left(nbit, diff - n);
      end if;
      if flag = 1 then
        result := (not result) + 1;
      end if;
    end if;

    return result;
  end function;

end package body;
