# `fmtin`
<small>Defined in `fmtio_impl.h`</small>

## `void fmtin(const hedgehog& cont, std::istream& in = std::cin)`
Reads elements and puts them into references stored in `cont`.

### Parameters
**`cont`** - the target `hedgehog` with references to be filled.  
**`in`** - the input stream to be read.

### Return value
(none)

### Complexity
Linear.

---

## `template <readable T> void fmtin(T& x, std::istream& in = std::cin)`
Reads one element with type `T` from `in` and puts it into `x`.  
The program is ill-formed if `readable<T>` is `false`.

### Template parameters
**`T`** - type of the element to be read.

### Parameters
**`x`** - the variable to be filled.  
**`in`** - the input stream to be read.

### Return value
(none)

### Complexity
Constant.