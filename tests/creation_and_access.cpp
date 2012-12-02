//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_DYN_LINK

#include <echelon/echelon.hpp>

#define BOOST_TEST_MODULE "creation and access unit test"
#include <boost/test/unit_test.hpp>

#include "basic_fixture.hpp"

struct access_fixture : basic_fixture
{
    access_fixture()
    {
      temp_file.create_dataset<double>("dataset",{ 10 , 10 });
      temp_file.create_scalar_dataset("scalar_dataset",42.0);
      temp_file.create_group("group");
      temp_file.attributes.create("attribute",2);
    }
    
    ~access_fixture()
    {
    }
};

BOOST_AUTO_TEST_SUITE( creation_and_access_suite )

BOOST_FIXTURE_TEST_CASE( creation_test, basic_fixture )
{
      BOOST_CHECK_NO_THROW(temp_file.create_dataset<double>("dataset",{ 10 , 10 }));
      BOOST_CHECK_NO_THROW(temp_file.create_scalar_dataset("scalar_dataset",42.0));
      BOOST_CHECK_NO_THROW(temp_file.create_group("group"));
      BOOST_CHECK_NO_THROW(temp_file.attributes.create("attribute",2));
}

BOOST_FIXTURE_TEST_CASE( access_test, access_fixture )
{
      BOOST_CHECK_NO_THROW(echelon::dataset ds = temp_file["dataset"])
      BOOST_CHECK_NO_THROW(echelon::scalar_dataset sds = temp_file["scalar_dataset"])
      BOOST_CHECK_NO_THROW(echelon::group ds = temp_file["group"])
      BOOST_CHECK_NO_THROW(echelon::attribute ds = temp_file.attributes["attribute"])
}

BOOST_AUTO_TEST_SUITE_END()
