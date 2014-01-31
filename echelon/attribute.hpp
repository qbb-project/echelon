//  Copyright (c) 2012-2014 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_ATTRIBUTE_HPP
#define ECHELON_ATTRIBUTE_HPP

#include <echelon/type.hpp>
#include <echelon/object.hpp>

#include <echelon/hdf5/attribute.hpp>

#include <utility>

namespace echelon
{

/** \brief A handle to an echelon attribute.
 */
class attribute
{
public:
    using native_handle_type = hdf5::attribute;

    explicit attribute(native_handle_type native_handle_);

    /** \brief Writes the content of a variable into the attribute.
     *
     *  \tparam T Type of the written value.
     *
     *  \param attr  attribute, to which the value is written
     *  \param value value, which is written into the attribute
     */
    template <typename T>
    friend inline void operator<<=(attribute& attr, const T& value)
    {
        attr.attribute_handle_ <<= value;
    }

    /** \brief Reads the content of an attribute into a variable.
     *
     *  \tparam T Type of the read value.
     *
     *  \param value value, which is read from the attribute
     *  \param attr  attribute, from which the value is read
     */
    template <typename T>
    friend inline void operator<<=(T& value, const attribute& attr)
    {
        value <<= attr.attribute_handle_;
    }

    /** \brief The value type of the attribute.
     */
    type datatype() const;

private:
    hdf5::attribute attribute_handle_;
};
}

#endif
