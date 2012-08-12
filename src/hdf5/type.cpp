#include <echelon/hdf5/type.hpp>

#include <utility>

namespace echelon
{
namespace hdf5
{

type::type(hid_t type_id_, bool is_transient_)
: type_id_(type_id_), is_transient_(is_transient_)
{
    H5Iinc_ref(type_id_);
}

type::~type()
{
    if (is_transient_ && id() > -1)
        H5Tclose(id());
}

type::type(const type& other)
:type_id_(other.type_id_),is_transient_(other.is_transient_)
{
    H5Iinc_ref(type_id_);
}

type::type(type&& other)
:type_id_(other.type_id_),is_transient_(other.is_transient_)
{
    other.type_id_ = -1;
}

type& type::operator=(const type& other)
{
    using std::swap;

    type temp(other);
    swap(*this,temp);

    return *this;
}

type& type::operator=(type&& other)
{
    using std::swap;

    swap(type_id_,other.type_id_);
    swap(is_transient_,other.is_transient_);

    return *this;
}

std::size_t type::size() const
{
    return H5Tget_size(id());
}

type type::char_()
{
    return type(H5T_NATIVE_CHAR);
}

type type::uchar()
{
    return type(H5T_NATIVE_UCHAR);
}

type type::int_()
{
    return type(H5T_NATIVE_INT);
}

type type::uint()
{
    return type(H5T_NATIVE_UINT);
}

type type::short_()
{
    return type(H5T_NATIVE_SHORT);
}

type type::ushort()
{
    return type(H5T_NATIVE_USHORT);
}

type type::long_()
{
    return type(H5T_NATIVE_LONG);
}

type type::ulong()
{
    return type(H5T_NATIVE_ULONG);
}

type type::long_long()
{
    return type(H5T_NATIVE_LLONG);
}

type type::ulong_long()
{
    return type(H5T_NATIVE_ULLONG);
}

type type::float_()
{
    return type(H5T_NATIVE_FLOAT);
}

type type::double_()
{
    return type(H5T_NATIVE_DOUBLE);
}

type type::string()
{
    hid_t tid = H5Tcopy(H5T_C_S1);
    H5Tset_size (tid, H5T_VARIABLE);

    return type(tid);
}

type type::empty_compound_type(std::size_t size)
{
    return type(H5Tcreate(H5T_COMPOUND,size));
}

type type::object_reference()
{
    return type(H5T_STD_REF_OBJ);
}

void type::insert_member(const std::string& name,
                         std::size_t offset, const type& field_type)
{
    H5Tinsert(id(),name.c_str(),offset,field_type.id());
}

type type::clone()const
{
    return type(H5Tcopy(id()),is_transient_);
}

hid_t type::id() const
{
    return type_id_;
}

}
}
