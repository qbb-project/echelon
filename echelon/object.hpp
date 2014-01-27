//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_OBJECT_HPP
#define ECHELON_OBJECT_HPP

#include <echelon/hdf5/object.hpp>
#include <exception>
#include <string>
#include <utility>

namespace echelon
{

class group;
class dataset;
class scalar_dataset;

class object_reference;

/** \brief Polymorphic handle to an HDF5 object.
 */
class object
{
public:
    /** \brief Type of the underlying HDF5 low-level handle
     */
    using native_handle_type = hdf5::object;

    explicit object(native_handle_type native_handle_);

    /** \brief Constructs a handle from an pre-existing group handle.
     *
     *  \param object_ a handle to the shared object
     */
    object(const group& object_);

    /** \brief Constructs a handle from an pre-existing dataset handle.
     *
     *  \param object_ a handle to the shared object
     */
    object(const dataset& object_);

    /** \brief Constructs a handle from an pre-existing scalar dataset handle.
     *
     *  \param object_ a handle to the shared object
     */
    object(const scalar_dataset& object_);

    /** \brief Changes the object, which is referenced by the handle.
     *
     *  \param object_ a handle to the shared object
     */
    object& operator=(const group& object_);

    /** \brief Changes the object, which is referenced by the handle.
     *
     *  \param object_ a handle to the shared object
     */
    object& operator=(const dataset& object_);

    /** \brief Changes the object, which is referenced by the handle.
     *
     *  \param object_ a handle to the shared object
     */
    object& operator=(const scalar_dataset& object_);

    /** \brief Constructs a group handle, which shares ownership with this
     *         handle.
     *
     *  The incorporated conversion might fail, if the generated handle can't
     *  reference the object, which is referenced by this handle.
     *  An exception is thrown in this case.
     *
     *  \throws wrong_object_type_exception is thrown, if the generated handle
     *                                      would be incompatible with the
     *                                      referenced object.
     */
    operator group() const;

    /** \brief Constructs a dataset handle, which shares ownership with this
     *         handle.
     *
     *  The incorporated conversion might fail, if the generated handle can't
     *  reference the object, which is referenced by this handle.
     *  An exception is thrown in this case.
     *
     *  \throws wrong_object_type_exception is thrown, if the generated handle
     *                                      would be incompatible with the
     *                                      referenced object.
     */
    operator dataset() const;

    /** \brief Constructs a scalar dataset handle, which shares ownership with
     *         this handle.
     *
     *  The incorporated conversion might fail, if the generated handle can't
     *  reference the object, which is referenced by this handle.
     *  An exception is thrown in this case.
     *
     *  \throws wrong_object_type_exception is thrown, if the generated handle
     *                                      would be incompatible with the
     *                                      referenced object.
     */
    operator scalar_dataset() const;

    /** \brief A HDF5 object reference to this object.
     */
    object_reference ref() const;

    /** \brief The underlying HDF5 low-level handle.
     */
    native_handle_type native_handle() const;

private:
    hdf5::object object_handle_;
};
}

#endif
