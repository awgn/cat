Cat: C++14 functional library
=============================

### What is cat?

Cat is a C++14 functional library inspired by category theory 
and the Haskell language.
The purpose of cat is to provide utility functions and type classes not included
into the standard library and enable the functional programming paradigm in C++.

### Rationale 

The purpose of the Cat library is twofold: 

* on the one hand it provides utilities and functions not available in the standard 
C++ library with respect to functional programming. 
* on the other hand it implements an extensible framework of type classes to attain 
a better reusability of code through the principles of category theory.


### Features

The Cat library:

* is lightweight. Each function and object shipped with the library
takes advantage of modern C++ features, such as `constexpr` constructs, movability, 
perfect forwarding, etc.
* is written in portable C++14, without using macro black-magic and with a pinch 
of template meta-programming, limited only in the implementation of type traits.
* extends the standard type traits with a set specifically tailored to generic and 
functional programming (e.g. `is_container`, `is_associative_container`, `function_type`, 
`function_arity`, `return_type`, `is_callable` to mention a few).
* provides utilities, like callable wrappers, enabling functional composition and 
currying on-top-of any kind of callable types (including `std::bind` expressions 
and generic lambdas).
* provides functions that operate on `std::tuple` and generic containers. 
* provides `forward_as` and `forward_iterator` that allow for perfect forwarding from
containers in for-loop statements and via iterator.  
* includes an extensible framework for building type classes and provides the
implementation of the following ones:
    * Functor
    * Bifunctor
    * Applicative
    * Alternative 
    * Monoid 
    * Monad
    * MonadPlus
    * Foldable
    * Show
    * Read
    * ...

* provides instances of standard C++ types for the above-mentioned type classes.

### Notes

At the present moment the Cat library is tested under Linux with both glibcxx and libc++
library and with the following compilers:
    
* gcc-4.9
* gcc-5 
* clang-3.5 
* clang 3.6 


### Acknowledgements

The Cat library takes inspiration from the following projects:

* The [Haskell](https://www.haskell.org) language
* [Fit library](https://github.com/pfultz2/Fit) by Paul Fultz II
* [FTL library](https://github.com/beark/ftl) by Bjorn Aili


