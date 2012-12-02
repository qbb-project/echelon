//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_COMPLEX_HPP
#define ECHELON_COMPLEX_HPP

#include <complex>
#include <echelon/adapt_type.hpp>

ECHELON_ADAPT_PACKED_COMPOUND_TYPE(std::complex<double>,((double,real))((double,imag)))

#endif
