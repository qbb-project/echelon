//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHLON_BASIC_FIXTURE_HPP
#define ECHLON_BASIC_FIXTURE_HPP

struct basic_fixture
{
    echelon::file temp_file;
    
    basic_fixture()
    :temp_file("basic_io_fixture.h5",echelon::file::create_mode::truncate)
    {
    }
    
    ~basic_fixture()
    {
    }
};

#endif
