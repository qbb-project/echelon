//  Copyright (c) 2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/uuid.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/name_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace echelon
{

std::string generate_unique_identifier(const std::string& name)
{
    boost::uuids::string_generator sgen;
    boost::uuids::uuid echelon_namespace_uuid = sgen(uuid);

    boost::uuids::name_generator ngen(echelon_namespace_uuid);
    boost::uuids::uuid unique_identifier = ngen(name);

    return to_string(unique_identifier);
}

}
