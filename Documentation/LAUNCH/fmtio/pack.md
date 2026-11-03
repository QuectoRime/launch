# `pack`
<small>Defined in `fmtio_impl.h`</small>

## `template <readable... Ts> hedgehog pack(Ts&... args)`
Packs the given references into a `hedgehog` as `std::reference_wrapper`s.

### Template parameters
**`Ts`** - types of references.

### Parameters
**`args`** - references to be packed.

### Return value
A `hedgehog` with given references as `std::reference_wrapper`s.

### Complexity
Linear.

### Note
`pack` is not intended to (and rarely could) be used with `fmtout`.