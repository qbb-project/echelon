#define BOOST_ARCHIVE_SOURCE
#include <boost/archive/detail/archive_serializer_map.hpp>
#include <boost/archive/impl/archive_serializer_map.ipp>
#include <echelon/hdf5_type_oarchive.hpp>

namespace boost
{
namespace archive
{
namespace detail
{

template class archive_serializer_map<echelon::hdf5_type_oarchive>;

}
}
}
