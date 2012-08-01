#include <echelon/hdf5/file.hpp>

#include <utility>

namespace echelon
{
namespace hdf5
{

file::file(const std::string& name_, unsigned flags_, hid_t fcpl_id_,
           hid_t fapl_id_)
: file_id_(H5Fcreate(name_.c_str(), flags_, fcpl_id_, fapl_id_))
{
}

file::file(const std::string& name_, unsigned flags_, hid_t fapl_id_)
: file_id_(H5Fopen(name_.c_str(), flags_, fapl_id_))
{
}

file::~file()
{
    H5Fclose(id());
}

file::file(const file& other)
:file_id_(other.file_id_)
{
    H5Iinc_ref(file_id_);
}

file::file(file&& other)
:file_id_(other.id())
{
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
