# `arrat`
<small>Defined in `arithing_impl.h`</small>

## `class arrat;`
`arrat` is a rational class.

### Member functions
|Function|Description|
|---|---|
|(constructor)|constructs `arrat` object<br><small>(public member function)</small>|
|(destructor)|destroys `arrat` object<br><small>(public member function)</small>|
|`operator+=`<br>`operator-=`<br>`operator*=`<br>`operator/=`<br>`operator%=`|compound assignments of two `arrat`s<br><small>(public member functions)</small>|
|`operator+`<br>`operator-`|apply unary operators to `arrat`s<br><small>(public member functions)</small>|
|`adjust`|simplifies an `arrat`<br><small>(private member function)</small>|

### Non-member functions
|Function|Description|
|---|---|
|`operator+`<br>`operator-`<br>`operator*`<br>`operator/`<br>`operator%`|perform arithmetic on two `arrat`s|
|`operator<=>`<br>`operator==`|compare two `arrat`s|
|`operator<<`<br>`operator>>`|serialize and deserialize the value|
|`numerator`<br>`denominator`|get the numerator and denominator of an `arrat`|
|`to_decimal`|converts an `arrat` to decimal|