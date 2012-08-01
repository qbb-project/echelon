#ifndef ECHELON_HDF5_TYPE_OARCHIVE_HPP
#define ECHELON_HDF5_TYPE_OARCHIVE_HPP

#include <cstddef>
#include <boost/archive/detail/common_oarchive.hpp>
#include <boost/archive/detail/register_archive.hpp>
#include <boost/serialization/nvp.hpp>

#include <cassert>
#include <stack>
#include <type_traits>
#include <echelon/type.hpp>
#include <echelon/type_traits.hpp>

namespace echelon
{

template<typename T>
inline typename std::enable_if<is_predefined_hdf5_type<T>::value, type>::type
get_hdf5_type();

template<typename T>
inline typename std::enable_if<is_hdf5_type<T>::value &&
                               !is_predefined_hdf5_type<T>::value, type>::type
get_hdf5_type();

template<typename T>
inline typename std::enable_if<!is_hdf5_type<T>::value, type>::type
get_hdf5_type();

class hdf5_type_oarchive:
                          public boost::archive::detail::common_oarchive<
                          hdf5_type_oarchive>
{
public:
    template<typename T>
    hdf5_type_oarchive(const T& t)
    : layout_(sizeof(T)), base_address_(reinterpret_cast<const char*>(&t))
    {
        static_assert(std::is_standard_layout<T>::value,"T must be a standard layout type");
    }

    typedef boost::archive::detail::common_oarchive<hdf5_type_oarchive> base_type;

    template<class T>
    void save_override(T&, BOOST_PFTO int)
    {
        static_assert(sizeof(T) != sizeof(T),"hdf5_type_oarchive can only serialize name/value pairs");
    }

    template<class T>
    void save_override(const ::boost::serialization::nvp<T> & t, BOOST_PFTO int)
    {
        name_stack_.push(t.name());
        this->base_type::save_override(t.const_value(), 0);
        name_stack_.pop();
    }

    template<typename T>
    void save_override(const boost::serialization::array<T> & t, int)
    {
        this->base_type::save_override(t, 0);
    }

    void save_override(const boost::archive::object_id_type & t, int)
    {
        //save nothing
    }

    void save_override(const boost::archive::object_reference_type & t, int)
    {
        //save nothing
    }

    void save_override(const boost::archive::version_type & t, int)
    {
        //save nothing
    }

    void save_override(const boost::archive::class_id_type & t, int)
    {
        //save nothing
    }

    void save_override(const boost::archive::class_id_optional_type & t, int)
    {
        //save nothing
    }

    void save_override(const boost::archive::class_id_reference_type & t, int)
    {
        //save nothing
    }

    void save_override(const boost::archive::class_name_type & t, int)
    {
        //save nothing
    }

    void save_override(const boost::archive::tracking_type & t, int)
    {
        //save nothing
    }

    struct use_array_optimization
    {
        template<typename T>
        struct apply: public is_hdf5_type<T>
        {
        };
    };

    template<class ValueType>
    void save_array(boost::serialization::array<ValueType> const&, unsigned int)
    {
        //should never be reached
        assert(false);
    }

    void save_binary(void*, std::size_t)
    {
        //should never be reached
        assert(false);
    }

    type get_result_type()
    {
        return type::compound_type(layout_);
    }
private:
    friend class boost::archive::save_access;

    template<class T>
    void save(T& t)
    {
        static_assert(is_hdf5_type<T>::value,"T is not a HDF5 type");

        std::string name = name_stack_.top();
        std::size_t offset = reinterpret_cast<const char*>(&t) - base_address_;
        type member_type = get_hdf5_type<T>();

        layout_.add_element(name, member_type, offset);
    }

    type_layout layout_;
    std::stack<const char*> name_stack_;
    const char* base_address_;
};

}

BOOST_SERIALIZATION_REGISTER_ARCHIVE(echelon::hdf5_type_oarchive)
BOOST_SERIALIZATION_USE_ARRAY_OPTIMIZATION(echelon::hdf5_type_oarchive)

#endif
