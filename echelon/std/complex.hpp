#ifndef ECHELON_COMPLEX_HPP
#define ECHELON_COMPLEX_HPP

#include <complex>
#include <echelon/adapt_type.hpp>

ECHELON_ADAPT_PACKED_COMPOUND_TYPE(std::complex<double>,((double,real))((double,imag)))

#endif
