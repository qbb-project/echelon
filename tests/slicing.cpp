//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_DYN_LINK

#include <echelon/echelon.hpp>
#include <vector>

#define BOOST_TEST_MODULE "slicing unit test"
#include <boost/test/unit_test.hpp>

#include "basic_fixture.hpp"

BOOST_FIXTURE_TEST_CASE(slicing_test, basic_fixture)
{
    using echelon::_;
    using echelon::range;

    auto ds = temp_file.create_dataset<double>("dataset", {10});

    std::vector<double> data = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    ds <<= data;

    std::vector<double> subrange1(4);

    subrange1 <<= ds(range(3, 7));

    std::vector<double> subrange2(5);

    subrange2 <<= ds(range(_, 5));

    std::vector<double> subrange3(5);

    subrange3 <<= ds(range(5, _));

    std::vector<double> subrange4(10);

    subrange4 <<= ds(_);

    BOOST_CHECK_EQUAL_COLLECTIONS(begin(subrange1), end(subrange1), begin(data) + 3,
                                  begin(data) + 7);
    BOOST_CHECK_EQUAL_COLLECTIONS(begin(subrange2), end(subrange2), begin(data), begin(data) + 5);
    BOOST_CHECK_EQUAL_COLLECTIONS(begin(subrange3), end(subrange3), begin(data) + 5, end(data));
    BOOST_CHECK_EQUAL_COLLECTIONS(begin(subrange4), end(subrange4), begin(data), end(data));
}

BOOST_FIXTURE_TEST_CASE(slicing_bounds_via_vector_test, basic_fixture)
{
      echelon::multi_array<double> C1({2, 2});

      C1(0, 0) = 1;
      C1(0, 1) = 2;
      C1(1, 0) = 3;
      C1(1, 1) = 4;

      auto ds = temp_file.create_dataset<double>("ds", {4, 4});

      std::vector<echelon::totally_bound_range_t> bounds;
      bounds.push_back(echelon::range(1, 3));
      bounds.push_back(echelon::range(1, 3));

      ds(bounds) <<= C1;

      echelon::multi_array<double> C2({4, 4});

      C2 <<= ds;

      BOOST_CHECK_EQUAL(C2(1, 1), 1);
      BOOST_CHECK_EQUAL(C2(1, 2), 2);
      BOOST_CHECK_EQUAL(C2(2, 1), 3);
      BOOST_CHECK_EQUAL(C2(2, 2), 4);
}