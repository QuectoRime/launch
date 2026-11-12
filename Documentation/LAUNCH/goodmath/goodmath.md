# `goodmath`

## Constants
<small>Defined in `goodmath_impl.h`</small>
|Constant|Value|
|---|---|
|`quiet_nan`|`std::numeric_limits<double>::quiet_NaN()`|
|`signaling_nan`|`std::numeric_limits<double>::signaling_NaN()`|
|`nan`|`quiet_nan`|
|`infinity`|`std::numeric_limits<double>::infinity()`|

## Functions
<small>Defined in `goodmath_impl.h`</small>
|Function|Description|
|---|---|
|`sign_bit`|returns the sign bit of the number|
|`exponent_bits`|returns the exponent bits of the number|
|`mantissa_bits`|returns the mantissa bits of the number|
|`issub`|tells if the number is a subnormal number|
|`isinf`|tells if the number is infinity|
|`isnan`|tells if the number is NaN|
|`abs`|`constexpr` version of `std::abs`|
|`feq`|tells if the two numbers are roughly equal|
|`exponent`|returns the exponent of the number|
|`mantissa`|returns the mantissa of the number|
|`mod`|`constexpr` version of `std::fmod`|
|`trunc`|`constexpr` version of `std::trunc`|
|`floor`|`constexpr` version of `std::floor`|
|`ceil`|`constexpr` version of `std::ceil`|
|`round`|`constexpr` version of `std::round`|
|`is2pos4`|tells if the number is too positive for type `T`|
|`is2neg4`|tells if the number is too negative for type `T`|
|`itrunc`|`constexpr` version of `std::trunc`, returns `long long`|
|`ifloor`|`constexpr` version of `std::floor`, returns `long long`|
|`iceil`|`constexpr` version of `std::ceil`, returns `long long`|
|`iround`|`constexpr` version of `std::round`, returns `long long`|
|`isint`|tells if the number is an integer|
|`ipow`|calculates the number to the power of n, in which n is an integer|
|`ln`|`constexpr` version of `std::log`|
|`log`|calculates the logarithm of the number with the given base|
|`lg`|`constexpr` version of `std::log10`|
|`log2`|`constexpr` version of `std::log2`|
|`fac`|calculates the factorial of the number|
|`exp`|`constexpr` version of `std::exp`|
|`pow`|`constexpr` version of `std::pow`|
|`nrt`|calculates the nth root of the number|
|`sqrt`|`constexpr` version of `std::sqrt`|
|`cbrt`|`constexpr` version of `std::cbrt`|
|`hypot`|`constexpr` version of `std::hypot`|
|`dtor`|converts degrees to radians|
|`rtod`|converts radians to degrees|
|`sin`|`constexpr` version of `std::sin`|
|`cos`|`constexpr` version of `std::cos`|
|`tan`|`constexpr` version of `std::tan`|
|`arcsin`|`constexpr` version of `std::asin`|
|`arccos`|`constexpr` version of `std::acos`|
|`arctan`|`constexpr` version of `std::atan`|
|`arctan2`|`constexpr` version of `std::atan2`|
|`sinh`|`constexpr` version of `std::sinh`|
|`cosh`|`constexpr` version of `std::cosh`|
|`tanh`|`constexpr` version of `std::tanh`|
|`arsinh`|`constexpr` version of `std::asinh`|
|`arcosh`|`constexpr` version of `std::acosh`|
|`artanh`|`constexpr` version of `std::atanh`|

## Note
In `ln`, `exp`, `sin`, `arcsin` and other functions that depend on them, the infinite series are approximated with 16 terms. However, in most cases, the precision they provide is sufficient.