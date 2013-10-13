#define BOOST_TEST_DYN_LINK

#include <echelon/echelon.hpp>

//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE "type equality"
#include <boost/test/unit_test.hpp>

#include "basic_fixture.hpp"

#include "hdf5_hl.h"

#include <vector>
#include <string>
#include <ostream>

namespace echelon
{

std::ostream& operator<<(std::ostream& os,
                         const type& datatype)
{
    std::size_t len;

    H5LTdtype_to_text(datatype.native_handle().id(),0,H5LT_DDL,&len);

    std::vector<char> buffer(len);

    H5LTdtype_to_text(datatype.native_handle().id(),buffer.data(),H5LT_DDL,&len);

    return os << std::string(begin(buffer),end(buffer));
}

}

BOOST_FIXTURE_TEST_CASE( type_equality_test, basic_fixture )
{
    echelon::type t1 = echelon::type::double_();
    echelon::type t2 = t1.clone(); 
    
    BOOST_CHECK_EQUAL(t1,t1);
    BOOST_CHECK_EQUAL(t1,t2);

    echelon::type t3 = echelon::type::char_();
    echelon::type t4 = echelon::type::string();

    BOOST_CHECK_NE(t3,t4);

    echelon::type_layout tl1(sizeof(double) + sizeof(int));

    tl1.add_element("test1",echelon::type::double_(),0);
    tl1.add_element("test2",echelon::type::int_(),sizeof(double));

    echelon::type_layout tl2(sizeof(double) + sizeof(int));

    tl2.add_element("test3",echelon::type::double_(),0);
    tl2.add_element("test4",echelon::type::int_(),sizeof(double));

    echelon::type_layout tl3(sizeof(double) + sizeof(int));

    tl3.add_element("test1",echelon::type::int_(),0);
    tl3.add_element("test2",echelon::type::double_(),sizeof(int));

    echelon::type compound1 = echelon::type::compound_type(tl1);
    echelon::type compound2 = echelon::type::compound_type(tl2);
    echelon::type compound3 = echelon::type::compound_type(tl3);
    echelon::type compound4 = echelon::type::compound_type(tl1);

    BOOST_CHECK_NE(compound1,compound2);
    BOOST_CHECK_NE(compound1,compound3);
    BOOST_CHECK_EQUAL(compound1,compound4);
}
