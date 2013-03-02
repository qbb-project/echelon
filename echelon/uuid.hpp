//  Copyright (c) 2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_UUID_HPP
#define ECHELON_UUID_HPP

#include <string>

namespace echelon
{

constexpr char uuid[] = "{9160ef20-47c3-11e2-bcfd-0800200c9a66}";

std::string generate_unique_identifier(const std::string& name);

}

#endif
