//  Copyright (c) 2013-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/link.hpp>

#include <utility>

namespace echelon
{

link::link(object origin_, std::string name_)
: native_link_{hdf5::link(origin_.native_handle(), std::move(name_))}
{
}

link::link(hdf5::link native_link_) : native_link_{std::move(native_link_)}
{
}

object link::destination() const
{
    return object(native_link_.destination());
}

object link::origin() const
{
    return object(native_link_.origin());
}

const std::string& link::name() const
{
    return native_link_.name();
}
}
