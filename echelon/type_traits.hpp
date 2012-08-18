#ifndef ECHELON_TYPE_TRAITS_HPP
#define ECHELON_TYPE_TRAITS_HPP

#include <echelon/object_reference.hpp>

#include <string>
#include <boost/mpl/bool.hpp>

namespace echelon
{

template<typename T>
struct is_predefined_hdf5_type: boost::mpl::false_
{
};

template<>
struct is_predefined_hdf5_type<char> : boost::mpl::true_
{
};

template<>
struct is_predefined_hdf5_type<unsigned char> : boost::mpl::true_
{
};

template<>
struct is_predefined_hdf5_type<short> : boost::mpl::true_
{
};

template<>
struct is_predefined_hdf5_type<unsigned short> : boost::mpl::true_
{
};

template<>
struct is_predefined_hdf5_type<int> : boost::mpl::true_
{
};

template<>
struct is_predefined_hdf5_type<unsigned int> : boost::mpl::true_
{
};

template<>
struct is_predefined_hdf5_type<long> : boost::mpl::true_
{
};

template<>
struct is_predefined_hdf5_type<unsigned long> : boost::mpl::true_
{
};

template<>
struct is_predefined_hdf5_type<long long> : boost::mpl::true_
{
};

template<>
struct is_predefined_hdf5_type<unsigned long long> : boost::mpl::true_
{
};

template<>
struct is_predefined_hdf5_type<float> : boost::mpl::true_
{
};

template<>
struct is_predefined_hdf5_type<double> : boost::mpl::true_
{
};

template<>
struct is_predefined_hdf5_type<bool> : boost::mpl::true_
{
};

template<>
struct is_predefined_hdf5_type<std::string> : boost::mpl::true_
{
};

template<std::size_t N>
struct is_predefined_hdf5_type<char[N]> : boost::mpl::true_
{
};

template<>
struct is_predefined_hdf5_type<object_reference> : boost::mpl::true_
{
};

template<typename T>
struct is_predefined_hdf5_type<T const> : is_predefined_hdf5_type<
                                             typename std::remove_cv<T>::type>
{
};

template<typename T>
struct is_hdf5_type: is_predefined_hdf5_type<T>
{
};

template<typename T>
struct is_hdf5_type<T const> : is_hdf5_type<typename std::remove_cv<T>::type>
{
};

}

#endif
