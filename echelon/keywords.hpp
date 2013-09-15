//  Copyright (c) 2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_KEYWORDS_HPP
#define ECHELON_KEYWORDS_HPP

#include <echelon/utility/function_options.hpp>

#include <vector>

#include <hdf5.h> //for hsize_t

namespace echelon
{

ECHELON_DECLARE_KEYWORD(int, compression_level)
ECHELON_DECLARE_KEYWORD(std::vector<hsize_t>, chunk_shape)
}

#endif
