//  Copyright (c) 2012 Christopher Hinz
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ECHELON_TYPE_HPP
#define ECHELON_TYPE_HPP

#include <echelon/hdf5/type.hpp>

#include <string>
#include <vector>

namespace echelon
{
class type_layout;

/** \brief A handle to an HDF5 type.
 */
class type
{
public:
    explicit type(hdf5::type type_wrapper_);

    /** \brief Returns a handle to the primitive type 'char'.
     */
    static type char_();

    /** \brief Returns a handle to the primitive type 'short'.
     */
    static type short_();

    /** \brief Returns a handle to the primitive type 'int'.
     */
    static type int_();

    /** \brief Returns a handle to the primitive type 'long'.
     */
    static type long_();

    /** \brief Returns a handle to the primitive type 'long long'.
     */
    static type long_long();

    /** \brief Returns a handle to the primitive type 'unsigned char'.
     */
    static type uchar();

    /** \brief Returns a handle to the primitive type 'unsigned short'.
     */
    static type ushort();

    /** \brief Returns a handle to the primitive type 'unsigned int'.
     */
    static type uint();

    /** \brief Returns a handle to the primitive type 'unsigned long'.
     */
    static type ulong();

    /** \brief Returns a handle to the primitive type 'unsigned long long'.
     */
    static type ulong_long();

    /** \brief Returns a handle to the primitive type 'float'.
     */
    static type float_();

    /** \brief Returns a handle to the primitive type 'double'.
     */
    static type double_();


    /** \brief Returns a handle to the primitive type 'string'.
     */
    static type string();

    /** \brief Returns a handle to the primitive type 'object reference'.
     */
    static type object_reference();


    /** \brief Creates a new compound type using a given layout.
     *
     *  \param layout the layout, which describes the type's structure
     *
     *  \return a handle to the new compound type
     */
    static type compound_type(const type_layout& layout);


    /** \brief Clones the type.
     *
     *  \post t == t.clone()
     *
     *  \return a handle to the new type
     */
    type clone()const;

    /** \brief The underlying HDF5 low-level handle.
     */
    const hdf5::type& get_native_type()const;
private:
    hdf5::type type_wrapper_;
};

/** \brief Tests two types for equality.
 *
 *  Type equality within echelon is defined by the following rules:
 *  - All primitive types are not equal.
 *  - Types with a different type class are not equal.
 *  - Two compound types are equal, if and only if:
 *      - They have the same number of members.
 *      - Each member of the first type is equal to the corresponding member of the second type.
 *      - The name of each member of the first type is equal to the name of the corresponding member of the second type.
 *
 *  \relates echelon::type
 *
 *  \param lhs left-hand side of the comparison
 *  \param rhs right-hand side of the comparison
 *
 *  \return true, if both types are equal, and false otherwise
 */
bool operator==(const type& lhs,const type& rhs);

/** \brief Tests two types for inequality.
 *
 *  The same as !(lhs == rhs).
 *
 *  For a detailed description of type equality within echelon
 *  see ::operator==.
 *
 *  \relates echelon::type
 *
 *  \param lhs left-hand side of the comparison
 *  \param rhs right-hand side of the comparison
 *
 *
 *  \return true, if both types are not equal, and false otherwise
 */
bool operator!=(const type& lhs,const type& rhs);

/** \brief Description of a compound type's internal structure.
 */
class type_layout
{
public:
    /** \brief Tuple containing a name, a type and an offset, which are used to
     *         describe a single element of a compound type.
     */
    struct element
    {
        /** \brief Constructs a new element.
         *
         *  \param name name of the member
         *  \param type type of the member
         *  \param offset offset of the member within the compound type in bytes
         */
        element(const std::string& name, const type& type, std::size_t offset)
        : name(name), type_(type.clone()), offset(offset)
        {}

        /** \brief name of the member
         */
        std::string name;

        /** \brief type of the member
         */
        type type_;

        /** \brief offset of the member within the compound type in bytes
         */
        std::size_t offset;
    };

    /** \brief Constructs an empty layout for a compound type with a given total size.
     *
     *  \param size_ total size of the compound type in bytes
     */
    explicit type_layout(std::size_t size_)
    : size_(size_)
    {}

    /** \brief Adds a new member to the compound type.
     *
     *  \param name name of the member
     *  \param element_type type of the member
     *  \param offset offset of the member within the compound type in bytes
     */
    void add_element(std::string name, const type& element_type,
                     std::size_t offset)
    {
        elements_.emplace_back(name, element_type, offset);
    }

    /** Type of an iterator over all members of the compound type.
     */
    typedef std::vector<element>::const_iterator iterator;

    /** Returns an iterator, which points to the first member within the compound type.
     */
    iterator begin() const
    {
        using std::begin;

        return begin(elements_);
    }

    /** Returns an iterator, which points beyond the last member within the compound type.
     */
    iterator end() const
    {
        using std::end;

        return end(elements_);
    }

    /** Total size of the compound type in bytes.
     */
    std::size_t size() const
    {
        return size_;
    }
private:
    std::vector<element> elements_;
    std::size_t size_;
};

}

#endif
