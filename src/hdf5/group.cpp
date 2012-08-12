#include <echelon/hdf5/group.hpp>

#include <utility>

namespace echelon
{
namespace hdf5
{

namespace
{

herr_t iterate_proxy_op(hid_t g_id, const char *name, const H5L_info_t *info,
                        void *op_data)
{
    using op_type = std::function<herr_t(hid_t,const char*)>;

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
    H5Iinc_ref(group_id_);
}

group::group(hid_t loc_id_, const std::string& name_, hid_t lcpl_id_,
             hid_t gcpl_id_, hid_t gapl_id_)
: group_id_(H5Gcreate2(loc_id_, name_.c_str(), lcpl_id_, gcpl_id_, gapl_id_))
{
}

group::group(hid_t loc_id_, const std::string& name_, hid_t gapl_id_)
: group_id_(H5Gopen2(loc_id_, name_.c_str(), gapl_id_))
{
}

group::~group()
{
    if (id() > -1)
        if( H5Gclose(id()) < 0)
            throw 0;
}

group::group(const group& other)
:group_id_(other.group_id_)
{
    if(id() > -1)
    {
        if( H5Iinc_ref(group_id_) < 0 )
            throw 0;
    }
}

group::group(group&& other)
:group_id_(other.id())
{
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

    H5Literate(id(), index_type, order, &current_index, &iterate_proxy_op, &op);

    return current_index;
}

hid_t group::id() const
noexcept
{
    return group_id_;
}

}
}
