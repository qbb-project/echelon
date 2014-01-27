//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_OBJECT_REFERENCE_HPP
#define ECHELON_OBJECT_REFERENCE_HPP

#include <echelon/object.hpp>

#include <echelon/hdf5/object_reference.hpp>
#include <echelon/hdf5/customization_hooks.hpp>
#include <echelon/hdf5/type_factory.hpp>
#include <echelon/hdf5/type_traits_fwd.hpp>

#include <boost/mpl/bool.hpp>

#include <string>
#include "hdf5/attribute.hpp"

namespace echelon
{

/** \brief A reference to an HDF5 object.
 *
 * \note Note that the concept of a HDF5 reference has nothing to do with C++
 *       references. Since a HDF5 reference has a null state, it resembles a C++ pointer
 *       without pointer arithmetic and correspondingly it has a similar interface.
 */
class object_reference
{
public:
    /** \brief Creates a null reference.
     */
    object_reference();

    explicit object_reference(hdf5::object_reference native_handle_);

    /** \brief Dereferences this reference.
     *
     *  \return handle to the referenced object
     */
    object operator*() const;

    /** \brief Tests, if the reference is non-null.
     *
     *  \return true, if the reference is non-null and false otherwise
     */
    explicit operator bool() const;

    /** \brief The underlying low-level reference.
     */
    const hdf5::object_reference& raw_ref() const
    {
        return reference_handle_;
    }

private:
    hdf5::object_reference reference_handle_;
};

namespace hdf5
{

template <>
struct type_lowering_hook<echelon::object_reference>
{
    typedef echelon::object_reference original_type;
    typedef hdf5::precursor::object_reference lowered_type;

    template <typename Sink>
    static lowered_type lower_type(const original_type& value, const Sink& sink)
    {
        return type_lowering_hook<hdf5::object_reference>::lower_type(value.raw_ref(), sink);
    }

    template <typename Source>
    static original_type raise_type(lowered_type value, const Source& source)
    {
        return object_reference(
            type_lowering_hook<hdf5::object_reference>::raise_type(value, source));
    }
};

template <>
struct hdf5_type_selector<echelon::object_reference>
{
    static type get()
    {
        return type::object_reference();
    }
};

template <>
struct is_predefined_hdf5_type<echelon::object_reference> : boost::mpl::true_
{
};
}
}

#endif
