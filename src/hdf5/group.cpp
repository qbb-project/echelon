//  Copyright (c) 2012-2013 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <echelon/hdf5/group.hpp>
#include <echelon/hdf5/error_handling.hpp>

#include <utility>
#include <exception>

namespace echelon
{
namespace hdf5
{

namespace
{

struct iterate_proxy_data
{
    explicit iterate_proxy_data(std::function<herr_t(hid_t, const char*)> op)
    : op{op}, caught_exception{}
    {
    }

    std::function<herr_t(hid_t, const char*)> op;
    std::exception_ptr caught_exception;
};

herr_t iterate_proxy_op(hid_t g_id, const char *name, const H5L_info_t *info,
                        void *op_data) noexcept
{
    iterate_proxy_data& data = *static_cast<iterate_proxy_data*>(op_data);

    try
    {
        return data.op(g_id, name);
    }
    catch (...)
    {
        data.caught_exception = std::current_exception();

        return 1;
    }
}


struct visit_objects_proxy_data
{
    explicit visit_objects_proxy_data(std::function<herr_t(hid_t, const char*)> op)
    : op{op}, caught_exception{}
    {
    }

    std::function<herr_t(hid_t, const char*)> op;
    std::exception_ptr caught_exception;
};

herr_t visit_objects_proxy_op(hid_t g_id, const char *name, const H5O_info_t *info,
                              void *op_data) noexcept
{
    visit_objects_proxy_data& data = *static_cast<visit_objects_proxy_data*>(op_data);

    try
    {
        return data.op(g_id, name);
    }
    catch (...)
    {
        data.caught_exception = std::current_exception();

        return 1;
    }
}

}

group::group()
: group_id_(-1)
{
}

group::group(hid_t group_id_)
: group_id_(group_id_)
{
    ECHELON_ASSERT_MSG(id() == -1 || H5Iget_type(id()) == H5I_GROUP,
                       "ID does not refer to a group");
    ECHELON_ASSERT_MSG(id() == -1 || H5Iis_valid(id()) > 0,
                       "invalid object ID");
}

group::group(const object& other)
:group_id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iget_type(id()) == H5I_GROUP,
                       "ID does not refer to a group");
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

    ECHELON_VERIFY_MSG(H5Iinc_ref(id()) > 0,"unable to increment the reference count");
}

group::group(hid_t loc_id_, const std::string& name_, const property_list& lcpl_,
             const property_list& gcpl_, const property_list& gapl_)
: group_id_(H5Gcreate2(loc_id_, name_.c_str(), lcpl_.id(), gcpl_.id(), gapl_.id()))
{
    if(id() < 0)
        throw_on_hdf5_error();
}

group::group(hid_t loc_id_, const std::string& name_, const property_list& gapl_)
: group_id_(H5Gopen2(loc_id_, name_.c_str(), gapl_.id()))
{
    if(id() < 0)
        throw_on_hdf5_error();
}

group::~group()
{
    if (id() > -1)
    {
        ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

        ECHELON_VERIFY_MSG(H5Gclose(id()) >= 0,"unable to close the group");
    }
}

group::group(const group& other)
:group_id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

    ECHELON_VERIFY_MSG(H5Iinc_ref(id()) > 0,"unable to increment the reference count");
}

group::group(group&& other)
:group_id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

    other.group_id_ = -1;
}

group& group::operator=(const group& other)
{
    using std::swap;

    group temp(other);

    swap(*this,temp);

    return *this;
}

group& group::operator=(group&& other)
{
    using std::swap;

    swap(group_id_,other.group_id_);

    return *this;
}

hsize_t group::iterate_links(H5_index_t index_type,
                             H5_iter_order_t order,
                             hsize_t start_index,
                             std::function<herr_t(hid_t, const char*)> op) const
{
    hsize_t current_index = start_index;

    iterate_proxy_data data(op);

    herr_t error_code = H5Literate(id(), index_type, order,
                                   &current_index,
                                   &iterate_proxy_op, &data);

    if (error_code < 0)
        throw_on_hdf5_error();

    if (data.caught_exception)
        std::rethrow_exception(data.caught_exception);

    return current_index;
}

void group::visit_links(H5_index_t index_type, H5_iter_order_t order,
                        std::function<herr_t(hid_t,const char*)> visitor)const
{
    iterate_proxy_data data(visitor);

    herr_t error_code = H5Lvisit(id(), index_type, order,
                                 &iterate_proxy_op, &data);

    if (error_code < 0)
        throw_on_hdf5_error();

    if (data.caught_exception)
        std::rethrow_exception(data.caught_exception);
}

void group::visit_objects(H5_index_t index_type, H5_iter_order_t order,
                          std::function<herr_t(hid_t,const char*)> visitor)const
{
    visit_objects_proxy_data data(visitor);

    herr_t error_code = H5Ovisit(id(), index_type, order,
                                 &visit_objects_proxy_op, &data);

    if (error_code < 0)
        throw_on_hdf5_error();

    if (data.caught_exception)
        std::rethrow_exception(data.caught_exception);
}

void group::remove(const std::string& name) const
{
    if(H5Ldelete(id(),name.c_str(),H5P_DEFAULT) < 0)
        throw_on_hdf5_error();
}

hid_t group::id() const
{
    return group_id_;
}

}
}
