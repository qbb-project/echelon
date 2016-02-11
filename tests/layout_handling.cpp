//  Copyright (c) 2015-2016 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_DYN_LINK

#include <echelon/echelon.hpp>

#include <boost/multi_array.hpp>

#define BOOST_TEST_MODULE "layout handling test"
#include <boost/test/unit_test.hpp>

#include "basic_fixture.hpp"

#include <cassert>

namespace echelon
{
namespace hdf5
{

template <typename T, std::size_t NumDims>
inline std::vector<std::size_t> shape(const boost::multi_array<T, NumDims>& container, adl_enabler)
{
    auto shape = container.shape();

    return std::vector<std::size_t>(shape, shape + NumDims);
}

/* Note: This function shouldn't be used outside of this test since
 *       it only works under the assumption that we only use
 *       multi_arrays with fortran_storage_order.
 */
template <typename T, std::size_t NumDims>
inline column_major_storage_order<std::vector<std::size_t>>
storage_order(const boost::multi_array<T, NumDims>& container, adl_enabler)
{
    assert(container.storage_order() == boost::fortran_storage_order());

    return column_major_storage_order<std::vector<std::size_t>>(shape_adl(container));
}
}
}

struct layout_test_fixture : basic_fixture
{
    layout_test_fixture()
    {
        temp_file.create_dataset<double>("dataset", {10, 10});
        temp_file.create_dataset<double>("dataset_rect", {10, 20});
    }

    virtual ~layout_test_fixture() = default;
};

BOOST_AUTO_TEST_SUITE(layout_handling_suite)

BOOST_FIXTURE_TEST_CASE(row_major_storage_test, layout_test_fixture)
{
    echelon::multi_array<double> data({10, 10});

    for (long int i = 0; i < 10; ++i)
    {
        for (long int j = 0; j < 10; ++j)
        {
            data(i, j) = i * 10 + j;
        }
    }

    echelon::dataset ds = temp_file["dataset"];

    ds <<= data;

    echelon::multi_array<double> data2({10, 10});

    data2 <<= ds;

    for (long int i = 0; i < 10; ++i)
    {
        for (long int j = 0; j < 10; ++j)
        {
            BOOST_CHECK_EQUAL(data2(i, j), i * 10 + j);
        }
    }
}

BOOST_FIXTURE_TEST_CASE(column_major_storage_test, layout_test_fixture)
{
    boost::multi_array<double, 2> data(boost::extents[10][10], boost::fortran_storage_order());

    for (long int i = 0; i < 10; ++i)
    {
        for (long int j = 0; j < 10; ++j)
        {
            data[i][j] = i * 10 + j;
        }
    }

    echelon::dataset ds = temp_file["dataset"];

    ds <<= data;

    echelon::multi_array<double> data2({10, 10});

    data2 <<= ds;

    for (long int i = 0; i < 10; ++i)
    {
        for (long int j = 0; j < 10; ++j)
        {
            BOOST_CHECK_EQUAL(data2(i, j), i * 10 + j);
        }
    }
}

BOOST_FIXTURE_TEST_CASE(rect_column_major_storage_test, layout_test_fixture)
{
    boost::multi_array<double, 2> data(boost::extents[10][20], boost::fortran_storage_order());

    for (long int i = 0; i < 10; ++i)
    {
        for (long int j = 0; j < 20; ++j)
        {
            data[i][j] = i * 10 + j;
        }
    }

    echelon::dataset ds = temp_file["dataset_rect"];

    ds <<= data;

    echelon::multi_array<double> data2({10, 20});

    data2 <<= ds;

    for (long int i = 0; i < 10; ++i)
    {
        for (long int j = 0; j < 20; ++j)
        {
            BOOST_CHECK_EQUAL(data2(i, j), i * 10 + j);
        }
    }
}

BOOST_FIXTURE_TEST_CASE(column_major_storage_slicing_test, layout_test_fixture)
{
    using echelon::_;
    using echelon::range;

    boost::multi_array<double, 2> data(boost::extents[10][10], boost::fortran_storage_order());

    for (long int i = 0; i < 10; ++i)
    {
        for (long int j = 0; j < 10; ++j)
        {
            data[i][j] = i * 10 + j;
        }
    }

    echelon::dataset ds = temp_file["dataset"];

    ds <<= data;

    boost::multi_array<double, 2> data2(boost::extents[10][2], boost::fortran_storage_order());

    data2 <<= ds(_, range(4, 6));

    for (long int i = 0; i < 10; ++i)
    {
        for (long int j = 4; j < 6; ++j)
        {
            BOOST_CHECK_EQUAL(data2[i][j - 4], i * 10 + j);
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()