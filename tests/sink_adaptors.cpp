//  Copyright (c) 2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_DYN_LINK

#include <echelon/echelon.hpp>
#include <vector>

#define BOOST_TEST_MODULE "sink adaptor unit test"
#include <boost/test/unit_test.hpp>

#include "basic_fixture.hpp"

struct access_fixture : basic_fixture
{
    access_fixture()
    {
      temp_file.create_dataset<double>("dataset",{ 10 });
    }
    
    ~access_fixture()
    {
    }
};

BOOST_FIXTURE_TEST_CASE( auto_reshape_test, access_fixture )
{
      echelon::dataset ds = temp_file["dataset"];
      
      std::vector<double> data;
      
      BOOST_CHECK_NO_THROW(echelon::auto_reshape(data) <<= ds)
} 
