#include <echelon/hdf5/file.hpp>

#include <utility>

#include <echelon/hdf5/error_handling.hpp>

namespace echelon
{
namespace hdf5
{

file::file(const std::string& name_, unsigned flags_, hid_t fcpl_id_,
           hid_t fapl_id_)
: file_id_(H5Fcreate(name_.c_str(), flags_, fcpl_id_, fapl_id_))
{
    if(id() < 0)
        throw_on_hdf5_error();
}

file::file(const std::string& name_, unsigned flags_, hid_t fapl_id_)
: file_id_(H5Fopen(name_.c_str(), flags_, fapl_id_))
{
    if(id() < 0)
        throw_on_hdf5_error();
}

file::~file()
{
    if (id() > -1)
    {
        ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

        ECHELON_VERIFY_MSG(H5Fclose(id()) >= 0,"unable to close the file");
    }
}

file::file(const file& other)
:file_id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

    ECHELON_VERIFY_MSG(H5Iinc_ref(id()) > 0,"unable to increment the reference count");
}

file::file(file&& other)
:file_id_(other.id())
{
    ECHELON_ASSERT_MSG(H5Iis_valid(id()) > 0,"invalid object ID");

    other.file_id_ = -1;
}

file& file::operator=(const file& other)
{
    using std::swap;

    file temp(other);
    swap(*this,temp);

    return *this;
}

file& file::operator=(file&& other)
{
    using std::swap;

    swap(file_id_,other.file_id_);

    return *this;
}

hid_t file::id() const noexcept
{
    return file_id_;
}

}
}
