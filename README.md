# some-templated-containers

**some-templated-containers** is a **header-only C++ library** that extends the standard library with additional container and utility features. It provides:

- A `std::vector` extension for **fast O(1) removal**.
- Three **singleton implementations** (Lazy, Eager, and Explicit).
- **Bitwise and arithmetic operators** for `enum class`.

## Repository Structure

| Folder         | Description                                          |
|----------------|------------------------------------------------------|
| `include/stc/` | Header files for the library components.             |
| `src/`         | Inline implementations of the library components.    |
| `examples/`    | Standalone example files demonstrating each feature. |
| `tests/`       | Contains unit tests and the testing framework.       |

## Features Documentation

| Name/Link | Include statement | Header | Example |
|-----------|-------------------|--------|---------|
| [Swap Back Array](#swap-back-array) | `#include <stc/swap_back_array.h>`    | [Header][swap_back_array.h]    | [Example][swap_back_array_ex]    |
| [Lazy Singleton](#singletons)       | `#include <stc/lazy_singleton.h>`     | [Header][lazy_singleton.h]     | [Example][lazy_singleton_ex]     |
| [Eager Singleton](#singletons)      | `#include <stc/eager_singleton.h>`    | [Header][eager_singleton.h]    | [Example][eager_singleton_ex]    |
| [Explicit Singleton](#singletons)   | `#include <stc/explicit_singleton.h>` | [Header][explicit_singleton.h] | [Example][explicit_singleton_ex] |
| [Enum Operators](#enum-operators)   | `#include <stc/enum_operators.h>`     | [Header][enum_operators.h]     | [Example][enum_operators_ex]     |

[swap_back_array.h]: https://github.com/lvocanson/some-templated-containers/blob/main/include/stc/swap_back_array.h
[lazy_singleton.h]: https://github.com/lvocanson/some-templated-containers/blob/main/include/stc/lazy_singleton.h
[eager_singleton.h]: https://github.com/lvocanson/some-templated-containers/blob/main/include/stc/eager_singleton.h
[explicit_singleton.h]: https://github.com/lvocanson/some-templated-containers/blob/main/include/stc/explicit_singleton.h
[enum_operators.h]: https://github.com/lvocanson/some-templated-containers/blob/main/include/stc/enum_operators.h
[swap_back_array_ex]: https://github.com/lvocanson/some-templated-containers/blob/main/examples/swap_back_array_example.cpp
[lazy_singleton_ex]: https://github.com/lvocanson/some-templated-containers/blob/main/examples/lazy_singleton_example.cpp
[eager_singleton_ex]: https://github.com/lvocanson/some-templated-containers/blob/main/examples/eager_singleton_example.cpp
[explicit_singleton_ex]: https://github.com/lvocanson/some-templated-containers/blob/main/examples/explicit_singleton_example.cpp
[enum_operators_ex]: https://github.com/lvocanson/some-templated-containers/blob/main/examples/enum_operators_example.cpp

### Swap Back Array

Extends `std::vector` to enable **fast O(1) removal** of an element by swapping it with the last element before deletion.

> :warning: **Warning**  
> This optimization sacrifices element order.

> :bulb: **Tip**  
> Ideal for **unordered object lists** that frequently grow and shrink, such as game entities, object pools, or real-time systems.

### Singletons

A **singleton** is a design pattern that ensures a class has only one instance and provides a global point of access to it.

Each variant offers different trade-offs:

| Variant | Pros | Cons |
|---------|------|------|
| `lazy_singleton`     | - Simple to use.<br>- Constructed **on first access**, delaying initialization until needed.     | - **Cannot pass arguments** to the constructor.<br>- Deferred initialization may cause a noticeable delay on first access. |
| `eager_singleton`    | - Constructed **before `main()`**, ensuring immediate availability.                              | - **Cannot pass arguments** to the constructor.<br>- Subject to the [Static Initialization Order Fiasco (SIOF)][siof].     |
| `explicit_singleton` | - Allows **on-demand construction** with arguments.<br>- Supports **destruction & re-creation**. | - Requires **manual instantiation**.<br>- Slight overhead for tracking.                                                    |

> :memo: **Recommendation**  
> To fully leverage singleton syntax, it is advised to use the **[CRTP idiom][crtp]**.

> :bulb: **Info**  
> All singleton instances are **allocated in static memory**, avoiding **heap allocation**.

[siof]: https://en.cppreference.com/w/cpp/language/siof
[crtp]: https://en.cppreference.com/w/cpp/language/crtp

### Enum Operators

This library extends `enum class` (particularly bit flags) by enabling **seamless bitwise and arithmetic operations**.

---

## Building

- As a header-only library, no compilation is needed for usage.
- Simply add the `include/` folder to your compiler’s include paths.

---

## Testing

- The testing framework is in `tests/`.
- To run tests, compile `tests/main.cpp` with the `TESTS_ENABLED` macro defined along with the specific test file for the component you wish to test (e.g. for swap_back_array, compile both `tests/main.cpp` and `tests/swap_back_array_tests.cpp`).

---

## Contributing

Contributions are welcome! Feel free to:

- Fork the repository
- Open issues
- Submit pull requests with improvements
