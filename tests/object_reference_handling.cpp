//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_DYN_LINK

#include <echelon/echelon.hpp>
#include <string>
#include <vector>

#define BOOST_TEST_MODULE "object reference handling unit test"
#include <boost/test/unit_test.hpp>

#include "basic_fixture.hpp"

BOOST_FIXTURE_TEST_SUITE( object_reference_handling_suite, basic_fixture )

BOOST_AUTO_TEST_CASE( scalar_object_reference_handling_test )
{
      auto sd1 = temp_file.create_scalar_dataset("scalar_dataset1", 1);
      auto sd2 = temp_file.create_scalar_dataset("scalar_dataset2", 2);
      auto sd3 = temp_file.create_scalar_dataset("scalar_dataset3", 3);
      auto sd4 = temp_file.create_scalar_dataset("scalar_dataset4", 4);
      
      auto sd1_ref = temp_file.create_scalar_dataset("scalar_dataset_ref1", sd1.ref());
      auto sd2_ref = temp_file.create_scalar_dataset("scalar_dataset_ref2", sd2.ref());
      auto sd3_ref = temp_file.create_scalar_dataset("scalar_dataset_ref3", sd3.ref());
      auto sd4_ref = temp_file.create_scalar_dataset("scalar_dataset_ref4", sd4.ref());
      
      echelon::object_reference ref1, ref2, ref3 ,ref4;
      
      ref1 <<= sd1_ref;
      ref2 <<= sd2_ref;
      ref3 <<= sd3_ref;
      ref4 <<= sd4_ref;
      
      echelon::scalar_dataset sd1_v2 = *ref1;
      echelon::scalar_dataset sd2_v2 = *ref2;
      echelon::scalar_dataset sd3_v2 = *ref3;
      echelon::scalar_dataset sd4_v2 = *ref4;
      
      int value1, value2, value3, value4;
      
      value1 <<= sd1_v2;
      value2 <<= sd2_v2;
      value3 <<= sd3_v2;
      value4 <<= sd4_v2;
      
      BOOST_CHECK_EQUAL(value1,1);
      BOOST_CHECK_EQUAL(value2,2);
      BOOST_CHECK_EQUAL(value3,3);
      BOOST_CHECK_EQUAL(value4,4);
}

BOOST_AUTO_TEST_SUITE_END() 
