#include <echelon/hdf5/group.hpp>

#include <utility>

#include <echelon/hdf5/error_handling.hpp>

namespace echelon
{
namespace hdf5
{

namespace
{

herr_t iterate_proxy_op(hid_t g_id, const char *name, const H5L_info_t *info,
                        void *op_data)
{
    typedef std::function<herr_t(hid_t,const char*)> op_type;

    op_type& op = *static_cast<op_type*>(op_data);

    return op(g_id, name);
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

group::group(hid_t loc_id_, const std::string& name_, hid_t lcpl_id_,
             hid_t gcpl_id_, hid_t gapl_id_)
: group_id_(H5Gcreate2(loc_id_, name_.c_str(), lcpl_id_, gcpl_id_, gapl_id_))
{
    if(id() < 0)
        throw_on_hdf5_error();
}

group::group(hid_t loc_id_, const std::string& name_, hid_t gapl_id_)
: group_id_(H5Gopen2(loc_id_, name_.c_str(), gapl_id_))
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

hsize_t group::iterate(hid_t group_id, H5_index_t index_type,
                       H5_iter_order_t order,
                       hsize_t start_index,
                       std::function<herr_t(hid_t, const char*)> op) const
{
    hsize_t current_index = start_index;

    herr_t error_code = H5Literate(id(), index_type, order,
                                   &current_index, &iterate_proxy_op, &op);

    if(error_code < 0)
        throw_on_hdf5_error();

    return current_index;
}

hid_t group::id() const
{
    return group_id_;
}

}
}
