//  Copyright (c) 2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_DYN_LINK

#include <echelon/echelon.hpp>

#define BOOST_TEST_MODULE "dynamic shape unit test"
#include <boost/test/unit_test.hpp>

#include "basic_fixture.hpp"

BOOST_FIXTURE_TEST_CASE( dynamic_shape, basic_fixture )
{   
      echelon::dataset ds1 = temp_file.create_dataset<double>("ds1", { 10 , 10 }, { 10, 20 });
      
      echelon::multi_array<double> data({10, 10});
      
      BOOST_CHECK_NO_THROW(ds1.extend_along(1, data));
      
      echelon::group g = temp_file.create_group("g");
      
      echelon::dataset ds2 = g.create_dataset<double>("ds2", { 10 , 10 }, { 10, 20 });
      
      BOOST_CHECK_NO_THROW(ds2.extend_along(1, data));
}