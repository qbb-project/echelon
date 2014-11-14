//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/group.hpp>

#include <echelon/file.hpp>

namespace echelon
{

group::group(const file& loc, const std::string& name) : group_handle_(loc.native_handle()[name])
{
}

group::group(group::native_handle_type native_handle_) : group_handle_(std::move(native_handle_))
{
}

group group::create_group(const std::string& name)
{
    auto new_group = group_handle_.create_group(name);
    new_group.attributes().create("echelon.class", "group");

    return group(new_group);
}

dataset group::create_dataset(const std::string& name, const type& datatype,
                              const std::vector<hsize_t>& dims, const dataset_options& options)
{
    return create_dataset(name, datatype, dims, dims, options);
}

dataset group::create_dataset(const std::string& name, const type& datatype,
                              const std::vector<hsize_t>& dims,
                              const std::vector<hsize_t>& max_dims, const dataset_options& options)
{
    return create_dataset(name, datatype, dims, max_dims, options.compression_level(),
                          options.auto_chunking(), options.shuffle_filter(), options.chunk_shape());
}

dataset group::create_dataset(const std::string& name, const type& datatype,
                              const std::vector<hsize_t>& dims,
                              const std::vector<hsize_t>& max_dims, int comp_level,
                              bool auto_chunking, bool shuffle_filter,
                              const std::vector<hsize_t> chunk_shape)
{
    hdf5::dataset_options options;

    options.compression_level(comp_level)
        .auto_chunking(auto_chunking)
        .chunk_shape(chunk_shape)
        .shuffle_filter(shuffle_filter);

    auto containing_group = group_handle_.create_group(name);
    containing_group.create_dataset("data", datatype.native_handle(), dims, max_dims, options);

    containing_group.attributes().create("echelon.class", "dataset");

    return dataset(containing_group);
}

scalar_dataset group::create_scalar_dataset(const std::string& name, const type& datatype)
{
    auto containing_group = group_handle_.create_group(name);

    containing_group.create_scalar_dataset("data", datatype.native_handle());

    containing_group.attributes().create("echelon.class", "scalar_dataset");

    return scalar_dataset(containing_group);
}

object group::operator[](const std::string& name) const
{
    return object(group_handle_[name]);
}

void group::remove(const std::string& name) const
{
    group_handle_.remove(name);
}

group group::require_group(const std::string& name)
{
    return group(group_handle_.require_group(name));
}

dataset group::require_dataset(const std::string& name, const type& datatype,
                               const std::vector<hsize_t>& dims, const dataset_options& options)
{
    // FIXME: Is this test correct, shouldn't we simply test for existence??
    if (hdf5::exists(native_handle(), name) &&
        hdf5::get_object_type_by_name(native_handle(), name) == hdf5::object_type::group)
    {
        dataset ds = (*this)[name];

        // FIXME: check class here

        if (ds.shape() != dims)
            throw hdf5::broken_contract_exception("The required shape doesn't "
                                                  "match the shape of the dataset.");

        if (ds.datatype() != datatype)
            throw hdf5::broken_contract_exception("The required datatype doesn't "
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
    // FIXME: Is this test correct, shouldn't we simply test for existence??
    if (hdf5::exists(native_handle(), name) &&
        hdf5::get_object_type_by_name(native_handle(), name) == hdf5::object_type::group)
    {
        scalar_dataset ds = (*this)[name];

        // FIXME: check class here
        if (ds.datatype() != datatype)
            throw hdf5::broken_contract_exception("The required datatype doesn't "
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
    group_handle_.iterate_links([&](const hdf5::link& l)
                                { op(link(l)); });
}

void group::visit_links(const std::function<void(const link&)>& visitor) const
{
    group_handle_.visit_links([&](const hdf5::link& l)
                              { visitor(link(l)); });
}

void group::visit_objects(const std::function<void(const object&)>& visitor) const
{
    group_handle_.visit_objects([&](const hdf5::object& l)
                                { visitor(object(l)); });
}

object_reference group::ref() const
{
    return object_reference(group_handle_.ref());
}

group::native_handle_type group::native_handle() const
{
    return group_handle_;
}

group::operator bool() const
{
    return static_cast<bool>(native_handle());
}

attribute_repository<group> group::attributes() const
{
    return attribute_repository<group>(native_handle());
}
}
