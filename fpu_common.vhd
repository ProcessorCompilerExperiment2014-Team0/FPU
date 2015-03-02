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

  constant VAL_NAN: fpu_data_t := x"7fffffff";
  constant VAL_MINUS_NAN: fpu_data_t := x"ffc00000";
  constant VAL_PLUS_ZERO: fpu_data_t := x"00000000";
  constant VAL_MINUS_ZERO: fpu_data_t := x"80000000";
  constant VAL_PLUS_INF: fpu_data_t := x"7f800000";
  constant VAL_MINUS_INF: fpu_data_t := x"ff800000";

  function float_type(f: float_t) return float_type_t;

  function is_metavalue(v: std_logic_vector) return boolean;
  function is_metavalue(v: unsigned) return boolean;

  function leading_zero (a : unsigned(25 downto 0)) return integer;
  function or_nbit (a : unsigned(24 downto 0); n : integer range 0 to 25) return integer;

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

  function leading_zero (
    a : unsigned(25 downto 0))
    return integer is
  begin

    if    a(25) = '1' then return 0;
    elsif a(24) = '1' then return 1;
    elsif a(23) = '1' then return 2;
    elsif a(22) = '1' then return 3;
    elsif a(21) = '1' then return 4;
    elsif a(20) = '1' then return 5;
    elsif a(19) = '1' then return 6;
    elsif a(18) = '1' then return 7;
    elsif a(17) = '1' then return 8;
    elsif a(16) = '1' then return 9;
    elsif a(15) = '1' then return 10;
    elsif a(14) = '1' then return 11;
    elsif a(13) = '1' then return 12;
    elsif a(12) = '1' then return 13;
    elsif a(11) = '1' then return 14;
    elsif a(10) = '1' then return 15;
    elsif a(9)  = '1' then return 16;
    elsif a(8)  = '1' then return 17;
    elsif a(7)  = '1' then return 18;
    elsif a(6)  = '1' then return 19;
    elsif a(5)  = '1' then return 20;
    elsif a(4)  = '1' then return 21;
    elsif a(3)  = '1' then return 22;
    elsif a(2)  = '1' then return 23;
    elsif a(1)  = '1' then return 24;
    elsif a(0)  = '1' then return 25;
    else  return 26;  end if;

  end function;


  function or_nbit (
    a : unsigned(24 downto 0);
    n : integer range 0 to 25)
    return integer is
    variable cond : boolean;
  begin

    case n is
      when 0  => cond := true;
      when 1  => cond := a(0 downto 0) = 0;
      when 2  => cond := a(1 downto 0) = 0;
      when 3  => cond := a(2 downto 0) = 0;
      when 4  => cond := a(3 downto 0) = 0;
      when 5  => cond := a(4 downto 0) = 0;
      when 6  => cond := a(5 downto 0) = 0;
      when 7  => cond := a(6 downto 0) = 0;
      when 8  => cond := a(7 downto 0) = 0;
      when 9  => cond := a(8 downto 0) = 0;
      when 10 => cond := a(9 downto 0) = 0;
      when 11 => cond := a(10 downto 0) = 0;
      when 12 => cond := a(11 downto 0) = 0;
      when 13 => cond := a(12 downto 0) = 0;
      when 14 => cond := a(13 downto 0) = 0;
      when 15 => cond := a(14 downto 0) = 0;
      when 16 => cond := a(15 downto 0) = 0;
      when 17 => cond := a(16 downto 0) = 0;
      when 18 => cond := a(17 downto 0) = 0;
      when 19 => cond := a(18 downto 0) = 0;
      when 20 => cond := a(19 downto 0) = 0;
      when 21 => cond := a(20 downto 0) = 0;
      when 22 => cond := a(21 downto 0) = 0;
      when 23 => cond := a(22 downto 0) = 0;
      when 24 => cond := a(23 downto 0) = 0;
      when 25 => cond := a(24 downto 0) = 0;
    end case;

    if cond then return 0; else return 1; end if;

  end function or_nbit;

end package body;
