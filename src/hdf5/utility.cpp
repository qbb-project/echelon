//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/hdf5/utility.hpp>

#include <echelon/hdf5/precursor/dataspace.hpp>
#include <echelon/hdf5/precursor/dataset.hpp>
#include <echelon/hdf5/precursor/property_list.hpp>

namespace echelon
{
namespace hdf5
{
object_type get_object_type(hid_t obj_id)
{
    H5O_info_t obj_info;
    H5Oget_info(obj_id, &obj_info);

    if (obj_info.type == H5O_TYPE_GROUP)
        return object_type::group;
    else if (obj_info.type == H5O_TYPE_DATASET)
    {
        hdf5::precursor::dataspace space(H5Dget_space(obj_id));

        if (H5Sget_simple_extent_type(space.id()) == H5S_SCALAR)
            return object_type::scalar_dataset;
        else
            return object_type::dataset;
    }
    else
    {
        // If we can't determine the type of an HDF5 object simply signal this
        // to the user code.
        // This might be handy, if we want to introspect HDF5 files, which were
        // not written with echelon.
        // For example we could use this to deal with future object types, which
        // are not handled by echelon yet.
        // When we have a better overview of all use cases of get_object_type,
        // we should consider to throw an
        // appropriate exception instead, to force the user to deal with this
        // case. As of now I am not sure,
        // if this is a normal or an exceptional use case.
        return object_type::unknown;
    }
}

object_type get_object_type_by_name(const object& loc, const std::string& name)
{
    H5O_info_t obj_info;
    H5Oget_info_by_name(loc.native_handle().id(), name.c_str(), &obj_info, H5P_DEFAULT);

    if (obj_info.type == H5O_TYPE_GROUP)
        return object_type::group;
    else if (obj_info.type == H5O_TYPE_DATASET)
    {
        hdf5::precursor::dataset ds(loc.native_handle().id(), name,
                                    hdf5::precursor::default_property_list);

        hdf5::precursor::dataspace space = ds.get_space();

        if (H5Sget_simple_extent_type(space.id()) == H5S_SCALAR)
            return object_type::scalar_dataset;
        else
            return object_type::dataset;
    }
    else
    {
        // If we can't determine the type of an HDF5 object simply signal this
        // to the user code.
        // This might be handy, if we want to introspect HDF5 files, which were
        // not written with echelon.
        // For example we could use this to deal with future object types, which
        // are not handled by echelon yet.
        // When we have a better overview of all use cases of get_object_type,
        // we should consider to throw an
        // appropriate exception instead, to force the user to deal with this
        // case. As of now I am not sure,
        // if this is a normal or an exceptional use case.
        return object_type::unknown;
    }
}

bool exists(const object& loc, const std::string& name)
{
    return hdf5::precursor::exists(loc.native_handle(), name);
}
}
}
