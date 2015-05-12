//  Copyright (c) 2015 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_DYN_LINK

#include <echelon/echelon.hpp>

#define BOOST_TEST_MODULE "static shape writable regression test"
#include <boost/test/unit_test.hpp>

#include <vector>

#include "../basic_fixture.hpp"

struct static_shape_writable_fixture : basic_fixture
{
    static_shape_writable_fixture()
    {
        ds = temp_file.create_dataset<double>("dataset", {10, 10});
    }

    ~static_shape_writable_fixture()
    {
    }

    echelon::dataset ds;
};

BOOST_AUTO_TEST_SUITE(static_shape_writable_suite)

BOOST_FIXTURE_TEST_CASE(static_shape_writable_test, static_shape_writable_fixture)
{
    std::vector<double> data(100);
    echelon::multi_array_view<double> data_view(data.data(), {10, 10});

    data_view <<= ds;
}

BOOST_FIXTURE_TEST_CASE(static_shape_readable_test, static_shape_writable_fixture)
{
std::vector<double> data(100);
echelon::multi_array_view<double> data_view(data.data(), {10, 10});

ds <<= data_view;
}

BOOST_AUTO_TEST_SUITE_END()