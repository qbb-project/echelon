//  Copyright (c) 2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_DYN_LINK

#include <echelon/echelon.hpp>
#include <vector>

#define BOOST_TEST_MODULE "null state unit test"
#include <boost/test/unit_test.hpp>

#include "basic_fixture.hpp"

BOOST_FIXTURE_TEST_CASE( null_state_test, basic_fixture )
{
      BOOST_CHECK_EQUAL(static_cast<bool>(temp_file), true);
    
      echelon::attribute attr;
      echelon::dataset ds;
      echelon::scalar_dataset sds;
      echelon::file file;
      echelon::group grp;
      echelon::type type;
      
      BOOST_CHECK_EQUAL(static_cast<bool>(attr), false);
      BOOST_CHECK_EQUAL(static_cast<bool>(ds), false);
      BOOST_CHECK_EQUAL(static_cast<bool>(sds), false);
      BOOST_CHECK_EQUAL(static_cast<bool>(file), false);
      BOOST_CHECK_EQUAL(static_cast<bool>(grp), false);
      BOOST_CHECK_EQUAL(static_cast<bool>(type), false);
}
