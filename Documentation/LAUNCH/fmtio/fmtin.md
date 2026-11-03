# `fmtin`
<small>Defined in `fmtio_impl.h`</small>

## `void fmtin(const std::string& fmt, const hedgehog& cont, std::istream& in = std::cin);`
Reads elements from `in` according to the format string `fmt`, and puts them into references stored in `cont`.

### Parameters
**`fmt`** - the format string. It consists of
- ordinary characters (except `{`, `}`, `[`, `]` and `|`), which are expected to be read unchanged from the input,
- escape sequences `{{`, `}}`, `[[`, `]]` and `||`, which are expected to be literal `{`, `}`, `[`, `]` and `|`s respectively in the input,
- placeholders `|`, which are ignored and could be used to prevent greedy matching, and
- replacement fields.  
Each replacement field has the following format:  
<code>{ *elem-id* <small>(optional)</small> }</code>  
Represents a single element in `cont`.  
*`elem-id`* - specifies the index of the element in `cont` which is going to be filled; if it is omitted, the elements are filled in order.  
The *`elem-id`* s in a format string must all be present or all be omitted. Mixing manual and automatic indexing is an error.  
<code>[ *separator* ]</code>  
Represents all elements in `cont`.  
*`separator`* - specifies the expected separator between elements in the input. Placeholders and escape sequences are also applicable to *`separator`*.  
Must not be empty.

**`cont`** - the target `hedgehog` with references to be filled.  
**`in`** - the input stream to be read.

### Return value
(none)

### Exceptions
Throws `std::format_error` if `fmt` is not a valid format string for the provided `hedgehog`.
Throws `std::runtime_error` if any error occurred during the parsing of the input, or expected characters are not present.

### Complexity
Linear.

### Note
An element must not appear twice in the format string. Implicit appearances in bracket replacement fields also count.

---

## `template <readable T> void fmtin(T& x, std::istream& in = std::cin)`
Reads one variable with type `T` from `in` and puts it into `x`.  
The program is ill-formed if `readable<T>` is `false`.

### Template parameters
**`T`** - type of the variable to be filled.

### Parameters
**`x`** - the variable to be filled.  
**`in`** - the input stream to be read.

### Return value
(none)

### Complexity
Constant.