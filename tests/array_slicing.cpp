//  Copyright (c) 2015 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_DYN_LINK

#include <echelon/echelon.hpp>

#include <iostream>

#define BOOST_TEST_MODULE "array slicing unit test"
#include <boost/test/unit_test.hpp>

#include "basic_fixture.hpp"

BOOST_AUTO_TEST_SUITE(array_slicing_suite)

BOOST_FIXTURE_TEST_CASE(array_slicing_test, basic_fixture)
{
    using echelon::_;
    using echelon::range;

    auto ds = temp_file.create_dataset<double>("ds", {10});
    auto ds2 = temp_file.create_dataset<double>("ds2", {4});
    auto ds3 = temp_file.create_dataset<double>("ds3", {2, 2});

    echelon::multi_array<double> C({10, 10});

    std::vector<double> D = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<double> D2 = {0, 1, 2, 3};
    echelon::multi_array<double> D3({2, 2}, 9);

    ds <<= D;
    ds2 <<= D2;
    ds3 <<= D3;

    C(_, 1) <<= ds;
    C(range(5, 9), 2) <<= ds2;
    C(range(5, 7), range(6, 8)) <<= ds3;
    C(range(0, 4, 2), range(0, 4, 2)) <<= ds3;

    for (std::size_t i = 0; i < 10; ++i)
    {
        BOOST_CHECK_EQUAL(C(i, 1), i);
    }

    for (std::size_t i = 5; i < 9; ++i)
    {
        BOOST_CHECK_EQUAL(C(i, 2), i - 5);
    }

    for (std::size_t i = 5; i < 7; ++i)
    {
        for (std::size_t j = 6; j < 8; ++j)
        {
            BOOST_CHECK_EQUAL(C(i, j), 9);
        }
    }

    for (std::size_t i = 0; i < 4; i += 2)
    {
        for (std::size_t j = 0; j < 4; j += 2)
        {
            BOOST_CHECK_EQUAL(C(i, j), 9);
        }
    }
}

BOOST_FIXTURE_TEST_CASE(double_slicing_test, basic_fixture)
{
    using echelon::_;
    using echelon::range;

    auto ds = temp_file.create_dataset<double>("ds", {15});
    auto ds2 = temp_file.create_dataset<double>("ds2", {8});

    echelon::multi_array<double> C({10, 10});

    std::vector<double> D = {0, 0, 0, 10, 21, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<double> D2 = {11, 22, 0, 1, 2, 3, 33, 44};

    ds <<= D;
    ds2 <<= D2;

    C(_, 1) <<= ds(range(5, _));
    C(range(5, 9), 2) <<= ds2(range(2, 6));

    for (std::size_t i = 0; i < 10; ++i)
    {
        BOOST_CHECK_EQUAL(C(i, 1), i);
    }

    for (std::size_t i = 5; i < 9; ++i)
    {
        BOOST_CHECK_EQUAL(C(i, 2), i - 5);
    }
}

BOOST_FIXTURE_TEST_CASE(array_slicing_bounds_via_vector_test, basic_fixture)
{
    auto ds = temp_file.create_dataset<double>("ds", {2, 2});

    echelon::multi_array<double> C1({2, 2});

    C1(0, 0) = 1;
    C1(0, 1) = 2;
    C1(1, 0) = 3;
    C1(1, 1) = 4;

    ds <<= C1;

    echelon::multi_array<double> C2({4, 4});

    std::vector<echelon::totally_bound_range_t> bounds;
    bounds.push_back(echelon::range(1, 3));
    bounds.push_back(echelon::range(1, 3));

    C2(bounds) <<= ds;

    BOOST_CHECK_EQUAL(C2(1, 1), 1);
    BOOST_CHECK_EQUAL(C2(1, 2), 2);
    BOOST_CHECK_EQUAL(C2(2, 1), 3);
    BOOST_CHECK_EQUAL(C2(2, 2), 4);
}

BOOST_AUTO_TEST_SUITE_END()
