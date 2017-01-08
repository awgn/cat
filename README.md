Cat: C++14 functional library
=============================

### What is cat?

Cat is a C++14 functional library inspired by category theory 
and Haskell language.
The purpose of cat is to provide utility functions and type classes 
to enable the functional programming paradigm in C++.

### Rationale 

The purpose of Cat library is twofold: 

* to provide utility classes and functions that enable the functional programming paradigm in C++. 
* to implement an extensible framework of type classes to attain a better reusability of code 
through the principles of category theory.


### Features

The Cat library:

* is lightweight. Each function and object shipped with the library
takes advantage of modern C++ features, such as `constexpr` constructs, movability, 
perfect forwarding, etc.
* is written in portable C++14 without macro black-magic, whereas 
template meta-programming is restricted only to the implementation of type traits.
* implements type traits tailored for generic and functional programming (e.g. `is_container`, 
`is_associative_container`, `function_type`, `function_arity`, `return_type`, `is_callable` to mention a few).
* provides utilities, like callable wrappers, which enable functional composition and 
currying on-top-of any kind of callable types (including `std::bind` expressions 
and generic lambdas).
* provides functions that operate on `std::tuple` and generic containers. 
* provides utilities that enable perfect forwarding from L/R value containers 
by means of `forward_as` and `forward_iterator`.  
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

* includes instances of standard C++14 and Technical Specification (TS) containers/types for the above-mentioned type classes.

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

