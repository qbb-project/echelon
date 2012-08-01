#ifndef ECHELON_FILE_HPP
#define ECHELON_FILE_HPP

#include <echelon/group.hpp>
#include <echelon/hdf5/file.hpp>

#include <string>
#include <memory>

namespace echelon
{

class file
{
public:
    enum class create_mode
    {
        truncate,
        exclusive
    };

    enum class open_mode
    {
        read_only,
        read_write
    };

    file(const std::string& path, create_mode mode);
    file(const std::string& path, open_mode mode);

    group& root_group();

    hid_t id() const noexcept;
private:
    hdf5::file file_wrapper_;
    group root_group_;
};

}

#endif
