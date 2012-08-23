#include <echelon/hdf5/handle.hpp>

#include <utility>

#include <echelon/hdf5/attribute.hpp>
#include <echelon/hdf5/dataset.hpp>
#include <echelon/hdf5/dataspace.hpp>
#include <echelon/hdf5/file.hpp>
#include <echelon/hdf5/group.hpp>
#include <echelon/hdf5/object.hpp>
#include <echelon/hdf5/property_list.hpp>
#include <echelon/hdf5/type.hpp>

namespace echelon
{

namespace hdf5
{

handle::handle(hid_t id_)
:id_(id_)
{
}

handle::handle(const attribute& other)
:id_(other.id())
{
    H5Iinc_ref(id());
}

handle::handle(const dataset& other)
:id_(other.id())
{
    H5Iinc_ref(id());
}

handle::handle(const dataspace& other)
:id_(other.id())
{
    H5Iinc_ref(id());
}

handle::handle(const file& other)
:id_(other.id())
{
    H5Iinc_ref(id());
}

handle::handle(const group& other)
:id_(other.id())
{
    H5Iinc_ref(id());
}

handle::handle(const object& other)
:id_(other.id())
{
    H5Iinc_ref(id());
}

handle::handle(const property_list& other)
:id_(other.id())
{
    H5Iinc_ref(id());
}

handle::handle(const type& other)
:id_(other.id())
{
    H5Iinc_ref(id());
}

handle::handle(const handle& other)
:id_(-1)
{
    H5Iinc_ref(other.id());

    id_ = other.id();
}

handle::handle(handle&& other)
:id_(other.id_)
{
    other.id_ = -1;
}

handle::~handle()
{
    if(id() != -1)
        H5Idec_ref(id());
}

handle& handle::operator=(const handle& other)
{
    handle temp(other);

    std::swap(*this,temp);

    return *this;
}

handle& handle::operator=(handle&& other)
{
    std::swap(id_,other.id_);

    return *this;
}

hid_t handle::id()const
{
    return id_;
}

}

}
