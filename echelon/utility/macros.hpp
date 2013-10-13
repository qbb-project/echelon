//  Copyright (c) 2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_MACROS_HPP
#define ECHELON_MACROS_HPP

#ifdef __GNUC__

#define ECHELON_UNUSED(x) x __attribute__((unused))  

#ifdef NDEBUG

#define ECHELON_UNUSED_RELEASE(x) ECHELON_UNUSED(x)

#else

#define ECHELON_UNUSED_RELEASE(x) x

#endif

#else

#error "unused attribute not supported"

#endif

#endif
