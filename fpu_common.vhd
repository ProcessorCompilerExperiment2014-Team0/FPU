library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

package fpu_common_p is

  subtype fpu_data_t is unsigned(31 downto 0);

  type float_t is record
    sign: unsigned(0 downto 0);
    expt: unsigned(7 downto 0);
    frac: unsigned(22 downto 0);
  end record;

  function float(v: fpu_data_t) return float_t;
  function fpu_data(v: float_t) return fpu_data_t;


  type float_type_t is (
    PLUS_ZERO,
    MINUS_ZERO,
    FORMAL,
    INFORMAL,
    PLUS_INF,
    MINUS_INF,
    NAN);

  function float_type(f: float_t) return float_type_t;

  function is_metavalue(v: std_logic_vector) return boolean;
  function is_metavalue(v: unsigned) return boolean;

end package;

package body fpu_common_p is

  function float(v: fpu_data_t)
    return float_t is
    variable f: float_t;
  begin

    f.sign := v(31 downto 31);
    f.expt := v(30 downto 23);
    f.frac := v(22 downto 0);

    return f;

  end function;

  function fpu_data(v: float_t)
    return fpu_data_t is
  begin

    return v.sign & v.expt & v.frac;

  end function;

  function float_type(f: float_t)
    return float_type_t is
  begin

    if f.expt = 0 and f.frac = 0 then
      if f.sign = 0 then
        return PLUS_ZERO;
      else
        return MINUS_ZERO;
      end if;
    elsif f.expt = 0 then
      return INFORMAL;
    elsif f.expt = 255 and f.frac = 0 then
      if f.sign = 0 then
        return PLUS_INF;
      else
        return MINUS_INF;
      end if;
    elsif f.expt = 255 then
      return NAN;
    else
      return FORMAL;
    end if;

  end function;

  function is_metavalue(v: std_logic_vector)
    return boolean is
  begin

    for i in v'range loop
      if v(i) /= '0' and v(i) /= '1' then
        return true;
      end if;
    end loop;

    return false;

  end function;

  function is_metavalue(v: unsigned)
    return boolean is
  begin
    return is_metavalue(std_logic_vector(v));
  end function;

end package body;
