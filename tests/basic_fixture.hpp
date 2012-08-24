#ifndef ECHLON_BASIC_FIXTURE_HPP
#define ECHLON_BASIC_FIXTURE_HPP

struct basic_fixture
{
    echelon::file temp_file;
    echelon::group root;
    
    basic_fixture()
    :temp_file("basic_io_fixture.h5",echelon::file::create_mode::truncate),root(temp_file.root_group())
    {
    }
    
    ~basic_fixture()
    {
    }
};

#endif
