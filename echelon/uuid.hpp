//  Copyright (c) 2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_UUID_HPP
#define ECHELON_UUID_HPP

#include <string>

namespace echelon
{

extern const char uuid[];

std::string generate_unique_identifier(const std::string& name);
}

#endif
