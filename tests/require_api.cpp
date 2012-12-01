//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_DYN_LINK

#include <echelon/echelon.hpp>

#define BOOST_TEST_MODULE "require API unit test"
#include <boost/test/unit_test.hpp>

#include "basic_fixture.hpp"

BOOST_FIXTURE_TEST_CASE( require_api, basic_fixture )
{   
      echelon::group g1 = root.require_group("g1");
    
      BOOST_CHECK_NO_THROW(root["g1"]);
      
      echelon::dataset ds1 = root.create_dataset<double>("ds1",{ 10 , 10 });
      
      BOOST_CHECK_THROW(ds1 = root.require_dataset<double>("ds1",{ 20 , 10 }),echelon::broken_contract_exception);
}
