//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_DYN_LINK

#include <echelon/echelon.hpp>
#include <string>
#include <vector>

#define BOOST_TEST_MODULE "string support unit test"
#include <boost/test/unit_test.hpp>

#include "basic_fixture.hpp"

BOOST_FIXTURE_TEST_SUITE( string_support_suite, basic_fixture )

BOOST_AUTO_TEST_CASE( std_string_support_test )
{
      std::string foo = "foo";
      std::string bar = "bar";
      std::vector<std::string> hello_world = { "hello" , "world" };
  
      auto ds = temp_file.create_dataset<std::string>("dataset",{ 2 });
      auto sds = temp_file.create_scalar_dataset<std::string>("scalar_dataset");
      auto attr = temp_file.attributes().create<std::string>("attribute");

      ds <<= hello_world;
      sds <<= foo;
      attr <<= bar;
      
      std::vector<std::string> ds_value(2);
      std::string sds_value;
      std::string attr_value;
      
      ds_value <<= ds;
      sds_value <<= sds;
      attr_value <<= attr;
      
      BOOST_CHECK_EQUAL_COLLECTIONS(begin(ds_value),end(ds_value),begin(hello_world),end(hello_world));
      BOOST_CHECK_EQUAL(sds_value,foo);
      BOOST_CHECK_EQUAL(attr_value,bar);
}

BOOST_AUTO_TEST_CASE( cstring_support_test )
{
      auto sds = temp_file.create_scalar_dataset("scalar_dataset","foo");
      auto attr = temp_file.attributes().create("attribute","bar");
      
      std::string sds_value;
      std::string attr_value;
      
      sds_value <<= sds;
      attr_value <<= attr;
      
      BOOST_CHECK_EQUAL(sds_value,"foo");
      BOOST_CHECK_EQUAL(attr_value,"bar");
}

BOOST_AUTO_TEST_SUITE_END()
