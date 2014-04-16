//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_OBJECT_REFERENCE_HPP
#define ECHELON_OBJECT_REFERENCE_HPP

#include <echelon/object.hpp>

#include <echelon/hdf5/object_reference.hpp>

#include <string>

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

}

#endif
