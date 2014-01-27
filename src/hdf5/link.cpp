//  Copyright (c) 2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/hdf5/link.hpp>
#include <utility>
namespace echelon
{
namespace hdf5
{
link::link(object origin_, std::string name_) : origin_{std::move(origin_)}, name_{std::move(name_)}
{
}

object link::destination() const
{
    return object(hdf5::precursor::object(origin_.native_handle().id(), name()));
}

object link::origin() const
{
    return origin_;
}

const std::string& link::name() const
{
    return name_;
}
}
}
