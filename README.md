# OneFileWonders

Random collection of one-file programs in C++ to explore an idea and to provide
me with inspiration further down the line.

## glob.cpp

Inspired by a tsoding livestream, I implemented glob in C++. Build with C++20
features enabled.

- [Link](https://www.youtube.com/watch?v=B2VS_zeuTQ4) to livestream

## binary_heap.cpp

My implementation of the binary heap through a functional-style interface that
only uses type constraints on ranges. Build with C++20 features enabled. Doesn't
have rigorous testing and is intended more as a demonstration of ranges and
constraints in C++20.

- [Wikipedia](https://en.wikipedia.org/wiki/Binary_heap) page

## constexpr_cstr_append.cpp

An implementation of a compile-tile method for appending C-string literals.

## comma_delimiter_locale.cpp

A demonstration of how the mask table in the C locale can be modified to
introduce more characters as "whitespace" which can be automatically skipped by
formatted streams.
