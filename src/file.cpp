#include <echelon/file.hpp>

namespace echelon
{
file::file(const std::string& path, create_mode mode)
: file_wrapper_(path,
                mode == create_mode::truncate ? H5F_ACC_TRUNC : H5F_ACC_EXCL,
                H5P_DEFAULT,
                H5P_DEFAULT),
  root_group_(*this)
{
}

file::file(const std::string& path, open_mode mode)
: file_wrapper_(path,
                mode == open_mode::read_only ? H5F_ACC_RDONLY : H5F_ACC_RDWR,
                H5P_DEFAULT),
  root_group_(*this)
{
}

group& file::root_group()
{
    return root_group_;
}

hid_t file::id() const
noexcept
{
    return file_wrapper_.id();
}
}
