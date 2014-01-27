//  Copyright (c) 2012-2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/hdf5/group.hpp>

#include <echelon/hdf5/file.hpp>

namespace echelon
{
namespace hdf5
{
group::group(const file& loc, const std::string& name)
: group_wrapper_(loc.native_handle().id(), name, hdf5::precursor::default_property_list)
{
}

group::group(const object& parent, const std::string& name, creation_mode mode) : group_wrapper_(-1)
{
    if (mode == creation_mode::create)
    {
        hdf5::precursor::property_list link_creation_properties(
            hdf5::precursor::property_list_class(H5P_LINK_CREATE));
        link_creation_properties.set_char_encoding(H5T_CSET_UTF8);

        group_wrapper_ = hdf5::precursor::group(
            parent.native_handle().id(), name, link_creation_properties,
            hdf5::precursor::default_property_list, hdf5::precursor::default_property_list);
    }
    else
    {
        group_wrapper_ = hdf5::precursor::group(parent.native_handle().id(), name,
                                                hdf5::precursor::default_property_list);
    }
}

group::group(hdf5::precursor::group group_wrapper_) : group_wrapper_(std::move(group_wrapper_))
{
}

group group::create_group(const std::string& name)
{
    return group(*this, name, creation_mode::create);
}

dataset group::create_dataset(const std::string& name, const type& datatype,
                              const std::vector<hsize_t>& dims, const dataset_options& options)
{
    return create_dataset(name, datatype, dims, options.compression_level(),
                          options.auto_chunking(), options.shuffle_filter(), options.chunk_shape());
}

dataset group::create_dataset(const std::string& name, const type& datatype,
                              const std::vector<hsize_t>& dims, int comp_level, bool auto_chunking,
                              bool shuffle_filter, const std::vector<hsize_t> chunk_shape)
{
    return dataset(*this, name, datatype, dims, comp_level, auto_chunking, shuffle_filter,
                   chunk_shape);
}

scalar_dataset group::create_scalar_dataset(const std::string& name, const type& datatype)
{
    return scalar_dataset(*this, name, datatype);
}

object group::operator[](const std::string& name) const
{
    return object(hdf5::precursor::object(native_handle().id(), name));
}

void group::remove(const std::string& name) const
{
    group_wrapper_.remove(name);
}

group group::require_group(const std::string& name)
{
    if (exists(*this, name) && get_object_type_by_name(*this, name) == object_type::group)
    {
        return group(hdf5::precursor::group(native_handle().id(), name,
                                            hdf5::precursor::default_property_list));
    }
    else
    {
        return create_group(name);
    }
}

dataset group::require_dataset(const std::string& name, const type& datatype,
                               const std::vector<hsize_t>& dims, const dataset_options& options)
{
    // FIXME: add a more precise type test here
    if (exists(*this, name) && get_object_type_by_name(*this, name) == object_type::dataset)
    {
        dataset ds = (*this)[name];

        if (ds.shape() != dims)
            throw broken_contract_exception("The required shape doesn't "
                                            "match the shape of the dataset.");

        if (ds.datatype() != datatype)
            throw broken_contract_exception("The required datatype doesn't "
                                            "match the datatype of the dataset.");

        return ds;
    }
    else
    {
        return create_dataset(name, datatype, dims, options);
    }
}

scalar_dataset group::require_scalar_dataset(const std::string& name, const type& datatype)
{
    if (exists(*this, name) && get_object_type_by_name(*this, name) == object_type::scalar_dataset)
    {
        scalar_dataset ds(hdf5::precursor::dataset(native_handle().id(), name,
                                                   hdf5::precursor::default_property_list));

        if (ds.datatype() != datatype)
            throw broken_contract_exception("The required datatype doesn't "
                                            "match the datatype of the dataset.");

        return ds;
    }
    else
    {
        return create_scalar_dataset(name, datatype);
    }
}

void group::iterate_links(const std::function<void(const link&)>& op) const
{
    group_wrapper_.iterate_links(H5_INDEX_NAME, H5_ITER_NATIVE, 0,
                                 [&op](hid_t loc_id, const char* name)->hid_t
                                 {
        op(link(object(hdf5::precursor::object(loc_id, hdf5::precursor::share_ownership)),
                std::string(name)));

        return 0;
    });
}

void group::visit_links(const std::function<void(const link&)>& visitor) const
{
    group_wrapper_.visit_links(H5_INDEX_NAME, H5_ITER_NATIVE,
                               [&visitor](hid_t loc_id, const char* name)->hid_t
                               {
        visitor(link(object(hdf5::precursor::object(loc_id, hdf5::precursor::share_ownership)),
                     std::string(name)));

        return 0;
    });
}

void group::visit_objects(const std::function<void(const object&)>& visitor) const
{
    group_wrapper_.visit_objects(H5_INDEX_NAME, H5_ITER_NATIVE,
                                 [&visitor](hid_t loc_id, const char* name)->hid_t
                                 {
        visitor(object(hdf5::precursor::object(loc_id, std::string(name))));

        return 0;
    });
}

object_reference group::ref() const
{
    return object_reference(object(*this));
}

const group::native_handle_type& group::native_handle() const
{
    return group_wrapper_;
}

attribute_repository<group> group::attributes() const
{
    return attribute_repository<group>(*this);
}
}
}
