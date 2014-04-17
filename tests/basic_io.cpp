//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_DYN_LINK

#include <echelon/echelon.hpp>
#include <vector>

#define BOOST_TEST_MODULE "basic IO unit test"
#include <boost/test/unit_test.hpp>

#include "basic_fixture.hpp"

BOOST_FIXTURE_TEST_CASE( basic_io_test, basic_fixture )
{
      auto ds = temp_file.create_dataset<double>("dataset",{ 10 });
      auto sds = temp_file.create_scalar_dataset<int>("scalar_dataset");
      auto attr = temp_file.attributes().create<int>("attribute");
      
      std::vector<double> data(10,2);
      
      ds <<= data;
      sds <<= 42;
      attr <<= 7;
      
      std::vector<double> ds_value(10);
      int sds_value;
      int attr_value;
      
      ds_value <<= ds;
      sds_value <<= sds;
      attr_value <<= attr;
      
      BOOST_CHECK_EQUAL_COLLECTIONS(begin(ds_value),end(ds_value),begin(data),end(data));
      BOOST_CHECK_EQUAL(sds_value,42);
      BOOST_CHECK_EQUAL(attr_value,7);
}
