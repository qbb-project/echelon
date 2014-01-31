//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_DYN_LINK

#include <echelon/echelon.hpp>

#include <echelon/hdf5/precursor/error_handling.hpp>

#define BOOST_TEST_MODULE "error handling"
#include <boost/test/unit_test.hpp>

#include "basic_fixture.hpp"

BOOST_FIXTURE_TEST_CASE( error_handling_test, basic_fixture )
{
    BOOST_CHECK_THROW(temp_file["bar"],echelon::hdf5::precursor::can_not_open_object_exception);

    temp_file.create_group("foo");
    BOOST_CHECK_THROW(temp_file.create_group("foo"),echelon::hdf5::precursor::symbol_already_exists_exception);
}
