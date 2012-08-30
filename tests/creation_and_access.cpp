#define BOOST_TEST_DYN_LINK

#include <echelon/echelon.hpp>

#define BOOST_TEST_MODULE "creation and access unit test"
#include <boost/test/unit_test.hpp>

#include "basic_fixture.hpp"

struct access_fixture : basic_fixture
{
    access_fixture()
    {
      root.create_dataset<double>("dataset",{ 10 , 10 });
      root.create_scalar_dataset("scalar_dataset",42.0);
      root.create_group("group");
      root.attributes.create("attribute",2);      
    }
    
    ~access_fixture()
    {
    }
};

BOOST_AUTO_TEST_SUITE( creation_and_access_suite )

BOOST_FIXTURE_TEST_CASE( creation_test, basic_fixture )
{
      BOOST_CHECK_NO_THROW(root.create_dataset<double>("dataset",{ 10 , 10 }));
      BOOST_CHECK_NO_THROW(root.create_scalar_dataset("scalar_dataset",42.0));
      BOOST_CHECK_NO_THROW(root.create_group("group"));
      BOOST_CHECK_NO_THROW(root.attributes.create("attribute",2));
}

BOOST_FIXTURE_TEST_CASE( access_test, access_fixture )
{
      BOOST_CHECK_NO_THROW(echelon::dataset ds = root["dataset"])
      BOOST_CHECK_NO_THROW(echelon::scalar_dataset sds = root["scalar_dataset"])
      BOOST_CHECK_NO_THROW(echelon::group ds = root["group"])
      BOOST_CHECK_NO_THROW(echelon::attribute ds = root.attributes["attribute"])
}

BOOST_AUTO_TEST_SUITE_END()